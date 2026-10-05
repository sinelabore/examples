"""Runtime support for Python generated from SysML v2 by Sinelabore.

Put this module next to the generated one, or on PYTHONPATH.
"""

from __future__ import annotations

import time
from collections import deque
from dataclasses import dataclass, field
from enum import Enum
from typing import Callable, Generic, TypeVar

T = TypeVar("T")


class ActionStatus(Enum):
    """What one call of an action achieved."""

    FINISHED = "finished"
    YIELDED = "yielded"
    BLOCKED = "blocked"

    def __str__(self) -> str:
        """The status as its plain word."""
        return self.value


def now_ms() -> float:
    """Milliseconds on a monotonic clock, for 'accept after' timeouts."""
    return time.monotonic() * 1000.0


#: Replaceable source of the current instant, in seconds. None means the wall clock.
time_source: Callable[[], float] | None = None


def sysml_now() -> float:
    """The current instant in seconds, for 'accept at'."""
    source = time_source
    if source is None:
        return time.time()
    return float(source())


#: Milliseconds per SysML time unit, in the singular spelling.
_MS_PER_UNIT = {
    "nanosecond": 1e-6, "microsecond": 1e-3, "millisecond": 1.0,
    "second": 1000.0, "minute": 60_000.0, "hour": 3_600_000.0,
    "day": 86_400_000.0,
}


def ms_from_time_unit(amount: float, unit: str) -> float:
    """Convert an amount of a named time unit to milliseconds.

    Called only by modules generated before 7.4: since bug 393 the generator converts an
    'accept after' duration itself, for every duration unit of the SysML library.
    """
    simple = unit.rsplit("::", 1)[-1].strip("'\"").lower()
    if simple.endswith("s") and simple[:-1] in _MS_PER_UNIT:
        simple = simple[:-1]
    if simple not in _MS_PER_UNIT:
        raise ValueError(f"unknown time unit {unit!r}")
    return amount * _MS_PER_UNIT[simple]


def _default_trace_sink(kind: str, context: str, name: str, detail: str | None) -> None:
    """Print one trace line: ``debug: <context>.<name> [<kind>] <detail>``."""
    line = "debug: "
    if context:
        line += context + "."
    line += "%s [%s]" % (name, kind)
    if detail:
        line += " " + detail
    print(line)


#: Replace to redirect tracing, or set to None to drop it. Only code generated
#: with -d calls emit_trace.
trace_sink: Callable[[str, str, str, str | None], None] | None = _default_trace_sink


def emit_trace(kind: str, context: str, name: str, detail: str | None = None) -> None:
    """Report one traced event through :data:`trace_sink`."""
    sink = trace_sink
    if sink is not None:
        sink(kind, context, name, detail)


class ConstrainedList(list, Generic[T]):
    """A list of a declared size, which reports its size when it is built."""

    def __init__(self, fill_count: int, factory: Callable[[], T]) -> None:
        """Report the count, then fill from factory.

        Printed directly, as ``ConstrainedVector`` does in framework.h - the two runtimes
        say the same thing in the same way, and the testcase references hold these lines.
        Tracing stays redirectable through :data:`trace_sink`, which framework.h also
        offers; this line is not a trace.
        """
        super().__init__()
        count = int(fill_count)
        print("emplace:%d" % count)
        for _ in range(count):
            self.append(factory())


@dataclass(slots=True)
class Part:
    """Base of every generated part.

    ``instance_name`` is the containment path from the root part, such as
    ``"System.monitor"``. An owner names the parts it holds.
    """

    instance_name: str = ""
    # The owning part (see owner()). Not part of repr or ==: the owner holds this part, so
    # either would run in a circle, and it says where the part sits, not what it is.
    _owner: "Part | None" = field(default=None, repr=False, compare=False)

    def set_instance_name(self, name: str) -> None:
        """Name this part."""
        self.instance_name = name

    def set_owned_name(self, owner: "Part", member: str) -> None:
        """Name this part ``<owner path>.<member>`` and remember its owner."""
        self._owner = owner
        self.instance_name = f"{owner.instance_name}.{member}" if owner.instance_name else member

    def owner(self) -> "Part | None":
        """The part that holds this one, or None for the root and before the owner's init().

        A state machine uses it for a 'send ... to' a part it reaches through an
        enclosing part (bug 399), as C++ ``Part::owner()`` does.
        """
        return self._owner

    def init(self) -> None:
        """Set this part up. A generated part overrides it; empty here."""

    def process(self) -> None:
        """Run one step of this part and of everything it owns; empty here."""

    def _accept(self, name: str, payload: object) -> bool:
        """Store an accepted payload in the named field and return True."""
        setattr(self, name, payload)
        return True

    def _rising_edge(self, field: str, condition: bool) -> bool:
        """True when condition has just become true; remembers it in the named field."""
        previous = bool(getattr(self, field, False))
        held = bool(condition)
        setattr(self, field, held)
        return held and not previous

    def elapsed_ms_since(self, started: float) -> float:
        """Milliseconds since a timestamp taken with :func:`now_ms`."""
        return now_ms() - started


@dataclass(slots=True)
class Port(Generic[T]):
    """An output port: hands each sent payload to everything connected to it."""

    receivers: list[Callable[[T], None]] = field(default_factory=list)

    def send(self, data: T) -> None:
        """Deliver one payload to every receiver, synchronously."""
        # Over a copy, so a receiver may connect another port while it handles this one.
        for receive in list(self.receivers):
            receive(data)

    def add_receiver(self, receiver: Callable[[T], None]) -> None:
        """Call this receiver on every send."""
        self.receivers.append(receiver)

    def connect(self, other: "InputPort[T]") -> None:
        """Wire this output to an input port."""
        self.receivers.append(other.receive)


class _AbsentPortPayload:
    """Stands in for a payload before one has arrived: every field reads as None."""

    def __getattr__(self, name: str) -> None:
        """None for any field; a dunder is refused, so protocol lookups still fail."""
        if name.startswith("__") and name.endswith("__"):
            raise AttributeError(name)
        return None


_ABSENT_PORT_PAYLOAD = _AbsentPortPayload()


@dataclass(slots=True)
class InputPort(Generic[T]):
    """An input port: a queue the owning part polls with :meth:`has_event`.

    ``observer`` is a read-only side channel, told about an arrival without
    taking it. It runs inside the sender's ``send``, so keep it cheap, and never
    dispatch a state machine from it.
    """

    queue: deque[T] = field(default_factory=deque)
    observer: Callable[[T], None] | None = None
    current: T | None = None
    step_holds_: bool = False

    def receive(self, data: T) -> None:
        """Queue one payload, telling the observer first."""
        if self.observer is not None:
            self.observer(data)
        self.queue.append(data)

    def has_event(self) -> bool:
        """Take the next payload into ``current``; False when the queue is empty."""
        if not self.queue:
            return False
        self.current = self.queue.popleft()
        return True

    def step_take(self) -> bool:
        """Take the next payload for one run-to-completion step of a state machine.

        Loads it into ``current`` as :meth:`has_event` does and holds it until
        :meth:`step_end`. Every transition tests :meth:`step_holds`, which does
        not pop, so an inner state, its parent and every region see the same
        payload (bug 399). ``has_event`` stays the receive of an action.
        """
        self.step_holds_ = self.has_event()
        return self.step_holds_

    def step_holds(self) -> bool:
        """True while the payload taken by :meth:`step_take` is held."""
        return self.step_holds_

    def step_end(self) -> None:
        """Drop the held payload at the end of the step, taken or not."""
        self.step_holds_ = False

    def set_observer(self, observer: Callable[[T], None]) -> None:
        """Call this observer whenever a payload arrives."""
        self.observer = observer

    def queue_depth(self) -> int:
        """How many payloads have arrived and not yet been taken."""
        return len(self.queue)

    def __getattr__(self, name: str) -> object:
        """Read a field of the payload ``has_event`` took, or None before the first.

        A dunder is refused rather than answered, so ``copy`` and ``pickle`` find
        no method that is not there.
        """
        if name.startswith("__") and name.endswith("__"):
            raise AttributeError(name)
        current = object.__getattribute__(self, "current")
        if current is not None:
            return getattr(current, name)
        return _ABSENT_PORT_PAYLOAD

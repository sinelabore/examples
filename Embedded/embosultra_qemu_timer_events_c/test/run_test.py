#!/usr/bin/env python3
"""A scripted run of the oven: type keys into the console at set times and
compare the output with test/expected.txt.

    run_test.py -- <command that runs QEMU with the console on stdin/stdout>

The keys are typed only after the machine has started (its first output), and
each lies half a second or more away from any timer's deadline, so the output
does not depend on how fast the host is. After the last key the run waits
for the oven to finish, then ends QEMU.
"""
import difflib
import os
import subprocess
import sys
import threading
import time

HERE = os.path.dirname(os.path.abspath(__file__))
EXPECTED = os.path.join(HERE, "expected.txt")
ACTUAL = os.path.join(HERE, "..", "build", "test.out")

# (seconds to wait before, keys)
SCRIPT = [
    (0.5, "+++"),       # 3 s
    (0.3, "-"),         # 2 s
    (0.3, "+"),         # 3 s
    (0.3, "pP"),        # power 400 W, then 1200 W
    (0.3, "c"),         # cooks 3 s: two dots (the every timer restarts itself), then
                        # off; the third dot falls on the end and is dropped as stale
    (4.0, "o"),         # door open after cooking: off
    (0.3, "++"),        # 2 s
    (0.3, "c"),         # cooks
    (0.5, "o"),         # pause after 0.5 s: off, 1.5 s left
    (0.5, "c"),         # resume: one dot after 1 s, then off after 1.5 s
]
# after the last key: long enough for the oven to finish
SETTLE = 2.5


def main():
    if "--" not in sys.argv:
        sys.exit(__doc__)
    cmd = sys.argv[sys.argv.index("--") + 1:]
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT)
    out = bytearray()
    started = threading.Event()

    def read():
        while True:
            b = proc.stdout.read(1)
            if not b:
                break
            out.extend(b)
            if b"Oven::off" in out:
                started.set()

    reader = threading.Thread(target=read, daemon=True)
    reader.start()
    if not started.wait(60):
        proc.kill()
        sys.exit("FAIL: the oven did not start\n" + out.decode(errors="replace"))
    for delay, keys in SCRIPT:
        time.sleep(delay)
        proc.stdin.write(keys.encode())
        proc.stdin.flush()
    time.sleep(SETTLE)
    proc.terminate()                    # docker passes the signal on to QEMU
    try:
        proc.wait(30)
    except subprocess.TimeoutExpired:
        proc.kill()
    reader.join(5)

    actual = out.decode(errors="replace").replace("\r", "")
    # QEMU's own note that the terminate above ended it
    actual = "".join(l for l in actual.splitlines(True)
                     if not l.startswith("qemu-system-arm: terminating on signal"))
    os.makedirs(os.path.dirname(ACTUAL), exist_ok=True)
    with open(ACTUAL, "w") as f:
        f.write(actual)
    with open(EXPECTED) as f:
        expected = f.read()
    if actual != expected:
        sys.stdout.writelines(difflib.unified_diff(
            expected.splitlines(True), actual.splitlines(True), "expected.txt", "build/test.out"))
        sys.exit("FAIL: build/test.out differs from test/expected.txt")
    print("PASS: build/test.out matches test/expected.txt")


if __name__ == "__main__":
    main()

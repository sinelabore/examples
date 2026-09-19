/*
 * (c) 2026 Sinelabore Software Tools GmbH. All rights reserved.
 *
 * Runs one test system from test.sysml (named on the command line, default TestTOn) and prints its signals scan by scan.
 */

#include "system.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace IEC61131_3;
using namespace BoolTests;
using namespace LogicBasic;

namespace {

/** Follows what a port sends into {@code into}, beside whatever it is connected to. */
void watch(Port<BoolPort>& port, bool& into) {
    port.addReceiver([&into](const BoolPort& sent) {
        into = std::get<BoolPortDef>(sent).boolPortPayload.v;
    });
}

/** A tick well below the shortest time in the model - the timers follow the clock. */
void tick() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

/** constant -> and -> not -> not -> on-delay timer. */
void testTOn() {
    TestTOn system;
    system.init();

    bool and2 = false, not1 = false, not2 = false, ton = false;
    watch(system.and2_1->q, and2);
    watch(system.not_1->q, not1);
    watch(system.not_2->q, not2);
    watch(system.ton_1->q, ton);

    std::cout << "cycle | and2_1.q not_1.q not_2.q ton_1.q\n";
    for (int cycle = 0; cycle < 10; cycle++) {
        system.process();
        std::cout << "   " << cycle << "  |        " << and2 << "       " << not1
                  << "       " << not2 << "       " << ton << "\n";
        tick();
    }
}

/**
 * A single pulse into an off-delay timer.
 *
 * toff_1.q follows the pulse up at once and stays up for offDelay (0.2 s) after it drops,
 * so the run has to outlast the pulse by that much to show the tail.
 */
void testToff() {
    TestToff system;
    system.init();

    bool pulse = false, toff = false;
    watch(system.sp_2->q, pulse);
    watch(system.toff_1->q, toff);

    std::cout << "cycle | sp_2.q toff_1.q\n";
    for (int cycle = 0; cycle < 20; cycle++) {
        system.process();
        // the only run that passes cycle 9, so the number is padded to keep the columns
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   |      " << pulse
                  << "        " << toff << "\n";
        tick();
    }
}

/** A single pulse into a rising-edge detector and, through a not, a falling-edge one. */
void testFallingEdge() {
    TestFallingEdge system;
    system.init();

    bool pulse = false, inverted = false, rising = false, falling = false;
    watch(system.sp_1->q, pulse);
    watch(system.not_1->q, inverted);
    watch(system.rtrig_1->q, rising);
    watch(system.ftrig_1->q, falling);

    std::cout << "cycle | sp_1.q not_1.q rtrig_1.q ftrig_1.q\n";
    for (int cycle = 0; cycle < 10; cycle++) {
        system.process();
        std::cout << "  " << cycle << "   |      " << pulse << "       " << inverted
                  << "         " << rising << "         " << falling << "\n";
        tick();
    }
}

/**
 * A blinking clock into the count-up input of a counter.
 *
 * cud_1.cv rises by one per rising edge of the clock. qu goes true when cv reaches pv,
 * which starts ton_1; onDelay later ton_1.q resets the counter, so cv sawtooths and
 * overshoots pv by whatever the delay is worth.
 */
void testCountUp() {
    TestCountUp system;
    system.init();

    bool clock = false, qu = false, qd = false, reset = false;
    watch(system.pulse_1->q, clock);
    watch(system.cud_1->qu, qu);
    watch(system.cud_1->qd, qd);
    watch(system.ton_1->q, reset); // what ton_1 sends into cud_1.reset

    std::cout << "cycle | pulse_1.q cud_1.cv   qu qd   reset\n";
    for (int cycle = 0; cycle < 40; cycle++) {
        system.process();
        const int cv = system.cud_1->cv;
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   |         " << clock
                  << (cv < 10 ? "       " : "      ") << cv
                  << "     " << qu << "  " << qd << "    " << reset << "\n";
        tick();
    }
}

/**
 * A blinking clock into all three inputs of an and3.
 *
 * blink_2 is on for ton (0.01 s) and off for toff (0.04 s). The on time is shorter than a
 * tick, so it shows as a single cycle. The off time is exactly two ticks, so it shows as two
 * cycles or three, and the table differs from run to run. With the same wire on every input,
 * and3_1.q is the clock itself.
 */
void testAsyncBlink() {
    TestAsyncBlink system;
    system.init();

    bool clock = false, and3 = false;
    watch(system.blink_2->q, clock);
    watch(system.and3_1->q, and3);

    std::cout << "cycle | blink_2.q and3_1.q\n";
    for (int cycle = 0; cycle < 20; cycle++) {
        system.process();
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   |         " << clock
                  << "        " << and3 << "\n";
        tick();
    }
}

/**
 * A clock into four T_FFs, each one clocked by the bit below it: a 4 bit counter.
 *
 * One count per cycle, and the count is read straight after process() rather than on an edge.
 * Both of those are earned: the clock is SingleClockPulse, which changes level once per scan,
 * the first bit toggles on either edge so every scan is a count, and the blocks are declared
 * in the order the carry travels so the whole word settles in the scan that clocked it. Any
 * of the three undone and the reading has to wait for the word to stop moving.
 */
void testCounter4() {
    TestCounter4 system;
    system.init();

    bool clock = false, b0 = false, b1 = false, b2 = false, b3 = false;
    watch(system.clock->q, clock);
    watch(system.counter->b0, b0);
    watch(system.counter->b1, b1);
    watch(system.counter->b2, b2);
    watch(system.counter->b3, b3);

    std::cout << "cycle | b3 b2 b1 b0 | count\n";
    for (int cycle = 0; cycle < 30; cycle++) {
        system.process();
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   |  " << b3 << "  " << b2
                    << "  " << b1 << "  " << b0 << "  |   "
                    << ((b3 << 3) | (b2 << 2) | (b1 << 1) | b0) << "\n";
    }
}

/**
 * A 4 bit shift register with the last stage fed back inverted: a Johnson counter.
 *
 * Where Counter4 wants its blocks declared along the signal so a carry settles at once, this
 * wants them declared against it: the four stages are clocked together and each must capture
 * what its neighbour held before the edge. Declared the other way the bit races through all
 * four in one scan and the register reads 0000, 1111, 0000.
 */
void testShiftReg4() {
    TestShiftReg4 system;
    system.init();

    bool clk = false, q0 = false, q1 = false, q2 = false, q3 = false;
    watch(system.clock->q, clk);
    watch(system.reg->q0, q0);
    watch(system.reg->q1, q1);
    watch(system.reg->q2, q2);
    watch(system.reg->q3, q3);

    std::cout << "cycle | clk | q0 q1 q2 q3\n";
    for (int cycle = 0; cycle < 20; cycle++) {
        system.process();
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   |  " << clk << "  |  "
                  << q0 << "  " << q1 << "  " << q2 << "  " << q3 << "\n";
    }
}

/**
 * A 2 to 4 decoder with its two select lines counting.
 *
 * Both the enable and the outputs are active low, as on a 74HC139: the selected line is the
 * one reading 0. Nothing here holds state, so the outputs follow the inputs inside the same
 * scan - contrast Counter4 and ShiftReg4, which are all about what a scan remembers.
 */
void testDecoder2to4() {
    TestDecoder2to4 system;
    system.init();

    bool a0 = false, a1 = false, y0 = false, y1 = false, y2 = false, y3 = false;
    watch(system.clock->q, a0);
    watch(system.half->q, a1);
    watch(system.dec->y0, y0);
    watch(system.dec->y1, y1);
    watch(system.dec->y2, y2);
    watch(system.dec->y3, y3);

    std::cout << "cycle | a1 a0 | y0 y1 y2 y3 | selected\n";
    for (int cycle = 0; cycle < 12; cycle++) {
        system.process();
        int selected = -1;
        if (!y0) selected = 0;
        if (!y1) selected = 1;
        if (!y2) selected = 2;
        if (!y3) selected = 3;
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   |  " << a1 << "  " << a0
                  << "  |  " << y0 << "  " << y1 << "  " << y2 << "  " << y3 << "  |    "
                  << (selected < 0 ? std::string("-") : std::to_string(selected)) << "\n";
    }
}

/**
 * An SR latch built from two cross-coupled NAND gates.
 *
 * Both inputs are active low. The interesting column is q while nS and nR are both 1: the
 * latch holds what the last active input wrote, with nothing in the model storing it but the
 * loop between the two gates.
 */
void testSRLatch() {
    TestSRLatch system;
    system.init();

    bool nS = false, nR = false, q = false, nQ = false;
    watch(system.t1->q, nS);
    watch(system.t2->q, nR);
    watch(system.latch->q, q);
    watch(system.latch->nQ, nQ);

    std::cout << "cycle | nS nR | q nQ | what\n";
    for (int cycle = 0; cycle < 18; cycle++) {
        system.process();
        const char* what = !nS && !nR ? "forbidden" : (!nS ? "set" : (!nR ? "reset" : "hold"));
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   |  " << nS << "  " << nR
                  << "  | " << q << "  " << nQ << "  | " << what << "\n";
    }
}

/**
 * A 4 bit adder: a running count on a, a constant 3 on b.
 *
 * Both operands and the whole carry chain are gates only - no flip-flop anywhere in the adder
 * - so the sum settles in the same scan the count changes in, and sum == count + 3 on every
 * line, with cout taking the overflow.
 */
void testAdder4() {
    TestAdder4 system;
    system.init();

    bool a0 = false, a1 = false, a2 = false, a3 = false;
    bool s0 = false, s1 = false, s2 = false, s3 = false, cout_ = false;
    watch(system.counter->b0, a0);
    watch(system.counter->b1, a1);
    watch(system.counter->b2, a2);
    watch(system.counter->b3, a3);
    watch(system.adder->s0, s0);
    watch(system.adder->s1, s1);
    watch(system.adder->s2, s2);
    watch(system.adder->s3, s3);
    watch(system.adder->cout, cout_);

    std::cout << "cycle |  a | + 3 | cout sum | check\n";
    for (int cycle = 0; cycle < 18; cycle++) {
        system.process();
        const int a = (a3 << 3) | (a2 << 2) | (a1 << 1) | a0;
        const int s = (s3 << 3) | (s2 << 2) | (s1 << 1) | s0;
        std::cout << (cycle < 10 ? "  " : " ") << cycle << "   | " << (a < 10 ? " " : "") << a
                  << " |     |   " << cout_ << "   " << (s < 10 ? " " : "") << s << "  |  "
                  << (((cout_ << 4) | s) == a + 3 ? "ok" : "WRONG") << "\n";
    }
}

} // namespace

int main(int argc, char** argv) {
    std::string wanted = argc > 1 ? argv[1] : "TestTOn";

    if (wanted == "TestTOn") {
        testTOn();
    } else if (wanted == "TestToff") {
        testToff();
    } else if (wanted == "TestFallingEdge") {
        testFallingEdge();
    } else if (wanted == "TestCountUp") {
        testCountUp();
    } else if (wanted == "TestAsyncBlink") {
        testAsyncBlink();
    } else if (wanted == "TestCounter4") {
        testCounter4();
    } else if (wanted == "TestShiftReg4") {
        testShiftReg4();
    } else if (wanted == "TestDecoder2to4") {
        testDecoder2to4();
    } else if (wanted == "TestSRLatch") {
        testSRLatch();
    } else if (wanted == "TestAdder4") {
        testAdder4();
    } else {
        std::cerr << "usage: " << argv[0]
                  << " [TestTOn|TestToff|TestFallingEdge|TestCountUp|TestAsyncBlink|TestCounter4|TestShiftReg4|TestDecoder2to4|TestSRLatch|TestAdder4]\n";
        return 1;
    }
    return 0;
}

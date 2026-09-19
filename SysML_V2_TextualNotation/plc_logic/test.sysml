/*
 * Test harnesses for the block library: one part per network, each drawn by the
 * Makefile and run by main.cpp.
 *
 * (c) 2026 Sinelabore Software Tools GmbH. All rights reserved.
 */
private import ScalarValues::*;
private import SI::*;
private import IEC61131_3::*;
private import LogicBasic::*;
private import Logic74xxx::*;

package BoolTests{

    part def TestTOn{

        part constTrue : ConstTrue;
        part and2_1 : BoolAnd2;
        part not_1 : BoolNot;
        part not_2 : BoolNot;
        part ton_1 : TON {
            attribute :>> onDelay = 0.1;
        }

        connect constTrue.q to and2_1.i1;
        connect constTrue.q to and2_1.i2;
        connect and2_1.q to not_1.i;
        connect not_1.q to not_2.i;
        connect not_2.q to ton_1.set;
    }

    part def TestToff{
        part sp_2:TonToffPulse{
            attribute :>> ton = 0.1;
            attribute :>> toff = 0.1;
        }
        part toff_1:TOF {
            attribute :>> offDelay = 0.1;
        }

        connect sp_2.q to toff_1.set;
    }

    part def TestFallingEdge{
        part sp_1:TonToffPulse{
            attribute :>> ton = 0.1;
            attribute :>> toff = 0.1;
        }
        part not_1:BoolNot;
        part rtrig_1:R_TRIG;
        part ftrig_1:F_TRIG;

        connect sp_1.q to not_1.i;
        connect sp_1.q to rtrig_1.set;
        connect not_1.q to ftrig_1.set;
    }

    part def TestCountUp{

        // clock for counting
        part pulse_1:AsyncBlink{
            attribute :>> ton = 0.02;
            attribute :>> toff = 0.02;            
        }

        part cud_1:CTUD{
            attribute :>> pv = 5;
        }

        // reset after some time
        part ton_1 : TON {
            attribute :>> onDelay = 0.1;
        }

        connect pulse_1.q to cud_1.cu;

        // qu starts the delay, the delay resets the counter: cv sawtooths between 0 and pv
        connect cud_1.qu to ton_1.set;
        connect ton_1.q to cud_1.reset;

    }

    part def TestAsyncBlink{
        part blink_2:AsyncBlink{
            attribute :>> ton = 0.01;
            attribute :>> toff = 0.05;            
        }

        part and3_1 : BoolAnd3;
        connect blink_2.q to and3_1.i1;
        connect blink_2.q to and3_1.i2;        
        connect blink_2.q to and3_1.i3;
    }

    part def TestCounter4 {
        part clock : SingleClockPulse;

        part counter : Counter4;

        connect clock.q to counter.clk;
    }

    // The register wired as a Johnson counter: the last stage feeds the first one back inverted
    part def TestShiftReg4 {
        part clock : SingleClockPulse;
        part reg : ShiftReg4;
        part inv : BoolNot;

        connect clock.q to reg.clk;
        connect reg.q3 to inv.i;
        connect inv.q to reg.serIn;
    }

    // The decoder walked through all four of its input combinations.
    part def TestDecoder2to4 {
        part clock : SingleClockPulse;
        part half : T_FF;

        part tru : ConstTrue;
        part notEn : BoolNot;

        part dec : Decoder2to4;

        connect clock.q to half.clk;
        connect clock.q to dec.a0;
        connect half.q to dec.a1;

        connect tru.q to notEn.i;
        connect notEn.q to dec.e;
    }


    part def TestSRLatch {
        part clock : SingleClockPulse;
        part t1 : T_FF;
        part t2 : T_FF;

        part latch : SRLatch;

        connect clock.q to t1.clk;
        connect t1.q to t2.clk;
        connect t1.q to latch.nS;
        connect t2.q to latch.nR;
    }

    part def TestAdder4 {
        part clock : SingleClockPulse;
        part counter : Counter4;

        part tru : ConstTrue;
        part notTru : ConstFalse;

        part adder : Adder4;

        connect clock.q to counter.clk;

        connect counter.b0 to adder.a0;
        connect counter.b1 to adder.a1;
        connect counter.b2 to adder.a2;
        connect counter.b3 to adder.a3;

        // b = 0011, and no carry in
        connect tru.q to adder.b0;
        connect tru.q to adder.b1;
        connect notTru.q to adder.b2;
        connect notTru.q to adder.b3;
        connect notTru.q to adder.cin;
    }
}

/*
 * (c) 2026 Sinelabore Software Tools GmbH. All rights reserved.
 */
private import ScalarValues::*;
private import SI::*;
private import IEC61131_3::*;
private import LogicBasic::*;

// Simple logic blocks similar to the famous 74xxx family of chips
package Logic74xxx {

    // Four bits counting the edges of clk - both of them, so a clock that changes every scan
    // counts every scan. Only the first bit is edge-doubled; above it each bit is a plain
    // T_FF clocked by the bit below falling back to 0, which is the ripple. The whole word
    // settles in the scan that clocked it and can be read as soon as process() returns.
    part def Counter4 {
        in port clk : ~BoolPort;
        out port b0 : BoolPort;
        out port b1 : BoolPort;
        out port b2 : BoolPort;
        out port b3 : BoolPort;

        part t0 : T_FF_AnyEdge;
        part f0 : F_TRIG;
        part t1 : T_FF;
        part f1 : F_TRIG;
        part t2 : T_FF;
        part f2 : F_TRIG;
        part t3 : T_FF;

        connect t0.q to f0.set;
        connect f0.q to t1.clk;
        connect t1.q to f1.set;
        connect f1.q to t2.clk;
        connect t2.q to f2.set;
        connect f2.q to t3.clk;

        // The counter's own ports are the ports of the bits: nothing forwards anything.
        bind clk = t0.clk;
        bind b0 = t0.q;
        bind b1 = t1.q;
        bind t2.q = b2;
        bind b3 = t3.q;
    }

    // Four D flip-flops in a row, all clocked together: what serIn holds at the edge appears
    // at q0, what q0 held appears at q1, and so on.
    //
    // The stages capture on either edge of clk, so a clock that changes level every scan
    // shifts every scan. With plain D_FFs it would take two - one scan to raise the clock and
    // one to drop it again before the next rising edge.
    //
    part def ShiftReg4 {
        in port serIn : ~BoolPort;
        in port clk : ~BoolPort;
        out port q0 : BoolPort;
        out port q1 : BoolPort;
        out port q2 : BoolPort;
        out port q3 : BoolPort;

        part fan : BoolFanOut4;

        part s3 : D_FF_AnyEdge;
        part s2 : D_FF_AnyEdge;
        part s1 : D_FF_AnyEdge;
        part s0 : D_FF_AnyEdge;

        connect fan.o0 to s0.clk;
        connect fan.o1 to s1.clk;
        connect fan.o2 to s2.clk;
        connect fan.o3 to s3.clk;

        connect s0.q to s1.d;
        connect s1.q to s2.d;
        connect s2.q to s3.d;

        bind clk = fan.i;
        bind serIn = s0.d;
        bind q0 = s0.q;
        bind q1 = s1.q;
        bind q2 = s2.q;
        bind q3 = s3.q;
    }

    // A 2 to 4 line decoder. a1 a0 select which of the four outputs
    // goes active, and the enable turns all of them off.
    //
    part def Decoder2to4 {
        in port e : ~BoolPort;
        in port a0 : ~BoolPort;
        in port a1 : ~BoolPort;
        out port y0 : BoolPort;
        out port y1 : BoolPort;
        out port y2 : BoolPort;
        out port y3 : BoolPort;

        part fanA0 : BoolFanOut4;
        part fanA1 : BoolFanOut4;

        part notE : BoolNot;
        part notA0 : BoolNot;
        part notA1 : BoolNot;

        part and0 : BoolAnd3;
        part and1 : BoolAnd3;
        part and2 : BoolAnd3;
        part and3 : BoolAnd3;

        part inv0 : BoolNot;
        part inv1 : BoolNot;
        part inv2 : BoolNot;
        part inv3 : BoolNot;

        connect fanA0.o0 to notA0.i;
        connect fanA1.o0 to notA1.i;

        connect notE.q to and0.i1;
        connect notA1.q to and0.i2;
        connect notA0.q to and0.i3;

        connect notE.q to and1.i1;
        connect notA1.q to and1.i2;
        connect fanA0.o1 to and1.i3;

        connect notE.q to and2.i1;
        connect fanA1.o1 to and2.i2;
        connect notA0.q to and2.i3;

        connect notE.q to and3.i1;
        connect fanA1.o2 to and3.i2;
        connect fanA0.o2 to and3.i3;

        connect and0.q to inv0.i;
        connect and1.q to inv1.i;
        connect and2.q to inv2.i;
        connect and3.q to inv3.i;

        bind e = notE.i;
        bind a0 = fanA0.i;
        bind a1 = fanA1.i;
        bind y0 = inv0.q;
        bind y1 = inv1.q;
        bind y2 = inv2.q;
        bind y3 = inv3.q;
    }

    // An SR latch, memory made of nothing but two gates.
    //
    part def SRLatch {
        in port nS : ~BoolPort;
        in port nR : ~BoolPort;
        out port q : BoolPort;
        out port nQ : BoolPort;

        part nandQ : BoolNand2;
        part nandNQ : BoolNand2;

        connect nandQ.q to nandNQ.i2;
        connect nandNQ.q to nandQ.i2;

        bind nS = nandQ.i1;
        bind nR = nandNQ.i1;
        bind q = nandQ.q;
        bind nQ = nandNQ.q;
    }

    // Sum and carry of a + b + cin, from five gates.
    //
    //     sum  = a xor b xor cin
    //     cout = (a and b) or (cin and (a xor b))
    //
    // a, b and cin each feed two gates, so each arrives at a fan-out first; the half sum
    // (a xor b) is wanted twice as well but needs no such thing, being an out port already.
    part def FullAdder {
        in port a : ~BoolPort;
        in port b : ~BoolPort;
        in port cin : ~BoolPort;
        out port sum : BoolPort;
        out port cout : BoolPort;

        part fanA : BoolFanOut4;
        part fanB : BoolFanOut4;
        part fanCin : BoolFanOut4;

        part xor1 : BoolXor2;
        part and1 : BoolAnd2;
        part xor2 : BoolXor2;
        part and2 : BoolAnd2;
        part or1 : BoolOr2;

        connect fanA.o0 to xor1.i1;
        connect fanB.o0 to xor1.i2;

        connect fanA.o1 to and1.i1;
        connect fanB.o1 to and1.i2;

        connect xor1.q to xor2.i1;
        connect fanCin.o0 to xor2.i2;

        connect fanCin.o1 to and2.i1;
        connect xor1.q to and2.i2;

        connect and1.q to or1.i1;
        connect and2.q to or1.i2;

        bind a = fanA.i;
        bind b = fanB.i;
        bind cin = fanCin.i;
        bind sum = xor2.q;
        bind cout = or1.q;
    }

    // Four full adders with the carry rippling from one to the next.
    //
    // Declared along the carry, like Counter4 and for the same reason - the whole sum then
    // settles in the scan that changed an input, rather than a bit per scan.
    part def Adder4 {
        in port a0 : ~BoolPort;
        in port a1 : ~BoolPort;
        in port a2 : ~BoolPort;
        in port a3 : ~BoolPort;
        in port b0 : ~BoolPort;
        in port b1 : ~BoolPort;
        in port b2 : ~BoolPort;
        in port b3 : ~BoolPort;
        in port cin : ~BoolPort;
        out port s0 : BoolPort;
        out port s1 : BoolPort;
        out port s2 : BoolPort;
        out port s3 : BoolPort;
        out port cout : BoolPort;

        part fa0 : FullAdder;
        part fa1 : FullAdder;
        part fa2 : FullAdder;
        part fa3 : FullAdder;

        connect fa0.cout to fa1.cin;
        connect fa1.cout to fa2.cin;
        connect fa2.cout to fa3.cin;

        bind a0 = fa0.a;
        bind b0 = fa0.b;
        bind cin = fa0.cin;
        bind a1 = fa1.a;
        bind b1 = fa1.b;
        bind a2 = fa2.a;
        bind b2 = fa2.b;
        bind a3 = fa3.a;
        bind b3 = fa3.b;
        bind s0 = fa0.sum;
        bind s1 = fa1.sum;
        bind s2 = fa2.sum;
        bind s3 = fa3.sum;
        bind cout = fa3.cout;
    }
}

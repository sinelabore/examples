/*
 * (c) 2026 Sinelabore Software Tools GmbH. All rights reserved.
 */
private import ScalarValues::*;
private import SI::*;
private import LogicBasic::*;

// Some basic logic blocks typically used in PLCs
package IEC61131_3 {



	// combinational

	part def BoolAnd2 {
		in port i1 : ~BoolPort;
		in port i2 : ~BoolPort;
		out port q : BoolPort;

		attribute v1 : Boolean default false;
		attribute v2 : Boolean default false;

		action exec {
			action takeI1 accept s1 : BoolSignal via i1 {
				assign v1 := s1.v;
			}
			then action takeI2 accept s2 : BoolSignal via i2 {
				assign v2 := s2.v;
			}
			then send new BoolSignal(v1 and v2) via q;
		}

		state And2 {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// 74HC00. Not-and: the one gate every other gate can be built from.
	part def BoolNand2 {
		in port i1 : ~BoolPort;
		in port i2 : ~BoolPort;
		out port q : BoolPort;

		attribute v1 : Boolean default false;
		attribute v2 : Boolean default false;

		action exec {
			action takeI1 accept s1 : BoolSignal via i1 {
				assign v1 := s1.v;
			}
			then action takeI2 accept s2 : BoolSignal via i2 {
				assign v2 := s2.v;
			}
			then send new BoolSignal(not (v1 and v2)) via q;
		}

		state Nand2 {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// 74HC32.
	part def BoolOr2 {
		in port i1 : ~BoolPort;
		in port i2 : ~BoolPort;
		out port q : BoolPort;

		attribute v1 : Boolean default false;
		attribute v2 : Boolean default false;

		action exec {
			action takeI1 accept s1 : BoolSignal via i1 {
				assign v1 := s1.v;
			}
			then action takeI2 accept s2 : BoolSignal via i2 {
				assign v2 := s2.v;
			}
			then send new BoolSignal(v1 or v2) via q;
		}

		state Or2 {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// 74HC86. True when the two inputs differ - the sum bit of an addition, and the test for
	// "has this changed" that the edge detectors make with an attribute instead.
	part def BoolXor2 {
		in port i1 : ~BoolPort;
		in port i2 : ~BoolPort;
		out port q : BoolPort;

		attribute v1 : Boolean default false;
		attribute v2 : Boolean default false;

		action exec {
			action takeI1 accept s1 : BoolSignal via i1 {
				assign v1 := s1.v;
			}
			then action takeI2 accept s2 : BoolSignal via i2 {
				assign v2 := s2.v;
			}
			then send new BoolSignal(v1 != v2) via q;
		}

		state Xor2 {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	part def BoolAnd3 {
		in port i1 : ~BoolPort;
		in port i2 : ~BoolPort;
		in port i3 : ~BoolPort;
		out port q : BoolPort;

		attribute v1 : Boolean default false;
		attribute v2 : Boolean default false;
		attribute v3 : Boolean default false;

		action exec {
			action takeI1 accept s1 : BoolSignal via i1 {
				assign v1 := s1.v;
			}
			then action takeI2 accept s2 : BoolSignal via i2 {
				assign v2 := s2.v;
			}
			then action takeI3 accept s3 : BoolSignal via i3 {
				assign v3 := s3.v;
			}
			then send new BoolSignal(v1 and v2 and v3) via q;
		}

		state And3 {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	part def BoolNot {
		in port i : ~BoolPort;
		out port q : BoolPort;

		attribute v1 : Boolean default false;

		action exec {
			action takeI1 accept s1 : BoolSignal via i {
				assign v1 := s1.v;
			}
			then send new BoolSignal(not v1) via q;
		}

		state Not {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// edge triggers

	// one cycle of q on the rising edge of set
	part def R_TRIG {
		in port set : ~BoolPort;
		out port q : BoolPort;

		attribute clk : Boolean default false;
		attribute old : Boolean default false;

		action exec {
			action takeSet accept s : BoolSignal via set {
				assign clk:= s.v;
			}
			if (old==false) and (clk==true){
				then send new BoolSignal(true) via q;
			}else{
				then send new BoolSignal(false) via q;
			}
			then assign old := clk;
		}

		state RTrig {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// one cycle of q on the falling edge of set
	part def F_TRIG {
		in port set : ~BoolPort;
		out port q : BoolPort;

		attribute clk : Boolean default false;
		attribute old : Boolean default false;

		action exec {
			action takeSet accept s : BoolSignal via set {
				assign clk:= s.v;
			}
			if (old==true) and (clk==false){
				then send new BoolSignal(true) via q;
			}else{
				then send new BoolSignal(false) via q;
			}
			then assign old := clk;
		}

		state FTrig {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// timers

	// q follows set, but only after set has been true for onDelay
	part def TON {
		in port set : ~BoolPort;
		out port q : BoolPort;

		attribute onDelay : Real default 1.0;

		attribute sv : Boolean default false;

		action takeSet {
			action a accept s : BoolSignal via set {
				assign sv := s.v;
			}
		}

		action emit {in qVal:Boolean; send new BoolSignal(qVal) via q; }


		state TONSM {
			do takeSet;
			entry; then Off;

			state Off {
				entry action emit{in qVal=false;}
			}
			if sv==true then Waiting;

			state Waiting{
				entry action emit{in qVal=false;}
			}
			accept after onDelay [SI::second] then On;
			if sv==false then Off;

			state On {
				entry action emit{in qVal=true;}
			}
			if sv==false then Off;
		}
	}

	// q follows set, but stays true for offDelay after set went false
	part def TOF {
		in port set : ~BoolPort;
		out port q : BoolPort;

		attribute offDelay : Real default 1.0;

		attribute sv : Boolean default false;

		action takeSet {
			action a accept s : BoolSignal via set {
				assign sv := s.v;
			}
		}

		action emit {in qVal:Boolean; send new BoolSignal(qVal) via q; }

		state TOFSM {
			do takeSet;
			entry; then Off;

			state Off {
				entry action emit{in qVal=false;}
			}
			if sv==true then On;

			state On {
				do action emit{in qVal=true;}
			}
			if sv==false then Holding;

			// q is still true here; it goes false when the delay is up
			state Holding{
				do action emit{in qVal=true;}
			}
			accept after offDelay [SI::second] then Off;
			if sv==true then On;
		}
	}

	// counters

	// counts rising edges up on cu and down on cd; reset clears the count.
	// pv is the preset the count is compared against - a parameter, so an attribute.
	// Up/down counter, IEC 61131-3.
	//
	//   reset sets cv to 0, load sets it to pv; reset wins over load, and both win over
	//   counting. A rising edge on cu counts up, one on cd counts down; edges on both in
	//   the same scan count neither. cv stays inside [cvMin, cvMax], the range the
	//   standard counts in, so it never wraps.
	part def CTUD {
		in port cu : ~BoolPort; // cu increments the counter on a rising edge
		in port cd : ~BoolPort; // cd decrements it on a rising edge
		in port reset : ~BoolPort; // R: cv := 0
		in port load : ~BoolPort; // LD: cv := pv

		out port qu : BoolPort; // reached upper limit, CV >= PV
		out port qd : BoolPort; // reached lower limit, CV <= 0

		attribute pv : Integer default 0; // preset value
		attribute cv : Integer default 0; // current counter value

		// the range cv is held in, PVmax and PVmin of the standard
		attribute cvMax : Integer default 32767;
		attribute cvMin : Integer default -32768;

		attribute cuv : Boolean default false;
		attribute oldCu : Boolean default false;
		attribute cdv : Boolean default false;
		attribute oldCd : Boolean default false;
		attribute rv : Boolean default false;
		attribute ldv : Boolean default false;

		// the edges of this scan, so the rules below read as the standard writes them
		attribute cuEdge : Boolean default false;
		attribute cdEdge : Boolean default false;

		action exec {
			action takeCu accept su : BoolSignal via cu {
				assign cuv := su.v;
			}
			then action takeCd accept sd : BoolSignal via cd {
				assign cdv := sd.v;
			}
			then action takeReset accept sr : BoolSignal via reset {
				assign rv := sr.v;
			}
			then action takeLoad accept sl : BoolSignal via load {
				assign ldv := sl.v;
			}

			then assign cuEdge := (oldCu==false) and (cuv==true);
			then assign cdEdge := (oldCd==false) and (cdv==true);

			then if rv==true {
				assign cv := 0;
			}else if ldv==true {
				assign cv := pv;
			}else if (cuEdge==true) and (cdEdge==false) and (cv<cvMax) {
				assign cv := cv + 1;
			}else if (cdEdge==true) and (cuEdge==false) and (cv>cvMin) {
				assign cv := cv - 1;
			}

			// Remember inputs for next scan
			then assign oldCu := cuv;
			then assign oldCd := cdv;

			// Outputs
			then send new BoolSignal(cv >= pv) via qu;
			then send new BoolSignal(cv <= 0) via qd;
		}

		state Ctud {
			do exec;
			entry; then Running;
			state Running;
		}
	}
}

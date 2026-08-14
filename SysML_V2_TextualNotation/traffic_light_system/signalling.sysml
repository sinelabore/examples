private import ScalarValues::*;

package Signalling {

    part def Lamp{
        attribute power:Real default 2.2 [SI::W];
        attribute switchCounter:Natural default 0;
        attribute lampStatus:Boolean;

        action setOn{
            assign switchCounter:=switchCounter+1;
            assign lampStatus:=true;
        }

        action setOff{
            assign lampStatus:=false;
        }
    }
}
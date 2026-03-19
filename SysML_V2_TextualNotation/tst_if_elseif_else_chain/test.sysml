private import ScalarValues::*;
package MyPackage {

    part def BatterySystem{
	
        attribute batLevel:Integer default 0;
        attribute n:Integer;
        attribute m:Integer;
        
        action def setM{in x:Integer;
            assign m:=x;
        }

        action def ChargeBattery {

            first start;
            then if batLevel<4{
                attribute a:Integer default 33;
                then assign n:=a;
                then action s:setM{in x=n;}
            }else if batLevel>=5 and batLevel<90{
                attribute a:Integer default 66;
                then  assign n:=a;
                then action s:setM{in x=a;}
            }else if batLevel>=90 and batLevel<95{
                attribute a:Integer default 99;
                then assign n:=a;
                then action s:setM{in x=a;}
            }else {
                attribute a:Integer default 100;
                then assign n:=a;
                then action s:setM{in x=n;}
            }
        }
    }
}
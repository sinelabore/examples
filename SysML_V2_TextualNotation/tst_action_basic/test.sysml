private import ScalarValues::*;

package Test { 

    part def AParent{
        attribute anAttributeOfAParent:Integer;
    }

    part def A specializes AParent {
        attribute anAttributeOfA : Integer;

        action def ParentAction{}
        
        action def ParentWithPara{
            attribute test:Integer;
            in x:Real;
            out p:Real;

            assign p := x*x;
            assign anAttributeOfA := p;
            assign anAttributeOfAParent := p;
        
        }



        action def run{
            in n:Real;
            out p:Real;
            out u:Real;

            attribute x : Real;

            action act1:ParentAction;
            action act2:ParentWithPara;
            action act3{in n:Real; out u:Real;
                assign u:=8;
            }
            
            action Path1{}
            then terminate;

            action Path2;
            then terminate;

            first start;
            then assign x := n;
            then act1;
            then assign x:=2;
            then action act1_1;
            then act2{in x:Real;out p:Real;}
            then assign p:=act2.p;
            then act3{in n:Real;out u:Real;}
            then assign u:=act3.u;

            then decide d1;
            if n==1 then Path1;
            if act2.p==4 then Path2;
            else done;
        }
    }
} 
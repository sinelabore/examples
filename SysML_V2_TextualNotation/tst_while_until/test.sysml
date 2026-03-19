private import ScalarValues::*;
package MyPackage {

    part def A{
        attribute whileGuard:Integer default 1;
        attribute untilGuard:Boolean default true;

        action def SecondAction{
            assign whileGuard := whileGuard +1;
            if whileGuard == 5{
                assign untilGuard := false;
            }
        }

        action def WhileUntilAction {
            first start;
            then while whileGuard >= 1 {
                action secondAction:SecondAction{}

                first start;
                then secondAction;
            }until untilGuard == false;
            
            then done;
        }
    }
}
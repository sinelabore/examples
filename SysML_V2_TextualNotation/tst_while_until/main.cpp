#include <system.h>
#include <iostream>

// ========== main ========== //
int main() {
    // Create your custom system
    MyPackage::A a;

    a.init();

    MyPackage::A::WhileUntilActionDef wua;   
    std::cout << "whileGuard: " << a.whileGuard << std::endl;
    std::cout << "untilGuard: " << a.untilGuard << std::endl;
    std::cout << "--------" << std::endl;

    wua(a);
    std::cout << "whileGuard: " << a.whileGuard << std::endl;
    std::cout << "untilGuard: " << a.untilGuard << std::endl;

    
    return 0;
}

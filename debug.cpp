// File for normal debugging
#include <iostream>
#include "Lib/debug.hpp"

int main () {
        
        debug::print("\n\nDebug\n\n");
        debug::print("Messages\n\n");

        return 0;
}

void debug::print(std::string msg) {
        std::cout << msg;
}

void debug::print(float msg) {
        std::cout << msg << "\n";
}

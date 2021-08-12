#include "ControllerCoreForString.h"
#include <algorithm>

bool ControllerCoreForString::randomiseTree(int size)
{
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    
    for (int i = 0; i < size; i++) {
        std::string str(size, 0);
        std::generate_n(str.begin(), size, randchar);
        _tree->add(str);
    }

    return true;
}

std::string ControllerCoreForString::processGetValueFromStr(std::string str)
{
    return str;
}

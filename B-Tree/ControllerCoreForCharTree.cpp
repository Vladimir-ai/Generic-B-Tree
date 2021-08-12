#include "ControllerCoreForCharTree.h"
#include <boost/algorithm/string/trim.hpp>

bool ControllerCoreForCharTree::randomiseTree(int size)
{
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    const size_t charset_size = sizeof(charset);

    for (int i = 0; i < size; i++) {
        _tree->add(charset[rand() % charset_size]);
    }
    return true;
}

char ControllerCoreForCharTree::processGetValueFromStr(std::string str)
{
    boost::trim(str);
    return str.front();
}

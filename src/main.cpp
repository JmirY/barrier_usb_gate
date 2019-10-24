// main.cpp
// JMIRY

#include "App.h"

#include <iostream>

int main()
{
    int result = 0;
    App app = App();

    // run actual app
    result = app.run();

    if (result == 0)
        std::cout << "[DEBUG] Program ended properly" << std::endl;
    else
        std::cout << "[ERR] Something goes wrong" << std::endl;
    return result;
}
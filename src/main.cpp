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

    std::cout << "[DEBUG] Program ended properly" << std::endl;
    return result;
}
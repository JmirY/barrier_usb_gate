// App.cpp
// JMIRY

#include "App.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>

int
App::run()
{
    // decide which class to create
    eBarrierProcess result;
    result = detectProcess();

    // create designated Gate class
    Gate* barrierGate = NULL;
    switch (result)
    {
    case kIsServer:
        barrierGate = new ServerSideGate();
        barrierGate = dynamic_cast<ServerSideGate*>(barrierGate);
        break;
    case kIsClient:
        barrierGate = new ClientSideGate();
        barrierGate = dynamic_cast<ClientSideGate*>(barrierGate);
        break;
    case kNoBarrier:
        std::cout << "[DEBUG] No barrier process" << std::endl;
        return -1;
    }

    // init Gate class
    int status = 0;
    status = barrierGate->init();
    if (status != 0)
    {
        std::cout << "[ERR] Gate initialization failed" << std::endl;
        return -1;
    }

    // create socket & fill address info
    barrierGate->fillAddrInfo();
    status = barrierGate->createSocket();
    if (status != 0)
    {
        std::cout << "[ERR] Socket creation failed" << std::endl;
        return -1;
    }

    // connect to barrier socket
    status = barrierGate->connectSocket();
    if (status != 0)
    {
        std::cout << "[ERR] Socket connection failed" << std::endl;
        return -1;
    }

    // create EventQueue class then start socket/USB polling thread
    std::thread tPollSock(
        &Gate::pollSocket,
        barrierGate
    );
    std::thread tPollUSB(
        &Gate::pollUSB,
        barrierGate
    );
    tPollSock.join();
    tPollUSB.join();

    // flush usb buffer
    barrierGate->flush();

    // end program
    delete barrierGate;
    return 0;
}

App::eBarrierProcess
App::detectProcess()
{
    // use prompt command & redirect output to text file
    system("tasklist | findstr barrier 1> tmp.txt");

    // get output from redirected file
    char buffer[BUF_LEN] = { 0 };
    std::ifstream outputFile;
    outputFile.open("tmp.txt");
    if ( !outputFile.is_open() )
    {
        std::cout << "[ERROR] file is not opened well" << std::endl;
        return kNoBarrier;
    }

    eBarrierProcess result = kNoBarrier;
    char *procName = NULL, *context = NULL;
    outputFile.getline(buffer, BUF_LEN);
    while (outputFile.gcount() > 0)
    {
        procName = strtok_s(buffer, " ", &context);
        if (!strcmp(procName, "barriers.exe"))
        {
            std::cout << "[DEBUG] Server process detected" << std::endl;
            result = kIsServer;
            break;
        }
        else if (!strcmp(procName, "barrierc.exe"))
        {
            std::cout << "[DEBUG] Client process detected" << std::endl;
            result = kIsClient;
            break;
        }
        outputFile.getline(buffer, BUF_LEN);
    }

    // close stream & delete redirected file
    outputFile.close();
    system("del tmp.txt");
    return result;
}
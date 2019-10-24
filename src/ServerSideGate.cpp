// ServerSideGate.cpp
// JMIRY

#include "ServerSideGate.h"

#include <iostream>

int
ServerSideGate::connectSocket()
{
    int status = 0;
    status = connect(
        m_sock,
        (const sockaddr*)&m_addrInfo,
        sizeof(struct sockaddr_in)
    );
    if (status == SOCKET_ERROR)
    {
        std::cout << "[ERR] connect() failed, errcode: "
                  << WSAGetLastError() << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] Connection established" << std::endl;
    return 0;
}

int
ServerSideGate::sendTransfer(unsigned char* buf, int bufSize)
{
    // send sync bulk transfer
    std::cout << "[DEBUG] Sending..." << std::endl;

    int status = 0, bytesWrote = 0;
    // timeout parameter is HARDCODED
    status = libusb_bulk_transfer(
        m_usbDevHandle,
        EP_OUT,
        buf,
        bufSize,
        &bytesWrote,
        1000);
    if (status != 0)
    {
        std::cout << "[ERR] USB sending failed:  "
            << libusb_strerror(libusb_error(status)) << std::endl;
    }
    else
    {
        std::cout << "[DEBUG] Transfer sent: "
            << bytesWrote << "bytes" << std::endl;
    }
    return bytesWrote;
}

int
ServerSideGate::receiveTransfer(unsigned char* buf, int bufSize)
{
    // receive sync bulk transfer
    std::cout << "[DEBUG] Receiving..." << std::endl;

    int status = 0, bytesRead = 0;
    // timeout parameter is HARDCODED
    status = libusb_bulk_transfer(
        m_usbDevHandle,
        EP_IN,
        buf,
        bufSize,
        &bytesRead,
        1000);
    if (status != 0)
    {
        std::cout << "[ERR] USB receiving failed:  "
            << libusb_strerror(libusb_error(status)) << std::endl;
    }
    else
    {
        std::cout << "[DEBUG] Transfer received: "
            << bytesRead << "bytes" << std::endl;
    }
    return bytesRead;
}
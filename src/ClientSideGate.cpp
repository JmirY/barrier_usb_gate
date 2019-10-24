// ClientSideGate.cpp
// JMIRY

#include "ClientSideGate.h"

#include <iostream>

int
ClientSideGate::connectSocket()
{
    // bind socket
    int status = 0;
    status = bind(
        m_sock,
        (const sockaddr*)&m_addrInfo,
        sizeof(struct sockaddr_in)
    );
    if (status == SOCKET_ERROR)
    {
        std::cout << "[ERR] bind() error, errcode: "
                  << WSAGetLastError() << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] Socket binded" << std::endl;

    // make socket listen to barrierc.exe
    status = listen(m_sock, 1);
    if (status == SOCKET_ERROR)
    {
        std::cout << "[ERR] listen() error, errcode: "
                  << WSAGetLastError() << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] Socket listening..." << std::endl;

    // accept connection
    memset(&m_addrInfo, 0, sizeof(struct sockaddr_in));
    int addrlen = sizeof(struct sockaddr_in);
    m_sock = accept(
        m_sock,
        (struct sockaddr*)&m_addrInfo,
        &addrlen
    );
    if (m_sock == INVALID_SOCKET)
    {
        std::cout << "[ERR] accept() error" << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] Socket accepted" << std::endl;

    return 0;
}

int
ClientSideGate::sendTransfer(unsigned char* buf, int bufSize)
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
ClientSideGate::receiveTransfer(unsigned char* buf, int bufSize)
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
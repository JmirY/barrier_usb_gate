// Gate.cpp
// JMIRY

#include "Gate.h"

#include <iostream>
#include <chrono>

// define static member variable
bool Gate::m_isEnd = false;

Gate::Gate()
{
    // set event handler
    if ( !SetConsoleCtrlHandler(eventHandler, TRUE) )
    {
        std::cout << "[WARN] EventHandler registration failed" << std::endl;
    }
}


Gate::~Gate()
{
    // cleanup libusb
    if (m_usbDevHandle != NULL)
        libusb_close(m_usbDevHandle);
    if (m_usbContext != NULL)
        libusb_exit(m_usbContext);

    // cleanup winsock
    WSACleanup();
}

int
Gate::init()
{
    // init winsock2
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        std::cout << "[ERR] WSAStartup() error" << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] WinSock2 initiated" << std::endl;

    // init libusb context
    int status = 0;
    status = libusb_init(&m_usbContext);
    if (status != 0)
    {
        std::cout << "[ERR] libusb_init() failed: "
            << libusb_strerror(libusb_error(status)) << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] libusb initiated" << std::endl;

    // get device list & find target
    libusb_device** devList;
    int cnt = 0;
    cnt = libusb_get_device_list(m_usbContext, &devList);

    for (int i = 0; i < cnt; ++i)
    {
        if (isTarget(devList[i]))
        {
            // grab device handle of target
            status = libusb_open(devList[i], &m_usbDevHandle);
            if (status != 0)
            {
                std::cout << "[ERR] libusb_open() failed: "
                    << libusb_strerror(libusb_error(status));
                return -1;
            }
            else
            {
                std::cout << "[DEBUG] Device handle opened" << std::endl;
                break;
            }
        }
    }

    // free usb device list
    libusb_free_device_list(devList, 1);

    // claim data transfer interface
    // interface number is HARDCODED
    status = libusb_claim_interface(m_usbDevHandle, 5);
    if (status != 0)
    {
        std::cout << "[ERR] libusb_claim_interface() failed:  "
            << libusb_strerror(libusb_error(status)) << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] Interface claimed" << std::endl;

    return 0;
}

bool
Gate::isTarget(libusb_device* dev)
{
    libusb_device_descriptor devDesc;

    libusb_get_device_descriptor(dev, &devDesc);
    // product / vendor ID is HARDCODED
    if (devDesc.idVendor == 0xea0 && devDesc.idProduct == 0x7301)
        return true;
    else
        return false;
}

int
Gate::createSocket()
{
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET)
    {
        std::cout << "[ERR] socket() failed" << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] Socket created" << std::endl;
    return 0;
}

int
Gate::sendSocket(char* buf, int len)
{
    int result = 0;
    result = send(m_sock, buf, len, 0);
    if (result == SOCKET_ERROR)
    {
        std::cout << "[ERR] send() failed" << std::endl;
    }
    else
    {
        std::cout << "[DEBUG] Socket sent " 
                  << result << "bytes" << std::endl;
    }

    return result;
}

int
Gate::recvSocket(char* buf, int len)
{
    int result = 0;
    result = recv(m_sock, buf, len, 0);

    if (result == SOCKET_ERROR)
    {
        std::cout << "[ERR] recv() failed" << std::endl;
    }
    else
    {
        std::cout << "[DEBUG] Socket received "
                  << result << "bytes" << std::endl;
    }
    
    return result;
}

void
Gate::fillAddrInfo()
{
    memset(&m_addrInfo, 0, sizeof(struct sockaddr_in));
    m_addrInfo.sin_family = AF_INET;
    m_addrInfo.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    m_addrInfo.sin_port = htons(PORT);
}

void
Gate::pollUSB()
{
    int result = 0;
    char buffer[BUF_LEN] = { 0 };
    while(!m_isEnd)
    {
        result = receiveTransfer(
            (unsigned char*)buffer,
            BUF_LEN
        );

        if (result > 0)
        {
            sendSocket(buffer, result);
            memset(buffer, 0, BUF_LEN);
        }
    }
}

void
Gate::pollSocket()
{
    int result = 0;
    char buffer[BUF_LEN] = { 0 };
    while (!m_isEnd)
    {
        result = recvSocket(buffer, BUF_LEN);

        if (result > 0)
        {
            sendTransfer((unsigned char*)buffer, result);
            memset(buffer, 0, result);
        }
        else if (result == 0)
        {
            std::cout << "[DEBUG] Disconnected gracefully" << std::endl;
            m_isEnd = true;
        }
    }
}

void
Gate::flush()
{
    unsigned char buf[BUF_LEN];
    while (receiveTransfer(buf, BUF_LEN));
}

BOOL WINAPI
Gate::eventHandler(DWORD event)
{
    switch (event)
    {
    case CTRL_C_EVENT:
        std::cout << "[DEBUG] Keyboard Interrupt detected" << std::endl;
        m_isEnd = true;
        return TRUE;
    default:
        return FALSE;
    }
}
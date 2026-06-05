

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <string>
#include "SerialPort.h"

static HANDLE handle = INVALID_HANDLE_VALUE;

int OpenSerialPort(const char* portName)
{
    handle = CreateFileA(
        portName,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (handle == INVALID_HANDLE_VALUE)
        return 0;

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(handle, &dcb))
        return 0;

    dcb.BaudRate = CBR_9600;   // Must match Serial.begin(115200)
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;

    if (!SetCommState(handle, &dcb))
        return 0;

    COMMTIMEOUTS timeouts = {0};

    timeouts.ReadIntervalTimeout = 1
    ;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;

    SetCommTimeouts(handle, &timeouts);

    return 1;
}

int ReadAngle()
{
    static std::string serialBuffer;

    char temp[64];
    DWORD bytesRead = 0;

    if(ReadFile(handle, temp, sizeof(temp) - 1, &bytesRead, NULL))
    {
        if(bytesRead > 0)
        {
            temp[bytesRead] = '\0';

            serialBuffer += temp;

            size_t pos = serialBuffer.find('\n');

            if(pos != std::string::npos)
            {
                std::string line = serialBuffer.substr(0, pos);

                serialBuffer.erase(0, pos + 1);

                return atoi(line.c_str());
            }
        }
    }

    return -1;
}

void CloseSerialPort()
{
    if (handle != INVALID_HANDLE_VALUE)
        CloseHandle(handle);
}
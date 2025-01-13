#include "Lib\serial.hpp"
#include <winbase.h>
#include <winuser.h>

bool Serial::Init() {
        hPort = CreateFile(port.c_str(),
                        GENERIC_READ,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);

        if (hPort == INVALID_HANDLE_VALUE) {
                return true;
        }

        if (Configure()) {
                return true;
        }

        if (!PurgeComm(hPort, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
                return true;
        }

        return false;
}

bool Serial::Configure() {
        // Initialize 
        DCB dcb;
        dcb = {0};
        dcb.DCBlength = sizeof(dcb);

        if (!GetCommState(hPort, &dcb)) {
                return true;
        }

        dcb.BaudRate = CBR_9600;
        dcb.ByteSize = 8;
        dcb.Parity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;

        if (!SetCommState(hPort, &dcb)) {
                return true;
        }

        // Communication timeouts
        COMMTIMEOUTS timeouts;

        if (!GetCommTimeouts(hPort, &timeouts)) {
                return true;
        }

        timeouts.ReadIntervalTimeout = MAXDWORD; // No wait between characters
        timeouts.ReadTotalTimeoutConstant = 0; // Do not wait for data
        timeouts.ReadTotalTimeoutMultiplier = 0;

        if (!SetCommTimeouts(hPort, &timeouts)) {
                return true;
        }

        return false;
}

bool Serial::Read() {
        if (hPort == INVALID_HANDLE_VALUE) {
                const std::string err = "Disconnected from " + std::string(port) + "."; 

                // Show error once
                if (!disconnectedShown) {
                        disconnectedShown = true;
                        // If user quits error message, then message can be shown again
                        disconnectedShown = MessageBox(NULL, err.c_str(), "Error", MB_ICONERROR) == IDOK ? false : true;
                } 

                Init();
                return true;
        }

        // Skip reading if no data in buffer
        if (isEmpty()) {
                return true;  
        }

        const DWORD max_bytes = 1024;
        DWORD bytes;
        char * msg = new char [max_bytes];
        
        if (!msg) {
                return true;
        }


        if(!ReadFile(hPort, msg, max_bytes, &bytes, NULL)) {
                delete [] msg;
                return true;
        }

        // Check if empty message was not sent
        if (bytes == 0 || msg[0] == '\0') {
                delete [] msg;
                return true;
        }
        
        if (bytes == max_bytes) {
                msg [max_bytes-1] = '\0';
        } else {
                msg [bytes] = '\0';
        }

        data += std::string(msg);

        if (checkFormat()) {
                delete [] msg;
                return true; 
        }

        delete [] msg;
        return false; 
}

bool Serial::checkFormat() {
        /*
         * Expected format #DATA\n
         * Return true if format is incorrect
         * Return false if format correct
         *
         * Data is intended to be complete after multiple reads if needed to be ready to be used and manipulated
         */
        
        // Store data only starting correctly
        size_t start = data.find("#");
        if (start == std::string::npos || data.empty()) {
                data = "";
                return true;
        }

        // If data doesnt end, then we leave program to be able to complete it later
        size_t end = data.find("\n");
        if (end == std::string::npos) {
                return true;
        }

        const size_t len = end - start;
        data = data.substr(start+1, len - 1);

        return false;
}

void Serial::Reset() {
        PurgeComm(hPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
        data = "";
}

bool Serial::isEmpty() { 
        // Checks if available bytes for reading
        
        COMSTAT stat;

        if (!ClearCommError(hPort, NULL, &stat)) {
                return true;
        }

        return stat.cbInQue == 0; 
}


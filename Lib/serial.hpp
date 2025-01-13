#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

// True  - Error
// False - Successful

class Serial { 
        private:
        HANDLE hPort = NULL;
        OVERLAPPED Overlapped;

        bool Configure();
        bool checkFormat();
        bool isEmpty();

        bool disconnectedShown = false;

        public: 
        std::string port = "COM5"; 
        std::string data = "";

        void Reset();
        bool Init();
        bool Read();
};

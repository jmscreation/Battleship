#include "message.h"
#include "windows.h"

void showMessage(const std::string& msg, const std::string& title, MSG_TYPE type){
    UINT mty = 0;
    switch(type){
        case MSG_NONE: break;
        case MSG_ERROR: mty = MB_ICONERROR; break;
        case MSG_WARN: mty = MB_ICONEXCLAMATION; break;
        case MSG_INFO: mty = MB_ICONINFORMATION; break;
    }

    MessageBox(NULL, msg.data(), title.data(), mty);
}

bool showQuestion(const std::string& msg, const std::string& title, MSG_TYPE type){
    UINT mty = 0;
    switch(type){
        case MSG_NONE: break;
        case MSG_ERROR: mty = MB_ICONERROR; break;
        case MSG_WARN: mty = MB_ICONEXCLAMATION; break;
        case MSG_INFO: mty = MB_ICONINFORMATION; break;
    }
    mty = mty | MB_YESNO;
    return (MessageBox(NULL, msg.data(), title.data(), mty) == IDYES);
}
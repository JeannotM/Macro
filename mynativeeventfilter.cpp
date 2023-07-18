#include "mynativeeventfilter.h"

bool MyNativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    if (eventType == "windows_generic_MSG")
    {
        MSG *msg = static_cast<MSG *>(message);
        if(msg->message == WM_HOTKEY)
        {
            emit buttonClick(eventType, message, result);
            return true;
        }

    }

    return false;
}

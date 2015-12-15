// Exception body

#include "exception.h"
#include "hge.h"

CException::CException(char* m)
{
    message = m;
}

void CException::Report()
{
    MessageBox(NULL, message, "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
}

// This is an exception class

#ifndef EXCEPTION
#define EXCEPTION

class CException
{
public:
    char* message;
    CException( char* m );
    void Report();
};

#endif

#ifndef CERROR_H
#define CERROR_H

class CError
{
public:
    CError(){}
    enum Error{
        ALLFILESSENT,
        NOCONFIGFILE,
        UNKNOWNDIR,
        NOPATIENT,
        INVALIDDATE,
        EMPTYFIELD,
        RENAME,
        NOMOREFILE,
        DELETEFILE,
        NETWORKPROBLEM,
        NOPATIENTLIST,
        NOTCONNECTED,
        NOCONFIGUREDNETWORK
    };
};

#endif

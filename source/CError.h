#ifndef CERROR_H
#define CERROR_H

class CError
{
public:
    CError(){}
    enum Error{
        NOCONFIGFILE,
        UNKNOWNDIR,
        NOPATIENT,
        INVALIDDATE,
        EMPTYFIELD,
        RENAME,
        NOMOREFILE,
        DELETEFILE,
        NOPATIENTLIST
    };
};

#endif

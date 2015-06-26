#include <QObject>
#include <QtPlugin>

#include "Hello.h"

class HelloPlugin : public QObject, public Hello
{
    Q_OBJECT
    Q_INTERFACES(Hello)

public:


};


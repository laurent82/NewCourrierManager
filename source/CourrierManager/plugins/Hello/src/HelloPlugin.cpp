#include "HelloPlugin.h"
#include "Hello.h"

HelloPlugin::HelloPlugin()
{

}

CAbstractFilter *HelloPlugin::createFilter()
{
    return new Hello("hello-plugin");
}

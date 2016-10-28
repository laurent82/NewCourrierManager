#include "CPluginLoader.h"


#include "CAbstractPlugin.h"
#include <memory>
#include <plugins_config.h>


#if CM_WITH_HELLO
    #include <HelloPlugin.h>
#endif

#if CM_WITH_CLEARLINE
    #include <ClearLinePlugin.h>
#endif

#if CM_WITH_OCR
    #include "Ocr/COcrManager.h"
    #include <COcrEngine.h>
#endif

CPluginLoader::CPluginLoader()
{
    init();
}

CPluginLoader::~CPluginLoader()
{

}

QList<CAbstractPlugin *> CPluginLoader::getList() const
{
    return m_list;
}

void CPluginLoader::init()
{

#if CM_WITH_HELLO
    HelloPlugin* helloPlugin = new HelloPlugin;
    m_list.append(helloPlugin->createFilter());
    delete helloPlugin;
#endif

#if CM_WITH_CLEANLINE
    ClearLinePlugin* linePlugin = new ClearLinePlugin;
    m_list.append(linePlugin->createFilter());
    delete linePlugin;
#endif

#if CM_WITH_OCR
    COcrEngine* engine = new COcrEngine(); 
    m_list.append( engine );
#endif
}


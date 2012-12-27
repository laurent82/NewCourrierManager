TARGET = NewCourrierManager
TEMPLATE = app
CONFIG += qt
QT += network
MOC_DIR += ../../build/moc/
UI_DIR = ../../build/ui
OBJECTS_DIR = ../../build/obj/
DESTDIR = ../../build/bin/

QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
#INCLUDEPATH += include/
#DEPENDPATH += src \
#        ui
RC_FILE = mainRes.rc
#ICON = iconmac.icns
HEADERS += CView.h \
    CControler.h \
    CDrawPanelWidget.h \
    CFastSearch.h \
    CError.h \
    CPatient.h \
    CFileManager.h \
    CConfigFrame.h \
    CDate.h \
    CNetwork.h
SOURCES += main.cpp \
    CView.cpp \
    CControler.cpp \
    CDrawPanelWidget.cpp \
    CFastSearch.cpp \
    CPatient.cpp \
    CFileManager.cpp \
    CConfigFrame.cpp \
    CDate.cpp \
    CNetwork.cpp

FORMS += courriermanager.ui

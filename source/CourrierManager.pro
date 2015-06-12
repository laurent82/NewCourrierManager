TARGET = NewCourrierManager
TEMPLATE = app
CONFIG += qt
QT += network
QT += printsupport
QT += widgets
MOC_DIR += ../build/moc/
UI_DIR = ../build/ui
OBJECTS_DIR = ../build/obj/
DESTDIR = ../build/bin/

unix:macx{
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
    #ICON = iconmac.icns
}

#        ui
RC_FILE = mainRes.rc
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

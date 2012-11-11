TARGET = NewCourrierManager
TEMPLATE = app
CONFIG += qt
MOC_DIR += ../build/moc/
UI_DIR = ../build/ui
OBJECTS_DIR = ../build/obj/
DESTDIR = ../../build/bin/
#INCLUDEPATH += include/
#DEPENDPATH += src \
#        ui
HEADERS += CView.h \
    CControler.h \
    CDrawPanelWidget.h \
    CFastSearch.h \
    CError.h \
    CPatient.h \
    CFileManager.h \
    CConfigFrame.h
SOURCES += main.cpp \
    CView.cpp \
    CControler.cpp \
    CDrawPanelWidget.cpp \
    CFastSearch.cpp \
    CPatient.cpp \
    CFileManager.cpp \
    CConfigFrame.cpp

FORMS += courriermanager.ui

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
    CGestion.h \
    CDrawPanelWidget.h \
    CFastSearch.h \
    CError.h
SOURCES += main.cpp \
    CView.cpp \
    CControler.cpp \
    CGestion.cpp \
    CDrawPanelWidget.cpp \
    CFastSearch.cpp

FORMS += courriermanager.ui

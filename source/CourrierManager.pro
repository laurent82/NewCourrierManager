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
HEADERS += CourrierManager.h \
    CGestion.h \
    CDrawPanel.h \
    CFastSearch.h
SOURCES += main.cpp \
    CourrierManager.cpp \
    CGestion.cpp \
    CDrawPanel.cpp \
    CFastSearch.cpp

FORMS += courriermanager.ui

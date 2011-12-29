
TARGET = NewCourrierManager
TEMPLATE = app
INCLUDEPATH += ./include
DEPENDPATH += src \
	ui
SOURCES += main.cpp \
    CourrierManager.cpp \
    CGestion.cpp \
    CDrawPanel.cpp \
    CFastSearch.cpp
HEADERS += CourrierManager.h \
    CGestion.h \
    CDrawPanel.h \
    CFastSearch.h
FORMS += courriermanager.ui

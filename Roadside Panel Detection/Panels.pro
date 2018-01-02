QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Panels
TEMPLATE = app
CONFIG += c++11
CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    panelfinder.cpp \
    textprocess.cpp \
    hunspell-lib/affentry.cxx \
    hunspell-lib/affixmgr.cxx \
    hunspell-lib/csutil.cxx \
    hunspell-lib/dictmgr.cxx \
    hunspell-lib/filemgr.cxx \
    hunspell-lib/hashmgr.cxx \
    hunspell-lib/hunspell.cxx \
    hunspell-lib/hunzip.cxx \
    hunspell-lib/phonet.cxx \
    hunspell-lib/replist.cxx \
    hunspell-lib/suggestmgr.cxx \
    hunspell-lib/utf_info.cxx

HEADERS  += mainwindow.h \
    panelfinder.h \
    textprocess.h \
    hunspell-lib/affentry.hxx \
    hunspell-lib/affixmgr.hxx \
    hunspell-lib/atypes.hxx \
    hunspell-lib/baseaffix.hxx \
    hunspell-lib/csutil.hxx \
    hunspell-lib/dictmgr.hxx \
    hunspell-lib/filemgr.hxx \
    hunspell-lib/hashmgr.hxx \
    hunspell-lib/htypes.hxx \
    hunspell-lib/hunspell.h \
    hunspell-lib/hunspell.hxx \
    hunspell-lib/hunvisapi.h \
    hunspell-lib/hunzip.hxx \
    hunspell-lib/langnum.hxx \
    hunspell-lib/phonet.hxx \
    hunspell-lib/replist.hxx \
    hunspell-lib/suggestmgr.hxx \
    hunspell-lib/w_char.hxx

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include/tesseract
INCLUDEPATH += /usr/local/include/hunspell
INCLUDEPATH += /usr/lib
INCLUDEPATH += /usr/local/lib
INCLUDEPATH += hunspell-lib/


LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_features2d -lopencv_objdetect
LIBS += -ltesseract
LIBS += -lhunspell-1.6

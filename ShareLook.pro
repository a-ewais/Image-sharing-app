#-------------------------------------------------
#
# Project created by QtCreator 2017-11-25T02:57:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShareLook
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        login.cpp \
    profile.cpp \
    imageinformation.cpp \
    peerprofile.cpp \
    requests.cpp \
    User.cpp \
    UDPSocket.cpp \
    UDPServerSocket.cpp \
    UDPClientSocket.cpp \
    ServiceDiscovery.cpp \
    ServerPeer.cpp \
    Server.cpp \
    Parameter.cpp \
    MessageDecoder.cpp \
    Message.cpp \
    DatabaseLinker.cpp \
    Client.cpp

HEADERS += \
        login.h \
    profile.h \
    imageinformation.h \
    peerprofile.h \
    requests.h \
    User.h \
    UDPSocket.h \
    UDPServerSocket.h \
    UDPClientSocket.h \
    ServiceDiscovery.h \
    ServerPeer.h \
    Server.h \
    Parameter.h \
    MessageDecoder.h \
    Message.h \
    DatabaseLinker.h \
    Client.h \
    Base64_Marshal.h

FORMS += \
        login.ui \
    profile.ui \
    imageinformation.ui \
    peerprofile.ui \
    requests.ui

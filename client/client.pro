TEMPLATE = app
TARGET = client

QT += widgets

LIBS += -L/usr/local/include -lraspicam

SOURCES += main.cpp\
            client.cpp\
            camera.cpp\

HEADERS += client.h\
            camera.h

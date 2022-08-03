TARGET = harbour-owncloud-daemon

CONFIG += qt c++11 ghostcloud_link
QT = dbus network xml

include($$PWD/../common/common.pri)

contains(CONFIG, clickphotobackup) {
    DEFINES += GHOSTCLOUD_UBUNTU_TOUCH_PHOTOBACKUP
}

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/filesystem.cpp \
    $$PWD/networkmonitor.cpp \
    $$PWD/dbushandler.cpp \
    $$PWD/uploader.cpp

HEADERS += \
    $$PWD/filesystem.h \
    $$PWD/networkmonitor.h \
    $$PWD/dbushandler.h \
    $$PWD/uploader.h

OTHER_FILES += harbour-owncloud-daemon.service

service.path = /usr/lib/systemd/user/
service.files += harbour-owncloud-daemon.service
target.path = /usr/bin/

INSTALLS += target service

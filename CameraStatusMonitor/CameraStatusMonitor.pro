#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T13:44:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraStatusMonitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DataModel/CustomTableModel.cpp \
    Camera/camera6467.cpp \
    Camera/cameraresult.cpp \
    Controller/camconnectcontrl.cpp \
    LogModel/glogmodel.cpp \
    EventCheck/BaseEventCheck.cpp \
    EventCheck/StatustCheck.cpp \
    EventCheck/UnlicensePlateCheck.cpp \
    AlarmModule/AlarmModule.cpp

HEADERS  += mainwindow.h \
    camstatusmodel.h \
    DataModel/CustomTableModel.h \
    Camera/camera6467.h \
    Camera/cameraresult.h \
    HvDeviceAPI/HvCameraType.h \
    HvDeviceAPI/HvDeviceCommon.h \
    HvDeviceAPI/HvDeviceEx.h \
    HvDeviceAPI/LinuxBmp.h \
    HvDeviceAPI/swerror.h \
    HvDeviceAPI/HVAPI_HANDLE_CONTEXT.h \
    HvDeviceAPI/HVAPI_HANDLE_CONTEXT_EX.h \
    HvDeviceAPI/HvAutoLink.h \
    HvDeviceAPI/HvAutoLinkDevice.h \
    HvDeviceAPI/HvCamera.h \
    HvDeviceAPI/HvDevice.h \
    HvDeviceAPI/HvDeviceBaseDef.h \
    HvDeviceAPI/HvDeviceBaseType.h \
    HvDeviceAPI/HvDeviceCommDef.h \
    HvDeviceAPI/HvDeviceDLL.h \
    HvDeviceAPI/HvDeviceNew.h \
    HvDeviceAPI/HvDeviceUtils.h \
    HvDeviceAPI/HvListenCMD.h \
    HvDeviceAPI/HvResultFilter.h \
    HvDeviceAPI/hvthreadex.h \
    HvDeviceAPI/hvvartypeex.h \
    HvDeviceAPI/SWBaseType.h \
    HvDeviceAPI/SWErrCode.h \
    Controller/camconnectcontrl.h \
    LogModel/glogmodel.h \
    EventCheck/BaseEventCheck.h \
    EventCheck/StatustCheck.h \
    EventCheck/UnlicensePlateCheck.h \
    AlarmModule/AlarmModule.h \
    data_commomdef.h

FORMS    += mainwindow.ui


LIBS += -L$$PWD/HvDeviceAPI -lHvDevice

INCLUDEPATH += $$PWD/HvDeviceAPI
DEPENDPATH += $$PWD/HvDeviceAPI

RESOURCES +=

VERSION = 1.0.0.1
QMAKE_TARGET_PRODUCT =
QMAKE_TARGET_COMPANY =
QMAKE_TARGET_DESCRIPTION =
QMAKE_TARGET_COPYRIGHT =
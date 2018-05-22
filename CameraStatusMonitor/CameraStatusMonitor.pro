#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T13:44:44
#
#-------------------------------------------------

QT       += core gui xml multimedia texttospeech

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraStatusMonitor
TEMPLATE = app

TRANSLATIONS = CamStatus_zh-cn.ts

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
    AlarmModule/AlarmModule.cpp \
    DataModel/custresultlist.cpp \
    UtilityFunction/utilityfunction.cpp \
    EventCheck/plateblacklistcheck.cpp \
    AlarmModule/textspeaker.cpp

HEADERS  += mainwindow.h \
    DataModel/CustomTableModel.h \
    Camera/camera6467.h \
    Camera/cameraresult.h \
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
    data_commomdef.h \
    DataModel/custresultlist.h \
    UtilityFunction/utilityfunction.h \
    EventCheck/plateblacklistcheck.h \
    AlarmModule/textspeaker.h

FORMS    += mainwindow.ui


LIBS += -L$$PWD/HvDeviceAPI -lHvDevice

INCLUDEPATH += $$PWD/HvDeviceAPI
DEPENDPATH += $$PWD/HvDeviceAPI

RESOURCES +=

RC_FILE = CameraStatusMonitor.rc

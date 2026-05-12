QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = QtMultiWindow

SOURCES += \
    main.cpp \
    LoginWindow.cpp \
    AdminDashboard.cpp \
    UserDashboard.cpp \
    SystemContext.cpp \
    ../Project/Admin.cpp \
    ../Project/Consumer.cpp \
    ../Project/FileManager.cpp \
    ../Project/Input.cpp \
    ../Project/LoginManager.cpp \
    ../Project/MonthlyRecords.cpp

HEADERS += \
    LoginWindow.h \
    AdminDashboard.h \
    UserDashboard.h \
    SystemContext.h \
    ../Project/Admin.h \
    ../Project/Consumer.h \
    ../Project/FileManager.h \
    ../Project/Input.h \
    ../Project/LoginManager.h \
    ../Project/MonthlyRecords.h

INCLUDEPATH += ../Project

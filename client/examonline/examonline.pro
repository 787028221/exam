#-------------------------------------------------
#
# Project created by QtCreator 2016-05-05T22:35:30
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = examonline
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    regstudent.cpp \
    regteacher.cpp \
    regadmin.cpp \
    studentpersonal.cpp \
    adminpersonal.cpp \
    teacherpersonal.cpp \
    socketcmd.cpp \
    tip.cpp \
    exam.cpp \
    tipbeginexam.cpp \
    scorepaper.cpp

HEADERS  += login.h \
    regstudent.h \
    regteacher.h \
    regadmin.h \
    studentpersonal.h \
    adminpersonal.h \
    teacherpersonal.h \
    socketcmd.h \
    tip.h \
    exam.h \
    tipbeginexam.h \
    scorepaper.h \
    scorepaper.h

FORMS    += login.ui \
    regStudent.ui \
    regteacher.ui \
    regadmin.ui \
    teacher.ui \
    historyscore.ui \
    serversetting.ui \
    readstudentparper.ui \
    paper.ui \
    studentpersonal.ui \
    exam.ui \
    tip.ui \
    adminpersonal.ui \
    teacherpersonal.ui \
    msg.ui \
    tipbegin.ui \
    scorepaper.ui

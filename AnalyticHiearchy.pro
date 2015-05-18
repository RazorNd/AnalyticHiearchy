#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T13:34:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.9
QMAKE_TARGET_COMPANY = RazorNd ltd.
QMAKE_TARGET_PRODUCT = Analytic Hiearchy

TARGET = AnalyticHiearchy
TEMPLATE = app


SOURCES += main.cpp \
    skewsymmetricmatrix.cpp \
    rating.cpp \
    abstractprioritizationmatrix.cpp \
    criteriamatrix.cpp \
    analytichiearchymodel.cpp \
    ratingeditdelegate.cpp \    
    alternativesmatrix.cpp \
    analytichiearchyview.cpp \
    prioritizationmatrixview.cpp \
    paramform.cpp \
    resultmatrix.cpp \
    resultview.cpp \
    analytichiearchymainwindow.cpp


HEADERS  += \
    analytichiearchymodel.h \
    skewsymmetricmatrix.h \
    rating.h \
    abstractprioritizationmatrix.h \
    ratingeditdelegate.h \    
    criteriamatrix.h \
    alternativesmatrix.h \
    analytichiearchyview.h \
    prioritizationmatrixview.h \
    paramform.h \
    resultmatrix.h \
    resultview.h \
    analytichiearchymainwindow.h

FORMS += \
    paramform.ui \
    prioritizationmatrixview.ui \
    resultview.ui \
    analytichiearchymainwindow.ui

RESOURCES += \
    resource.qrc

RC_ICONS = images/AnalyticHiearchyIcons.ico

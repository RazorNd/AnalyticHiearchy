#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T13:34:39
#
#-------------------------------------------------

QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.9.7.0
QMAKE_TARGET_COMPANY = RazorNd ltd.
QMAKE_TARGET_PRODUCT = Analytic Hiearchy

TARGET = AnalyticHiearchy
TEMPLATE = app

HEADERS += \
    src/model/abstractprioritizationmatrix.h \
    src/model/alternativesmatrix.h \
    src/model/analytichiearchymodel.h \
    src/model/criteriamatrix.h \
    src/model/resultmatrix.h \
    src/model/skewsymmetricmatrix.h \
    src/view/analytichiearchyview.h \
    src/view/prioritizationmatrixview.h \
    src/view/resultview.h \
    src/analytichiearchymainwindow.h \
    src/paramform.h \
    src/rating.h \
    src/ratingeditdelegate.h \
    src/model/entringdatamodel.h \
    src/settingsdialog.h \
    src/criteriadirectiondeligate.h \
    src/view/rankingresult.h \
    src/util/analytichiearchyexcelexporter.h \
    src/entringcriteria.h

SOURCES += \
    src/model/abstractprioritizationmatrix.cpp \
    src/model/alternativesmatrix.cpp \
    src/model/analytichiearchymodel.cpp \
    src/model/criteriamatrix.cpp \
    src/model/resultmatrix.cpp \
    src/model/skewsymmetricmatrix.cpp \
    src/view/analytichiearchyview.cpp \
    src/view/prioritizationmatrixview.cpp \
    src/view/resultview.cpp \
    src/analytichiearchymainwindow.cpp \
    src/main.cpp \
    src/paramform.cpp \
    src/rating.cpp \
    src/ratingeditdelegate.cpp \
    src/model/entringdatamodel.cpp \
    src/settingsdialog.cpp \
    src/criteriadirectiondeligate.cpp \
    src/view/rankingresult.cpp \
    src/util/analytichiearchyexcelexporter.cpp \
    src/entringcriteria.cpp

RC_ICONS = res/images/AnalyticHiearchyIcons.ico

RESOURCES += \
    res/resource.qrc

FORMS += \
    src/UI/analytichiearchymainwindow.ui \
    src/UI/paramform.ui \
    src/UI/prioritizationmatrixview.ui \
    src/UI/resultview.ui \
    src/UI/settingsdialog.ui \
    src/UI/rankingresult.ui



# Add more folders to ship with the application, here
#qml_folder.source = qml/dukto
#qml_folder.target = qml
#DEPLOYMENTFOLDERS = qml_folder

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += declarative widgets
QT += sensors

#for nullptr
greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11


VERSION = 6.1.0
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:RC_FILE = src/dukto.rc
win32:LIBS += libWs2_32 libole32 libNetapi32

mac:ICON = src/dukto.icns


# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices


unix {
	TARGET = dukto
	target.path = /usr/bin
	INSTALLS += target
	
	icon.path = /usr/share/pixmaps
        icon.files = src/dukto.png
	INSTALLS += icon
	
	desktop.path = /usr/share/applications/
        desktop.files = src/dukto.desktop
	INSTALLS += desktop
}

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    src/main.cpp \
    src/guibehind.cpp \
    src/platform.cpp \
    src/buddylistitemmodel.cpp \
    src/duktoprotocol.cpp \
    src/miniwebserver.cpp \
    src/ipaddressitemmodel.cpp \
    src/recentlistitemmodel.cpp \
    src/settings.cpp \
    src/destinationbuddy.cpp \
    src/duktowindow.cpp \
    src/ecwin7.cpp \
    src/theme.cpp \
    src/updateschecker.cpp

#Localizing
LANGUAGES = zh_TW

#a function to generate a TS file for each language
defineReplace(prependAll) {
 for(a,$$1):result ''= $$2$${a}$$3
 return($$result)
}
TRANSLATIONS = $$prependAll(LANGUAGES, $$PWD/language/, .ts)

#lrelease to generate the QM files
TRANSLATIONS_FILES =

#TODO: following can not handle chinese path
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
 qmfile = $$shadowed($$tsfile)
 qmfile ~= s,.ts$,.qm,
 qmdir = $$dirname(qmfile)
 !exists($$qmdir) {
 mkpath($$qmdir)|error("Aborting.")
 }
 command = $$LRELEASE -removeidentical $$tsfile -qm $$qmfile
 system($$command)|error("Failed to run: $$command")
 TRANSLATIONS_FILES''= $$qmfile
}


lupdate_only{
SOURCES = qml/dukto/*.qml \
          qml/dukto/*.js
}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    src/guibehind.h \
    src/platform.h \
    src/buddylistitemmodel.h \
    src/duktoprotocol.h \
    src/peer.h \
    src/miniwebserver.h \
    src/ipaddressitemmodel.h \
    src/recentlistitemmodel.h \
    src/settings.h \
    src/destinationbuddy.h \
    src/duktowindow.h \
    src/ecwin7.h \
    src/theme.h \
    src/updateschecker.h

RESOURCES += \
    qml.qrc \
    translations.qrc

include(qtsingleapplication/qtsingleapplication.pri)

OTHER_FILES +=

DISTFILES += \
    README.md \
    language/*.ts

#android
android {
HEADERS += \
    debug/qDebug2Logcat.h
SOURCES += \
    debug/qDebug2Logcat.cpp
#app's icon:
# ldpi : 36x36px (120 dpi / 47 dpcm)
# mdpi : 48x48px (160 dpi / 62 dpcm)
# hdpi : 72x72px (240 dpi / 94 dpcm)
# xhdpi : 96x96px (320 dpi / 126 dpcm)
# xxhdpi : 144x144px (480 dpi / 189 dpcm)
# xxxhdpi : 192x192px (640 dpi / 252 dpcm)

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-xdpi/icon.png \
    android/res/drawable-xxdpi/icon.png \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}










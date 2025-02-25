#include "mainwindow.h"
#include "confighandler.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDebug>
#include <iostream>
#include "startwindow.h"
#include "dialreasonreduct.h"
#include "fontmanager.h"
#include "popup.h"
#include <QSettings>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);

    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] %{type}: %{message}");

    QByteArray id;
    int mainWindowDir;

    // Setting with config.ini //
    qInfo() << "Config created";

    ConfigHandler config(QCoreApplication::applicationDirPath() + "/config.ini");

    QString hostname = config.getHostname();
    quint16 port = config.getPort();
    QString name = config.getName();

    qInfo() << "PHS IP: " << hostname;
    qInfo() << "PHS port: "<< port;
    qInfo() << "Station name: " << name;

   if(name == "P02" || name == "P04" || name == "P06" || name == "P08" || name == "P10" || name == "P12") {
       mainWindowDir = 2; // P02, P04, P06, P08, P10, P12
       qInfo() << "Align no. 2";
   }
   else if(name == "P03" || name == "P05" || name == "P07" || name == "P09" || name == "P11" || name == "P13") {
       mainWindowDir = 3; // P03, P05, P07, P09, P11, P13
       qInfo() << "Align no. 3";
   }
   else if (name == "P01") {
       mainWindowDir = 1; // P01
       qInfo() << "Align no. 1";
   }
   else if (name == "P14") {
       mainWindowDir = 4; // P14
       qInfo() << "Align no. 4";
   }

   mainWindowDir = 5;

    PHSBackend phs(QHostAddress(hostname), port, new SimpleAuthenticator(name, "hasło"), id);
    MainWindow mainWindow(&phs);
    StartWindow startWindow(&phs);
    PopUp popup(&phs);
    DialReasonReduct reason(&phs);

    QObject::connect(&mainWindow, SIGNAL(openReason()), &reason, SLOT(showwindow()));
    QObject::connect(&mainWindow, SIGNAL(showOnTopIfVisible()), &popup, SLOT(setOnTopIfVisible()));

    QObject::connect(&phs, SIGNAL(connectionStatus(bool)), &startWindow, SLOT(phsConnectionStatusChanged(bool)));
    QObject::connect(&phs, SIGNAL(connectionStatus(bool)), &reason, SLOT(phsConnectionStatusChanged(bool)));
    QObject::connect(&phs, SIGNAL(connectionStatus(bool)), &popup, SLOT(phsConnectionStatusChanged(bool)));

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication::setFont(FontManager::instance().getMontserratMedium());

    phs.registerObject(2, &mainWindow);
    mainWindow.setWindowFlags(Qt::FramelessWindowHint);
    mainWindow.showFullScreen();

    phs.registerObject(1, &startWindow);
    phs.registerObject(3, &popup);
    phs.registerObject(4, &reason);

    popup.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog
                         | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    reason.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog
                          | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    startWindow.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog
                               | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    popup.setAttribute(Qt::WA_TranslucentBackground);
    reason.setAttribute(Qt::WA_TranslucentBackground);
    startWindow.setAttribute(Qt::WA_TranslucentBackground);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int dialogX = (screenGeometry.width() - popup.width()) / 2;
    int dialogY = (screenGeometry.height() - popup.height() - 15) / 2;

    popup.move(dialogX, dialogY);

    int reasonX = (screenGeometry.width() - reason.width()) / 2;
    int reasonY = (screenGeometry.height() - reason.height()) / 2;

    reason.move(reasonX, reasonY);
    //reason.setVisible(true);
    phs.startConnection();

    return a.exec();
}

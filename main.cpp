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
#include "keyboardnumdialog.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);

    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] %{type}: %{message}");

    QByteArray id;

    // Setting with config.ini //
    ConfigHandler::initialize(QCoreApplication::applicationDirPath() + "/config.ini");
    qInfo() << "Config created";

    QString hostname = ConfigHandler::getHostname();
    quint16 port = ConfigHandler::getPort();ConfigHandler::initialize(QCoreApplication::applicationDirPath() + "/config.ini");
    QString name = ConfigHandler::getName();

    qInfo() << "PHS IP: " << hostname;
    qInfo() << "PHS port: "<< port;
    qInfo() << "Station name: " << name;

    PHSBackend phs(QHostAddress(hostname), port, new SimpleAuthenticator(name, "hasło"), id);
    MainWindow mainWindow(&phs);
    StartWindow startWindow(&phs);
    PopUp popup(&phs);
    DialReasonReduct reason(&phs);
    KeyboardNumDialog numeric(&phs);

    QObject::connect(&mainWindow, SIGNAL(openReason()), &reason, SLOT(showwindow()));
    QObject::connect(&mainWindow, SIGNAL(showOnTopIfVisible()), &popup, SLOT(setOnTopIfVisible()));

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

    return a.exec();
}

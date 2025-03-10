#include "confighandler.h"
#include <QDebug>

QSettings* ConfigHandler::settings = nullptr;

void ConfigHandler::initialize(const QString &configPath)
{
    if (!settings) {
        settings = new QSettings(configPath, QSettings::IniFormat);
    }
}

// PHS //
QString ConfigHandler::getHostname()
{
    return settings->value("PHS/IP", "192.168.122.30").toString();
}

quint16 ConfigHandler::getPort()
{
    return settings->value("PHS/port", 9003).toUInt();
}

QString ConfigHandler::getName()
{
    return settings->value("PHS/name", "W001").toString();
}
//time to reconnect to PHS after disconnecting
quint16 ConfigHandler::getPhsReconnectInterval() {
    return settings->value("PHS/reconnect_interval", 1000).toUInt();
}

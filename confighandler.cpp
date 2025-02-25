#include "confighandler.h"
#include <QDebug>

ConfigHandler::ConfigHandler(const QString &configPath)
    : settings(configPath, QSettings::IniFormat)
{}

// PHS //
QString ConfigHandler::getHostname() const
{
    return settings.value("PHS/IP", "192.168.122.30").toString();
}

quint16 ConfigHandler::getPort() const
{
    return settings.value("PHS/port", 9003).toUInt();
}

QString ConfigHandler::getName() const
{
    return settings.value("PHS/name", "W001").toString();
}

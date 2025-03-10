#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QSettings>
#include <QString>

class ConfigHandler
{
public:
    static void initialize(const QString &configPath);

    static QString getHostname();
    static quint16 getPort();
    static QString getName();
    static quint16 getPhsReconnectInterval();

private:
    ConfigHandler() = default;
    static QSettings* settings;
};

#endif // CONFIGHANDLER_H

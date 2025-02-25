#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QSettings>
#include <QString>

class ConfigHandler
{
public:
    ConfigHandler(const QString &configPath);
    QString getHostname() const;
    quint16 getPort() const;
    QString getName() const;
    bool hasFullScreen() const;
    bool hasScreensaver() const;

private:
    QSettings settings;
};

#endif // CONFIGHANDLER_H

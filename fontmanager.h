#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QFont>
#include <QFontDatabase>
#include <QApplication>
#include <QDebug>

class FontManager {
public:
    static FontManager& instance();

    QFont getMontserratMedium() const;
    QFont getMontserratBold() const;

private:
    FontManager();

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    void setMontserratMedium(const QFont& font);
    void setMontserratBold(const QFont& font);

    QFont getFontFromResources(QString path);

    QFont montserratMedium;
    QFont montserratBold;
};

#endif // FONTMANAGER_H

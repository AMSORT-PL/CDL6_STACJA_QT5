#include "fontmanager.h"

FontManager& FontManager::instance() {
    static FontManager instance;
    return instance;
}

FontManager::FontManager()
{
    setMontserratMedium(getFontFromResources(":/Montserrat-Medium.ttf"));

    setMontserratBold(getFontFromResources(":/Montserrat-Bold.ttf"));
}

QFont FontManager::getFontFromResources(QString path) {
    int fontId = QFontDatabase::addApplicationFont(path);
    if (fontId == -1) {
        qDebug() << "Couldn't load font in path:" << path << "\nPlease check if the font exists.";
    }

    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(family);
    return font;
}

QFont FontManager::getMontserratMedium() const {
    return montserratMedium;
}

QFont FontManager::getMontserratBold() const {
    return montserratBold;
}

void FontManager::setMontserratMedium(const QFont &font) {
    montserratMedium = font;
}

void FontManager::setMontserratBold(const QFont &font) {
    montserratBold = font;
}

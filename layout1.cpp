#include "layout1.h"

Layout1::Layout1(PHSBackend* _phs, QWidget *parent) :
      LayoutBase(parent),
      ui(new Ui::Layout1),
      phs(_phs)
{
    QSettings settings(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

    name = settings.value("PHS/name", "P00").toString();

    ui->setupUi(this);

    assignButtons();
    connectButtons();
}

QList<QPushButton*> Layout1::getButtons() const {
    return buttons;
}

void Layout1::assignButtons() {
    qInfo() << "Start of assigning buttons";
    for (int i = 1; i <= 45; ++i) {
        QString buttonName = QString("b%1").arg(i);
        QPushButton* button = this->findChild<QPushButton*>(buttonName);
        if (button) {
            qInfo() << "Setting id" << "to button" << buttonName;
            button->setProperty("id", i);
            buttons.append(button);
        }
    }
}

void Layout1::connectButtons() {
    qInfo() << "Start of connecting buttons";

    int baseId = 1100;

    for (QPushButton* button : buttons) {
        int propertyId = baseId + button->property("id").toInt();
        connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
        phs->registerObject(propertyId, button);
        qInfo() << QString("Button %1 has been properly registered for id=%2").arg(button->objectName()).arg(propertyId);
    }
}

void Layout1::buttonClicked()
{
    QPushButton *button = (QPushButton *)sender();
    int buttonId = button->property("id").toInt();

    if (buttonId <= 15) {
        qInfo() << QString("Location %1-01-%2").arg(name).arg(buttonId);
    }
    else if (buttonId <= 30) {
        qInfo() << QString("Location %1-02-%2").arg(name).arg(buttonId);
    }
    else if (buttonId <= 45) {
        qInfo() << QString("Location %1-03-%2").arg(name).arg(buttonId);
    }
}

Layout1::~Layout1() {
    delete ui;
}

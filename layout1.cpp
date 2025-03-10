#include "layout1.h"

Layout1::Layout1(PHSBackend* _phs, QWidget *parent) :
      LayoutBase(parent),
      ui(new Ui::Layout1),
      phs(_phs),
      markedButton(nullptr)
{

    this->setStyleSheet("border: 1px solid gray;");

    ui->setupUi(this);

    assignButtons();
    connectButtons();

   actionPanel = new ActionPanel(this);

   QVBoxLayout *layout = new QVBoxLayout(ui->action_frame);
   layout->addWidget(actionPanel);
   ui->action_frame->setLayout(layout);

   connect(actionPanel, &ActionPanel::ean_button_clicked, this, &Layout1::ean_button_clicked);
   connect(actionPanel, &ActionPanel::inventory_tote_clicked, this, &Layout1::inventory_tote_clicked);
   connect(actionPanel, &ActionPanel::inventory_product_clicked, this, &Layout1::inventory_product_clicked);
   connect(actionPanel, &ActionPanel::mark_KJ_clicked, this, &Layout1::mark_KJ_clicked);
   connect(actionPanel, &ActionPanel::close_carrier_clicked, this, &Layout1::close_carrier_clicked);
   connect(actionPanel, &ActionPanel::reprint_clicked, this, &Layout1::reprint_clicked);
   connect(actionPanel, &ActionPanel::run_receiving_container_clicked, this, &Layout1::run_receiving_container_clicked);
   connect(actionPanel, &ActionPanel::inventory_src_container_clicked, this, &Layout1::inventory_src_container_clicked);

   connect(this, &LayoutBase::buttonsUnlocked, actionPanel, &ActionPanel::buttonsUnlocked);
}

QList<QPushButton*> Layout1::getButtons() const {
    return buttons;
}

void Layout1::ean_button_clicked() {
    emit ean_button_clicked_forward();
}

void Layout1::inventory_tote_clicked() {
    emit inventory_tote_clicked_forward();
}

void Layout1::inventory_product_clicked() {
    emit inventory_product_clicked_forward();
}

void Layout1::mark_KJ_clicked() {
    emit mark_KJ_clicked_forward();
}

void Layout1::close_carrier_clicked() {
    emit close_carrier_clicked_forward();
}

void Layout1::reprint_clicked() {
    emit reprint_clicked_forward();
}

void Layout1::run_receiving_container_clicked() {
    emit run_receiving_container_clicked_forward();
}

void Layout1::inventory_src_container_clicked() {
    emit inventory_src_container_clicked_forward();
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

    emit askToMarkLocation(buttonId);
}

void Layout1::markLocation(int locationId) {
    qInfo() << "Layout1: Marking location:" << locationId;

    QPushButton* button = getButton(locationId);
    if (!button) {
        qWarning() << "Button for location" << locationId << "not found!";
        return;
    }

    if(markedButton && button == markedButton) {
        unmarkLocation();
        return;
    }
    else if (markedButton) {
        unmarkLocation();
    }

    button->setStyleSheet(ME::MARKED);
    markedButton = button;

    emit buttonsUnlocked(true);
}

void Layout1::unmarkLocation() {
    if(markedButton != nullptr) {
        qInfo() << "Unmarking location no." << markedButton->property("id").toInt();

        markedButton->setStyleSheet(ME::BASE);
        markedButton = nullptr;
        emit buttonsUnlocked(false);
    }
}

QPushButton* Layout1::getButton(int locationId) {
    auto it = std::find_if(buttons.begin(), buttons.end(), [&](QPushButton* btn) {
        return btn->property("id") == locationId;
    });

    if (it != buttons.end()) {
        return *it;
    }

    return nullptr;
}


Layout1::~Layout1() {
    delete ui;
}

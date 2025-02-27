#ifndef LAYOUTBASE_H
#define LAYOUTBASE_H

#include <QWidget>
#include <QPushButton>
#include <QList>

class LayoutBase : public QWidget {
    Q_OBJECT
public:
    explicit LayoutBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~LayoutBase() = default;

    virtual QList<QPushButton*> getButtons() const = 0;

signals:
    void ean_button_clicked_forward();
    void askToMarkLocation(int locationId);

public slots:
    virtual void markLocation(int locationId) = 0;
};

#endif // LAYOUTBASE_H

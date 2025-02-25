#ifndef LAYOUTBASE_H
#define LAYOUTBASE_H

#include <QWidget>
#include <QPushButton>
#include <QList>

class LayoutBase : public QWidget {
public:
    explicit LayoutBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual QList<QPushButton*> getButtons() const = 0;
    virtual ~LayoutBase() = default;
};

#endif // LAYOUTBASE_H

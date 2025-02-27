#include "tabinfo.h"
#include "QDateTime"
#include <QBrush>
#include <QVector>
#include <QString>
#include <QProgressBar>
#include <iostream>
#include <QVariant>

TabInfo::TabInfo(QObject *parent):
    PHSBaseModel(QVector<QString>{ "lp", "Towar", "Barcode", "Ilość", "Zebrano", "Status", "Nazwa lokacji", "Count", "Cardinal"},&rowsCreator, parent)
{}


void TabInfo::setCurrentTrackId(int id)
{
    if (id != currentId) {
        currentId = id;
    }
}

TabInfo::Row::Row(PHSBackend::TID id, int initialRow)
    : PHSBaseModel::IRow(id, initialRow)
    , datas(9, QString())
    , bgColor(0)
    , bgColorError(1)
    , bgColorSuccess(5)
    , fgColor(0)
    , palletId(-1)
    , checked(false)
{
}

QVariant TabInfo::Row::data(int col, int role) {
    switch(role) {
    case PHSBaseModel::FilterRole:
        std::cerr << "col: " << col << std::endl;
        if (col == 0)
            return palletId;
        break;
    case Qt::DisplayRole:
        if (col >= 0 && col < datas.size())
            return datas.at(col);
        break;
    case Qt::TextAlignmentRole:
        if (col >= 0 && col < datas.size())
            return Qt::AlignCenter;
        break;
    case Qt::BackgroundRole:
        if (col >= 0 && col < datas.size())
            return rowBgColors[bgColor];

        break;
    case Qt::ForegroundRole:
        if (col >= 0 && col < datas.size())
            return rowFgColors[fgColor];
        break;
    }
    return QVariant();
}

static inline void tranferData(int i, int j, QVector<QVariant> const& src, QVector<QString> & dest) {
    QVariant const& cell = src[i];
    if (! cell.isNull())
    {
        dest[j] = cell.toString();
    }

    else
        dest[j] = QString();
}

void TabInfo::Row::update(QVector<QVariant> const& newCols) {
    tranferData(3,  0, newCols, datas); // POSITION //
    tranferData(2,  1, newCols, datas); // ITEM NAME //
    tranferData(4,  2, newCols, datas); // CARRIER BARCODE //
    tranferData(5,  3, newCols, datas); // PICK QUANTITY //
    tranferData(6,  4, newCols, datas); // PICKED QUANTITY //
    tranferData(8,  5, newCols, datas); // STATUS //
    tranferData(1,  6, newCols, datas); // LOCATION //
    tranferData(10,  7, newCols, datas); // COUNT //
    tranferData(9,  8, newCols, datas); // CARDINAL //
    //colorsFlag = newCols[11].toInt();
}

void TabInfo::clearTable(){
    clear();
}

QBrush TabInfo::rowBgColors[] = {QBrush(QColor(Qt::white)), QBrush(QColor(255, 100, 75)), QBrush(QColor(Qt::darkGreen)), QBrush(QColor(Qt::yellow )), QBrush(QColor(255,153,51)), QBrush(QColor(100,173,100))};
QBrush TabInfo::rowFgColors[] = {QBrush(QColor(Qt::black)), QBrush(QColor(Qt::blue)), QBrush(QColor(255, 102, 0))};

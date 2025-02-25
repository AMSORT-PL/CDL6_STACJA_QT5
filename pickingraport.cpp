#include "pickingraport.h"

//PickingRaport::PickingRaport()
//{

//}

#include <QBrush>
#include <QVector>
#include <QString>

//<<< GUI(LIST_REPL ID:12 ID:79 #P:4 INT(79) STRING("BH-01-36") STRING("D1-10000008119") INT(6)) (36 octets, 0 frames obsoleted)


PickingRaport::PickingRaport(QObject *parent):
    PHSBaseModel(QVector<QString>{"SSCC","Data rozpocząecia", "Ilość kartonów", "ilość SKU", "ilość partii", "Status", "priorytet"},&rowsCreator, parent) // bylo 6 kolumn

{}



void PickingRaport::setCurrentTrackId(int id)
{
    if (id != currentId) {
        currentId = id;
    }
}

PickingRaport::Row::Row(PHSBackend::TID id, int initialRow)
    : PHSBaseModel::IRow(id, initialRow)
    , datas(12, QString())
    , bgColor(0)
    , fgColor(0)
{
}

QVariant PickingRaport::Row::data(int col, int role) {
    switch(role) {
    case PHSBaseModel::SortRole:
        break;

    case Qt::DisplayRole:
        if (col >= 0 && col < datas.size())
            return datas.at(col);
        break;
    case Qt::TextAlignmentRole:
        switch (col) {
        case 0: return Qt::AlignCenter + Qt::AlignVCenter;
        case 1: return Qt::AlignCenter + Qt::AlignVCenter;
        case 2: return Qt::AlignCenter + Qt::AlignVCenter;
        case 3: return Qt::AlignCenter + Qt::AlignVCenter;
        case 4: return Qt::AlignCenter + Qt::AlignVCenter;


        }
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
        dest[j] = cell.toString();

    else
        dest[j] = QString();

}

;
void PickingRaport::Row::update(QVector<QVariant> const& newCols) {
    tranferData(0,  0, newCols, datas);
    tranferData(1,  1, newCols, datas);
    tranferData(2,  2, newCols, datas);
    tranferData(3,  3, newCols, datas);
    tranferData(4,  4, newCols, datas);


    bool selected = false ; //newCols[5].toBool();
    int extraLocation = 0; //newCols[6].toInt();

    if (selected)

        bgColor = 3;
            else
                bgColor = 0;
            if (extraLocation != 0)
                fgColor = 1;
            else
                fgColor = 0;


}



QBrush PickingRaport::rowBgColors[] = {QBrush(QColor(Qt::white)), QBrush(QColor(Qt::red)), QBrush(QColor(Qt::darkGreen)), QBrush(QColor(Qt::yellow ))};
QBrush PickingRaport::rowFgColors[] = {QBrush(QColor(Qt::black)), QBrush(QColor(Qt::blue))};

#include "tabcontainercont.h"
#include "QDateTime"
#include <QBrush>
#include <QVector>
#include <QString>

tabContainerCont::tabContainerCont(QObject *parent):
    PHSBaseModel(QVector<QString>{"Barcode","Powód", "Waga ref","Waga", "Czas zjazdu"},&rowsCreator, parent)
{}

virtual ~tabContainerCont() {};

void tabContainerCont::setCurrentTrackId(int id)
{
    if (id != currentId) {
        currentId = id;
    }
}

tabContainerCont::Row::Row(PHSBackend::TID id, int initialRow)
    : PHSBaseModel::IRow(id, initialRow)
    , datas(7, QString())
    , bgColor(0)
    , fgColor(0)
    , palletId(-1)
{
}

QVariant tabContainerCont::Row::data(int col, int role) {
    switch(role) {
    case PHSBaseModel::SortRole:
        break;
    case PHSBaseModel::FilterRole:
        if (col == 0)
            //indeks kolumny filtrowanej powinnien być 0
            return palletId;

        // dodany break 25.05
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
  //      case 5: return Qt::AlignCenter + Qt::AlignVCenter;



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
void tabContainerCont::Row::update(QVector<QVariant> const& newCols) {
    tranferData(1,  0, newCols, datas);
    tranferData(2,  1, newCols, datas);
    tranferData(3,  2, newCols, datas);
    tranferData(4,  3, newCols, datas);


    //tranferData(5,  4, newCols, datas);

    // Ramp timestamp
    if (newCols[8].isNull()) {
        datas[4] = QString{};
         }
    else {
        datas[4] = QDateTime::fromSecsSinceEpoch(newCols[5].toLongLong()).toString("ddd, HH:mm:ss.zzz");
    }

  //  tranferData(6,  5, newCols, datas);
  //  tranferData(7,  6, newCols, datas);
//    tranferData(8,  7, newCols, datas);
//    tranferData(9,  8, newCols, datas);
//    tranferData(10,  9, newCols, datas);
//    tranferData(11,  10, newCols, datas);
//    tranferData(12,  11, newCols, datas);
//    tranferData(13,  12, newCols, datas);
//    tranferData(14,  13, newCols, datas);
//    tranferData(15,  14, newCols, datas);
   // tranferData(16,  15, newCols, datas);


    // indeks kolumny przesylanej z pHSA
   //  palletId = newCols[1].toInt();
   // qInfo() << "palletId z dats" + QString::number(palletId);


    bool selected = newCols[6].toBool();
    int extraLocation = false; //newCols[7].toInt();

    if (selected){
        bgColor = 2;
}


}



QBrush tabContainerCont::rowBgColors[] = {QBrush(QColor(Qt::white)), QBrush(QColor(Qt::red)), QBrush(QColor(Qt::darkGreen)), QBrush(QColor(Qt::yellow ))};
QBrush tabContainerCont::rowFgColors[] = {QBrush(QColor(Qt::black)), QBrush(QColor(Qt::blue))};

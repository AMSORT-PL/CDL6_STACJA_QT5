//#ifndef TABCONTAINERCONT_H
//#define TABCONTAINERCONT_H


//class tabContainerCont
//{
//public:
//    tabContainerCont();
//};

//#endif // TABCONTAINERCONT_H


#ifndef TABCONTAINERCONT_H
#define TABCONTAINERCONT_H


#include <QObject>
#include "phsbasemodel.h"

class tabContainerCont : public PHSBaseModel
{
    Q_OBJECT
    Q_PROPERTY(int currentTagId READ getCurentTrackId WRITE setCurrentTrackId)
public:
    explicit tabContainerCont(QObject *parent = 0);


    void setCurrentTrackId(int id);
    int getCurentTrackId() const { return currentId; }
signals:


public slots:

private:


    class Row : public PHSBaseModel::IRow {
    public:

        Row(PHSBackend::TID id, int initialRow);

    protected:
        virtual QVariant data(int col, int role = Qt::DisplayRole) override;
        virtual void update(QVector<QVariant> const& newCols) override;
    private:
        QVector<QString> datas;
        int bgColor;
        int fgColor;
        int palletId;


    };


    PHSSimpleRowsCreator<Row> rowsCreator;
    int currentId;
    static QBrush rowBgColors[];
    static QBrush rowFgColors[];
};

#endif // /// TABCONTAINERCONT_H


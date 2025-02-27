#ifndef TabInfo_H
#define TabInfo_H
#include <QObject>
#include "phsbasemodel.h"


class TabInfo : public PHSBaseModel
{
    Q_OBJECT
    Q_PROPERTY(int currentTagId READ getCurentTrackId WRITE setCurrentTrackId)
public:
    explicit TabInfo(QObject *parent = 0);
    void setCurrentTrackId(int id);
    int getCurentTrackId() const { return currentId; }
    void clearTable();
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
        int bgColorError;
        int bgColorSuccess;
        int fgColor;
        int palletId;
        int postep;
        bool checked;
        int colorsFlag;
    };


    PHSSimpleRowsCreator<Row> rowsCreator;
    int currentId;
    static QBrush rowBgColors[];
    static QBrush rowFgColors[];
};
#endif // TabInfo_H

#ifndef PHSBASEMODEL_H
#define PHSBASEMODEL_H

#include <QVector>
#include <QMap>
#include <QAbstractTableModel>
#include "phsbackend.h"

class PHSBaseModel : public QAbstractTableModel, public PHSBackend::IListModel
{
    Q_OBJECT

public:
    enum CustomRoles {
        SortRole = Qt::UserRole,
        FilterRole
    };

    class IRow {
    public:
        virtual QVariant data(int col, int role = Qt::DisplayRole) = 0;
        virtual void update(QVector<QVariant> const& newCols) = 0;
        virtual ~IRow() { }

        inline PHSBackend::TID getPhsId() const { return phsId; }
        virtual int getExtraId(int idType) {Q_UNUSED(idType) return -1; }
        inline int getRow() const { return row; }

    protected:
        PHSBackend::TID phsId;
        int row;

        inline IRow(PHSBackend::TID id, int aRow) : phsId(id), row(aRow) { }
        inline void movedUp() { row--; }
    };

    class IRowCreator {
    public:
        virtual IRow* newRow(PHSBackend::TID id, int rowNo, QVector<QVariant> const& cols) = 0;
    };

public:
    PHSBaseModel(QVector<QString> const& headers, IRowCreator* creator, QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QString &value, int role);

    // Basic functionality:
    /*
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int getRowCount();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    inline PHSBackend::TID getIdForRow(int rowNo, int idType = 0) const {
        if (rowNo >= 0 && rowNo < myRows.size()) {
            if (idType == 0)
                return myRows[rowNo]->getPhsId();
            else
                return myRows[rowNo]->getExtraId(idType);
        }
        return -1;
    }

    inline PHSBackend::TID getIdForIndex(const QModelIndex &index, int idType = 0) const {
        return getIdForRow(index.row(), idType);
    }

    inline int getRowForId(PHSBackend::TID id) const {
        auto iter = rowById.find(id);
        if (iter != rowById.end())
            return *iter;
        else
            return -1;
    }

    static inline QString toStringNotNull(QVariant const& src) {
        if (src.isNull())
            return QString();
        else
            return src.toString();
    }

protected:
    virtual void clear() override;
    virtual void addOrReplaceRow(PHSBackend::TID id, QVector<QVariant> const& cols) override;
    virtual void deleteRow(PHSBackend::TID id) override;

private:
    QVector<QString> myHeaders;
protected:
    QVector<IRow*> myRows;
private:
    IRowCreator* rowCreator;
    QMap<PHSBackend::TID, int> rowById;
};

template <typename _Row>
class PHSSimpleRowsCreator : public PHSBaseModel::IRowCreator {
    virtual PHSBaseModel::IRow* newRow(PHSBackend::TID id, int rowNo, QVector<QVariant> const& cols) override {
        PHSBaseModel::IRow* r = new _Row(id, rowNo);
        r->update(cols);
        return r;
    }
};

template <typename _Row, typename _Owner>
class PHSOwnedRowsCreator : public PHSBaseModel::IRowCreator {
    virtual PHSBaseModel::IRow* newRow(PHSBackend::TID id, int rowNo, QVector<QVariant> const& cols) override {
        PHSBaseModel::IRow* r = new _Row(id, rowNo, _owner);
        r->update(cols);
        return r;
    }
private:
    _Owner* _owner;

public:
    inline explicit PHSOwnedRowsCreator(_Owner* a_owner)
        : _owner(a_owner)
    { }
};

#endif // PHSBASEMODEL_H

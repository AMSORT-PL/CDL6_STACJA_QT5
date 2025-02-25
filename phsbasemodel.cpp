
#include "phsbasemodel.h"

PHSBaseModel::PHSBaseModel(const QVector<QString> &headers, IRowCreator* creator, QObject *parent)
    : QAbstractTableModel(parent)
    , myHeaders(headers)
    , rowCreator(creator)
{
}

QVariant PHSBaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole ||
            section < 0 || section > myHeaders.size())
        return QVariant();
    else
        return myHeaders[section];
}

bool PHSBaseModel::setHeaderData(int section, Qt::Orientation orientation, const QString &value, int role)
{
    if (section < myHeaders.size() && role == Qt::EditRole)
    {
        myHeaders.replace(section, value);
        //qDebug()<<section;
        emit headerDataChanged(orientation,section,section);
        return true;
    }
    return false;
}


int PHSBaseModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return myRows.size();
}

int PHSBaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return myHeaders.size();
}

QVariant PHSBaseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    if (row < 0 || row >= myRows.size()) {
        return QVariant();
    }
    return myRows.at(row)->data(col, role);
}

void PHSBaseModel::clear() {
    beginResetModel();
    for (IRow* row : myRows)
        delete row;
    myRows.clear();
    rowById.clear();
    endResetModel();
}
int PHSBaseModel::getRowCount()
{
    return rowById.size();
}
void PHSBaseModel::addOrReplaceRow(PHSBackend::TID id, QVector<QVariant> const& cols) {
    auto entry = rowById.find(id);
    if (entry != rowById.end()) {
        int row = entry.value();
        myRows.at(row)->update(cols);
        emit dataChanged(createIndex(row, 0), createIndex(row, columnCount()-1));
    }
    else {
        int rowNo = myRows.size();
        IRow* newRow = rowCreator->newRow(id, rowNo, cols);
        if (! newRow)
            return;
        beginInsertRows(QModelIndex(), rowNo, rowNo);
        myRows.push_back(newRow);
        rowById[id] = rowNo;
        endInsertRows();
        emit dataChanged(createIndex(rowNo, 0), createIndex(rowNo, columnCount()-1));
    }
}

void PHSBaseModel::deleteRow(PHSBackend::TID id) {
    auto entry = rowById.find(id);
    if (entry != rowById.end()) {
        int row = entry.value();
        beginRemoveRows(QModelIndex(), row, row);
        rowById.remove(id);
        for (entry = rowById.begin(); entry != rowById.end(); ++entry) {
            if (entry.value() > row)
                entry.value()--;
        }
        myRows.remove(row);
        endRemoveRows();
        emit dataChanged(createIndex(row, 0), createIndex(rowCount()-1, columnCount()-1));
    }
}

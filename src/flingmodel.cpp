#include "flingmodel.h"
#include "displaywidget.h"

FlingModel::FlingModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int FlingModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_flings.count();
}

int FlingModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant FlingModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid()) {
        Fling* f = m_flings.at(index.row());

//        if (role == Qt::DecorationRole)
//            return f->color();

        if (role == Qt::DisplayRole) {
            switch(index.column()) {
            case 0: return QString::number(f->velocity);
            case 1: return QString::number(f->angle);
            }
        }
    }

    return QVariant();
}

QVariant FlingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0: return tr("Velocity");
        case 1: return tr("Angle");
        }
    }

    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
        return section + 1;

    return QVariant();
}

void FlingModel::add(qreal velocity, qreal angle)
{
    Fling* f = new Fling;
    f->velocity = velocity;
    f->angle = angle;
    f->color = QColor(qrand() % 256, qrand() % 256, qrand() % 256).darker(125);

    beginInsertRows(QModelIndex(), m_flings.size(), m_flings.size());
    m_flings << f;
    endInsertRows();
}

void FlingModel::clear()
{
    emit beginResetModel();
    qDeleteAll(m_flings);
    m_flings.clear();
    emit endResetModel();
}

QList<Fling*> FlingModel::flings() const
{
    return m_flings;
}

Qt::ItemFlags FlingModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool FlingModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        Fling* f = m_flings.at(index.row());
        switch (index.column()) {
        case 0: f->velocity = value.toDouble(); break;
        case 1: f->angle = value.toDouble(); break;
        }
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

bool FlingModel::removeRows(int row, int count, const QModelIndex & parent)
{
    // introduce sanity checks and consequently proper return value
    beginRemoveRows(parent, row, row - 1 + count);

    while (count > 0) { // todo ... use iterators for that

        Fling* f = m_flings.takeAt(row);
        delete f;
        --count;
    }

    endRemoveRows();

    return true;
}

#ifndef FLINGMODEL_H
#define FLINGMODEL_H

#include <QAbstractTableModel>
#include <QColor>

struct Fling {
    qreal velocity;
    qreal angle;
    QColor color;
};

class FlingModel : public QAbstractTableModel
{
Q_OBJECT
public:
    explicit FlingModel(QObject *parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

    QList<Fling*> flings() const;
    void add(qreal, qreal);

public slots:
    void clear();

private:
    QList<Fling*> m_flings;
};

#endif // FLINGMODEL_H

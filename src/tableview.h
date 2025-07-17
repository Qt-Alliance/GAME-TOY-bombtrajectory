#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class TableView : public QTableView
{
Q_OBJECT
public:
    explicit TableView(QWidget *parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

signals:
    void clearRequested();
};

#endif // TABLEVIEW_H

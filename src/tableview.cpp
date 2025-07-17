#include "tableview.h"

#include <QModelIndex>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>

TableView::TableView(QWidget *parent) :
    QTableView(parent)
{
}

void TableView::contextMenuEvent(QContextMenuEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        return;

    QMenu menu(tr("Context Menu"));
    QAction* removeAct = new QAction(tr("&Remove"), this);
    QAction* removeAllAct = new QAction(tr("Remove &All"), this);

    menu.addAction(removeAct);
    menu.addAction(removeAllAct);

    QAction* act = menu.exec(event->globalPos());
    if ( act == removeAct)
        this->model()->removeRow(index.row(), QModelIndex());
    else if (act == removeAllAct)
        emit clearRequested();
}


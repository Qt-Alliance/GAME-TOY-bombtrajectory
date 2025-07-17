#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "displaywidget.h"

namespace Ui {
    class MainWindow;
}

class FlingModel;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;
    FlingModel* m_flingModel;
    void readSettings();
    void writeSettings();

private slots:
    void about();
    void onThrowButtonPressed();
};

#endif // MAINWINDOW_H

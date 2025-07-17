#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flingmodel.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());

    m_flingModel = new FlingModel(this);
    ui->flingView->setModel(m_flingModel);
    ui->displayWidget->setModel(m_flingModel);
    ui->displayWidget->updateAreaWidth(ui->areaWidthSpinBox->value());
    ui->displayWidget->updateAreaHeight(ui->areaHeightSpinBox->value());

    connect(ui->throwButton, SIGNAL(released()), this, SLOT(onThrowButtonPressed()));
    connect(ui->gravitySpinBox, SIGNAL(valueChanged(double)), ui->displayWidget, SLOT(updateGravity(double)));
    connect(ui->areaWidthSpinBox, SIGNAL(valueChanged(double)), ui->displayWidget, SLOT(updateAreaWidth(double)));
    connect(ui->areaHeightSpinBox, SIGNAL(valueChanged(double)), ui->displayWidget, SLOT(updateAreaHeight(double)));
    connect(ui->clearAct, SIGNAL(triggered()), m_flingModel, SLOT(clear()));
    connect(m_flingModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), ui->displayWidget, SLOT(update()));
    connect(m_flingModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)), ui->displayWidget, SLOT(update()));
    connect(m_flingModel, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), ui->displayWidget, SLOT(update()));
    connect(m_flingModel, SIGNAL(modelReset()), ui->displayWidget, SLOT(update()));
    connect(ui->flingView, SIGNAL(clearRequested()), m_flingModel, SLOT(clear()));

    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    QWidget::closeEvent(event);
}

void MainWindow::about()
{
    QMessageBox::about(this,
                       tr("About"),
                       tr("%1 %2 (C) 2009 by Mohammad Mehdi Salem Naraghi. This program is licensed under GPLv3 or any later version.")
                       .arg(QCoreApplication::applicationName())
                       .arg(QCoreApplication::applicationVersion()));
}


void MainWindow::onThrowButtonPressed()
{
    m_flingModel->add(ui->velocitySpinBox->value(), ui->angleSpinBox->value());
}

void MainWindow::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("mainwindow/geometry").toByteArray());
    restoreState(s.value("mainwindow/state").toByteArray());
    ui->velocitySpinBox->setValue(s.value("mainwindow/velocity", 15).toDouble());
    ui->angleSpinBox->setValue(s.value("mainwindow/angle", 45).toDouble());
    ui->gravitySpinBox->setValue(s.value("mainwindow/gravity", 9.8).toDouble());
    ui->areaWidthSpinBox->setValue(s.value("mainwindow/areawidth", 30).toDouble());
    ui->areaHeightSpinBox->setValue(s.value("mainwindow/areaheight", 30).toDouble());
}

void MainWindow::writeSettings()
{
    QSettings s;
    s.setValue("mainwindow/geometry", saveGeometry());
    s.setValue("mainwindow/state", saveState());
    s.setValue("mainwindow/velocity", ui->velocitySpinBox->value());
    s.setValue("mainwindow/angle", ui->angleSpinBox->value());
    s.setValue("mainwindow/gravity", ui->gravitySpinBox->value());
    s.setValue("mainwindow/areawidth", ui->areaWidthSpinBox->value());
    s.setValue("mainwindow/areaheight", ui->areaHeightSpinBox->value());

    ui->displayWidget->writeSettings();
}

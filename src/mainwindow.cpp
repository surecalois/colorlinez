#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "colorlinez_scene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->setMargin(0);
    colorlinez_scene *scene = new colorlinez_scene(QRectF(0,0,12*60,9*60),this);
    ui->statusBar->showMessage(tr("Ready"));
    ui->graphicsView->setScene(scene);
    this->setFixedSize((12+2)*60,(9+2)*60);

    connect(scene,SIGNAL(send_message(QString)),
            this,SLOT(show_received_message(QString)));

    connect(scene,SIGNAL(make_quit()),this,SLOT(close()));
}

void MainWindow::show_received_message(QString s)
{
    ui->statusBar->showMessage(s);
}

MainWindow::~MainWindow()
{
    delete ui;
}

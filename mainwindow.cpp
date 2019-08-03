#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fightfield.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FightField *fight_field = new FightField;
    this->setCentralWidget(fight_field);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "mainwindow.h"
#include "fightfield.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug()<<QApplication::applicationPid();

    FightField fight_field;
    fight_field.show();
    return a.exec();
}

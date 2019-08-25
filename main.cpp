#include "fightfield.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug()<<QApplication::applicationPid();

    QWidget *wdg = new QWidget;
    QVBoxLayout *lay =new QVBoxLayout;
    wdg->setLayout(lay);
    QPushButton *btn = new QPushButton("hello bitches");
    FightField *fight_field = new FightField(20, 30);
    lay->addWidget(btn);
    lay->addWidget(fight_field);

    wdg->show();
    return a.exec();
}

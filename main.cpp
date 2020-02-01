#include "seafightfield.h"
#include "view.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    qDebug()<<QApplication::applicationPid();

//    QWidget *wdg = new QWidget;
//    QVBoxLayout *lay =new QVBoxLayout;
//    wdg->setLayout(lay);
//    QPushButton *btn = new QPushButton("hello bitches");
//    QFont font("helvetica", 12);
//    SeaFightField *fight_field = new SeaFightField(8, 30, font);
//    //fight_field->highlight_cell(true);
//    fight_field->drag_ship(true);
//    //lay->addWidget(btn);
//    lay->addWidget(fight_field);

//    wdg->show();
    View *view = new View();
    view->show();

    return a.exec();
}

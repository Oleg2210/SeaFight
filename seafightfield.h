#ifndef SEAFIGHTFIED_H
#define SEAFIGHTFIED_H

#include "fightfield.h"

class SeaFightField: public FightField
{
    Q_OBJECT

public:
    SeaFightField(int padding=30, int column_size=30, QFont font=QApplication::font(), QWidget *parent = nullptr);
    ~SeaFightField();
};

#endif // SEAFIGHTFIED_H

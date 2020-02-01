#include "view.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>

View::View(QWidget *parent):
    QMainWindow(parent)
{
    startInstrucntionLabel = new QLabel(tr("To play input host adress and port of your opponent(your port is 00000)."));
    QLabel *startHostLabel = new QLabel(tr("Host adress"));
    QLabel *startPortLabel = new QLabel(tr("Port number"));
    startHostEdit = new QLineEdit;
    startPortEdit = new QLineEdit;
    QPushButton *startConnectButton = new QPushButton(tr("Connect"));
    QGridLayout *startLayout = new QGridLayout;

    startLayout->setVerticalSpacing(14);
    startInstrucntionLabel->setMinimumHeight(60);
    startHostEdit->setFixedWidth(140);
    startPortEdit->setFixedWidth(100);

    startLayout->addWidget(startInstrucntionLabel, 0, 0, 1, 4);
    startLayout->addWidget(startHostLabel, 2, 0);
    startLayout->addWidget(startHostEdit, 2, 1);
    startLayout->addWidget(startPortLabel, 2, 2);
    startLayout->addWidget(startPortEdit, 2, 3);
    startLayout->addWidget(startConnectButton, 3, 3, 1, 1);

    startWidget=new QWidget(this);
    startWidget->setLayout(startLayout);
    startWidget->setFixedSize(startLayout->sizeHint());

    connect(startConnectButton, SIGNAL(clicked()), this, SLOT(connectButtonClicked()));
    setCentralWidget(startWidget);
}

void View::resizeEvent(QResizeEvent *event){
    QSize widgetSize = centralWidget()->size();
    int widgetX = (width() - widgetSize.width()) / 2;
    int widgetY = (height() - widgetSize.height()) / 2;
    centralWidget()->setGeometry(widgetX, widgetY, widgetSize.width(), widgetSize.height());
}

void View::connectButtonClicked(){
    qDebug()<<"ok";
}


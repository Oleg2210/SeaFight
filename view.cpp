#include "view.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QRegularExpression>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QApplication>
#include <QDesktopWidget>


View::View(QWidget *parent):
    QMainWindow(parent)
{   _my_turn = false;
    setUpConnectionWidget();
}

void View::setPortNumber(quint16 port_number){
    _port_number = port_number;
    setUpConnectionWidget();
}

void View::setUpConnectionWidget(){
    QGridLayout *start_layout = new QGridLayout;
    setUpConnectionLayout(start_layout);
    _start_widget = new QWidget(this);
    _start_widget->setLayout(start_layout);
    _start_widget->setFixedSize(start_layout->sizeHint());

    connect(_start_connect_button, SIGNAL(clicked()), this, SLOT(connectButtonClicked()));
    setCentralWidget(_start_widget);
    resizeMainWindow(_start_widget->sizeHint());
}

void View::setUpConnectionLayout(QGridLayout *layout){
    QString instuction = "To play input host adress and port of your opponent(your port is %port_numb).";
    instuction.replace("%port_numb", QString::number(_port_number));
    _start_instrucntion_label = new QLabel(tr(qPrintable(instuction)));

    QLabel *start_host_label = new QLabel(tr("Host adress"));
    QLabel *start_port_label = new QLabel(tr("Port number"));
    _start_host_edit = new QLineEdit;
    _start_port_edit = new QLineEdit;
    _start_connect_button = new QPushButton(tr("Connect"));

    layout->setVerticalSpacing(14);
    _start_instrucntion_label->setMinimumHeight(60);
    _start_host_edit->setFixedWidth(140);
    _start_port_edit->setFixedWidth(100);

    layout->addWidget(_start_instrucntion_label, 0, 0, 1, 4);
    layout->addWidget(start_host_label, 2, 0);
    layout->addWidget(_start_host_edit, 2, 1);
    layout->addWidget(start_port_label, 2, 2);
    layout->addWidget(_start_port_edit, 2, 3);
    layout->addWidget(_start_connect_button, 3, 3, 1, 1);
}

void View::setUpBattleWidget(){
    _battle_widget = new QWidget(this);
    QGridLayout *battle_layout = new QGridLayout;
    setUpBattleLayout(battle_layout);
    _your_fight_field->dragShip(true);
    connect(_battle_shuffle_button, SIGNAL(clicked()), this, SLOT(shuffleButtonClicked()));
    connect(_battle_ready_button, SIGNAL(clicked()), this, SLOT(readyButtonClicked()));
    connect(_enemies_fight_field, SIGNAL(cellPressed(int)), this, SLOT(enemyFieldMousePressed(int)));

    _battle_widget->setLayout(battle_layout);
    _battle_widget->setFixedSize(battle_layout->sizeHint());
    setCentralWidget(_battle_widget);
    resizeMainWindow(_battle_widget->sizeHint());
}

void View::setUpBattleLayout(QGridLayout *layout){
    layout->setSpacing(0);
    _your_fight_field = new SeaFightField(true, _battle_widget);
    _enemies_fight_field = new SeaFightField(false, _battle_widget);

    _battle_shuffle_button = new QPushButton(tr("shuffle"));
    _battle_ready_button = new QPushButton(tr("ready"));
    _battle_state_label = new QLabel();
    _battle_shuffle_button->setFixedSize(QSize(132, 30));
    _battle_ready_button->setFixedSize(131, 30);
    toggleBattleStateLabel(false, "Opponent is preparing");

    layout->addWidget(_your_fight_field, 0, 0, 1, 15);
    layout->addWidget(_enemies_fight_field, 0, 15, 1, 15);
    layout->addWidget(_battle_shuffle_button, 1, 4);
    layout->addWidget(_battle_ready_button, 1, 10);
    layout->addWidget(_battle_state_label, 1, 22);
}

void View::resizeMainWindow(const QSize size){
    setFixedSize(size);
    setMaximumSize(QApplication::desktop()->size());
}

void View::resizeEvent(QResizeEvent *event){
    QSize widgetSize = centralWidget()->size();
    int widgetX = (width() - widgetSize.width()) / 2;
    int widgetY = (height() - widgetSize.height()) / 2;
    centralWidget()->setGeometry(widgetX, widgetY, widgetSize.width(), widgetSize.height());
    QWidget::resizeEvent(event);
}

void View::commandFromModel(QJsonObject json_obj){
    if(json_obj["command"] == SFcom::Commands::ERROR){
        errorNotify(json_obj);
    }else if(json_obj["command"] == SFcom::Commands::LETUSPLAY){
        letUsPlayNotify(json_obj);
    }else if(json_obj["command"] == SFcom::Commands::READY){
        readinessCheck(json_obj);
    }
    else if(json_obj["command"] == SFcom::Commands::STRIKE){
        strikeResult(json_obj);
    }
}

void View::errorNotify(QJsonObject json_obj){
    if(json_obj["payload"].toObject()["phase"] != SFcom::GamePhase::CONNECTION){
        setUpConnectionWidget();
        QString error_string = "connection error occurred";
        if(json_obj["status"] == SFcom::Status::LOGICERROR)
            error_string = "unknown error occurred";
        QMessageBox::critical(nullptr, "", error_string);
    }else{
        _start_connect_button->setEnabled(true);
    }
}

void View::connectButtonClicked(){
    QString ip_input = _start_host_edit->text();
    QString port_input = _start_port_edit->text();
    int port_number = port_input.toInt();

    QRegularExpression port_reg("^[0-9]{1,5}$");
    bool port_state = port_reg.match(port_input).hasMatch();
    port_state = port_state && (port_number <= 65535) && (port_number > 0);
    bool ip_state = !QHostAddress(ip_input).isNull();

    if(ip_state && port_state){
        QJsonObject payload = QJsonObject{{"IP", ip_input}, {"port", port_number}};
        QJsonObject json_obj = SFcom::createJsonCommand(SFcom::Commands::LETUSPLAY, SFcom::Status::REQUEST, payload);
        emit commandToModel(json_obj);
        _start_connect_button->setEnabled(false);
    }else{
        QMessageBox::information(_start_widget, "", tr("IP adress or port number are wrong!"));
    }
}


void View::shuffleButtonClicked(){
    _battle_shuffle_button->setDisabled(true);
    QThread::msleep(500);
    _your_fight_field->randomArrangement();
    _your_fight_field->update();
    _battle_shuffle_button->setDisabled(false);
}

void View::readyButtonClicked(){
    _battle_shuffle_button->setDisabled(true);
    _battle_ready_button->setDisabled(true);
    _your_fight_field->dragShip(false);

    QJsonObject json_obj = SFcom::createJsonCommand(SFcom::Commands::READY, SFcom::Status::REQUEST);
    emit commandToModel(json_obj);
}

void View::enemyFieldMousePressed(int cell_number){
    if(_my_turn && strikeValid(cell_number)){
        _my_turn = false;
        QJsonObject payload = QJsonObject{{"cell_number", cell_number}};
        emit commandToModel(SFcom::createJsonCommand(SFcom::Commands::STRIKE, SFcom::Status::REQUEST, payload));
        toggleBattleStateLabel(false, "Waiting for the opponent's answer");
    }
}

bool View::strikeValid(int cell_number){
    return true;
}

void View::letUsPlayNotify(QJsonObject json_obj){
    QString peer_ip = json_obj["payload"].toObject()["IP"].toString();
    if(json_obj["status"] == SFcom::Status::REQUEST){
        QString question = "User with IP \"" + peer_ip + "\" invites you to play. What will you answer?";
        QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Game request"), tr(qPrintable(question)),
                                                                QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::Yes){
            json_obj["status"] = SFcom::Status::OK;
            setUpBattleWidget();
        }
        else
            json_obj["status"] = SFcom::Status::NO;

        emit commandToModel(json_obj);

    }else if(json_obj["status"] == SFcom::Status::OK){
        setUpBattleWidget();
    }else if(json_obj["status"] == SFcom::Status::NO){
        QMessageBox::information(_start_widget, "", tr("Your offer was rejected."));
    }
}

void View::readinessCheck(QJsonObject obj){
    if(obj["status"] == SFcom::Status::OK){
        _my_turn = obj["payload"].toObject()["your_turn"].toBool();
        toggleBattleStateLabel(_my_turn, "");
        highlightField();
    }else if(obj["status"] == SFcom::Status::REQUEST){
        toggleBattleStateLabel(true, "Opponent is ready");
    }
}

void View::toggleBattleStateLabel(bool ready_state, QString label_text){
    if(!label_text.length()){
        QString turn_state = (ready_state) ? "your" : "opponent's";
        label_text = "now it is " + turn_state + " turn";
    }
    QString style_string = "QLabel {color : %color; font-size: 17px;}";
    QString label_color = (ready_state) ? "green" : "orange";
    style_string.replace("%color", label_color);
    _battle_state_label->setStyleSheet(style_string);
    _battle_state_label->setText(tr(qPrintable(label_text)));
}

void View::strikeResult(QJsonObject obj){
    _your_fight_field->update();
    _enemies_fight_field->update();
    QJsonObject payload = obj["payload"].toObject();
    bool turn = !(obj["status"] == SFcom::Status::MISS);
    bool my_request = payload["my_request"].toBool();
    _my_turn = ((turn && my_request) || (!turn && !my_request));
    toggleBattleStateLabel(_my_turn, "");
    highlightField();
}

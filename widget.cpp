#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    playList = new QMediaPlaylist();
    playList->addMedia(QUrl("qrc:/sounds/backgroundSound.mp3"));
    playList->setPlaybackMode(QMediaPlaylist::Loop);//reproduce el sonido del playList en modo bucle

    backgroundSound = new QMediaPlayer();
    backgroundSound->setPlaylist(playList);
    backgroundSound->play();

    buttonPressedSound = new QMediaPlayer();
    buttonPressedSound->setMedia(QUrl("qrc:/sounds/buttonPressedSound.mp3"));
    //Conexiones de los botones con el sonido
    connect(ui->playButton,SIGNAL(clicked(bool)),buttonPressedSound,SLOT(play()));
    connect(ui->LeaveButton,SIGNAL(clicked(bool)),buttonPressedSound,SLOT(play()));
    connect(ui->nextLevelButton,SIGNAL(clicked(bool)),buttonPressedSound,SLOT(play()));
    connect(ui->menuButton,SIGNAL(clicked(bool)),buttonPressedSound,SLOT(play()));

    winnerSound = new QMediaPlayer();
    winnerSound->setMedia(QUrl("qrc:/sounds/wellDoneSound.mp3"));

    invalidMovementSound = new QMediaPlayer();
    invalidMovementSound->setMedia(QUrl("qrc:/sounds/invalidMovementSound.mp3"));

     diskMovedSound = new QMediaPlayer();
     diskMovedSound->setMedia(QUrl("qrc:/sounds/moveDiskSound.mp3"));

    ui->playButton->installEventFilter(this);
    ui->LeaveButton->installEventFilter(this);
    ui->menuButton->installEventFilter(this);
    ui->nextLevelButton->installEventFilter(this);
    ui->resetLevelButton->installEventFilter(this);
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == (QObject*)ui->playButton || obj == (QObject*)ui->LeaveButton || obj == (QObject*)ui->menuButton ||obj == (QObject*)ui->nextLevelButton || obj == (QObject*)ui->resetLevelButton){
        if (event->type() == QEvent::Enter)
        {
            hoverSound = new QMediaPlayer;//reproducir el hover de los botones
            hoverSound->setMedia(QUrl("qrc:/sounds/hoverButtonSound.mp3"));
            hoverSound->play();
        }
        return QWidget::eventFilter(obj, event);
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::startLevel()
{
    hideInvalidMovementLabel();

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),ui->invalidMovementLabel,SLOT(hide()));//conecta el timer con el label de movimiento invalido

    hideEndGameLabel();
    hideNextLevelButton();
    hideWellDoneLabel();
    Disk::movementCounter = 0;
    ui->movementCounterLabel->setNum(Disk::movementCounter);
    updateCounterLevels(Disk::totalDiscs - 2);
    Disk::towers.clear();
    scene = new Scene(this);
    QLineF top(scene->sceneRect().topLeft(),scene->sceneRect().topRight());
    QLineF bottom(scene->sceneRect().bottomLeft(),scene->sceneRect().bottomRight());
    QLineF left(scene->sceneRect().topLeft(),scene->sceneRect().bottomLeft());
    QLineF right(scene->sceneRect().topRight(),scene->sceneRect().bottomRight());
    QPen pen = QPen(Qt::blue);
    pen.setWidth(3);
    scene->addLine(top,pen);
    scene->addLine(bottom,pen);
    scene->addLine(left,pen);
    scene->addLine(right,pen);

    //Conectando señal de las torres con la ranura del Label
    connect(Disk::towers.at(0),SIGNAL(diskMoved(int)),ui->movementCounterLabel,SLOT(setNum(int)));
    connect(Disk::towers.at(1),SIGNAL(diskMoved(int)),ui->movementCounterLabel,SLOT(setNum(int)));
    connect(Disk::towers.at(2),SIGNAL(diskMoved(int)),ui->movementCounterLabel,SLOT(setNum(int)));

    //conectando la torre C con el sonido Well done
    connect(Disk::towers.at(2),SIGNAL(winner()),winnerSound,SLOT(play()));

    //conectando la señal del sonido ganar conlas torres
    connect(Disk::towers.at(0),SIGNAL(invalidMovement()),invalidMovementSound,SLOT(play()));
    connect(Disk::towers.at(1),SIGNAL(invalidMovement()),invalidMovementSound,SLOT(play()));
    connect(Disk::towers.at(2),SIGNAL(invalidMovement()),invalidMovementSound,SLOT(play()));

    //conectando el sonido al mover un disco
    connect(Disk::towers.at(0),SIGNAL(diskMoved(int)),diskMovedSound,SLOT(play()));
    connect(Disk::towers.at(1),SIGNAL(diskMoved(int)),diskMovedSound,SLOT(play()));
    connect(Disk::towers.at(2),SIGNAL(diskMoved(int)),diskMovedSound,SLOT(play()));

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignLeft);
}

void Widget::activateInvalidMovementLabel()
{
    //Se muestra el Label y se oculta luego de 3 segundos
    showInvalidMovementLabel();
    timer->start(700);
    /*
    showInvalidMovementLabel();//OTRO METODO MAS SENCILLO SIN NECESIDAD DE CREAR UN OBJETO TIMER
    QTimer::singleShot(700,ui->invalidMovementLabel,SLOT(hide()));*/
}

void Widget::updateCounterLevels(qint32 count)
{
    ui->levelCounterLabel->setText(QString::number((count)));
}

void Widget::showNextLevelButton()
{
    ui->nextLevelButton->setVisible(true);
}

void Widget::hideNextLevelButton()
{
    ui->nextLevelButton->setVisible(false);
}

void Widget::showWellDoneLabel()
{
    ui->wellDoneLabel->setVisible(true);
}

void Widget::hideWellDoneLabel()
{
    ui->wellDoneLabel->setVisible(false);
}

void Widget::showEndGameLabel()
{
    ui->endGameLabel->setVisible(true);
}

void Widget::hideEndGameLabel()
{
    ui->endGameLabel->setVisible(false);
}

void Widget::showInvalidMovementLabel()
{
    ui->invalidMovementLabel->setVisible(true);
    //ui->invalidMovementLabel->setVisible(true);
}

void Widget::hideInvalidMovementLabel()
{
    ui->invalidMovementLabel->setVisible(false);
    //ui->invalidMovementLabel->setVisible(false);
}

void Widget::on_nextLevelButton_clicked()
{
    Disk::totalDiscs += 1;
    startLevel();
}

void Widget::on_resetLevelButton_clicked()
{
    if(buttonPressedSound->state() == QMediaPlayer::PlayingState){
        buttonPressedSound->setPosition(0);
    }else{
        buttonPressedSound->play();
    }
    Tower::selectedDiskIndex = -1;
    startLevel();
}

void Widget::on_playButton_clicked()
{
    Disk::totalDiscs = 3;
    Tower::selectedDiskIndex = -1;
    startLevel();
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_menuButton_clicked()
{
    QString text = "Si vuelve al menú principal se perderá todo el progreso\n¿Desea continuar?";
    QMessageBox::StandardButton reply = QMessageBox::warning(this,"Atención",text,QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void Widget::on_LeaveButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Atención","¿Está seguro que quiere salir?");
    if(reply == QMessageBox::Yes){
        QApplication::quit();
    }
}

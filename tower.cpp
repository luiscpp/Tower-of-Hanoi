#include "tower.h"
#include "widget.h"
#include <QPainter>
#include <QDebug>
#include <QThread>

#define DISCS_TO_END 6

Widget *Tower::widgetPointer;
qint32 Tower::selectedDiskIndex = -1;

void Tower::desactivateTowers()
{
    for(int i=0; i<3;i++){
        Disk::towers.at(i)->setEnabled(false);
    }
}

bool Tower::win()
{
    return Disk::towers.at(2)->discs.size() == Disk::totalDiscs;
}

bool Tower::isEndGame()
{
    return Disk::towers.at(2)->discs.size() == DISCS_TO_END;
}

void Tower::endGame()
{
    Disk::desactivateDiscs();
    Tower::desactivateTowers();
    Tower::widgetPointer->showWellDoneLabel();
    Tower::widgetPointer->showEndGameLabel();
}

void Tower::nextLevel()
{
    Disk::desactivateDiscs();
    Tower::desactivateTowers();
    Tower::widgetPointer->showNextLevelButton();
    Tower::widgetPointer->showWellDoneLabel();
    Tower::selectedDiskIndex = -1;
}

Tower::Tower(QSizeF size)
{
    this->size = size;
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Tower::setSize(QSizeF size)
{
    this->size = size;
}

QSizeF Tower::getSize()
{
    return this->size;
}

void Tower::setTowerNumber(qint32 index)
{
    this->towerNumber = index;
}

qint32 Tower::getTowerNumber()
{
    return this->towerNumber;
}

void Tower::setSelected(bool selected)
{
    this->selected = selected;
}

bool Tower::getSelected()
{
    return this->selected;
}

QStack<Disk*> Tower::getStackDisk()
{
    return discs;
}

void Tower::insertDisk(Disk *disk)
{
    this->discs.push(disk);
}

//posXTower+(WidthTower/2)-(WidthDisk/2)
void Tower::moveDisk()
{
    double x = pos().rx() + (getSize().rwidth()/2) - (Disk::towers.at(Tower::selectedDiskIndex)->discs.top()->getSize().rwidth()/2);
    double y = 485-getVerticalPixels();//485 es la altura exacta para el primer elemento en caso de que esté vacia
    Disk::towers.at(Tower::selectedDiskIndex)->discs.top()->setPos(x,y);
    Disk::towers.at(Tower::selectedDiskIndex)->discs.top()->setAssociatedTower(towerNumber);
    Disk::towers.at(Tower::selectedDiskIndex)->discs.top()->setSelected(false);
    discs.push(Disk::towers.at(Tower::selectedDiskIndex)->discs.pop());
    Tower::selectedDiskIndex = -1;//OJETE CON ESTE INDICE
    Disk::movementCounter += 1;

    emit diskMoved(Disk::movementCounter);//Emitiendo señal de que un disco en la torre ha sido movido
    //Tower::widgetPointer->updateCounterMovements(Disk::movementCounter); REEMPLAZADO POR LA SEÑAL
}

double Tower::getVerticalPixels()
{
    double summatory = 0;
    for(int i=0;i<discs.size();i++){
        summatory += 50;/////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    return summatory;
}


bool Tower::validMovement()
{
    if(discs.isEmpty() || Disk::towers.at(Tower::selectedDiskIndex)->discs.top()->getSize().rwidth() < discs.top()->getSize().rwidth()){
        return true;

    }
    return false;
}

QRectF Tower::boundingRect() const{
    return QRectF(QPointF(0,0),size);//tamaño (dimensiones weigth - high), el punto lo establece setPos
}

void Tower::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    QLineF top(boundingRect().topLeft(),boundingRect().topRight());//dibujando bordes del limitador de la imagen
    QLineF bottom(boundingRect().bottomLeft(),boundingRect().bottomRight());
    QLineF left(boundingRect().topLeft(),boundingRect().bottomLeft());
    QLineF right(boundingRect().topRight(),boundingRect().bottomRight());
    painter->drawLine(top);
    painter->drawLine(bottom);
    painter->drawLine(left);
    painter->drawLine(right);
    scene()->update();
}

void Tower::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(Tower::selectedDiskIndex != -1){
        if(validMovement()){
            moveDisk();
            scene()->update();
            Disk::activateDiscs();
        }else{
            //Movimiento invalido
            emit invalidMovement();
            Disk::towers.at(Tower::selectedDiskIndex)->discs.top()->setSelected(false);
            Disk::activateDiscs();
            Tower::selectedDiskIndex = -1;
            Tower::widgetPointer->activateInvalidMovementLabel();
        }
    }else{
        //qDebug()<<"No hay disco seleccionado!";
    }

    if(Tower::win()){
        emit winner();//Emite la señal de ganar
        if(!Tower::isEndGame()){
            Tower::nextLevel();
        }else{
            Tower::endGame();
        }
    }
}

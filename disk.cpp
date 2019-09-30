#include "disk.h"
#include "scene.h"
#include <QPainter>
#include <QList>

QList<Tower*> Disk::towers;
qint32 Disk::totalDiscs = 3;
qint32 Disk::movementCounter = 0;

void Disk::activateDiscs()
{
    for(int i=0; i<Disk::towers.size();i++){
        if(!Disk::towers.at(i)->getStackDisk().isEmpty()){
            Disk::towers.at(i)->getStackDisk().top()->setEnabled(true);
            Disk::towers.at(i)->getStackDisk().top()->setCursor(Qt::PointingHandCursor);
        }
    }
}

void Disk::desactivateDiscs()
{
    for(int i=0; i<Disk::towers.size();i++){
        if(!Disk::towers.at(i)->getStackDisk().isEmpty()){
            Disk::towers.at(i)->getStackDisk().top()->setEnabled(false);
            Disk::towers.at(i)->getStackDisk().top()->setCursor(Qt::ArrowCursor);
        }
    }
}

Disk::Disk(QSizeF size)
{
    this->size = size;
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Disk::setSize(QSizeF size)
{
    this->size = size;
}

QSizeF Disk::getSize()
{
    return this->size;
}

void Disk::setAssociatedTower(qint32 index)
{
    this->associatedTower = index;
}

qint32 Disk::getAssociatedTower()
{
    return this->associatedTower;
}

void Disk::setSelected(bool selected)
{
    this->selected = selected;
}

bool Disk::getSelected()
{
    return this->selected;
}

void Disk::setColor(QColor color)
{
    this->color = color;
}

QRectF Disk::boundingRect() const{
    return QRectF(QPointF(0,0),size);//tamaño (dimensiones weigth - high), el punto lo establece setPos
}

void Disk::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->fillRect(QRect(0,0,size.rwidth(),size.rheight()),color);//colorea el disco
    QLineF top(boundingRect().topLeft(),boundingRect().topRight());//dibujando bordes del limitador del disco
    QLineF bottom(boundingRect().bottomLeft(),boundingRect().bottomRight());
    QLineF left(boundingRect().topLeft(),boundingRect().bottomLeft());
    QLineF right(boundingRect().topRight(),boundingRect().bottomRight());
    painter->drawLine(top);
    painter->drawLine(bottom);
    painter->drawLine(left);
    painter->drawLine(right);
    scene()->update();
    //Q_UNUSED(option);
    //Q_UNUSED(widget);
}

void Disk::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!getSelected()){
        Disk::desactivateDiscs();
        setEnabled(true);
        Tower::selectedDiskIndex = getAssociatedTower();
        setSelected(true);
        //qDebug()<<"Seleccionado!";
    }else{
        Tower::selectedDiskIndex = -1;
        Disk::activateDiscs();
        setSelected(false);
        //qDebug()<<"Deseleccionado!";
    }
}

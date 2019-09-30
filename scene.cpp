#include "scene.h"
#include <QCursor>
#include <QRandomGenerator>

#define XPOS 0.0
#define YPOS 0.0
#define WIDTH 780.0
#define HEIGHT 597.0

Scene::Scene(QObject *parent): QGraphicsScene(XPOS, YPOS, WIDTH, HEIGHT, parent){
    Tower *A = new Tower(QSizeF(155,455));
    A->setPos(54,80);
    A->setTowerNumber(0);
    addItem(A);
    Disk::towers.append(A);

    Tower *B = new Tower(QSizeF(155,455));
    B->setPos(311,80);
    B->setTowerNumber(1);
    addItem(B);
    Disk::towers.append(B);

    Tower *C = new Tower(QSizeF(155,455));
    C->setPos(568,80);
    C->setTowerNumber(2);
    addItem(C);
    Disk::towers.append(C);

    //agregando discos a la torre A
    double MaxWidth = 220;
    double MaxPosX = 22;
    double MaxPosY = 455+A->pos().ry()-50;

    for(int i=0;i<Disk::totalDiscs;i++){
        std::uniform_int_distribution<int> distribution(0,255);//Agregando color aleatorio
        qint32 red = distribution(*QRandomGenerator::global());
        qint32 green = distribution(*QRandomGenerator::global());
        qint32 blue = distribution(*QRandomGenerator::global());

        Disk *d = new Disk(QSizeF(MaxWidth,50));
        d->setColor(QColor(red,green,blue));
        d->setEnabled(false);
        if(i == Disk::totalDiscs -1){
            d->setEnabled(true);
            d->setCursor(Qt::PointingHandCursor);
        }
        d->setSelected(false);
        d->setPos(MaxPosX,MaxPosY);
        d->setAssociatedTower(0);
        addItem(d);
        A->insertDisk(d);
        MaxWidth -= 40;
        MaxPosX += 20;
        MaxPosY -= 50;
    }
    //54, 311, 568 quitar 32 para MaxPosX
}

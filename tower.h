#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsItem>
#include <QSizeF>
#include <QWidget>
#include <QStack>
#include <QObject>

class Disk;//porque la clase Tower tiene una pila de Disk
class Widget;//porque la clase Tower tiene un Widget

class Tower: public QObject, public QGraphicsItem{//El punto viene dado por la clase heredada QGraphicsItem junto con sus metodos get, set
    Q_OBJECT
private:
    QSizeF size;
    qint32 towerNumber;
    QStack<Disk*> discs;
    bool selected;
public:
    static Widget *widgetPointer;//Miembros estaticos
    static qint32 selectedDiskIndex;

    static void desactivateTowers();//Metodos estaticos
    static bool win();
    static bool isEndGame();
    static void endGame();
    static void nextLevel();

    Tower(QSizeF size);
    void setSize(QSizeF size);
    QSizeF getSize();
    void setTowerNumber(qint32 index);
    qint32 getTowerNumber();
    void setSelected(bool selected);
    bool getSelected();
    QStack<Disk*> getStackDisk();
    void insertDisk(Disk *disk);
    void moveDisk();
    double getVerticalPixels();
    bool validMovement();
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void diskMoved(int);
    void winner();
    void invalidMovement();
};

#endif // TOWER_H

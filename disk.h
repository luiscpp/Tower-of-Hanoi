#ifndef DISK_H
#define DISK_H

#include <QGraphicsItem>
#include <QSizeF>
#include <QList>
#include <QColor>

class Tower;//Porque la clase Disk tiene una lista de Tower

class Disk: public QGraphicsItem{//El punto viene dado por la clase heredada QGraphicsItem junto con sus metodos get, set
private:
    QSizeF size;
    qint32 associatedTower;
    QColor color;
    bool selected;
public:
    static QList<Tower*> towers;//Miembros estaticos
    static qint32 totalDiscs;
    static qint32 movementCounter;

    static void activateDiscs();//Metodos estaticos
    static void desactivateDiscs();

    Disk(QSizeF size);
    void setSize(QSizeF size);
    QSizeF getSize();
    void setAssociatedTower(qint32 index);
    qint32 getAssociatedTower();    
    void setSelected(bool selected);
    bool getSelected();
    void setColor(QColor color);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // DISK_H

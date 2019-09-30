#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "disk.h"
#include "tower.h"

class Scene : public QGraphicsScene{
public:
    Scene(QObject *parent = nullptr);
};


#endif // SCENE_H

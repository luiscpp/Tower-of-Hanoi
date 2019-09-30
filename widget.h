#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "disk.h"
#include "scene.h"
#include "tower.h"
#include <QTimer>
#include <QMediaPlaylist>
#include <QMediaPlayer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    bool eventFilter(QObject *obj, QEvent *event);//Para hacer el hover a los botones
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void startLevel();
    /*void updateCounterMovements(qint32 count);
    FUE REEMPLAZADO POR UNA SEÑAL PARA NO USAR UNA REFERENCIA AL WIDGET (*widgetPointer) DESDE LA CLASE TOWER*/
    void activateInvalidMovementLabel();
    void updateCounterLevels(qint32 count);
    void showNextLevelButton();
    void hideNextLevelButton();
    void showWellDoneLabel();
    void hideWellDoneLabel();
    void showEndGameLabel();
    void hideEndGameLabel();
    void showInvalidMovementLabel();
    void hideInvalidMovementLabel();
private slots:
    void on_nextLevelButton_clicked();

    void on_resetLevelButton_clicked();

    void on_playButton_clicked();

    void on_menuButton_clicked();

    void on_LeaveButton_clicked();
private:
    Ui::Widget *ui;
    Scene *scene;
    QTimer *timer;
    QMediaPlaylist *playList;
    QMediaPlayer *backgroundSound;
    QMediaPlayer *buttonPressedSound;
    QMediaPlayer *winnerSound;
    QMediaPlayer *hoverSound;
    QMediaPlayer *invalidMovementSound;
    QMediaPlayer *diskMovedSound;
};

#endif // WIDGET_H

#ifndef COLORLINEZ_SCENE_H
#define COLORLINEZ_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "colorlinez2.h"

class colorlinez_scene:public QGraphicsScene
{
    Q_OBJECT
public:
    colorlinez_scene(QRectF rect, QObject *parent);
	
private:

    int grid_size[2]={60,60};
    int net_size[2]={12,9};
    colorlinez2 *model;

    int active_pos[2]={-1,-1};
    QGraphicsEllipseItem *active;

    void draw_grids();
    void draw_matrix();
    void update_iterms();
    void infor();
    void game_over();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void send_message(QString);
    void make_quit();
};

#endif // COLORLINEZ_SCENE_H

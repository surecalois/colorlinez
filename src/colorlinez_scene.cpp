#include "colorlinez_scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <cmath>
#include <QDebug>

colorlinez_scene::colorlinez_scene(QRectF rect,QObject *parent ):QGraphicsScene(rect,parent)
{
    model = new colorlinez2(net_size[0],net_size[1]);
    model->player_add();
    draw_matrix();
}

void colorlinez_scene::draw_grids(){
    int width = this->width();
    int height = this->height();
    int width_step = this->width()/(net_size[0]);
    int height_step = this->height()/(net_size[1]);

    for(int ii = 0;ii <= net_size[0];ii++){
        this->addLine(ii*width_step,0,ii*width_step,height);
    }
    for(int ii = 0;ii <= net_size[1];ii++){
           this->addLine(0,ii*height_step,width,ii*height_step);
    }
    grid_size[0] = width_step;
    grid_size[1] = height_step;
    QPen pen;
    pen.setWidth(3);
    pen.setBrush(QBrush(Qt::black));
    active = this->addEllipse(0,0,width_step,height_step, pen);
    active->setVisible(false);
}

void colorlinez_scene::draw_matrix()
{
    this->clear();
    draw_grids();
    model->change.clear();
    MAT map = model->get_matrix();
    QBrush brushs[]={QBrush(),
                     QBrush(Qt::red),
                     QBrush(Qt::green),
                     QBrush(Qt::blue),
                     QBrush(Qt::yellow),
                     QBrush(Qt::cyan),
                     QBrush(Qt::magenta),
                     QBrush(QColor().fromRgb(139,69,19))};
    //what is the last color for? it is brown
    int d = 5;
    for(int x = 0;x < net_size[0];x++){
        for(int y = 0;y < net_size[1];y++){
            if(map[y][x] > 0){
               QGraphicsEllipseItem *p = this->addEllipse(d,d,grid_size[0]-2*d,grid_size[1]-2*d);
               p->setPos(x*grid_size[0],y*grid_size[1]);
               p->setBrush(brushs[map[y][x]]);
            }
        }
    }
}

void colorlinez_scene::infor()
{
    int score = model->get_score();
    QString question = "Your score is "+QString::number(score)+". Restart?";

    QMessageBox::StandardButton ans = QMessageBox::question(NULL, "Score",
               question,QMessageBox::Yes|QMessageBox::No,QMessageBox::No);

    if(ans == QMessageBox::Yes) {
            model->reset();
            model->player_add();
            draw_matrix();//because the change is empty, update won't work.
    }
}

void colorlinez_scene::game_over()
{
    QString question = "Your score is "+QString::number(model->get_score())+". Close the Game?";

    QMessageBox::StandardButton ans = QMessageBox::question(NULL, "Game Over",
               question,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);

    if(ans == QMessageBox::Yes) {
          emit make_quit();
    }else{
        model->reset();
        model->player_add();
        draw_matrix();//because the change is empty, update won't work.
    }
}


void colorlinez_scene::update_iterms(){
    active->setVisible(false);
    QBrush brushs[]={QBrush(),
                     QBrush(Qt::red),
                     QBrush(Qt::green),
                     QBrush(Qt::blue),
                     QBrush(Qt::yellow),
                     QBrush(Qt::cyan),
                     QBrush(Qt::magenta),
                     QBrush(QColor().fromRgb(139,69,19))};
    //what is the last color for? it is brown
    int d = 5;

    for(unsigned int ii = 0; ii < model->change.size(); ii+=3){
        int x = model->change[ii];
        int y = model->change[ii+1];
        int value = model->change[ii+2];
        //qDebug()<<x<<y<<value;
        if(value != 0){

            QGraphicsEllipseItem *p = this->addEllipse(d,d,grid_size[0]-2*d,grid_size[1]-2*d);
            p->setPos(x*grid_size[0],y*grid_size[1]);
            if(value > 7) value = 0;
            p->setBrush(brushs[value]);

        }else{

            QGraphicsItem *p = this->itemAt((x+0.5)*grid_size[0],
                                        (y+0.5)*grid_size[1],
                                         QTransform());
            if(p!=NULL){
                this->removeItem(p);
                p->~QGraphicsItem();
            }
        }
    }
    model->change.clear();
    send_message(QString::number(model->get_score()));
}


void colorlinez_scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qreal x = event->scenePos().x();
    qreal y = event->scenePos().y();
    int gx = floor(x/grid_size[0]);
    int gy = floor(y/grid_size[1]);

    if(x > 0 && x < this->width() && y > 0 && y < this->height()){

        if(active_pos[0] == gx && active_pos[1] == gy){
            //click the same piece, cancel the selection
            active->setVisible(false);
            active_pos[0] = -1;
            active_pos[1] = -1;
            return;
        }else{

            QGraphicsItem *p = this->itemAt(
                        (gx+0.5)*grid_size[0],
                        (gy+0.5)*grid_size[1],
                        QTransform());

            if(p!=NULL){

                    //selection the piece
                    active_pos[0] = gx;
                    active_pos[1] = gy;
                    active->setPos(gx*grid_size[0],gy*grid_size[1]);
                    active->setVisible(true);
             }else{

                //click an empty cell

                if(active_pos[0] != -1 && active_pos[0] != -1){
                    int z = model->get_score();
                    int legal_move = model->player_move(active_pos[0],active_pos[1],gx,gy);
                    active->setVisible(false);
                    active_pos[0] = -1;
                    active_pos[1] = -1;

                    if(legal_move==0){
                        if(z == model->get_score())
                            model->player_add();

                        update_iterms();
                        if(model->gameover()) game_over();
                    }
                    else send_message(tr("bad move!"));

                }else send_message(tr("emtyp clikced"));
            } //end of empty cell click
        }//end of the same cell click
    }else infor();//click outside the grid will go here.
}


#ifndef COLORLINEZ2_H
#define COLORLINEZ2_H

#include <vector>

#define MAT std::vector<std::vector<int> >

#include <iostream>

void printm(std::vector<std::vector<int> > mt);


class colorlinez2
{
public:
    std::vector<int> change; //x,y,value

    colorlinez2(int nx=12,int ny=9);
    int player_move(int ox,int oy, int dx, int dy);
    int player_add(int num = 3);
    int get_score();
    MAT get_matrix();
    void reset();
    bool gameover();


private:
    int score;
    int total;
    int x_size;
    int y_size;
    MAT matrix;

    bool connected(int x1,int y1, int x2,int y2);
    void grow_region(MAT &map,int x, int y);
    int check_matrix(int x, int y);
    int player_add(int x,int y,int c);//only for testing
};

#endif // COLORLINEZ2_H

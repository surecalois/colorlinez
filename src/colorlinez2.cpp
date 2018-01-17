#include "colorlinez2.h"
#include <cstdlib>
#include <ctime>

#include <QDebug>

#include <iostream>
#include <vector>

void printm(MAT mt){
    int x = mt.size();
    int y = mt[0].size();
    for(int ii = 0;ii<x;ii++){
        for(int jj = 0;jj<y;jj++){
            std::cout<<mt[ii][jj]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"================"<<std::endl;
}


colorlinez2::colorlinez2(int nx, int ny)
{
    x_size = nx;
    y_size = ny;
    score = 0;
    total = 0;
    std::vector<int> tmp;

    for(int ii = 0;ii<nx;ii++)
        tmp.push_back(0);

    for(int jj = 0;jj< ny;jj++)
            matrix.push_back(tmp);
}

MAT colorlinez2::get_matrix(){
    return matrix;
}

int colorlinez2::get_score(){
    return score;
}

bool colorlinez2::gameover(){
    bool a = total >= x_size*y_size - 1;

    return a;
}

void colorlinez2::reset(){
    score = 0;
    total = 0;
    change.clear();
    for(int jj = 0;jj< y_size;jj++){
        for(int ii = 0;ii<x_size;ii++)
            matrix[jj][ii]=0;
    }
}


int colorlinez2::player_add(int x,int y,int c){
    if(matrix[y][x] == 0 && c>=1 && c<=7 ) {
        matrix[y][x] = c;
        return 0;
    }else{
        return 1;
    }
}

int colorlinez2::player_add(int n)
{
    srand(time(NULL));
    while(n>0 && total < x_size*y_size){
        int x = rand()%x_size;
        int y = rand()%y_size;
        int c = 1 + rand()%7;


        if(matrix[y][x] == 0){
            matrix[y][x] = c;
            total++;
            //std::cout<<"total =  "<<total<<std::endl;
            n--;
            change.push_back(x);
            change.push_back(y);
            change.push_back(c);
        }
    }
    //if n==0, the operation is done, if n !=0, the game is over
    int luck = 0; //I also calculate the lucky, I don't want to put in the score now.
    unsigned int nn = change.size();
    if(n == 0){
        for(unsigned int ii = 0;ii < nn; ii+=3 )
                luck += check_matrix(change[ii],change[ii+1]);
    }

    return n;
}

bool colorlinez2::connected(int x1,int y1, int x2,int y2){
    MAT map;
    for(int ii = 0;ii < y_size;ii++){
        map.push_back(std::vector<int>());
        for(int jj = 0;jj< x_size;jj++){
            map[ii].push_back(matrix[ii][jj] == 0 ? 1:0);
        }
    }
    map[y1][x1] = 1;
    grow_region(map,x1,y1);
    bool ans = map[y2][x2] == 2;
    //std::cout<<"("<<x1<<","<<y1<<")  ("<<x2<<","<<y2<<") is "<< ans<<std::endl;
    if(!ans){
        //printm(map);
    }
    return ans;
}

void colorlinez2::grow_region(MAT &map, int x, int y){
    if(map[y][x] == 1){
        map[y][x] = 2;
        if(x > 0)        grow_region(map,x-1,y);
        if(x < x_size-1) grow_region(map,x+1,y);
        if(y > 0)        grow_region(map,x,y-1);
        if(y < y_size-1) grow_region(map,x,y+1);
    }
}

int colorlinez2::player_move(int ox, int oy, int dx, int dy){
    if(matrix[dy][dx]!=0) return 1; //if return 0 the operation is done
    if(connected(ox,oy,dx,dy)){
        matrix[dy][dx] = matrix[oy][ox];
        change.push_back(dx);
        change.push_back(dy);
        change.push_back(matrix[oy][ox]);

        matrix[oy][ox] = 0;
        change.push_back(ox);
        change.push_back(oy);
        change.push_back(0);
        score += check_matrix(dx,dy);
        return 0;
    }else{
        return 2;
    }
}

int colorlinez2::check_matrix(int x, int y){
    int value = matrix[y][x];
    if(value == 0 ) return 0;
    int row1 = 0;
    for(int i =x+1; i<x_size;i++){
        if(matrix[y][i]==value) row1++;
        else break;
    }

    int row2 = 0;
    for(int i =x-1; i>=0;i--){
        if(matrix[y][i]==value) row2++;
        else break;
    }

    int col1 = 0;
    for(int i =y+1; i<y_size;i++){
        if(matrix[i][x]==value) col1++;
        else break;
    }

    int col2 = 0;
    for(int i =y-1; i>=0;i--){
        if(matrix[i][x]==value) col2++;
        else break;
    }

    int dig1 = 0;
    for(int i =x+1,j=y+1; i<x_size && j< y_size;i++,j++){
        if(matrix[j][i]==value) dig1++;
        else break;
    }

    int dig2 = 0;
    for(int i =x-1,j=y-1; i>=0 && j >=0;i--,j--){
        if(matrix[j][i]==value) dig2++;
        else break;
    }

    int gid1 = 0;//(+1,-1)
    for(int i =x+1,j=y-1; i<x_size && j>=0;i++,j--){
        if(matrix[j][i]==value) gid1++;
        else break;
    }

    int gid2 = 0;//(-1,+1)
    for(int i =x-1,j=y+1; i>=0 && j <y_size;i--,j++){
        if(matrix[j][i]==value) gid2++;
        else break;
    }

    int row = 1+row1+row2;//(x-row2,y)->(x+row1,y);
    for(int i = 0;i<row && row >= 5;i++)
    {
        matrix[y][x-row2+i] = 0;
        change.push_back(x-row2+i);
        change.push_back(y);
        change.push_back(0);
    }

    int col = 1+col1+col2;//(x,y-col2)->(x,y+col2);
    for(int i = 0;i<col && col >= 5;i++)
    {
        matrix[y-col2+i][x] = 0;
        change.push_back(x);
        change.push_back(y-col2+i);
        change.push_back(0);
    }

    int dig = 1+dig1+dig2;//(x-dig2,y-dig2)->(x+dig1,y+dig1);
    for(int i = 0;i<dig && dig >= 5;i++)
    {
        matrix[y-dig2+i][x-dig2+i] = 0;
        change.push_back(x-dig2+i);
        change.push_back(y-dig2+i);
        change.push_back(0);
    }

    int gid = 1+gid1+gid2;//(x-gid2,y+gid2)->(x+gid1,y-gid1);//gid1:(+1,-1),gid2:(-1,+1)
    for(int i = 0;i<gid && gid >= 5 ;i++)
    {
        matrix[y+gid2-i][x-gid2+i] = 0;
        change.push_back(x-gid2+i);
        change.push_back(y+gid2-i);
        change.push_back(0);
    }

    //score calculation cross reaction has extra score, no reaction is zero
    row = row >= 5? row: 0;
    col = col >= 5? col: 0;
    dig = dig >= 5? dig: 0;
    gid = gid >= 5? gid: 0;
    int count = row+col+dig+gid;

    int extra = 3;  //I can calculate the extra, if you want
    if(row == 0) extra--;
    if(col == 0) extra--;
    if(dig == 0) extra--;
    if(gid == 0) extra--;
    if(count!=0) total-=count+extra;
    return count;
}

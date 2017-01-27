#include <cstdio>
#include <cmath>
#include <float.h>
#include <queue>
#include <fstream>
using namespace std;

char pizza[1000][1000];
int maxRectSize,minIngredient;
int rows,cols;

struct Rect{
    int x1,y1,x2,y2;

    Rect(int x1=0,int y1=0,int x2=0,int y2=0){
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }

    Rect(const Rect& other){
        this->x1 = other.x1;
        this->y1 = other.y1;
        this->x2 = other.x2;
        this->y2 = other.y2;
    }

    void setRect(int x1=0,int y1=0,int x2=0,int y2=0){
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }

    void toString(){
        printf("start %d %d\n",x1,y1);
        printf("end   %d %d\n",x2,y2);
    }
};

int size(Rect& area){
    int height = abs(area.y2 - area.y1) + 1;
    int width  = abs(area.x1 - area.x2) + 1;
    return height * width;
}

bool containsEnoughIngredients(Rect& area){
    int mush = 0, tom = 0;
    for(int x = area.x1; x <= area.x2;x++)
        for(int y = area.y1; y <= area.y2;y++)
            pizza[y][x] == 'M' ? mush++ : tom++;
    return mush >= minIngredient && tom >= minIngredient;
}

double ratio(Rect &area){
    int mush = 0, tom = 0;
    for(int x = area.x1; x <= area.x2;x++)
        for(int y = area.y1; y < area.y2;y++)
            pizza[y][x] == 'M' ? mush++ : tom++;

    if(mush == 0) return DBL_MAX;
    if(tom  == 0) return DBL_MIN;

    return mush/double(tom);
}

Rect *splitRegion(Rect& area){
    Rect *optimalRect = new Rect[2];
    double optimalDiff = DBL_MAX;

    for(int x = area.x1;x < area.x2;x++){
        Rect one(area.x1, area.y1,x,area.y2);
        Rect two(x+1, area.y1,area.x2,area.y2);
        double ratioOne = ratio(one),ratioTwo = ratio(two);

        if(abs(ratioOne-ratioTwo) < optimalDiff){
            optimalDiff = abs(ratioOne-ratioTwo);
            *optimalRect = one;
            *(optimalRect + 1) = two;
        }
    }

    for(int y = area.y1;y < area.y2;y++){
        Rect one(area.x1,area.y1,area.x2,y);
        Rect two(area.x1,y+1,area.x2,area.y2);
        double ratioOne = ratio(one),ratioTwo = ratio(two);

        if(abs(ratioOne-ratioTwo) < optimalDiff){
            optimalDiff = abs(ratioOne-ratioTwo);
            *optimalRect = one;
            *(optimalRect + 1) = two;
        }
    }

    return optimalRect;
}

int main(){
    ifstream stream("input.txt");
    stream>>rows>>cols>>minIngredient>>maxRectSize;
    rows -= 1;
    cols -= 1;

    for(int y = 0; y <= rows;y++)
        for(int x = 0; x <= cols;x++)
            stream>>pizza[y][x];
    stream.close();

    queue<Rect*> toProcess,completed;
    toProcess.push(new Rect(0,0,cols,rows));

    while (!toProcess.empty()) {
        Rect *tmp = toProcess.front();
        toProcess.pop();

        if(!containsEnoughIngredients(*tmp))
            continue;
        if(size(*tmp) < maxRectSize){
            completed.push(tmp);
        }else{
            Rect *newRect = splitRegion(*tmp);
            toProcess.push(newRect);
            toProcess.push(newRect+1);
        }
    }

    while(!completed.empty()){
        Rect *tmp = completed.front();
        completed.pop();

        printf("%d\t%d\n",tmp->x1,tmp->y1);
        printf("%d\t%d\n",tmp->x2,tmp->y2);
    }
    return 0;
}

#include <cstdio>
#include <cmath>
#include <utility>
#include <queue>
#include <fstream>
#include <iostream>
using namespace std;

char pizza[1000][1000];
int  mushCount[1000][1000];
int  tomCount[1000][1000];

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
        printf("%d %d",y1,x1);
        printf(" %d %d\n",y2,x2);
    }
};

void genCountMatrix(int summationMap[1000][1000],
    Rect& area,char charToCount){
    for(int y = 0; y <= area.y2;y++){
        int sum = 0;
        for(int x = 0; x <= area.x2;x++){
            sum += pizza[y][x] == charToCount ? 1 : 0;
            summationMap[y][x] = sum;
        }
    }
}

int size(Rect& area){
    int height = abs(area.y2 - area.y1) + 1;
    int width  = abs(area.x1 - area.x2) + 1;
    return height * width;
}

int countIngredient(int aggrTable[1000][1000],Rect& area){
    int total = 0;
    for(int y = area.y1; y <= area.y2;y++)
        total += aggrTable[y][area.x2];

    int outsideRect = area.x1 - 1;
    if(outsideRect > -1)
        for(int y = area.y1; y <= area.y2;y++)
            total -= aggrTable[y][outsideRect];

    return total;
}

bool containsEnoughIngredients(Rect& area){
    int mush = countIngredient(mushCount,area),
      tom = countIngredient(tomCount,area);
      return mush >= minIngredient && tom >= minIngredient;
}

int score(Rect &one,Rect &two){
    int score1 = 0,score2 = 0,rectSize;

    if(containsEnoughIngredients(one)) score1 += 10;
    if(containsEnoughIngredients(two)) score2 += 10;
    if((rectSize = size(one)) <= maxRectSize) score1 *= rectSize;
    if((rectSize = size(two)) <= maxRectSize) score2 *= rectSize;

    return score1 * score2;
}

Rect *splitRegion(Rect& area){
    Rect *optimalRect = new Rect[2];
    int maxScore = INT32_MIN;

    for(int x = area.x1;x < area.x2;x++){
        Rect one(area.x1, area.y1,x,area.y2);
        Rect two(x+1, area.y1,area.x2,area.y2);
        int value = score(one,two);

        if(value > maxScore){
            maxScore = value;
            *optimalRect = one;
            *(optimalRect + 1) = two;
        }
    }

    for(int y = area.y1;y < area.y2;y++){
        Rect one(area.x1,area.y1,area.x2,y);
        Rect two(area.x1,y+1,area.x2,area.y2);
        int value = score(one,two);

        if(value > maxScore){
            maxScore = value;
            *optimalRect = one;
            *(optimalRect + 1) = two;
        }
    }

    return optimalRect;
}

int main(){
    ifstream stream("big.in");
    stream>>rows>>cols>>minIngredient>>maxRectSize;
    rows -= 1;
    cols -= 1;

    for(int y = 0; y <= rows;y++)
        for(int x = 0; x <= cols;x++)
            stream>>pizza[y][x];
    stream.close();

    queue<Rect*> toProcess,completed;
    toProcess.push(new Rect(0,0,cols,rows));

    genCountMatrix(mushCount,*toProcess.front(),'M');
    genCountMatrix(tomCount,*toProcess.front(),'T');

    while (!toProcess.empty()) {
        Rect *tmp = toProcess.front();
        toProcess.pop();

        if(!containsEnoughIngredients(*tmp))
            continue;
        if(size(*tmp) <= maxRectSize){
            completed.push(tmp);
        }else{
            Rect *newRect = splitRegion(*tmp);
            toProcess.push(newRect);
            toProcess.push(newRect+1);
        }
    }

    cout<<completed.size()<<"\n";
    while(!completed.empty()){
        Rect *tmp = completed.front();
        completed.pop();

        tmp->toString();
    }
    return 0;
}

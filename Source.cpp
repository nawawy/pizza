#include <iostream>
#include<fstream>
#include <queue>
using namespace std;

char pizza[1000][1000];
int r[1000][1000];
int R, C, H, L;

void result(int[][1000]);

void result(int r [][1000])
{

}

struct Coord{
	int x, y;
	Coord operator=(Coord t)
	{
		Coord  a;
		a.x = t.x; 
		a.y = t.y; 

		return a;
	}
};


double ratio(Coord start, Coord end){
	double tom = 0, mush = 0;
	for (int i = start.x; i < end.x; i++){
		for (int j = start.y; j < end.y; j++){
			pizza[i][j] == 'M' ? tom++ : mush++;
		}
	}

	if (mush == 0 || tom == 0) return INFINITY;

	return tom / mush;
}

void split(Coord& start, Coord& end,Coord& start2, Coord& end2){
	double topDiff = INFINITY;
	Coord best;
	best.x = 0;
	best.y = 0;
	bool horizontal = false;

	for (int i = start.x+1; i < end.x-1; i++){
		Coord one, two;
		one.x = i;
		one.y = start.y;

		two.x = one.x+1;
		two.y = start.y;

		double ratioOne = ratio(start, one);
		double ratioTwo = ratio(two, end);

		if (abs(ratioOne - ratioTwo) < topDiff){
			topDiff = abs(ratioOne - ratioTwo);
			best.x = i;
			best.y = start.y;
			horizontal = true;
		}
	}

	for (int i = start.y + 1; i < end.y - 1; i++){
		Coord one, two;
		one.y = i;
		one.x = start.x;

		two.x = one.x;
		two.y = one.y+1;

		double ratioOne = ratio(start, one);
		double ratioTwo = ratio(two, end);

		if (abs(ratioOne - ratioTwo) < topDiff){
			topDiff = abs(ratioOne - ratioTwo);
			best.y = i;
			best.x = start.x;
			horizontal = false;
		}
	}

	end = best;
	start2 = best;
	if (horizontal){
		start2.x++;
	}
	else
		start2.y++;
}

bool isValidSize(Coord one, Coord two, int validSize){
	return abs(one.x - two.x)*abs(two.y - one.y) <= validSize;
}

bool containsEnoughIngr(Coord one, Coord two, int min){
	int tom = 0, mush = 0;
	for (int i = one.x; i < two.x; i++){
		for (int j = one.y; j < two.y; j++){
			pizza[i][j] != 'M' ? tom++ : mush++;
		}
	}
	return tom >= min && mush >= min;
}

int main()
{
	ifstream input;
	input.open("small.in");
	input >> R >> C >> L >> H;

	for (int i = 0; i < R; i++)
	for (int j = 0; j < C; j++)
		input >> pizza[i][j];

	queue <pair <Coord, Coord>> in_progress;
	queue <pair <Coord, Coord>> complete;

	pair <Coord, Coord> whole;
	whole.first.x = 0;
	whole.first.y = 0;
	whole.second.x = R - 1;
	whole.second.y = C - 1;
	
	in_progress.push(whole);
	while (!in_progress.empty()) {
		pair <Coord, Coord> tmp = in_progress.front();
		in_progress.pop();
		if (!containsEnoughIngr(tmp.first, tmp.second, L))
			continue;
		if (isValidSize(tmp.first, tmp.second, H))
			complete.push(tmp);
		else {
			pair <Coord, Coord> split_rtrn;
			split(tmp.first, tmp.second, split_rtrn.first, split_rtrn.second);
			in_progress.push(split_rtrn);
		}
	}

	while(!complete.empty())
	{

		pair <Coord, Coord> com = complete.front();
		Coord r1 = com.first;
		Coord r2 = com.second;

		cout << r1.x << "   " << r1.y << "   " << r2.x << "   " << r2.y << endl;
		
		complete.pop();
	}
	system("pause");
	return 0;
}

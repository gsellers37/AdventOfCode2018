#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#define BOUNDS 360
#define CUTOFF 10000

struct Point {
    int id,x,y;
    Point(int id, int x, int y)
    {
        this->id = id;
        this->x = y;
        this->y = x;
    }
};

int manhattanDistance(Point p1, Point p2)
{
    return std::abs(p2.x-p1.x) + abs(p2.y-p1.y);
}

std::vector<Point> parseInput(std::string filename)
{
    std::vector<Point> points;

    std::ifstream inputFile;
    inputFile.open(filename);

    if (inputFile.is_open())
    {
        std::string line;
        int id = 1;
        while(getline(inputFile, line))
        {
            int x, y;
            sscanf(line.c_str(), "%d, %d", &x, &y);
            points.push_back(Point(id++,x,y));
        }
    }

    return points;
}

void print(std::vector<Point> points)
{
    for (std::vector<Point>::iterator it = points.begin(); it != points.end(); it++)
    {
        std::cout << it->id << ": " << it->x << "," << it->y << std::endl;
    }
}

void print(int board[][BOUNDS])
{
    for (int i = 0; i < BOUNDS; i++)
    {
        for (int j = 0; j < BOUNDS; j++)
        {
            std::cout<< board[i][j];
        }
        std::cout << std::endl;
    }
}

int findMinDis(int i, int j, std::vector<Point> points)
{
    int minDis = INT32_MAX;
    int id = 0;
    for (std::vector<Point>::iterator it = points.begin(); it != points.end(); it++)
    {
        int pDis = manhattanDistance(Point(0,i,j),*it);
        
        if (pDis < minDis)
        {
            minDis = pDis;
            id = it->id;
        }
        else if(pDis==minDis)
            id = 0;
    }
    return id;
}


int findSumOfManhattans(int i, int j, std::vector<Point> points)
{
    int sum = 0;
    for (std::vector<Point>::iterator it = points.begin(); it != points.end(); it++)
    {
        sum += manhattanDistance(Point(0,i,j),*it);
    }
    return sum;
}

int problemOne(std::vector<Point> points, int board[][BOUNDS])
{
    std::map<int,int> regionsAndSizes;
    std::set<int> infiniteRegions;
    for (int i = 0; i < BOUNDS; i++)
    {
        for (int j = 0; j < BOUNDS; j++)
        {
            int id = findMinDis(i,j,points);

            board[i][j] = id;

            if (i == 0 || i == BOUNDS-1 || j == 0 || j == BOUNDS-1)
            {
                infiniteRegions.insert(id);
                
                // internal regions can be iterated over before id was added to the set
                // so we need to erase that information
                if (regionsAndSizes.count(id)>0)
                    regionsAndSizes.erase(id);
            }

            // If not an infinite region
            if (infiniteRegions.find(id) == infiniteRegions.end())
            {
                // Add to its value in regionsAndSizes
                if (regionsAndSizes.count(id)>0)
                    regionsAndSizes[id]++;
                else
                    regionsAndSizes[id] = 1;
            }

        }
    }

    int answer = 0;
    // Find biggest region that isn't infinite
    for (std::map<int,int>::iterator it = regionsAndSizes.begin(); it!= regionsAndSizes.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
        if (it->second > answer)
            answer = it->second;
    }
    return answer;
}

int problemTwo(std::vector<Point> points, int board[][BOUNDS])
{
    std::map<int,int> regionsAndSizes;
    std::set<int> infiniteRegions;

    int regionSize = 0;
    for (int i = 0; i < BOUNDS; i++)
    {
        for (int j = 0; j < BOUNDS; j++)
        {
            int sum = findSumOfManhattans(i,j,points);

            if (sum < CUTOFF)
            {
                regionSize ++;
            }
        }
    }
    return regionSize;
}
int main(int argc, char const *argv[])
{
    std::vector<Point> points = parseInput(argv[1]);
    if (points.empty())
    {
        return -1;
    }

    print(points);

    int board[BOUNDS][BOUNDS] = {0};
    std::cout << "P1 Answer: " << problemOne(points,board) << std::endl;
    // print(board);

    board[BOUNDS][BOUNDS] = {0};
    std::cout << "P2 Answer: " << problemTwo(points,board) << std::endl;
    // print(board);

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>

// With help from AKQuaternion on the AoC subreddit
// My breadth first search was working correctly, but not my graph filling
// I ended up implementing a similar bfs to them, as it seemed cleaner

typedef std::pair<int,int> point_t;
typedef std::map<point_t,std::vector<point_t>> graph_t;

point_t N(const point_t point){   return point_t(point.first,point.second-1);   }
point_t S(const point_t point){   return point_t(point.first,point.second+1);   }
point_t E(const point_t point){   return point_t(point.first+1,point.second);   }
point_t W(const point_t point){   return point_t(point.first-1,point.second);   }



void keepWindowOpen()
{
    std::cout << "Press any key to exit";
    std::cin.get();
}

void connect(graph_t & graph, point_t & point1, const point_t point2)
{
    graph[point1].push_back(point2);
    graph[point2].push_back(point1);
    point1 = point2;
}
void fillGraph(std::string directions, int & i, graph_t & graph, point_t branch)
{
    point_t original = branch;

    while (true)
    {
        switch(directions[i++])
        {
            case '(':
                fillGraph(directions,i,graph,branch);
                break;
            case '|':
                branch = original;
                break;
            case ')':
                return;
            case '$': 
                return;
            case '^':
                continue;
            case 'N':
                connect(graph,branch,N(branch));
                break;
            case 'S':
                connect(graph,branch,S(branch));
                break;
            case 'E':
                connect(graph,branch,E(branch));
                break;
            case 'W':
                connect(graph,branch,W(branch));
                break;

        }
    }
}

int runP1(std::string filename)
{
    std::string regex;
    std::ifstream inputFile(filename);
    if (inputFile.is_open())
        inputFile >> regex;
    else
        std::cout << "Could not open file: " << filename << std::endl;

    point_t start(0,0);
    graph_t graph;

    int i = 0;
    fillGraph(regex, i, graph, start);

    // Setup variables for BFS
    std::queue<std::pair<point_t,int>> q;
    q.emplace(start,0);
    std::set<point_t> visited;
    int longest = 0;

    int greaterThan1000 = 0;
    while (!q.empty())
    {
        std::pair<point_t,int> pointDis = q.front();
        point_t point = pointDis.first;
        int length = pointDis.second;
        q.pop();

        // If this room has been reached before, skip over
        if (visited.count(point))
            continue;
        visited.insert(point);

        if (length >= 1000)
            greaterThan1000++;

        longest = std::max(length,longest);

        for (point_t newPoint : graph[point])
            q.push({newPoint,length+1});
    }

    return greaterThan1000;

    
}

void runTestP1(std::string filename, int expectedOutput)
{
    int result;
    if ((result = runP1(filename)) == expectedOutput)
        std::cout << "Passed test: " << filename << std::endl;
    else
        std::cout << "Failed test: " << filename << " Result was " << result << ", expected " << expectedOutput << std::endl;
}

int main(int argc, char const *argv[])
{
    runTestP1("test1.txt",3);
    runTestP1("test2.txt",10);
    runTestP1("test3.txt",18);
    runTestP1("test4.txt",23);
    runTestP1("test5.txt",31);
    runTestP1("test6.txt",8);
    runTestP1("test7.txt",4);
    runTestP1("test8.txt",2);
    std::cout << runP1("input.txt") << std::endl; //138 too low
    keepWindowOpen(); 
    return 0;
}

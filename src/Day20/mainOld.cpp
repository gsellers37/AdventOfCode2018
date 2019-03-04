#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>


typedef std::pair<int,int> Point;
Point goN(const Point point)
{
    return Point(point.first,point.second-1);
}

Point goS(const Point point)
{
    return Point(point.first,point.second+1);
}

Point goE(const Point point)
{
    return Point(point.first+1,point.second);
}

Point goW(const Point point)
{
    return Point(point.first-1,point.second);
}

class Node
{
    Node *N, *W, *E, *S;
    public:
        Node* addStr(std::string str)
        {
            if (str.size() == 0)
                return this;

            char c = str[0];
            std::string rest = str.substr(1);
           
            if (c == 'N')
            {
                if (!N)
                    N = new Node();
                return N->addStr(rest);
            }
            else if (c == 'W')
            {
                if (!W)
                    W = new Node();
                return W->addStr(rest);
            }
            else if (c == 'E')
            {
                if (!E)
                    E = new Node();
                return E->addStr(rest);
            }
            else if (c == 'S')
            {
                if (!S)
                    S = new Node();
                return S->addStr(rest);
            }
        }

        // void fillMap(int init, Point start, std::map<Point,int> *map)
        // {
        //     Point next;
        //     map->emplace(start,init);

        //     std::vector<Node*> nodes = {N,E,S,W};
        //     std::vector<Point (*)(const Point point)> funcs = {goN,goE,goS,goW};

        //     for (int i = 0; i < 4; i++)
        //     {
        //         next = funcs.at(i)(start);
        //         if (nodes.at(i))
        //             if (map->count(next) == 0)
        //                 nodes.at(i)->fillMap(init+1,next,map);
        //             else 
        //             {
        //                 int min = std::min((*map)[next],init+1);
        //                 // (*map)[next] = min;
        //                 nodes.at(i)->fillMap(min,next,map);
        //             }
        //     }
        // }
        
        ~Node()
        {
            if (N)
                delete N;
            if (S)
                delete S;
            if (E)
                delete E;
            if (W)
                delete W;
        }

    
};

void keepWindowOpen()
{
    std::cout << "Press any key to exit";
    std::cin.get();
}

void parse(std::string filename, std::string & line)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    
    if (inputFile.is_open())
        getline(inputFile,line);
    else
        std::cout << "Cannot open input file: " << filename << std::endl;
 
    inputFile.close();
}

std::vector<Node*> fillGraph(int & i, std::string directions, Node* graph)
{
    std::vector<Node*> ends = {graph};
    while (i < directions.size())
    {
        char c = directions[i];
        i++;

        if (c == '^')
            continue;
        else if (c == '$')
            continue;

        else if (c == '(')
        {   
            std::vector<Node * > newEnds;
            for (Node * end : ends)
            {
                std::vector<Node*> temp = fillGraph(i,directions,end);;
                for (Node * newEnd : temp)
                    newEnds.push_back(newEnd);
            }
            ends = newEnds;
        }
        else if (c == '|')
        {
            std::vector<Node*> temp = fillGraph(i,directions,graph);
            for (Node * newEnd : temp)
                ends.push_back(newEnd);
            return ends;
        }
        else if (c == ')')
        {
            return ends;
        }
        else{
            for (Node* & end : ends)
            {
                end = end->addStr(std::string(1,c));
            }
        }
    }
   
    // for (char c : directions)
    // {
    //     if (c == '^')
    //     {
            
    //     }
    //     else if (c == '$')
    //     {
            
    //     }
    //     else if (c == '(')
    //     {
            
    //     }
    //     else if (c == '|')
    //     {
            
    //     }
    //     else if (c == ')')
    //     {
            
    //     }
    //}
}

int runP1(std::string filename)
{
    std::string regex;
    parse(filename,regex);
    
    Node * base = new Node();
    int i = 0;
    fillGraph(i,regex, base);

    std::map<Point,int> map;
    Point start(0,0);
    base->fillMap(0,start,&map);
    delete base;

    return std::max_element(map.begin(),map.end(),[](
        const std::pair<Point,int>& p1, const std::pair<Point,int>& p2){
            return p1.second<p2.second;
            }
        )->second;
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

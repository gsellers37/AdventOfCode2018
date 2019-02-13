#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <map>
#include <set>

typedef std::tuple<int,int,int,int> point_t;

int manhattan(point_t p1, point_t p2){
    return abs(std::get<0>(p1) - std::get<0>(p2))+
           abs(std::get<1>(p1) - std::get<1>(p2))+
           abs(std::get<2>(p1) - std::get<2>(p2))+
           abs(std::get<3>(p1) - std::get<3>(p2));
}

std::ostream& operator<<(std::ostream& os, const point_t & point){
    return os << std::get<0>(point) << "," <<
            std::get<1>(point) << "," <<
            std::get<2>(point) << "," <<
            std::get<3>(point);
}
void buildConstellation(point_t point, std::map<point_t,std::set<point_t>> graph, std::set<point_t> & visited){
    visited.insert(point);
    for (point_t nextPoint : graph[point]){
        if (visited.count(nextPoint)==0) 
            buildConstellation(nextPoint,graph,visited);
    }
}


int run(std::string filename){
    std::vector<point_t> points;
    std::ifstream inputFile(filename);
    if  (inputFile.is_open()){
        std::string line;
        while (getline(inputFile,line)){
            int x,y,z,t;
            sscanf(line.c_str(),"%d, %d, %d, %d", &x, &y, &z, &t);
            points.push_back(std::make_tuple(x,y,z,t));
        }
    }
    int numConstellations = 0;
    std::map<point_t,std::set<point_t>> graph;
    std::set<point_t> inAConstellation;
    for (int i = 0; i < points.size(); i++){
        for (int j = i; j < points.size(); j++){
            if (manhattan(points.at(i),points.at(j)) <= 3){
                graph[points.at(i)].insert(points.at(j));
                graph[points.at(j)].insert(points.at(i));
            }
        }
    }

    std::set<point_t> visited;

    for (point_t point : points) {
        if (visited.count(point) == 0)
        {
            numConstellations++;
            buildConstellation(point, graph, visited);
        }
    }

    return numConstellations;
}
void runTest(std::string filename, int expectedAnswer){
    int answer;
    if ((answer = run(filename)) == expectedAnswer) std::cout << "Passed test: ";
    else std::cout << "Failed test, expecting: "<< expectedAnswer << " got " << answer << " in file ";
    std::cout << filename << std::endl;
}
int main(int argc, char const *argv[])
{
    runTest("test1.txt",2);
    runTest("test2.txt",4);
    runTest("test3.txt",3);
    runTest("test4.txt",8);

    std::cout << "Part 1: " << run("input.txt");
    
    std::cin.get();
    return 0;
}

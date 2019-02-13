#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>

// Part 2 help from /u/fizbin on /r/adventofcode 

struct Nanobot{
    long x,y,z,r;
    
    Nanobot(long _x, long _y, long _z, long _r)
    {
        x = _x; y = _y; z = _z; r = _r;
    }
    
    long manhattan(const Nanobot & other){
        return labs(other.x-x) + llabs(other.y-y) + llabs(other.z-z);
    }
    bool operator<(const Nanobot & other) const {
        return r < other.r;
    }
};
struct Octant {
    long numBots;
    long segmentSize;
    long disToOrig;
    long box[6];

    bool operator<(const Octant & other) const {
        return (numBots < other.numBots) ||
               (numBots == other.numBots && disToOrig > other.disToOrig) ||
               (numBots == other.numBots && disToOrig == other.disToOrig && segmentSize > other.segmentSize );   
    }
};

bool intersects(long box[6], Nanobot bot) {
    // Find the manhattan distance from bot to the box
    long dis = 0;

    if (bot.x < box[0]) dis += box[0] - bot.x;
    if (bot.y < box[1]) dis += box[1] - bot.y;
    if (bot.z < box[2]) dis += box[2] - bot.z;
    if (bot.x > box[3] - 1) dis += bot.x - box[3] + 1;
    if (bot.y > box[4] - 1) dis += bot.y - box[4] + 1;
    if (bot.z > box[5] - 1) dis += bot.z - box[5] + 1;

    return dis <= bot.r;
}

int main(int argc, char const *argv[])
{
    std::vector<Nanobot> bots;

    std::ifstream inputFile("input.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (getline(inputFile,line)) {
            long x,y,z,r;
            sscanf(line.c_str(),"pos=<%ld,%ld,%ld>, r=%ld",&x,&y,&z,&r);
            bots.emplace_back(x,y,z,r);
        }
    }
    else {
        printf("Unable to open file.");
    }
    std::sort(bots.begin(),bots.end());
    Nanobot largest = bots.back();
    
    int inRange = 0;
    for (Nanobot bot : bots) {

        if (bot.manhattan(largest) <= largest.r) {
            inRange++;
        }
    }

    std::cout << "Part 1: " << inRange << std::endl;

    Nanobot zero(0,0,0,0);
    long maxRange = 0;
    for (Nanobot bot : bots)
        maxRange = std::max(maxRange,zero.manhattan(bot)+bot.r);

    long boxsize = 1;
    while (boxsize <= maxRange)
        boxsize *= 2;
    boxsize *= 64;
    std::priority_queue<Octant> q;
    Octant boundingBox;
    boundingBox.box[0] = boundingBox.box[1] = boundingBox.box[2] = {-boxsize};
    boundingBox.box[3] = boundingBox.box[4] = boundingBox.box[5] = {boxsize};
    boundingBox.numBots = bots.size();
    boundingBox.segmentSize = 2*boxsize;
    q.push(boundingBox);

    int offsets[8][3] = {{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}};

    while(!q.empty()){
        Octant bound = q.top(); q.pop();
        
        if (bound.segmentSize == 1){
            Nanobot loc(bound.box[0],bound.box[1],bound.box[2],0);
            std::cout << "Part 2: " << zero.manhattan(loc);
            break;
        }

        long segmentSize = bound.segmentSize / 2;

        for (int i = 0; i < 8; i++) {
            long box[6];

            for (int j = 0; j < 3; j++) {
                box[j] = bound.box[j] + offsets[i][j]*segmentSize;
                box[j+3] = box[j] + segmentSize;
            }
            
            Nanobot centerOfBox((box[0]+box[3])/2,(box[1]+box[4])/2,(box[2]+box[5])/2,0);
            long disToOrig = centerOfBox.manhattan(zero);

            long numBots = 0;
            for (Nanobot bot : bots) {
                if (intersects(box,bot)) numBots++;
            }

            Octant newBox;
            for (int j = 0; j < 6; j++) {
                newBox.box[j] = box[j]; 
            }
            newBox.numBots = numBots;
            newBox.segmentSize = segmentSize;
            newBox.disToOrig = disToOrig;
            
            q.push(newBox);
        }
    }

    std::cin.get();
    return 0;
}

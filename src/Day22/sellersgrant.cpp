
#include <iostream>
#include <queue>
#include <vector>
#include <tuple>

enum ARGs {nARG, DEPTH, TARGET};

const int MAXN = 1000;
const int yMult = 16807;
const int xMult = 48271;
const int erosionMod = 20183;

long long  erosionLevels[MAXN][MAXN];

int dijk[MAXN][MAXN][3];

bool seen[MAXN][MAXN][3];

struct Node {
    int x,y,tool,dis;
    Node(int _x,int _y,int _tool,int _dis){
        x = _x; y = _y; tool = _tool; dis = _dis;
    }
    bool operator<(const Node & other) const {
        return dis > other.dis;
    }
};

int main(int argc, char const *argv[]) {

    if (argc != 3)
        return -1;
    
    std::string depthStr = argv[DEPTH];
    std::string targetStr = argv[TARGET];

    int depth;
    int x, y;
    std::pair<int,int> target;

    sscanf(depthStr.c_str(),"depth: %d", &depth);
    sscanf(targetStr.c_str(),"target: %d,%d", &x, &y);
    target = std::make_pair(x,y);
    
    // Part One
    for (x = 0; x < MAXN; x++) {
        for (y = 0; y < MAXN; y++) {

            long long geoIndex;
            
            if (x == 0){ geoIndex = y * xMult; }
            else if (y == 0){ geoIndex = x * yMult; }
            else if (x == 0 && y == 0){ geoIndex = 0; }
            else if (x == target.first && y == target.second){ geoIndex = 0; }
            else { geoIndex = erosionLevels[x-1][y] * erosionLevels[x][y-1]; }

            erosionLevels[x][y] = (geoIndex + depth) % erosionMod;
        }
    }

    for (x = 0; x <= MAXN; x++) {
        for (y = 0; y <= MAXN; y++) {
            erosionLevels[x][y] = erosionLevels[x][y] % 3;
        }
    }

    int riskLevel = 0;
    for (x = 0; x <= target.first; x++) {
        for (y = 0; y <= target.second; y++) {
            riskLevel += erosionLevels[x][y];
        }
    }
    printf("Part 1: %d\n", riskLevel); 



    std::priority_queue<Node> q;
    q.emplace(0,0,1,0);
    std::vector<int> dx = {-1,0,1,0};
    std::vector<int> dy = {0,1,0,-1};

    while(!q.empty()) {
        Node cur = q.top(); 
        q.pop();

        if (seen[cur.x][cur.y][cur.tool])
            continue;
        seen[cur.x][cur.y][cur.tool] = true;

        dijk[cur.x][cur.y][cur.tool] = cur.dis;
        if (cur.x == target.first && cur.y == target.second && cur.tool == 1)
            break;

        for (int i = 0; i < 4; i++){
            Node temp = cur;
            temp.x += dx[i];
            temp.y += dy[i];
            temp.dis++;;

            if (temp.x >= 0 && temp.y >= 0 && 
                temp.x < MAXN && temp.y < MAXN &&
                temp.tool != erosionLevels[temp.x][temp.y])
                q.push(temp);
        }

        for (int i = 0; i < 3; i++){
            Node temp = cur;
            temp.tool = i;
            temp.dis += 7;

            if (temp.tool != erosionLevels[temp.x][temp.y])
                q.push(temp);
        }

    }

    printf("Part 2: %d\n", dijk[target.first][target.second][1]);


    return 0;
}

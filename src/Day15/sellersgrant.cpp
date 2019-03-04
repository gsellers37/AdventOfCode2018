#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

#define LEFT  (Point(-1,0))
#define RIGHT (Point(1,0))
#define UP    (Point(0,-1))
#define DOWN  (Point(0,1))

#define NULLPOINT (Point(INT16_MAX,INT16_MAX))

class Point
{
    public:
        int x,y;
        
        Point(int posx, int posy)
        {
            x = posx;
            y = posy;
        }
        bool operator==(Point const other) const
        {
            return x==other.x && y==other.y;
        }
        Point operator+(Point other)
        {
            return Point(x+other.x,y+other.y);
        }
        Point operator-(Point other)
        {
            return Point(x-other.x,y-other.y);
        }
        int distanceTo(Point other)
        {
            return abs(x-other.x)+abs(y-other.y);
        }
        bool operator<(const Point other) const
        {
            return y < other.y ? true : y == other.y ? x < other.x : false;
        }
        void print()
        {
            printf("(%d,%d)\n",x,y);
        }
        int getX()
        {
            return x;
        }
        int getY()
        {
            return y;
        }
};
class Player
{
    int x, y, hp, ap;
    char team;

    public:
        Player(int posx,int posy, char teamChar)
        {
            x = posx;
            y = posy;
            hp = 200;
            ap = 3;
            team = teamChar;
        }
        void print()
        {
            printf("%c(X,Y) = (%d,%d), HP = %d\n",team,x,y,hp);
        }
        char getTeam()
        {
            return team;
        }
        void setAP(int _ap)
        {
            ap = _ap;
        }
        Point getPos()
        {
            return Point(x,y);
        }
        int getHP()
        {
            return hp;
        }
        int getAP()
        {
            return ap;
        }
        bool operator<(Player other)
        {
            return y < other.y ? true : y == other.y ? x < other.x : false;
        }
        std::set<Point> getAdjacentSpaces(std::vector<std::string> const & board, char toSearch)
        {
            std::vector<Point> dirs = {UP,LEFT,RIGHT,DOWN};
            std::set<Point> output;
            for (Point v : dirs)
                if (board[y+v.y][x+v.x] == toSearch)
                    output.insert(getPos()+v);
            return output;
        }
        bool isNearbyToAnEnemy(std::vector<std::string> board)
        {
            std::set<Point> enemies;
            if (team == 'G')
                enemies = getAdjacentSpaces(board,'E');
            else if (team == 'E')
                enemies = getAdjacentSpaces(board,'G');
            return (!(enemies.size()==0));
        }
        void move(Point newPoint, std::vector<std::string> & board)
        {
            board[y][x] = '.';
            x = newPoint.getX();
            y = newPoint.getY();
            board[y][x] = team;
        }
        bool attack(Player & enemy)
        {
            return enemy.loseHealth(ap);
        }
        bool loseHealth(int damage)
        {
            hp -= damage;
            return (hp<=0);
        }
        void die(std::vector<std::string> & board)
        {
            Point pos = getPos();
            board[pos.y][pos.x] = '.';
        }
};

void print(std::vector<Player> players)
{
    for (Player player : players)
        player.print();
}
void print(std::set<Point> points)
{
    for (Point point: points)
        point.print();
}
void print(std::vector<Point> points)
{
    for (Point point: points)
        point.print();
}
void print(std::vector<std::string> board)
{
    for (std::string line : board)
        std::cout << line << std::endl;
    std::cout << std::endl;
}
void print(std::vector<std::string> board,Point point, char symbol)
{
    board[point.y][point.x] = symbol;
    print(board);
    board[point.y][point.x] = symbol;
}
void print(std::vector<std::string> board,std::vector<Point> pointsToChange, char symbol)
{
    for (Point point : pointsToChange)
    {
        board[point.y][point.x] = symbol;
    }
    print(board);
    for (Point point : pointsToChange)
    {
        board[point.y][point.x] = '.';
    }
}
void print(std::vector<std::string> board,std::set<Point> pointsToChange, char symbol)
{
    for (Point point : pointsToChange)
    {
        board[point.y][point.x] = symbol;
    }
    print(board);
    for (Point point : pointsToChange)
    {
        board[point.y][point.x] = '.';
    }
}
std::pair<std::vector<std::string>,std::vector<Player>> getBoardAndPlayers(std::string filename)
{
    std::vector<Player> players;
    std::vector<std::string> board;

    std::ifstream inputFile;
    inputFile.open(filename);
    if (inputFile.is_open())
    {
        std::string line;
        int y = 0;
        while (getline(inputFile,line))
        {
            board.push_back(line);
            int x = 0;
            for (char c : line)
            {
                if (c=='E')
                    players.push_back(Player(x,y,'E'));
                else if (c=='G')
                    players.push_back(Player(x,y,'G'));
                x++;
            }
            y++;
        }
    }
    else
        std::cout << "Could not open file: " << filename << std::endl;
    
    inputFile.close();

    return std::pair<std::vector<std::string>,std::vector<Player>>(board,players);
}

std::vector<Player> getEnemies(std::vector<Player> & players, std::vector<Player>::iterator player_p)
{
    std::vector<Player> enemies;
    
    for (std::vector<Player>::iterator enemy_p = players.begin(); enemy_p != players.end(); enemy_p++)
    {
        // skip if the player is itself
        if (player_p == enemy_p)
            continue;
        // skip if the player is on the same team
        if (player_p->getTeam() == enemy_p->getTeam())
            continue;
        if (enemy_p->getHP() > 0)
            enemies.push_back(*enemy_p);
    }
    return enemies;
}

std::vector<std::vector<Player>::iterator> getEnemiesp(std::vector<Player> & players, std::vector<Player>::iterator player_p)
{
    std::vector<std::vector<Player>::iterator> enemies;
    
    for (std::vector<Player>::iterator enemy_p = players.begin(); enemy_p != players.end(); enemy_p++)
    {
        // skip if the player is itself
        if (player_p == enemy_p)
            continue;
        // skip if the player is on the same team
        if (player_p->getTeam() == enemy_p->getTeam())
            continue;
        if (enemy_p->getHP() > 0)
            enemies.push_back(enemy_p);
    }
    return enemies;
}

std::set<Point> getAdjacencies(std::vector<Player> enemies, std::vector<std::string> board)
{
    std::set<Point> adjacentSpaces;

    for (Player enemy : enemies)
    {
        std::set<Point> toAdd = enemy.getAdjacentSpaces(board,'.');
        for (Point point : toAdd)
            adjacentSpaces.insert(point);
    }

    return adjacentSpaces;
}


void extend(std::set<Point> & points1 ,std::vector<Point> points2)
{
    for (Point point : points2)
        points1.insert(point);
}

void extend(std::set<Point> & points1 ,std::set<Point> points2)
{
    for (Point point : points2)
        points1.insert(point);
}

std::set<Point> getReachableSpaces(Player player, std::vector<std::string> board)
{
    std::set<Point> openPoints;
    extend(openPoints,player.getAdjacentSpaces(board,'.'));

    int size = openPoints.size();
    int prevSize = 0;

    // breadth first search
    while (size != prevSize)
    {
        prevSize = size;
        for (Point point : openPoints)
        {
            for (Point dir : {UP,LEFT, RIGHT, DOWN})
            {
                if (board[(point+dir).y][(point+dir).x] == '.')
                    openPoints.insert(point+dir);            
            }
        }
        size = openPoints.size();
    }
    return openPoints;
}

int getWalkingDistance(Point center, Point target, std::vector<std::string> board)
{
    std::set<Point> openPoints;
    openPoints.insert(center);

    int dis = 0;
    // breadth first search
    while (true)
    {
        std::set<Point> newPoints;
        for (Point point : openPoints)
        {
            for (Point dir : {UP,LEFT, RIGHT, DOWN})
            {
                if (point + dir == target)
                    return dis;
                if (board[(point+dir).y][(point+dir).x] == '.')
                    newPoints.insert(point+dir);            
            }
        }
        extend(openPoints,newPoints);
        dis+=1;
    }
}
std::vector<Point> getNearestSpaces(std::vector<Player>::iterator player_p, std::vector<Point> points, std::vector<std::string> board)
{
    
    Point center = player_p->getPos();
    std::vector<Point> nearestPoints;

    int min = INT16_MAX;
    for (Point point : points)
    {
        int dis = getWalkingDistance(center,point,board);
        if (dis < min)
        {
            min = dis;
            nearestPoints = {point};
        }
        else if (dis == min)
            nearestPoints.push_back(point);
    }
    return nearestPoints;
}


std::set<Point> getTrajectory(Point target, std::vector<std::string> board,  Point player)
{
    std::set<Point> openPoints;
    openPoints.insert(target);

    // breadth first search
    bool playerFound = false;
    while (!playerFound)
    {
        std::set<Point> newPoints;
        for (Point point : openPoints)
        {
            for (Point dir : {UP,LEFT, RIGHT, DOWN})
            {
                if (point + dir == player)
                    playerFound = true;
                if (board[(point+dir).y][(point+dir).x] == '.')
                    newPoints.insert(point+dir);            
            }
        }
        extend(openPoints,newPoints);
    }
    return openPoints;
}

std::vector<Player>::iterator attack(std::vector<Player>::iterator player_p, std::vector<Player> & players, std::vector<std::string> & board)
{
    std::vector<std::vector<Player>::iterator> enemies = getEnemiesp(players,player_p);

    for (std::vector<std::vector<Player>::iterator>::iterator enemy_pp = enemies.begin(); enemy_pp != enemies.end();)
    {
        if ((*enemy_pp)->getPos().distanceTo(player_p->getPos())==1)
            enemy_pp++;
        else
            enemy_pp = enemies.erase(enemy_pp);
    }
    if (enemies.size()==0)
        return players.end();
    
    // std::sort(enemies.begin(),enemies.end());
    

    // Choose enemy with least health that is closest to 
    std::vector<Player>::iterator enemy_p;
    int leastHealth = 201;
    for (std::vector<Player>::iterator p : enemies)
    {
        if (p->getHP() < leastHealth)
        {
            leastHealth = p->getHP();
            enemy_p = p;
        }
        else if (p->getHP() == leastHealth)
        {
            if (p->getPos() < enemy_p->getPos())
                enemy_p = p;
        }
    }

    bool dead = player_p->attack(*enemy_p);
    
    if (dead) 
        return enemy_p;
    else
        return players.end();
}

bool move(std::vector<Player>::iterator & player_p, std::vector<Player> players,std::vector<std::string> & board)
{   
    std::vector<Player> enemies = getEnemies(players,player_p);

    std::set<Point> adjacentSpaces = getAdjacencies(enemies,board);

    std::set<Point> reachableSpaces = getReachableSpaces(*player_p,board);

    std::vector<Point> v;
    std::set_intersection(adjacentSpaces.begin(),adjacentSpaces.end(),
                        reachableSpaces.begin(),reachableSpaces.end(),
                        std::back_inserter(v));

    std::vector<Point> nearestSpaces = getNearestSpaces(player_p,v,board);
    
    if (nearestSpaces.size()==0)
        return false;

    std::sort(nearestSpaces.begin(),nearestSpaces.end());

    Point target = nearestSpaces.front();

    
    std::set<Point> trajectory = getTrajectory(target,board,player_p->getPos());

    std::set<Point> adjacencies = player_p->getAdjacentSpaces(board,'.');
    
    std::vector<Point> placesToGo;
    std::set_intersection(trajectory.begin(),trajectory.end(),
                        adjacencies.begin(),adjacencies.end(),
                        std::back_inserter(placesToGo));

    // if (player_p->getPos()== Point(10,19))
    // {
    //     print(board,adjacentSpaces,'?');
    //     print(board,reachableSpaces,'@');
    //     print(board,v,'$');
    //     print(board,nearestSpaces,'!');
    //     print(board,target,'+');
    //     print(board,trajectory,'*');
    //     print(board,adjacencies,'A');
    //     print(board,placesToGo,'O');
    // }
    target = placesToGo.front();

    player_p->move(target,board);
    return true;
}

bool isATeamDead(std::vector<Player> players)
{
    bool teamIsDead = true;
    
    if (players.size() <= 1)
        return  true;

    char initialTeam = ' ';
    for (Player player : players)
    {
        if (initialTeam==' ')
            initialTeam = player.getTeam();
        else
            teamIsDead &= (initialTeam == player.getTeam());
    }
    return teamIsDead;
}

int combat(std::vector<std::string> board, std::vector<Player> & players)
{
    bool teamIsDead = false;
    int rounds = 0;

    while(!teamIsDead)
    {
        for (std::vector<Player>::iterator player_p = players.begin(); player_p != players.end(); player_p++)
        {
            
            if (teamIsDead) // then this is not a full round, subtract one
            {   
                rounds--;
                break;            
            }
            
            if (!(player_p->isNearbyToAnEnemy(board)))
                move(player_p,players,board);
                
            std::vector<Player>::iterator attacked = attack(player_p,players,board);
            
            if (attacked != players.end())
            {
                attacked->die(board);
                if (distance(player_p,attacked)<0)
                    player_p--;
                players.erase(attacked);
            
            
                teamIsDead = isATeamDead(players);
            }    
        }
        rounds++;
        std::sort(players.begin(),players.end());
        // std::cout<<rounds<<std::endl;           
    }

    int totalHealth = 0;
    for (Player player : players)
        totalHealth += player.getHP();

    return rounds*totalHealth;
}

int runP1(std::string filename)
{
    std::pair<std::vector<std::string>,std::vector<Player>> boardAndPlayers = getBoardAndPlayers(filename);
    std::vector<std::string> board = boardAndPlayers.first;
    std::vector<Player> players = boardAndPlayers.second;

    return combat(board,players);      
}

int buffElves(std::vector<Player> & players, int AP)
{
    for (std::vector<Player>::iterator player = players.begin(); player != players.end(); player++)
    { 
        if (player->getTeam() == 'E')
            player->setAP(AP);
    }
}
int countElves(std::vector<Player> players)
{
    int elves = 0;
    for (Player player: players)
    {
        if (player.getTeam() == 'E')
            elves++;
    }
    return elves;
}

int runP2(std::string filename)
{
    std::pair<std::vector<std::string>,std::vector<Player>> boardAndPlayers = getBoardAndPlayers(filename);
    std::vector<std::string> board = boardAndPlayers.first;
    std::vector<Player> OGplayers = boardAndPlayers.second;
    
    int combatScore = 0;
    int lostElves = 1;
    int AP = 4;
    while (lostElves != 0)
    {
        std::vector<Player> players = OGplayers;
        buffElves(players,AP);
        int numElves = countElves(players);
        combatScore = combat(board,players);
        lostElves = numElves - countElves(players);
        AP++;
    }   
    return combatScore;
}








int main(int argc, char const *argv[])
{
    if (36334 == runP1("test1.txt"))
        std::cout << "test1 passed" << std::endl;
    if (39514 == runP1("test2.txt"))
        std::cout << "test2 passed" << std::endl;
    if (27755 == runP1("test3.txt"))
        std::cout << "test3 passed" << std::endl;
    if (28944 == runP1("test4.txt"))
        std::cout << "test4 passed" << std::endl;
    if (18740 == runP1("test5.txt"))
        std::cout << "test5 passed" << std::endl;
    // if (191216 == runP1("inputTest.txt"))
    //     std::cout << "inputTest passed";
    // printf("P1 input scored %d\n", runP1("input.txt"));
    // 346024 too low

    
    if (4988 == runP2("testP2-1.txt"))
        std::cout << "testP2-1 passed" << std::endl;
    if (31284 == runP2("testP2-2.txt"))
        std::cout << "testP2-2 passed" << std::endl;
    if (3478 == runP2("testP2-3.txt"))
        std::cout << "testP2-3 passed" << std::endl;
    if (6474 == runP2("testP2-4.txt"))
        std::cout << "testP2-4 passed" << std::endl;
    if (1140 == runP2("testP2-5.txt"))
        std::cout << "testP2-5 passed" << std::endl;
        
    printf("P2 input scored %d\n", runP2("input.txt"));

    return 0;
}

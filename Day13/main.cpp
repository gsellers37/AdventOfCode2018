#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define LEFT (std::pair<int,int>(-1,0))
#define RIGHT (std::pair<int,int>(1,0))
#define UP (std::pair<int,int>(0,-1))
#define DOWN (std::pair<int,int>(0,1))
#define NULLPAIR (std::pair<int,int>(-1,-1))

void keepWindowOpen()
{
    std::cout << "Press any key to exit" << std::endl;
    std::cin.get();
}

std::vector<std::string> readFile(std::string filename)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    
    std::string line;
    std::vector<std::string> board;

    while (inputFile.is_open() && getline(inputFile,line))
        board.push_back(line);

    return board;  
}

void print(std::vector<std::string> board)
{
    for (std::string line : board)
        std::cout << line << std::endl;
}

void writeBoard(char lastLoc, int x, int y, std::vector<std::string> & board)
{
    board[y][x] = lastLoc;
}

char readBoard(int x, int y, std::vector<std::string> & board)
{
    return board[y][x];
}

class Carts
{
    int x, y;
    char lastLoc;
    
    std::pair<int,int> dir;
    
    enum turnState_T {TURNLEFT,GOSTRAIGHT,TURNRIGHT};
    turnState_T turnState = TURNLEFT;
    
    public:
        Carts(int initialX, int initialY, std::pair<int,int> initialDir)
        {
            x = initialX;
            y = initialY;
            dir = initialDir;
            
            if (dir == LEFT || dir == RIGHT)
                lastLoc = '-';
            if (dir == UP || dir == DOWN)
                lastLoc = '|';
        }
        std::pair<int,int> advance(std::vector<std::string> & board)
        {   
            x += dir.first;
            y += dir.second;

            char instruction = readBoard(x,y, board);
            
            if (instruction == '+')
            {
                if (turnState == TURNLEFT)
                {    
                    turnLeft();
                    turnState = GOSTRAIGHT;
                }
                else if (turnState == TURNRIGHT)
                {
                    turnRight();
                    turnState = TURNLEFT;
                }
                else 
                {
                    turnState = TURNRIGHT;
                }
            }
            else if (instruction == '/')
            {
                if (dir == LEFT || dir == RIGHT)
                    turnLeft();
                else if (dir == UP || dir == DOWN)
                    turnRight();
            }
            else if (instruction == '\\')
            {
                if (dir == LEFT || dir == RIGHT)
                    turnRight();
                else if (dir == UP || dir == DOWN)
                    turnLeft();
            }
            else if (instruction == '>' || instruction == '<' || instruction == '^' || instruction == 'v')
                return std::pair<int,int>(x,y);

            lastLoc = instruction;

            return NULLPAIR;
        }
        void refreshBoard(std::vector<std::string> & board)
        {
            if (dir == RIGHT)
                writeBoard('>',x,y,board);
            else if (dir == UP)
                writeBoard('^',x,y,board);
            else if (dir == LEFT)
                writeBoard('<',x,y,board);
            else if (dir == DOWN)
                writeBoard('v',x,y,board);
        }
        void removeFromBoard(std::vector<std::string> & board)
        {
            writeBoard(lastLoc,x,y,board);
        }
        void turnLeft()
        {
            if (dir==LEFT)
                dir = DOWN;
            else if (dir == DOWN)
                dir = RIGHT;
            else if (dir == RIGHT)
                dir = UP;
            else if (dir == UP)
                dir = LEFT;
        }
        void turnRight()
        {
            if (dir==LEFT)
                dir = UP;
            else if (dir == DOWN)
                dir = LEFT;
            else if (dir == RIGHT)
                dir = DOWN;
            else if (dir == UP)
                dir = RIGHT;
        }
        bool operator<(Carts other)
        {
            return y < other.y ? true : y == other.y ? x < other.x : false;
        }
        bool operator==(Carts other)
        {
            return x == other.x && y == other.y;
        }
        void print()
        {
            std::cout << "Cart at x,y: " << x << "," << y << 
                         " with dir (" << dir.first << "," << dir.second << ") " <<
                         "and last location: " << lastLoc << std::endl;
        }
        std::pair<int,int> getPos()
        {
            return std::pair<int,int>(x,y);
        }
        int getLastLoc()
        {
            return lastLoc;
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

std::vector<Carts> getCarts(std::vector<std::string> const & board)
{
    std::vector<Carts> carts;
    for (int y = 0; y < board.size(); y++)
    {
        for (int x = 0; x < board[0].size(); x++)
        {
            char c = board[y][x];
            if (c == '>')
                carts.push_back(Carts(x,y,RIGHT));
            else if (c == '<')
                carts.push_back(Carts(x,y,LEFT));
            else if (c == '^')
                carts.push_back(Carts(x,y,UP));
            else if (c == 'v')
                carts.push_back(Carts(x,y,DOWN));
        }
    }
    // Carts were built in sorted order, no need to sort for advancing
    return carts;
}

void print(std::vector<Carts> carts)
{
    for (Carts cart : carts)
        cart.print();
}

std::pair<int,int> run(std::string filename, bool debug)
{
    if (debug)
        std::cout << "Debug" << std::endl;

    std::vector<std::string> board = readFile(filename);
    
    if (debug)
        print(board);

    std::vector<Carts> carts = getCarts(board);

    if (debug)
        print(carts);

    std::pair<int,int> val = NULLPAIR;
    while (val == NULLPAIR) // while no crash
    {
        for (std::vector<Carts>::iterator it = carts.begin(); it != carts.end(); it++)
        {
            it->removeFromBoard(board);
            val = it->advance(board);
            it->refreshBoard(board);
            if (val != NULLPAIR)
                break;
        }
        std::sort(carts.begin(),carts.end());
        if (debug)
            print(board);
    }
    if (debug)
        print(carts);
    return val;
}
std::pair<int,int> runP2(std::string filename, bool debug)
{
    if (debug)
        std::cout << "Debug" << std::endl;

    std::vector<std::string> board = readFile(filename);
    
    if (debug)
        print(board);

    std::vector<Carts> carts = getCarts(board);

    if (debug)
        print(carts);

    std::pair<int,int> val = NULLPAIR;
    
    while (carts.size()>1) // while no crash
    {
        std::vector<std::vector<Carts>::iterator> to_remove;
        for (std::vector<Carts>::iterator it = carts.begin(); it != carts.end();)
        {
            it->removeFromBoard(board);
            val = it->advance(board);
            
            if (val != NULLPAIR)
            {
                for (std::vector<Carts>::iterator ite = carts.begin(); ite != carts.end();)
                {
                    if (!distance(it,ite)==0 && *it == *ite)
                    {
                        if (distance(it,ite) < 0)
                            it--;
                        ite->removeFromBoard(board);
                        ite = carts.erase(ite);
                    }   
                    else
                        ite++;
                }
                it = carts.erase(it);
            }
            else
            {
                it->refreshBoard(board);
                it++;
            }
        }
        
        std::sort(carts.begin(),carts.end());
        
        if (debug)
            print(board);
        if (debug)
            print(carts);
    }
    if (carts.size()==0)
        return val;
    else
        return carts.front().getPos();
}

int main(int argc, char const *argv[])
{
    bool failedTest = false;
    if ( run("test.txt",false) == std::pair<int,int>(7,3))
        std::cout << "Passed test #1" << std::endl;
    else
    {
        failedTest = true;
        std::cout << "Failed test #1" << std::endl;
        run("test.txt",true);
    }

    if (!failedTest)
    {
        std::pair<int,int> output = run("input.txt",false);
        std::cout << "P1 X: " << output.first << " Y: " << output.second << std::endl;
    }

    failedTest = false;
    if ( runP2("testP2.txt",false) == std::pair<int,int>(6,4))
        std::cout << "Passed test #2" << std::endl;
    else
    {
        failedTest = true;
        std::cout << "Failed test #2" << std::endl;
        runP2("testP2.txt",true);
    }

    if (!failedTest)
    {
        std::pair<int,int> output = runP2("inputP2.txt",false);
        std::cout << "P2 X: " << output.first << " Y: " << output.second << std::endl;
    }

    keepWindowOpen();
    return 0;
}

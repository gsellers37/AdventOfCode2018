#include <iostream>
#include <vector>
#include <fstream>

#define MAXY 1768
#define XOFFSET 400
#define MAXX 201


bool moveSideways(std::vector<std::string> &, int, int, int);
void dropSideways(std::vector<std::string> &, int, int, int);
void drop(std::vector<std::string> & board, int, int);
void fill(std::vector<std::string> &, int, int);

void keepWindowOpen()
{
    std::cout << "Press any key to exit. " << std::endl;
    std::cin.get();
}

void print(std::vector<std::string> board)
{
    for (std::string line : board)
        std::cout << line << std::endl;
}

void parseFile(std::string filename, std::vector<std::string> & board)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    if(inputFile.is_open())
    {
        std::string line;
        while (getline(inputFile,line))
        {
            int pos, min, max;
            char char1, char2;
            sscanf(line.c_str(),"%c=%d, %c=%d..%d",&char1,&pos,&char2,&min,&max);
        
            if (char1 == 'x')
            {
                int x = pos;
                for (int y = min; y <= max; y++)
                    board[y][x-XOFFSET] = '#';
            }
            else if (char1 == 'y')
            {
                int y = pos;
                for (int x = min; x <= max; x++)
                    board[y][x-XOFFSET] = '#';
            }
        }
    }
    else    
        std::cout << "Unable to read file: " << filename << std::endl;
}


int runP1(std::string filename)
{
    std::vector<std::string> board;
    for (int i = 0; i < MAXY; i++)
    {
        std::string line;
        line.append(MAXX,'.');
        board.push_back(line);
    }

    parseFile(filename,board);

    board[0][100] = '+';

    int x = 100;
    int y = 1;
    drop(board,x,y);
    
    int sum = 7;
    print(board);

    
    for (std::string line : board)
        for (char c : line)
            if ( c == '~')
                sum++;
    return sum;
}


int main(int argc, char const *argv[])
{
    std::cout  << runP1("input.txt");
    keepWindowOpen();
    return 0;
}


bool moveSideways(std::vector<std::string> & board, int x, int y, int dir)
{
    while(board[y][x] != '#')
    {
        if (board[y+1][x] == '.' || board[y+1][x] == '|')
            return false;
        board[y][x] = '~';
        x += dir;
    }
    return true;
}
void dropSideways(std::vector<std::string> & board, int x, int y, int dir)
{
    while(board[y][x] != '#')
    {
        if (board[y+1][x] == '.' || board[y+1][x] == '|')
        {
            drop(board,x,y);
            return;
        }
        board[y][x] = '|';
        x += dir;
    }
}
void fill(std::vector<std::string> & board, int x, int y)
{
    while (moveSideways(board,x,y,-1) && moveSideways(board,x,y,1))
        y--;
    dropSideways(board,x,y,-1);
    dropSideways(board,x,y,1);
}
void drop(std::vector<std::string> & board, int x, int y)
{
    while(board[y][x] != '#')
    {
        board[y][x] = '|';
        if (y >= MAXY-1)
            return;
        y++;
    }
    y--;
    fill(board,x,y);
}

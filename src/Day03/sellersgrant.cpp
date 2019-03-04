#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstring>
#include <set>
#include <vector>

#define BOARD_SIZE 1000

struct Claim{
    int id, x, y, w, h;
};

inline bool operator<(const Claim& lhs, const Claim& rhs)
{
  return lhs.id < rhs.id;
}

Claim parseData(std::string line)
{
    Claim rect;

    std::size_t i_pound  = line.find("#");
    std::size_t i_at = line.find("@");
    std::size_t i_comma = line.find(",");
    std::size_t i_colon = line.find(":");
    std::size_t i_x = line.find("x");

    rect.id = std::stoi(line.substr(i_pound+1,
                                i_at-i_pound-2));
    rect.x = std::stoi(line.substr(i_at+1, 
                                i_comma-i_at-1));
    rect.y = std::stoi(line.substr(i_comma+1,
                                i_colon-i_comma-1));
    rect.w = std::stoi(line.substr(i_colon+2, 
                                i_x-i_colon-2));
    rect.h = std::stoi(line.substr(i_x+1, 
                                line.length()-i_x-1));

    return rect;
}

void resetFile(std::istream* inputFile)
{
    inputFile->clear();
    inputFile->seekg(0,std::ios::beg);
}

void printSet(std::set<Claim> ids)
{
    std::cout << "Set: " << std::endl;
    std::set<Claim>::iterator it;
    for (it = ids.begin(); it != ids.end(); it++)
    {
        std::cout << it->id << " ";
    }
    std::cout << std::endl;
}
void fillInRectangle(Claim rect,int* board)
{
    for (int i = rect.x; i< rect.x+rect.w; i++)
    {
        for (int j = rect.y; j < rect.y + rect.h; j++)
        {
            board[BOARD_SIZE*i+j]++;
        }
    }
}

bool checkCollision(int i, int j, std::set<Claim>::iterator it)
{
    return (i >= it->x && 
            i <= it->x + it->w &&
            j >= it->y && 
            j <= it->y + it->h);
}

std::set<Claim> fillInRectangleWhileChecking(Claim rect, int*board, std::set<Claim> uncontested_ids)
{
    int overlaps = false;
    for (int i = rect.x; i< rect.x+rect.w; i++)
    {
        for (int j = rect.y; j < rect.y + rect.h; j++)
        {
            if(board[BOARD_SIZE*i+j] > 0)
            {
                overlaps = true; 
                std::vector<std::set<Claim>::iterator> to_delete;
                for(std::set<Claim>::iterator it = uncontested_ids.begin(); it != uncontested_ids.end(); it++)
                {
                    if (checkCollision(i,j,it))
                        to_delete.push_back(it);            
                }
                while (!to_delete.empty())
                {
                    uncontested_ids.erase(to_delete.back());
                    to_delete.pop_back();
                }
            }

            board[BOARD_SIZE*i+j]++;
        }
    }

    if (!overlaps)
    {
        if (uncontested_ids.size()==uncontested_ids.max_size())
        {   
            std::cout << "Set is too big" << std::endl;
            return uncontested_ids;
        }
        uncontested_ids.insert(rect);
    }
    return uncontested_ids;
}

int countAnswer(int* board)
{
    int sum = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[BOARD_SIZE*i+j] >=2)
                sum++;
        }
    }
    return sum;
}

int* createBoard()
{
    int* board = new int[BOARD_SIZE*BOARD_SIZE];
    return board;
}

int problemOne(std::ifstream* inputFile)
{    
    std::string line;
    int sum = 0;

    int* board = createBoard();
    
    while (getline(*inputFile, line))
    {
        Claim rect = parseData(line);
        fillInRectangle(rect,board);
    }

    int answer = countAnswer(board);

    delete [] board;
    return answer;
}


int problemTwo(std::ifstream* inputFile)
{    
    std::string line;
    int sum = 0;

    int* board = createBoard();
    
    std::set<Claim> ids;
    while (getline(*inputFile, line))
    {
        Claim rect = parseData(line);

        ids = fillInRectangleWhileChecking(rect,board,ids);
    }
    
    resetFile(inputFile);
    delete [] board;

    printSet(ids);
    return 0;
}

void printAnswer(int answer)
{
    std::cout << "Sum = " << answer << std::endl;
}


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        return -1;
    }

    std::ifstream inputFile;
    inputFile.open(argv[1]);
    if (inputFile.is_open())
    {
        printAnswer(problemOne(&inputFile));

        resetFile(&inputFile);
        printAnswer(problemTwo(&inputFile));
    }
    else
    {
        std::cout << "Unable to open file." << argv[1];
        return -1;
    }

    inputFile.close();
    return 0;
}

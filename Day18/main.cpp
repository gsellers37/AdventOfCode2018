#include <iostream>
#include <fstream>
#include <vector>
#include <map>

typedef std::vector<std::pair<int,int>> Places;
typedef std::pair<int,int> Place;

void keepWindowOpen()
{
    std::cout << "Press any key to exit. " << std::endl;
    std::cin.get();
}

void print(std::vector<std::string> board)
{
    for (std::string line : board)
        std::cout << line << std::endl;
    std::cout << std::endl;
}
void parseFile(std::string filename, std::vector<std::string> & board)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    if(inputFile.is_open())
    {
        std::string line;
        while (getline(inputFile,line))
            board.push_back(line);
    }
    else
        std::cout << "Unable to open file: " << filename << std::endl;
}

void readBoard(std::vector<std::string> board, Places & lumber, Places & trees, Places & open)
{
    int xBound = board[0].size();
    int yBound = board.size();
    for (int x = 0; x < xBound; x++)
        for (int y = 0; y < yBound; y++)
        {
            char spot = board[y][x];
            Place p(x,y);
            if (spot == '.')    
                open.push_back(p);
            else if (spot == '#')
                lumber.push_back(p);
            else if (spot == '|')
                trees.push_back(p);
        }
}

void kernel(std::vector<std::string> board, Place place, int & numTree, int & numOpen, int & numLumber)
{
    int xBound = board[0].size();
    int yBound = board.size();
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            Place p(i+place.first,j+place.second);
            if (p.first<0 || p.second < 0 || p.first >= xBound || p.second >= yBound)
                continue;
            if (i == 0 && j == 0)
                continue;

            char spot = board[p.second][p.first];
            if (spot == '.')
                numOpen++;
            else if (spot == '#')
                numLumber++;
            else if (spot == '|')
                numTree++;
        }       
    }
}
void updateLumber(std::vector<std::string> board, Places places, Places & newLumber, Places & newTrees, Places & newOpen)
{
    for (Place place : places)
    {
        int numTrees = 0;
        int numOpen = 0;
        int numLumber = 0;
        kernel(board,place,numTrees,numOpen,numLumber);
        if (numLumber >= 1 && numTrees >= 1)
            newLumber.push_back(place);
        else
            newOpen.push_back(place);
    }
}

void updateTrees(std::vector<std::string> board, Places places, Places & newLumber, Places & newTrees, Places & newOpen)
{
    for (Place place : places)
    {
        int numTrees = 0;
        int numOpen = 0;
        int numLumber = 0;
        kernel(board,place,numTrees,numOpen,numLumber);
        if (numLumber >= 3)
            newLumber.push_back(place);
        else
            newTrees.push_back(place);
    }
}
void updateOpen(std::vector<std::string> board, Places places, Places & newLumber, Places & newTrees, Places & newOpen)
{
    for (Place place : places)
    {
        int numTrees = 0;
        int numOpen = 0;
        int numLumber = 0;
        kernel(board,place,numTrees,numOpen,numLumber);
        if (numTrees >= 3)
            newTrees.push_back(place);
        else
            newOpen.push_back(place);
    }
}

void updatePlaces(std::vector<std::string> board, Places & lumber, Places & trees, Places & open, Places & newLumber, Places & newTrees, Places & newOpen)
{
    updateLumber(board,lumber,  newLumber,  newTrees,  newOpen);
    updateTrees(board,trees,  newLumber,  newTrees,  newOpen);
    updateOpen(board,open,  newLumber, newTrees,  newOpen);
}
void writeBoard(std::vector<std::string> & board, Places lumber, Places tree, Places open)
{
    for (Place place : lumber)
        board[place.second][place.first] = '#';
    for (Place place : tree)
        board[place.second][place.first] = '|';
    for (Place place : open)
        board[place.second][place.first] = '.';
}

void print(Places places)
{
    for (Place place : places)
        printf("(%d,%d)\n",place.first,place.second);
    std::cout<<std::endl;
}
int countResources(std::vector<std::string> board)
{
    
    int numLumber = 0;
    int numTree = 0;
    for (int x = 0; x < board[0].size(); x++)
    {
        for (int y = 0; y < board.size(); y++)
        {
            char spot = board[y][x];
            if (spot == '|')
                numTree++;
            else if (spot == '#')
                numLumber++;
        }
    }
    return numLumber*numTree;
}
int runP1(std::string filename)
{
    std::vector<std::string> board;
    parseFile(filename, board);

 
    long long iters = 1000000000;
    std::map<std::vector<std::string>,long long> resourceMap;
    for (long long  i = 0; i < iters; i++)
    {
        std::cout << i << std::endl;
        Places lumber, trees, open;
        readBoard(board,lumber,trees,open);

        Places newTrees, newLumber, newOpen;
        updatePlaces(board,lumber,trees,open,newLumber,newTrees,newOpen);
        
        writeBoard(board,newLumber,newTrees,newOpen);
        // print(board);

        if (resourceMap.count(board) >= 1)
        {
            long long skip = (iters-i)/(i-resourceMap[board]);
            i += skip * (i-resourceMap[board]);
        }
        else
            resourceMap[board] = i;

    }
    return countResources(board);
}

int main(int argc, char const *argv[])
{
    std::cout << runP1("input.txt") << std::endl;
    keepWindowOpen();
    return 0;
}

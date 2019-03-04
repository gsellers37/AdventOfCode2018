#include <iostream>
#include <fstream>
#include <vector>
#include <list>

void print(std::list<int> circle)
{
    for (std::list<int>::iterator it = circle.begin(); it != circle.end(); it++)
        std::cout << *it << " ";
        
    std::cout << std::endl << std::endl;
}

uint64_t problemOne(std::string line)
{
    int numPlayers, lastMarble;
    sscanf(line.c_str(), "%d players; last marble is worth %d points", &numPlayers, &lastMarble);
    
    std::list<int> circle = {0};

    uint64_t score = 0;
    int marble = 1;
    std::list<int>::iterator position = circle.begin();

    std::list<uint64_t> players(numPlayers);
    std::list<uint64_t>::iterator player = players.begin();

    while (marble <= lastMarble)
    {
        if (marble % 23 == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (position == circle.begin())
                    position = circle.end();
                position--;
            }

            score = *position;
            position = circle.erase(position);
            
            if (position == circle.end())
                position = circle.begin();
            position++;

            *player += score + marble;
        }
        else{
            
            if (position == circle.end())
                position = circle.begin();
            position++;

            circle.insert(position, marble);
        }
        // print(circle);
        
        if (++player == players.end())
            player++;
        marble++;
    }

    // std::cout << "POSITION: " << *position;
    uint64_t highScore = 0;
    for (auto x : players)
    {
        if (x > highScore)
            highScore = x;
    }

    return highScore;
}

int main(int argc, char const *argv[])
{
    std::ifstream inputFile;
    std::string filename = "../inputs/Day09/sellersgrant.txt";
    inputFile.open(filename);
    if (inputFile.is_open())
    {
        std::string line;
        while (getline(inputFile, line))
        {
            std::cout << line << std::endl << problemOne(line) << std::endl;
        }
    }
    else
    {
        std::cout << "Unable to open file: " << filename;
        return -1;
    }

    return 0;
}

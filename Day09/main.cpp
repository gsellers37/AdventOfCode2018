#include <iostream>
#include <fstream>
#include <vector>
#include <list>
int keepWindowOpen()
{
    int x;
    std::cout << "Press any key to exit." << std::endl;
    std::cin >> x;
}

void print(std::list<int> circle)
{
    for (std::list<int>::iterator it = circle.begin(); it != circle.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << std::endl;
}

u_int64_t problemOne(std::string line)
{
    int numPlayers, lastMarble;
    sscanf(line.c_str(), "%d players; last marble is worth %d points", &numPlayers, &lastMarble);
    
    std::list<int> circle = {0};

    u_int64_t score = 0;
    int marble = 1;
    std::list<int>::iterator position = circle.begin();

    std::list<u_int64_t> players(numPlayers);
    std::list<u_int64_t>::iterator player = players.begin();

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
    inputFile.open(argv[1]);
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
        std::cout << "Unable to open file: " << argv[1];
        return -1;
    }

    keepWindowOpen();
    return 0;
}

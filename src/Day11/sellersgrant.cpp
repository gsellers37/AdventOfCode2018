#include <iostream>
#include <algorithm>
#define BOUND 300

int getHundredsDigit(int powerLevel)
{
    std::string toString = std::to_string(powerLevel);
    
    if (toString.size()>2)
        return (int)(toString[toString.size()-3])-48;
    else
        return 0;
}

int getEnergy(int x, int y, int serial)
{
    int rackID = x + 10;
    int powerLevel = rackID * y;
    powerLevel += serial;
    powerLevel *= rackID;
    powerLevel = getHundredsDigit(powerLevel);
    powerLevel -= 5;;

    return powerLevel;
}

int run(int serial, int KERNEL, bool debug)
{
    int board[BOUND*BOUND];
    for (int i = 0; i< BOUND; i++)
    {
        for (int j = 0; j < BOUND; j++)
        {
            board[i*BOUND + j] = getEnergy(j+1,i+1,serial);
        }
    }
    int energies[(BOUND-KERNEL)*(BOUND-KERNEL)] = {0};
    for (int i = 0; i < BOUND - KERNEL; i++)
    {
        for (int j = 0; j < BOUND - KERNEL; j++)
        {
            for (int k = 0; k < KERNEL; k++)
            {
                for (int l = 0; l < KERNEL; l++)
                {
                    energies[i*(BOUND-KERNEL) + j] += board[(i+k)*BOUND+j+l];
                }
            }
        }
    }

    int* maxEle = std::max_element(&energies[0], &energies[(BOUND-KERNEL)*(BOUND-KERNEL)]);
    int dis = std::distance(&energies[0], maxEle);
    int x, y;
    if (KERNEL == 300)
    {
        y = 1;
        x = 1;
    }
    else
    {
        y = dis / (BOUND-KERNEL) + 1;
        x = dis % (BOUND-KERNEL) + 1;
    }

    if (debug)
    {
        std::cout << "Serial: " << serial << std::endl;
        std::cout << "Max ELE: " << *maxEle << std::endl;
        std::cout << "Distance: " << dis << std::endl;
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
    }

    return *maxEle;
}

int main(int argc, char const *argv[])
{
    // PART ONE
    run(7989, 3, true);


    // PART TWO
    int max = -UINT16_MAX;
    int bestKernel = 0;
    for (int kernel = 1; kernel <= BOUND; kernel++)  
    {
        std::cout << kernel << std::endl;
        int next = run(7989, kernel, false);
        if (next > max)
        {
            max = next;
            bestKernel = kernel;
        }
    }
    std::cout << "Kernel size: " << bestKernel << std::endl;
    run(7989, bestKernel, true);
    return 0;
}

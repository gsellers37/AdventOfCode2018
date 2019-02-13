#include <iostream>
#include <fstream>
#include <vector>

void keepWindowOpen()
{
    int x;
    std::cout << "Press any key to exit" << std::endl;
    std::cin >> x;
}

void print(std::string polymer)
{
    for (std::string::iterator it = polymer.begin(); it != polymer.end(); it++)
    {
        std::cout << *it;
    }
    std::cout << std::endl;
}

bool check(char a, char b)
{
    bool sameChar = tolower(a) == tolower(b);
    bool sameCap  = std::isupper(a)==std::isupper(b) || std::islower(a) == std::islower(b);
    return  !(sameChar && !sameCap);
}

int main(int argc, char const *argv[])
{
    std::string polymer;

    std::ifstream inputFile;
    inputFile.open(argv[1]);

    char molecule;
    while (inputFile.get(molecule))
    {
        if (check(polymer.back(),molecule))
            polymer.push_back(molecule);
        else
            polymer.pop_back();
    }
    
    print(polymer);

    std::cout << "Answer: " << polymer.length();

    keepWindowOpen();
    return 0;
}

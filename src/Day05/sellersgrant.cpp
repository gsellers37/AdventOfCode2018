#include <iostream>
#include <fstream>
#include <vector>

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


int getPolymerSansElement(char culprit, std::string filename)
{
    std::string polymer;

    std::ifstream inputFile;
    inputFile.open(filename);

    char molecule;
    while (inputFile.get(molecule))
    {
        if (tolower(molecule) == culprit)
            continue;
        if (check(polymer.back(),molecule))
            polymer.push_back(molecule);
        else
            polymer.pop_back();
    }
    

    inputFile.close();
    
    return polymer.length();
}

int main(int argc, char const *argv[])
{
    std::cout << "Answer part 1: " << getPolymerSansElement(' ', argv[1]) << std::endl;

    for (char culprit = 'a'; culprit < 'z';culprit++)
    {
        std::cout << "Answer part 2" << culprit << ": " << getPolymerSansElement(culprit,argv[1]) << std::endl;
    }
    return 0;
}

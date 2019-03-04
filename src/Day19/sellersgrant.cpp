
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "operations.h"

// typedef std::vector<int> Registers;
// typedef std::pair<std::string,Registers> Instruction; 
typedef std::map<std::string,bool (*)(std::vector<int> &, int, int, int )> StrToFunc;

void parseData(std::string filename, int * ip, std::vector<std::pair<std::string,std::vector<int>>> * instructions)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    if (inputFile.is_open())
    {
        std::string line;
        getline(inputFile,line);
        sscanf(line.c_str(),"#ip %d", &ip);
        
        char Ins[4];
        int a, b, c;
        while(getline(inputFile,line))
        {
            sscanf(line.c_str(),"%s %d %d %d", &Ins, &a, &b, &c);
            std::vector<int> regs = {a,b,c};
            std::pair<std::string,std::vector<int>> op = std::make_pair(Ins, regs);
            instructions->push_back(op);
        }
    }
    else
        std::cout << "Unable to open file: " << filename << std::endl;

}

void print(std::vector<std::pair<std::string,std::vector<int>>> instructions)
{
    for (std::pair<std::string,std::vector<int>> ins : instructions)
    {
        std::cout << ins.first << ' ' <<
        ins.second[0] << ' ' <<
        ins.second[1] << ' ' <<
        ins.second[2] << std::endl;
    }
}
int runP1(std::string filename, const StrToFunc opcodeMap)
{
    std::vector<std::pair<std::string,std::vector<int>>> instructions;
    int ip = -1;
    parseData(filename, &ip, &instructions);

    if (ip == -1)
        return 0;
    
    print(instructions);
    
}

int main(int argc, char const *argv[])
{    
    StrToFunc opcodeMap;
    // opcodeMap;
    std::cout << runP1("../inputs/Day01/sellersgrant",opcodeMap) << std::endl;

    return 0;
}

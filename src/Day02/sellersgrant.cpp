#include <iostream>
#include <fstream>
#include <map>
#include <vector>

int problemOne(std::ifstream* inputFile)
{
    int sum2 = 0;
    int sum3 = 0;
    std::string line;
    
    while(getline(*inputFile, line))
    {
        std::string::iterator i;
        std::map<char, int> repeatMap;

        for (i = line.begin(); i != line.end(); i++)
        {
            if(repeatMap.count(*i))
            {
                repeatMap.find(*i)->second += 1;
            }
            else
            {               
                repeatMap.emplace(*i, 1);
            }
        }

        bool flag2 = false;
        bool flag3 = false;
        for (std::map<char, int>::iterator it = repeatMap.begin(); it != repeatMap.end(); it++)
        {
            if (it->second == 2 && !flag2){
                sum2++;
                flag2 = true;
            }
            if (it->second == 3 && !flag3)
            {
                sum3++;
                flag3 = true;
            }
        }
    }
    return sum2*sum3;
}

int problemTwo(std::ifstream* inputFile)
{
    std::string line;
    std::vector<std::string> list;
    while(getline(*inputFile,line))
    {
        list.push_back(line);
    }
    int len = line.length();
    
    for (std::vector<std::string>::iterator i = list.begin(); i != list.end(); i++)
    {
        for (std::vector<std::string>::iterator j = list.begin(); j != list.end(); j++)
        {
            int diffs = 0;
            int diffLoc = 0;
            for (int k = 0; k<len; k++)
            {
                if( (*i)[k] != (*j)[k])
                {
                    diffs +=1;
                    diffLoc = k;
                }
            }
            if (diffs == 1)
            {
                std::cout << *i << " " << *j << std::endl;
            }
        }
    }
}

void printAnswer(int answer)
{
    std::cout << "Answer: " << answer << std::endl;
}

int main(int argc, char const *argv[])
{
    std::ifstream inputFile;
    inputFile.open(argv[1]);

    if (inputFile.is_open())
    {    
        printAnswer(problemOne(&inputFile));    

        inputFile.clear();
        inputFile.seekg(0,std::ios_base::beg);

        printAnswer(problemTwo(&inputFile));
    }
    else
    {
        std::cout << "File not able to be opened." << std::endl;
        return -1;
    }
    inputFile.close();

    return 0;
}

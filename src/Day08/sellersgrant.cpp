#include <iostream> 
#include <fstream>
#include <stack>
#include <vector>

class Header 
{
    int numChildren, numMetadata;
    std::vector<Header> children;
    std::vector<int> metadata;
    
    public:
        Header(std::ifstream *inputFile)
        {
            std::string line;

            getline(*inputFile, line, ' ');
            numChildren = stoi(line);

            getline(*inputFile, line, ' ');
            numMetadata = stoi(line);

            for (int i = 0; i < numChildren; i++)
            {
                children.push_back(Header(inputFile));
            }
            for (int j = 0; j < numMetadata; j++)
            {
                getline(*inputFile, line, ' ');
                metadata.push_back(stoi(line));
            }

        }

        int getSum()
        {
            int sum = 0;

            for (std::vector<Header>::iterator it = children.begin(); it != children.end(); it++)
                sum += it->getSum();

            for (std::vector<int>::iterator it = metadata.begin(); it != metadata.end(); it++)
                sum += *it;

            return sum;
        }

        int getVal()
        {
            int val = 0;

            if (numChildren == 0)
            {
                for (std::vector<int>::iterator it = metadata.begin(); it != metadata.end(); it++)
                    val += *it;

            }

            else
            {
                for (std::vector<int>::iterator it = metadata.begin(); it != metadata.end(); it++)
                {
                    if (*it-1 < children.size())
                        val += children[*it-1].getVal();
                }
            }
            return val;
            
        }

};

int getProblemOne(std::ifstream * inputFile)
{
    Header top = Header(inputFile);
    return top.getSum();
}
int getProblemTwo(std::ifstream * inputFile)
{
    Header top = Header(inputFile);
    return top.getVal();
}

int main(int argc, char const *argv[])
{
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    if (inputFile.is_open())
    {
        std::cout << getProblemOne(&inputFile) << std::endl;

        inputFile.clear();
        inputFile.seekg(0,std::ios::beg);

        std::cout << getProblemTwo(&inputFile) << std::endl;
    }
    else{
        std::cout << "Unable to open file: " << argv[1] << std::endl;
        return -1;
    }
    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#define BOUNDS 26
#define OFFSET 61
#define WORKERS 5

void keepWindowOpen()
{
    int x;
    std::cout << "Press any key to exit. ";
    std::cin >> x;
}

int convertChar(char a)
{
    return (int)(a) - 65;
}

char convertInt(int a)
{
    return (char)(a + 65);
}

void extend(std::vector<int>* buffer, std::vector<int> to_append)
{
    for (std::vector<int>::iterator it = to_append.begin(); it != to_append.end(); it++)
    {
        if (std::find(buffer->begin(),buffer->end(), *it) == buffer->end())
            buffer->push_back(*it);
    }
}

class Graph
{
    int data[BOUNDS][BOUNDS] = {0};
    public:

        void insert(int a, int b)
        {
            data[a][b] = 1;
        }

        void print()
        {
            for (int i = 0; i<BOUNDS; i++)
            {
                for (int j = 0; j<BOUNDS; j++)
                {
                    std::cout << data[i][j];
                }
                std::cout << std::endl;
            }
        }
        
        void print(std::vector<int> toPrint)
        {
            for (std::vector<int>::iterator it = toPrint.begin(); it != toPrint.end(); it++)
                std::cout << *it;
        }

        std::string getProblemOne()
        {
            std::string output = "";
            std::vector<int> outputInt;
            std::vector<int> options = this->findBegins();

            while(!options.empty())
            {
                std::vector<int>::iterator toRemove;
                std::vector<int> new_options;
                //iterate over options, choosing the first one which has been previously sorted
                for (std::vector<int>::iterator it = options.begin(); it != options.end(); it++)
                {
                    // Check each element to see if the option has met given dependencies
                    if (checkRequirements(outputInt,*it))
                    {
                        // If the node hasn't already been added, add it. Otherwise, just remove from options
                        if (std::find(outputInt.begin(), outputInt.end(), *it) == outputInt.end())
                        {
                            outputInt.push_back(*it);
                            new_options = this->getRow(*it);
                        }
                        toRemove =  it;
                        break;
                    }
                }
                // Remove chosen element from viable options
                options.erase(toRemove);
                // Add the chosen elements branches to options
                extend(&options,new_options);                
                // Sort options so that the first valid option will appear in the next for loop
                std::sort(options.begin(),options.end());
            }

            // Convert integer indices to character vector
            for (std::vector<int>::iterator it = outputInt.begin(); it != outputInt.end(); it++)
            {
                output.push_back(convertInt(*it));
            }
            
            return output;
        }

        int getProblemTwo()
        {
            std::string output = "";
            std::vector<int> outputInt;
            
            std::vector<int> options = this->findBegins();

            std::vector<std::pair<int,int>> workers;

            int answer = -1; // since the first iteration is round second 0

            while(!options.empty() || workerIsBusy(workers))
            {

                std::vector<int> finishedWorkers = getFinishedWorkers(&workers);
                for (std::vector<int>::iterator it = finishedWorkers.begin(); it!= finishedWorkers.end(); it++)
                {
                    if (std::find(outputInt.begin(), outputInt.end(), *it) == outputInt.end())
                    {
                        outputInt.push_back(*it);
                        extend(&options,this->getRow(*it));                
                    }
                } 

                std::vector<int>::iterator toRemove = options.end();

                bool optionsAvailable = !options.empty();;
                while (workerIsFree(workers) && optionsAvailable)
                {
                    bool noOptionsMeetRequirements = true;
                    //iterate over options, choosing the first one which has been previously sorted
                    for (std::vector<int>::iterator it = options.begin(); it != options.end(); it++)
                    {
                        // Check each element to see if the option has met given dependencies
                        if (checkRequirements(outputInt,*it))
                        {
                            toRemove =  it;
                            if (std::find(outputInt.begin(), outputInt.end(), *it) == outputInt.end())
                                startWorker(&workers,*it);
                            
                            noOptionsMeetRequirements = false;
                            break;
                        }
                    }
                    // Remove chosen element from viable options
                    if (toRemove != options.end())
                        options.erase(toRemove);
                    // Sort options so that the first valid option will appear in the next for loop
                    std::sort(options.begin(),options.end());

                    optionsAvailable = !options.empty() & !noOptionsMeetRequirements;

                }

                answer++;
                advanceWorkers(&workers);
            }

            
            
            return answer;
        }
    
    private:
        void printO(std::vector<int> outputInt)
        {
            // Convert integer indices to character vector
            for (std::vector<int>::iterator it = outputInt.begin(); it != outputInt.end(); it++)
            {
                std::cout << convertInt(*it);
            }
        }

        void printW(std::vector<std::pair<int,int>> workers)
        {
            std::cout << " Workers: ";
            for (std::vector<std::pair<int,int>>::iterator it = workers.begin(); it != workers.end(); it++)
            {
                std::cout << convertInt(it->first);
            }
        }
        bool workerIsFree(std::vector<std::pair<int,int>> workers)
        {
            return workers.size() < WORKERS;
        }
        bool workerIsBusy(std::vector<std::pair<int,int>> workers)
        {
            return workers.size() >0 ;
        }

        void startWorker(std::vector<std::pair<int,int>>* workers, int element)
        {
            workers->push_back(std::pair<int,int>(element, element+OFFSET));
        }

        std::vector<int> getFinishedWorkers(std::vector<std::pair<int,int>>* workers)
        {
            std::vector<std::vector<std::pair<int,int>>::iterator> to_remove;
            std::vector<int> finishedWorkers;

            for (std::vector<std::pair<int,int>>::iterator it = workers->begin(); it != workers->end(); it++)
            {
                if (it->second == 0)
                    to_remove.push_back(it);
            }
            for (std::vector<std::vector<std::pair<int,int>>::iterator>::iterator it = to_remove.begin(); it!= to_remove.end(); it++)
            {
                finishedWorkers.push_back((*it)->first);
                workers->erase(*it);
            }
            return finishedWorkers;


        }

        void advanceWorkers(std::vector<std::pair<int,int>>* workers)
        {
            for (std::vector<std::pair<int,int>>::iterator it = workers->begin(); it != workers->end(); it++)
            {
                it->second--;
            }
        }
        bool checkRequirements(std::vector<int> output, int ele)
        {
            bool requirementsMet = true;

            std::vector<int> dependencies = this->getCol(ele);

                
            for (std::vector<int>::iterator it = dependencies.begin(); it != dependencies.end(); it++)
                requirementsMet &= std::find(output.begin(),output.end(),*it) != output.end();

            return requirementsMet;
        }

        std::vector<int> findBegins()
        {
            std::vector<int> output;
            for (int i = 0; i < BOUNDS; i++)
            {
                bool connectionExists = false;
                for (int j = 0; j < BOUNDS; j++)
                {
                    connectionExists |= data[j][i];
                }
                if (!connectionExists)
                    output.push_back(i);
            }
            return output;
        }
        std::vector<int> getRow(int i)
        {
            std::vector<int> output;
            for (int j = 0; j < BOUNDS; j++)
            {
                if(data[i][j])
                    output.push_back(j);
            }
            return output;
        }
        std::vector<int> getCol(int j)
        {
            std::vector<int> output;
            for (int i = 0; i < BOUNDS; i++)
            {
                if(data[i][j])
                    output.push_back(i);
            }
            return output;
        }
};



Graph parseInput(std::string filename)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    
    Graph graph;
    if (inputFile.is_open())
    {
        std::string line;
        while (getline(inputFile,line))
        {
            char a, b;
            sscanf(line.c_str(), "Step %c must be finished before step %c can begin.", &a,&b);
            graph.insert(convertChar(a),convertChar(b));
        }
    }

    return graph;
}

int main(int argc, char const *argv[])
{
    Graph graph = parseInput(argv[1]);

    graph.print();

    std::cout << graph.getProblemOne() << std::endl;

    
    std::cout << graph.getProblemTwo() << std::endl;


    keepWindowOpen();
    return 0;
}

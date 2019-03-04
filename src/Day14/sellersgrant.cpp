#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>


void print(std::list<int> recipes)
{
    for (int recipe : recipes)
        std::cout << recipe;
    std::cout<<std::endl;
}

int createNewRecipe(std::list<int> & recipeList, int elf1, int elf2)
{
    int sum = elf1 + elf2;
    if (sum >= 10)
    {
        recipeList.push_back(1);
        recipeList.push_back(sum%10);
        return 2;
    }
    else
    {
        recipeList.push_back(sum);
        return 1;
    }
}        
std::list<int>::iterator advanceIterator(std::list<int>::iterator elf, std::list<int> & recipeList)
{
    int numToAdvance = *elf;
    // advance(elf,numToAdvance);// 
    for (int j = 0; j <= numToAdvance; j++)
    {
        elf++;
        if (elf == recipeList.end())
            elf = recipeList.begin();
    }
    return elf;
}
unsigned long runP1(int recipeCount)
{
    std::list<int> recipeList;
    
    std::list<int>::iterator elf1, elf2;
    recipeList.push_back(3);
    recipeList.push_back(7);
    elf1 = recipeList.begin();
    elf2 = recipeList.begin();
    elf2++;
    
    int i = 0;
    while (i <= recipeCount + 10)
    {        
        createNewRecipe(recipeList,*elf1,*elf2);
        
        elf1 = advanceIterator(elf1, recipeList);
        elf2 = advanceIterator(elf2, recipeList);

        i++;
    }

    std::string recipeString;
    
    std::list<int>::iterator it = recipeList.begin();
    advance(it,recipeCount);
    i = 0;
    while (i < 10)
    {
        recipeString.push_back((char)(*it+48));
        it++;
        i++;
    }

    return std::stoul(recipeString);

}

void problemOne(std::string test, int input)
{
    std::ifstream inputFile;
    inputFile.open(test);
    
    std::vector<std::pair<int,unsigned long>> testCases;

    std::string line;
    if (inputFile.is_open())
    {
        while(getline(inputFile,line))
        {
            int recipeCount;
            unsigned long result;
            sscanf(line.c_str(), "%d,%lu", &recipeCount, &result);
            testCases.push_back(std::pair<int,unsigned long>(recipeCount,result));
        }
    }
    else
        std::cout << "Cannot find file: " << input << std::endl;
    
    inputFile.close();  

    for (std::vector<std::pair<int,unsigned long>>::iterator testCase = testCases.begin(); testCase != testCases.end(); testCase++)
    {
        if (testCase->second == runP1(testCase->first))
            std::cout << "Case " << testCase->first << "," << testCase->second << " passed!" << std::endl;   
        else
            std::cout << "Case " << testCase->first << "," << testCase->second << " failed!" << std::endl;           
    }


    printf("Problem 1 Answer: %lu\n",runP1(input));
}

std::string findLastRecipe(std::list<int> & recipeList, int sizeOfRecipe, int offset)
{
    std::string lastRecipe;

    std::list<int>::iterator stopIt = recipeList.end();    
    while(offset-- > 0)
        stopIt--;
    
    for (std::list<int>::iterator it = stopIt; it != recipeList.end() &&  distance(it,stopIt)!=sizeOfRecipe; it--)
        lastRecipe.insert(lastRecipe.begin(),(char)(*it+48));

    return lastRecipe;
}

unsigned long long  runP2(std::string recipe)
{
    std::list<int> recipeList;
    std::list<int>::iterator elf1, elf2;
    recipeList.push_back(3);
    recipeList.push_back(7);
    elf1 = recipeList.begin();
    elf2 = recipeList.begin();
    elf2++;
    
    std::string lastRecipe = "";
    
    int offset = 0; 
    while (lastRecipe != recipe)
    {
        // std::cout << ++i << std::endl;
        int numAdded = createNewRecipe(recipeList,*elf1,*elf2);
        elf1 = advanceIterator(elf1, recipeList);
        elf2 = advanceIterator(elf2, recipeList);


        while (numAdded > 0)
        {
            lastRecipe = findLastRecipe(recipeList, recipe.size(),numAdded);
            if (lastRecipe==recipe && numAdded==2)
            {
                offset = 1;
                break;
            }
            numAdded--;
        }// 
        // print(recipeList);
        // std::cout << recipe << ',' << lastRecipe << std::endl;
        
    }

    return recipeList.size()-offset-recipe.size();
}

void problemTwo(std::string test, std::string input)
{
    std::ifstream inputFile;
    inputFile.open(test);
    
    std::vector<std::pair<std::string, int>> testCases;

    std::string line;
    if (inputFile.is_open())
    {
        while(getline(inputFile,line))
        {
            std::string recipe = line.substr(0,line.find(','));
            int result = std::stoi(line.substr(line.find(',')+1));
            

            testCases.push_back(std::pair<std::string,int>(recipe,result));
        }
    }
    else
        std::cout << "Cannot find file: " << input << std::endl;
    inputFile.close();  

    for (std::vector<std::pair<std::string,int>>::iterator testCase = testCases.begin(); testCase != testCases.end(); testCase++)
    {
        if (testCase->second == runP2(testCase->first))
            std::cout << "Case " << testCase->first << "," << testCase->second << " passed!" << std::endl;   
        else
            std::cout << "Case " << testCase->first << "," << testCase->second << " failed!" << std::endl;   
    }

    std::cout << "Problem 2 Answer: ";
    std::cout << " ";
    unsigned long long answer = runP2(input);
    std::cout << answer;
    // printf("Problem 1 Answer: %lu\n",runP2(input));
}

int main(int argc, char const *argv[])
{
    problemOne("testP1.txt",702831);   
    problemTwo("testP2.txt","702831");
    //131757233
    return 0;
}

#include <iostream>
#include <fstream>
#include <map>

#define GENERATIONS_P1 20
#define GENERATIONS_P2 50000000000
#define DEPTH 5

class Rules
{

    char sign = NULL;
    int depth;
    Rules* pound = nullptr;
    Rules* period = nullptr;

    public:
        Rules(int n)
        {
            if (n > 0)
            {
                pound = new Rules(n-1);
                period = new Rules(n-1);
            }
            depth = DEPTH-n;
            std::cout << depth << std::endl;
        }
        ~Rules()
        {
            if (depth && depth < DEPTH)
            {
                delete pound;
                delete period;
            }
        }
        void addRule(std::string rule)
        {
            std::string restOfRule = rule.substr(1,rule.size()-1);
            if (rule[0] == '#')
                pound->addRule(restOfRule);
            else if (rule[0] == '.')
                period->addRule(restOfRule);
            else
                sign = rule[rule.size()-2];
        }
        char getSign(std::string plantString)
        {
            char current = plantString[0];
            if (current == '#')
            {
                std::string restOfPlants = plantString.substr(1,plantString.size()-1);
                return pound->getSign(restOfPlants);
            }
            else if (current == '.')
            {
                std::string restOfPlants = plantString.substr(1,plantString.size()-1);
                return period->getSign(restOfPlants);
            }
            else
                return sign;
        }
        void print(std::string substr)
        {
            if (depth < 5)
            {
                std::string periodstr = substr;
                std::string poundstr = substr;
                periodstr.push_back('.');
                poundstr.push_back('#');
                pound->print(poundstr);
                period->print(periodstr);
            }
            else
                std::cout << substr << ": " << sign << std::endl;
        }
};

std::string parseInitialState(std::string line)
{
    char* output;
    sscanf(line.c_str(), "initial state: %s", output);
    return output;
}

long long getValue(std::string state)
{
    long long sum = 0;
    for (int i = 0; i<state.size(); i++)
    {
        if (state[i] == '#')
            sum += (i - 5);
    }
    return sum;
}

long long run(std::string state, Rules rules, long long int gens)
{
    char sign;
    int offset = 5;

    long long previous = 0;
    long long prevDelta = 0;
    long long delta = 0;
    for (int i = 0; i< gens; i++)
    {
        // std::cout << "GEN: " << i << state << std::endl;

        std::string newstate = state;

        for (int j = 0; j < state.size()-2; j++)
        {
            sign = rules.getSign(state.substr(j,DEPTH));   
            if (sign)
                newstate[j+2] = sign;
        }

        state = newstate;
        
        long long value = getValue(state);
        delta = value - previous;

        if (delta == prevDelta)
            return value + (gens-(i+1))*delta;

        prevDelta = delta;
        previous = value;

        if (state.substr(state.size()-DEPTH, DEPTH) != ".....")
            state.append(5,'.');
    }

    return getValue(state);
}

int main(int argc, char const *argv[])
{
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    if (inputFile.is_open())
    {
        std::string line;
        std::string state;
        Rules rules(DEPTH);
        rules.print("");

        getline(inputFile,line);
        std::cout << line << std::endl;

        // split the line into the input state, remove the carriage return
        state = line.substr(15,line.size()-16);

        // Buffer the line for plants to grow
        state.append(3, '.');
        state.insert(state.begin(),5,'.');

        //Ignore spacing line
        getline(inputFile,line);

        while (getline(inputFile, line))
        {
            std::cout << line << std::endl;
            rules.addRule(line);
        }
        rules.print("");

        // std::cout << "SUM P1: " << run(state,rules,GENERATIONS_P1) << std::endl;
        std::cout << "SUM P2: " << run(state,rules,GENERATIONS_P2) << std::endl;
        
    }

    else
    {
        std::cout << "Unable to open file: " << argv[1];
    }

    return 0;
}

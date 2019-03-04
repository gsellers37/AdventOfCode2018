#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

// Includes the instruction set's functions
#include "operations.h"

class Opcode
{
    bool (*func)(std::vector<int> & registers, int A, int B, int C);
    
    public:
        int code = 16;
        std::set<int> possibleCodes;
        Opcode()
        {
        }
        Opcode(bool (*f)(std::vector<int> & registers, int A, int B, int C))
        {
            func = f;
        }
        bool run(std::vector<int> & registers, int A, int B, int C)
        {
            return func(registers,A,B,C);
        }
        void addPossibility(int op)
        {
            possibleCodes.insert(op);
        }
        bool operator<(const Opcode other) const
        {
            return this->code < other.code;
        }
        bool operator==(const Opcode other) const
        {
            return this->code == other.code;
        }
        void setCode(int _code)
        {
            code = _code;
        }
};

class Instruction
{
    public:
        int op,a,b,c;
        Instruction(std::string line)
        {
            sscanf(line.c_str(), "%d %d %d %d", &op,&a,&b,&c); 
        }
};

class Test
{
    std::vector<int> before;
    std::vector<int> after;
    public:
        int op,a,b,c;
        Test(const std::vector<std::string> & lines)
        {
            
            sscanf(lines[0].c_str(), "Before: [%d, %d, %d, %d]", &op,&a,&b,&c);
            before = {op,a,b,c};
            sscanf(lines[2].c_str(), "After:  [%d, %d, %d, %d]", &op,&a,&b,&c);
            after = {op,a,b,c};
            sscanf(lines[1].c_str(), "%d %d %d %d", &op,&a,&b,&c); 
            
        }
        std::vector<int> getBefore()
        {
            return before;
        }
        bool checkAfter(std::vector<int> registers)
        {
            bool valid = true;
            for (int i = 0; i < 4; i++)
            {
                valid &= (registers[i] == after[i]);
            }
            return valid;
        }
};


void parseFile(std::string filename,std::vector<Test> & tests, std::vector<Instruction> & instructions)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    if (inputFile.is_open())
    {
        std::string line1,line2,line3;
        int numBlankLines = 0;
        
        getline(inputFile,line1);
        getline(inputFile,line2);
        getline(inputFile,line3);
        while(numBlankLines <= 1)
        {
            tests.push_back(Test({line1,line2,line3}));
            getline(inputFile,line3); 
            getline(inputFile,line1);
            getline(inputFile,line2);
            getline(inputFile,line3);
            
            numBlankLines = 0;
            if (line1 == "\r")
                numBlankLines++;
            if (line2 == "\r")
                numBlankLines++;
        }
        instructions.push_back(Instruction(line3));
        while(getline(inputFile,line1))
        {
            instructions.push_back(Instruction(line1));
        }
    }
    else   
        std::cout << "Unable to open: "<< filename <<std::endl;
}
int runP1(std::string filename)
{
    std::vector<bool (*)(std::vector<int> & registers, int A, int B, int C)> opcodes = {
        addr,addi,mulr,muli,banr,bani,borr,bori,setr,seti,gtir,gtri,gtrr,eqir,eqri,eqrr 
        };

    std::vector<Test> tests;
    std::vector<Instruction> instructions; // ignored for problem one
    parseFile(filename,tests,instructions);

    int sum = 0;
    for (Test test : tests)
    {
        int numOpcodes = 0;
        for (Opcode code : opcodes)
        {
            std::vector<int> registers = test.getBefore();
            if (code.run(registers,test.a,test.b,test.c) && test.checkAfter(registers))
                numOpcodes++;
        }
        if (numOpcodes >= 3)
            sum ++;
    }
    return sum;
}

int runP2(std::string filename)
{
    std::vector<bool (*)(std::vector<int> & registers, int A, int B, int C)> ops = {
        addr,addi,mulr,muli,banr,bani,borr,bori,setr,seti,gtir,gtri,gtrr,eqir,eqri,eqrr 
        };

    std::vector<Opcode> opcodes;
    for (Opcode opcode : ops)
        opcodes.push_back(opcode);

    std::vector<Test> tests;
    std::vector<Instruction> instructions;
    parseFile(filename,tests,instructions);

    // Fill out possibilities for each opcode
    for (Test test : tests)
    {
        Opcode lastCode;
        int numOpcodes = 0;

        int op = 16;
        for (Opcode & code : opcodes)
        {
            std::vector<int> registers = test.getBefore();
            
            if (code.run(registers,test.a,test.b,test.c) && test.checkAfter(registers))
                code.addPossibility(test.op);
        }
    }
    
    // If there is only one opcode available, choose it
    // Use the chosen opcodes to narrow down the options of other opcodes.
    std::set<int> assignedOpcodes;
    while(assignedOpcodes.size()<16)
    {
        for (Opcode & code : opcodes)
        {
            for (std::set<int>::iterator it = code.possibleCodes.begin(); it != code.possibleCodes.end();)
            {
                if (std::find(assignedOpcodes.begin(),assignedOpcodes.end(),*it) != assignedOpcodes.end())
                    it = code.possibleCodes.erase(it);
                else
                    it++;
            }
            if (code.possibleCodes.size() == 1)
            {
                int op = *code.possibleCodes.begin();
                assignedOpcodes.insert(op);
                code.setCode(op);
            }
        }
    }

    // sort the opcodes so that they may be indexed in the right order
    std::sort(opcodes.begin(),opcodes.end());

    // Run the simulation
    std::vector<int> registers = {0,0,0,0};
    for (Instruction instruction : instructions)
    {
        opcodes[instruction.op].run(registers,instruction.a,instruction.b,instruction.c);
    }

    return registers[0];
}

int main(int argc, char const *argv[])
{
    std::cout << "P1 Answer: " << runP1("input.txt") << std::endl;
    std::cout << "P2 Answer: " << runP2("input.txt") << std::endl;
    
    return 0;
}





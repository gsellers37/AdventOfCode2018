#include <vector>

bool addr(std::vector<int> & registers, int A, int B, int C)
{   
    if (A >= registers.size())
        return false;
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A]+registers[B]; 
    return true;
}

bool addi(std::vector<int> & registers, int A, int B, int C)
{
    if (A >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A] + B; 
    return true;
}

bool mulr(std::vector<int> & registers, int A, int B, int C)
{   
    if (A >= registers.size())
        return false;
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A]*registers[B]; 
    return true;
}

bool muli(std::vector<int> & registers, int A, int B, int C)
{
    if (A >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A] * B; 
    return true;
}

bool banr(std::vector<int> & registers, int A, int B, int C)
{   
    if (A >= registers.size())
        return false;
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A] & registers[B]; 
    return true;
}

bool bani(std::vector<int> & registers, int A, int B, int C)
{
    if (A >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A] & B; 
    return true;
}

bool borr(std::vector<int> & registers, int A, int B, int C)
{   
    if (A >= registers.size())
        return false;
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A] | registers[B]; 
    return true;
}

bool bori(std::vector<int> & registers, int A, int B, int C)
{
    if (A >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A] | B; 
    return true;
}

bool setr(std::vector<int> & registers, int A, int B, int C)
{   
    if (A >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
    registers[C] = registers[A]; 
    return true;
}

bool seti(std::vector<int> & registers, int A, int B, int C)
{
    if (A >= registers.size())
        return false;
    registers[C] = A; 
    return true;
}

bool gtir(std::vector<int> & registers, int A, int B, int C)
{   
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;

    if (A > registers[B])
        registers[C] = 1;
    else
        registers[C] = 0;

    return true;
}

bool gtri(std::vector<int> & registers, int A, int B, int C)
{
    if (A >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
        
    if (registers[A] > B)
        registers[C] = 1;
    else
        registers[C] = 0;

    return true;
}
bool gtrr(std::vector<int> & registers, int A, int B, int C)
{   
    if (A >= registers.size())
        return false;
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;

    if (registers[A] > registers[B])
        registers[C] = 1;
    else
        registers[C] = 0;

    return true;
}

bool eqir(std::vector<int> & registers, int A, int B, int C)
{   
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;

    if (A == registers[B])
        registers[C] = 1;
    else
        registers[C] = 0;

    return true;
}

bool eqri(std::vector<int> & registers, int A, int B, int C)
{
    if (A >= registers.size())
        return false;
    if (C >= registers.size())
        return false;
        
    if (registers[A] == B)
        registers[C] = 1;
    else
        registers[C] = 0;

    return true;
}
bool eqrr(std::vector<int> & registers, int A, int B, int C)
{   
    if (A >= registers.size())
        return false;
    if (B >= registers.size())
        return false;
    if (C >= registers.size())
        return false;

    if (registers[A] == registers[B])
        registers[C] = 1;
    else
        registers[C] = 0;

    return true;
}

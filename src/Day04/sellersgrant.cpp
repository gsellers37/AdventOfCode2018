#include <iostream>   // std::cout and std::cin
#include <fstream>    // std::ifstream
#include <vector>     // std::vector
#include <algorithm>  // std::sort
#include <functional> // std::bind
#include <map>        // std::map

#define START 0
#define SLEEP 1
#define WAKE -1
#define NULLVALUE -99

class Entry{
    public:
        int year, month, day, hour, minute, start, state, guard = NULLVALUE;
        // enum state_t state = SLEEP; 
        
        void print()
        {
            std::cout << this->year << "-" << 
                        this->month << "-" << 
                        this->day  << " " << 
                        this->hour << ":" << 
                        this->minute;
            if (this->guard != -99)
                std::cout << " Guard " << this->guard;
            std::cout << " " << this->state << std::endl;
        }

        bool operator<(const Entry &other)
        {
            // return (((((this->year*12)+this->month)*30+this->day)*24+this->hour)*60+this->minute) <
            //        (((((other.year*12)+other.month)*30+other.day)*24+other.hour)*60+other.minute);
            return (this->year < other.year) ||
               (this->year == other.year && this->month < other.month) ||
               (this->year == other.year && this->month == other.month && this->day < other.day) ||
               (this->year == other.year && this->month == other.month && this->day == other.day && this->hour <other.hour) ||
               (this->year == other.year && this->month == other.month && this->day == other.day && this->hour == other.hour && this->minute < other.minute);
        
        
        }

    private:

};

int string_to_int(std::string str)
{
    if (str == "0 '\000'")
    {
        return -99;
    }
    else if (str == "Guard")
    {
        return START;
    }
    else if (str == "wakes up\r" || str == "wakes up")
    {
        return WAKE;
    }
    else if (str == "falls asleep\r" || str == "falls asleep")
    {
        return SLEEP;
    }
    else if (str == "alls" || str == "akes")
        return NULLVALUE;
    else 
    {
        return stoi(str);
    }
}

std::vector<Entry> parseFile(std::string filename)
{
    std::vector<Entry> entries;
    
    std::ifstream inputFile;
    inputFile.open(filename);

    if (inputFile.is_open())
    {
        std::string line;
        std::vector<std::string> delimiters = {"[","-","-"," ",":","] ", " #", " "};
        while (getline(inputFile,line))
        {
            Entry entry;
            std::string token;
            int* p_variables[] = { &entry.year, &entry.month, &entry.day, &entry.hour, &entry.minute, &entry.state, &entry.guard};
        
            for (int i = 0; i<delimiters.size(); i++)
            {
                size_t pos = line.find(delimiters.at(i));
                token = line.substr(0,pos);
                line.erase(0, pos + delimiters.at(i).length());
                if (i == 0)
                    continue;
                *p_variables[i-1] = string_to_int(token);
            }
            entries.push_back(entry);
        }
    }
    else
    {
        std::cout << "Could not open file: " << filename << std::endl;
    }
    return entries;
}

void printAll(std::vector<Entry> entries)
{
    for (std::vector<Entry>::iterator it = entries.begin(); it != entries.end(); it++)
    {
        it->print();
    }
}

bool checkListforGuard(std::pair<int,int[60]> guardlist, int guard)
{
    return guardlist.first == guard;
}

struct log_t { 
    int hours[60] = {0};
    int duration = 0;
};

std::pair<int,int> argmax(std::map<int,log_t> guards)
{
    int max = 0;
    int guard = NULLVALUE;

    for (std::map<int,log_t>::iterator it = guards.begin(); it != guards.end(); it++)
    {
        if (it->second.duration > max)
        {
            guard = it->first;
            max = it->second.duration;
        }
    }

    return std::make_pair(max,guard);
}

std::pair<int,int> findMinute(log_t log)
{
    int max = 0;
    int minute = -1;

    for (int i = 0; i < 60; i++)
    {
        if (log.hours[i] > max)
        {
            minute = i;
            max = log.hours[i];
        }
    }

    return std::make_pair(max,minute);
}

std::pair<log_t,log_t> findProbabilities(std::map<int,log_t> guards)
{
    log_t guard, log;

    for (int i = 0; i< 60; i++)
    {
        int max = 0;
        for (std::map<int,log_t>::iterator it = guards.begin(); it != guards.end(); it++)
        {
            if (it->second.hours[i] > max)
            {
                max = it->second.hours[i];
                guard.hours[i] = it->first;
                log.hours[i] = it->second.hours[i];
            }
        }
    }

    return std::make_pair(guard,log);
}

int main(int argc, char const *argv[])
{
    std::vector<Entry> entries = parseFile(argv[1]);
    if (entries.empty())
    {
        return -1;
    }
    log_t value = {0};

    std::sort(entries.begin(), entries.end());

    printAll(entries);

    int current_guard;
    int start_sleep = 0;
    std::map<int,log_t> guards;
    for (std::vector<Entry>::iterator it = entries.begin(); it != entries.end(); it++)
    {
        if (it->state == START)
        {
            current_guard = it->guard;

            if (guards.count(current_guard) == 0)
            {
                log_t log;
                guards.emplace(std::pair<int,log_t>(current_guard,log));
            }    
        }
        else if (it->state == SLEEP)
            start_sleep = it->minute;

        else if (it->state = WAKE)
        {
            guards.at(current_guard).duration += it->minute - start_sleep;
            for (int i = start_sleep; i <= it->minute-1; i++)
                guards.at(current_guard).hours[i] ++;
        }            
    }

    for (std::map<int,log_t>::iterator it = guards.begin(); it != guards.end(); it++)
    {
        std::cout << "ID: " << it->first << "=";
        for (int i = 0; i< 60; i++)
        {
            std::cout << it->second.hours[i] << ".";
        }
        std::cout << " TOTAL: " << it->second.duration << std::endl;
    }

    std::pair<int,int> longestSlept = argmax(guards);
    std::cout << "Longest slept: " << longestSlept.first << " by Guard #" << longestSlept.second << std::endl;
    
    std::pair<int,int> highestProbability = findMinute(guards.at(longestSlept.second));
    std::cout << "Most frequent times slept: " << highestProbability.first << " at minute " << highestProbability.second << std::endl;
    
    std::cout << "Answer: " << highestProbability.second*longestSlept.second << std::endl;

    //PART TWO

    std::pair<log_t,log_t> maxCol = findProbabilities(guards);

    for (int i = 0; i<60; i++)
    {
        std::cout << i << ": Guard #" << maxCol.first.hours[i] << " at min: " << maxCol.second.hours[i] << std::endl;
    }

    return 0;
}




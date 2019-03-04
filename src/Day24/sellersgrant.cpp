#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <regex>
#include <algorithm>
#include <map>

std::map<std::string,int> map;
class Group {
    private:
        int units, hp, initiative, damage;
        std::set<int> weak;
        std::set<int> immune;
        int damageType;
        

    public:
        int id;

        Group* target = nullptr;
        
        Group(std::string & line, int _id){
            id = _id;
            // cutoff the escape sequence
            line = line.substr(0,line.size()-1);
            
            const std::regex basePat {R"(^(\d+).*with (\d+).*does (\d+) (\w+) dam.* (\d+)$)"};
            const std::regex weakPat {R"(^.*\(?weak to ([\w\s,]+)(;|\)).*$)"};
            const std::regex immunePat {R"(^.*\(?immune to ([\w\s,]+)(;|\)).*$)"};

            std::smatch matches;
            if (std::regex_match(line, matches, basePat)){
                units = std::stoi(matches[1]);
                hp = std::stoi(matches[2]);
                damage = std::stoi(matches[3]);
                damageType = map[matches[4]];
                initiative = std::stoi(matches[5]);
            }
            else 
                std::cout << "REGEX FAILED" << std::endl;

            if (std::regex_match(line,matches,weakPat)) {
                std::istringstream iss(matches[1]);
                std::string defense;
                while(getline(iss,defense,',')){
                    if (defense[0]==' ') defense = defense.substr(1);
                    weak.insert(map[defense]);
                }
            }
            // regex will fail if no weaknesses

            if (std::regex_match(line,matches,immunePat)) {
                std::istringstream iss(matches[1]);
                std::string defense;
                while(getline(iss,defense,',')) {
                    if (defense[0]==' ') defense = defense.substr(1);
                    immune.insert(map[defense]);
                }
            }
            // regex will fail if no immunities
        }

        bool operator<(const Group & other) const {
            return (this->getEffectivePow() < other.getEffectivePow()) || 
                   (this->getEffectivePow() == other.getEffectivePow() && this->getInit() < other.getInit());
        }

        int getUnits() const { return units; }
        int getHp() const { return hp; }
        int getInit() const { return initiative; }
        int getDamage() const { return damage; }
        int getEffectivePow() const { return units * damage; }
        int getDamageType() const { return damageType; }
        
        void setTarget(Group* targ) { target = targ; }
        Group* getTarget() { return target; }

        int getMult(int type) const {
            if (weak.count(type) > 0) return 2;
            if (immune.count(type) > 0) return 0;
            return 1;
        }
        void attack(){
            if (target)
                target->loseHealth(target->getMult(damageType)*getEffectivePow());
        }
        void loseHealth(int totalHealth){
            // std::cout << "Killing " << totalHealth/hp << "units " << std::endl;
            units -= totalHealth / hp;
        }
        void boost(){
            damage++;
        }
};

std::ostream & operator<<( std::ostream & os, const Group& group){
    return os << 
            "Group " << group.id << " contains " << 
            group.getUnits() << " units, " <<
            group.getEffectivePow() << " effPow, " <<
            std::endl; 
}
std::ostream & operator<<( std::ostream & os, const std::vector<Group>& team){
    for (Group group : team){
        os << group;
    }
    os<<std::endl;
    return os;
}

std::vector<Group> immuneSystem;
std::vector<Group> infection;

int sumUnits(std::vector<Group> team) {
    int units = 0;
    for (Group group : team){
        units += group.getUnits();
    }
    return units;
}

struct maxGroupCompare{ 
    int damageType;
    maxGroupCompare(int damageType){this->damageType = damageType;}

    bool operator()(const Group * a,const Group * b) const { 
        if (a->getMult(this->damageType) == b->getMult(this->damageType))
            return ((*a)<(*b));
        else
            return (a->getMult(this->damageType) < b->getMult(this->damageType));
    } 
}; 
  
void selectTarget(std::vector<Group> & teamA, std::vector<Group>& teamB) {
    std::vector<Group*> toChooseFrom;
    for (Group& groupB : teamB)
        toChooseFrom.push_back(&groupB);

    for (Group & groupA : teamA) {
        
        std::sort(toChooseFrom.begin(),toChooseFrom.end(),maxGroupCompare(groupA.getDamageType()));
        
        if (toChooseFrom.size() > 0) { 
            Group* toAttack = toChooseFrom.back();
            if (toAttack->getMult(groupA.getDamageType()) != 0) {
                groupA.setTarget(toAttack);
                toChooseFrom.pop_back();
            }
            else
                groupA.setTarget(nullptr);
        }
        else
            groupA.setTarget(nullptr);
        // toChooseFrom.erase(enemy_it);
    }
}
void attack(std::vector<Group> & teamA, std::vector<Group> & teamB){
    std::vector<Group*> allPlayers;
    for (Group & a : teamA)
        allPlayers.push_back(&a);
    for (Group & b : teamB)
        allPlayers.push_back(&b);

    std::sort(allPlayers.begin(),allPlayers.end(),[](const Group* pGroupA, const Group* pGroupB){return pGroupA->getInit() > pGroupB->getInit(); });

    for (Group* group : allPlayers){
        if (group->getUnits() > 0)
            group->attack();
    }
}

bool checkDeadlock(std::vector<Group> & teamA, std::vector<Group>&  teamB){
    bool deadlocked = true;
    
    std::vector<Group> allPlayers;
    for (Group& a : teamA)
        allPlayers.push_back(a);
    for (Group& b : teamB)
        allPlayers.push_back(b);

    for (Group& group : allPlayers){
        bool targetHasTooMuchHp = true;
        if (group.getTarget() != nullptr){
            targetHasTooMuchHp =  (group.getTarget()->getHp() > group.getEffectivePow()*group.getTarget()->getMult(group.getDamageType()) );
        }
        deadlocked &= targetHasTooMuchHp; 
    }
    return deadlocked;
}

int run(std::vector<Group> immuneSystem, std::vector<Group> infection, bool & immuneWin) {
    while (immuneSystem.size() > 0 && infection.size() > 0) {
        // std::cout << "Immune " << std::endl << immuneSystem << " INFECTION " << std::endl << infection;

        std::sort(immuneSystem.rbegin(),immuneSystem.rend());
        std::sort(infection.rbegin(),infection.rend());
        
        selectTarget(immuneSystem, infection);
        selectTarget(infection, immuneSystem);

        if (checkDeadlock(immuneSystem,infection))
            return 0;

        attack(immuneSystem,infection);

        infection.erase(std::remove_if(
            infection.begin(),
            infection.end(),
            [](const Group & group){return group.getUnits()<=0;}
            ),infection.end()
        );
        immuneSystem.erase(std::remove_if(
            immuneSystem.begin(),
            immuneSystem.end(),
            [](const Group & group){return group.getUnits()<=0;}
            ),immuneSystem.end()
        );

    }

    if (immuneSystem.empty())
        return sumUnits(infection);
    else{
        immuneWin = true;
        return sumUnits(immuneSystem);

    }
}

int boost(std::vector<Group> immuneSystem, std::vector<Group> infection) {
    bool immuneWin = false;
    int output = 0;
    while (!immuneWin){
        for (Group & group : immuneSystem){
            group.boost();
        }
        output = run(immuneSystem,infection,immuneWin);
    }
    return output;
}
int main(int argc, char const *argv[])
{
    map["fire"] = 1;
    map["cold"] = 2;
    map["bludgeoning"] = 3;
    map["slashing"] = 4;
    map["radiation"] = 5;

    // PARSE INPUT
    std::ifstream inputFile("input.txt");
    if (inputFile.is_open())
    {
        std::string line;
        
        getline(inputFile,line);
        int groupID = 0;
        while (getline(inputFile,line))
        {
            if (line == "\r") break;
            immuneSystem.emplace_back(line,groupID++);
        }

        getline(inputFile,line);
        while (getline(inputFile,line))
            infection.emplace_back(line,groupID++);
    }
    else std::cout << "Unable to read file" << std::endl;

    bool immuneWins;
    std::cout << "Part 1: " << run(immuneSystem,infection,immuneWins) << std::endl;
    std::cout << "Part 2: " << boost(immuneSystem,infection) << std::endl;
    
    std::cin.get();
    return 0;
}

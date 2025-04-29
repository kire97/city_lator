#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <map>

class Item{
public:
    Item(std::string);
    std::string getName();
private:
    std::string mName;
};

Item::Item(std::string name){
    name = name;
}

std::string Item::getName(){
    return mName;
}

Item hoddog = Item("Hotdog");
Item money = Item("Money");

class HealthStatus{
public:
    HealthStatus(std::string);
    std::string getName();
private:
    std::string mName;
};

HealthStatus::HealthStatus(std::string name){
    mName = name;
}

std::string HealthStatus::getName(){
    return mName;
}

HealthStatus satation = HealthStatus("Satation");

class Person;

class Task{
public:
    Task(int (*action)(Person*));
    int call(Person*);
private:
    std::string mName;
    int (*mAction)(Person*);
};

Task::Task(int (*action)(Person*)){
    mAction = action;
}

int Task::call(Person* person){
    return mAction(person);
}

class Person{
public:
    Person(std::string);
    bool createTask();
    void consumeTask();
    bool trade(Item*, int);
    bool alter(HealthStatus*, float);
    std::string getName();

private:
    std::string                     mName;
    std::queue<Task>                mTasks;
    std::map<Item*, int>            mItems;
    std::map<HealthStatus*, float>  mHealth;
};

Person::Person(std::string name){
    mName = name;
    mHealth[&satation] = 1.0f;
    mItems[&money] = 100;
    std::cout << "Person ";
    std::cout << name;
    std::cout << " created\n";
}

std::string Person::getName(){
    return mName;
}

int buyDog(Person* person){
    if (person->trade(&money, -10)){
        person->trade(&hoddog, 1);
        std::cout << person->getName();
        std::cout << " buy Hoddog\n";
    }
    return 1;
}

int eatDog(Person* person){
    if (person->trade(&hoddog, -1)){
        person->alter(&satation, 0.4f);
        std::cout << person->getName();
        std::cout << " eet Hoddog\n";
    }
    return 1;
}

bool Person::createTask(){
    mHealth[&satation] -= 0.1f;

    if (mHealth[&satation] < 0.2f) {
        if (1 <= mItems[&hoddog]){
            mTasks.push(Task(eatDog));
            return true;
        }
        if (10 <= mItems[&money]){
            mTasks.push(Task(buyDog));
            return true;
        }
    }
    return false;
}

void Person::consumeTask(){
    if (!mTasks.empty()){
        mTasks.front().call(this);
        mTasks.pop();
    }
}

bool Person::trade(Item* item, int amount){
    if (amount < 0 && mItems[item] < amount) {return false;}
    mItems[item] += amount;
    return true;
}

bool Person::alter(HealthStatus* item, float amount){
    mHealth[item] += amount;
    return true;
}

int main(){
    std::vector<Person> people = {Person("Bobby"), Person("Kenta")};
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < people.size(); j++){
            people.at(j).createTask();
            people.at(j).consumeTask();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <thread>
#include <vector>

struct Position {
    float x;
    float y;
    float z;
};

struct ItemDescription {
    float red;
    float green;
    float blue;
    float size;
    float roundness;
    float hardness;
};

class Item {
public:
    Item(std::string, ItemDescription);
    std::string getName();
    ItemDescription getDescription();

private:
    std::string mName;
    ItemDescription mDescription;
};

Item::Item(std::string name, ItemDescription description) {
    mName = name;
    mDescription = description;
}

ItemDescription Item::getDescription() { return mDescription; }

std::string Item::getName() { return mName; }

Item hotdog = Item("Hotdog", {0.7f, 0.45f, 0.30f, 0.15f, 0.7f, 0.2f});
Item money = Item("Money", {0.7f, 0.45f, 0.30f, 0.1f, 0.1f, 0.7f});

class ItemKnowledge {
public:
    ItemKnowledge(Item*);
    float getOpinion();

private:
    ItemDescription mDescription;
    float mOpinion;
};

ItemKnowledge::ItemKnowledge(Item* item) {
    mDescription = item->getDescription();
}

class Location {
public:
    Location(std::string);
    std::string getName();

private:
    std::string mName;
};

Location::Location(std::string name) { mName = name; }

std::string Location::getName() { return mName; }

Location home = Location("Home");
Location park = Location("Park");

class LocationKnowledge {
public:
    LocationKnowledge(Location*);
    float getOpinion();

private:
    Location* mLocation;
    float mOpinion;
};

LocationKnowledge::LocationKnowledge(Location* location) {
    mLocation = location;
}

float LocationKnowledge::getOpinion() { return mOpinion; }

class ItemMemory {
public:
    ItemMemory(ItemKnowledge*, LocationKnowledge*, struct Position);
    Position getPosition();

private:
    ItemKnowledge* mItemKnowledge;
    LocationKnowledge* mLocationKnowledge;
    Position mPosition;
    float mCertainty;
};

ItemMemory::ItemMemory(ItemKnowledge* itemK, LocationKnowledge* locationK,
                       struct Position position) {
    mItemKnowledge = itemK;
    mLocationKnowledge = locationK;
    mPosition = position;
}

Position ItemMemory::getPosition() { return mPosition; }

class EventMemory {
public:
    EventMemory();

private:
    LocationKnowledge* mLocationKnowledge;
};

class HealthStatus {
public:
    HealthStatus(std::string);
    std::string getName();

private:
    std::string mName;
};

HealthStatus::HealthStatus(std::string name) { mName = name; }

std::string HealthStatus::getName() { return mName; }

HealthStatus satation = HealthStatus("Satation");

class Person;

union ActionArgs {

    struct {
        Person* person;
        Item* item;
    } transaction;
    struct {
        Person* person;
        Position position;
    } position;
};

class Action {
public:
    Action(void (*)(ActionArgs));
    void execute(ActionArgs);
    std::string getName();

private:
    void (*mAction)(ActionArgs);
    std::string mName;
    std::string mDescription;
};

Action::Action(void (*action)(ActionArgs)) { mAction = action; }

void Action::execute(ActionArgs args) { mAction(args); }

std::string Action::getName() { return mName; }

class Task {
public:
    Task(Action*, ActionArgs);
    void execute();

private:
    Action* mAction;
    ActionArgs mActionArgs;
};

Task::Task(Action* action, ActionArgs arguments) {
    mAction = action;
    mActionArgs = arguments;
}

void Task::execute() { mAction->execute(mActionArgs); }

class Person {
public:
    Person(std::string);
    bool createTask();
    void consumeTask();
    bool trade(Item*, int);
    bool alter(HealthStatus*, float);
    std::string getName();

private:
    std::string mName;
    std::queue<Task> mTasks;
    std::map<Item*, int> mItems;
    std::map<HealthStatus*, float> mHealth;
};

Person::Person(std::string name) {
    mName = name;
    mHealth[&satation] = 1.0f;
    mItems[&money] = 100;
    std::cout << "Person ";
    std::cout << name;
    std::cout << " created\n";
}

std::string Person::getName() { return mName; }

Action buyItem = Action([](ActionArgs args) {
    if (args.transaction.person->trade(&money, -10)) {
        args.transaction.person->trade(args.transaction.item, 1);
        std::cout << args.transaction.person->getName();
        std::cout << " bought ";
        std::cout << args.transaction.item->getName();
        std::cout << "\n";
    }
});

Action eatItem = Action([](ActionArgs args) {
    if (args.transaction.person->trade(args.transaction.item, -1)) {
        args.transaction.person->alter(&satation, 0.4f);
        std::cout << args.transaction.person->getName();
        std::cout << " ate ";
        std::cout << args.transaction.item->getName();
        std::cout << "\n";
    }
});

bool Person::createTask() {
    mHealth[&satation] -= 0.1f;

    if (mHealth[&satation] < 0.2f) {
        if (1 <= mItems[&hotdog]) {
            mTasks.push(Task(&eatItem, {this, &hotdog}));
            return true;
        }
        if (10 <= mItems[&money]) {
            mTasks.push(Task(&buyItem, {this, &hotdog}));
            return true;
        }
    }
    return false;
}

void Person::consumeTask() {
    if (!mTasks.empty()) {
        mTasks.front().execute();
        mTasks.pop();
    }
}

bool Person::trade(Item* item, int amount) {
    if (amount < 0 && mItems[item] < amount) {
        return false;
    }
    mItems[item] += amount;
    return true;
}

bool Person::alter(HealthStatus* item, float amount) {
    mHealth[item] += amount;
    return true;
}

int main() {
    std::vector<Person> people = {Person("Bobby"), Person("Kenta")};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < people.size(); j++) {
            people.at(j).createTask();
            people.at(j).consumeTask();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
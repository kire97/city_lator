#include <map>
#include <queue>
/** @cond */
#include <string>
/** @endcond */
#include <vector>

struct Position {
    float x;
    float y;
    float z;
};

struct Color {
    float red;
    float green;
    float blue;
};

struct PersonDescription {
    Color skin;
    Color eyes;
    Color hair;
    float age;
    float height;
    float weight;
};

struct ItemDescription {
    Color color;
    float size;
    float roundness;
    float hardness;
};

class Person;

class PersonKnowledge {
public:
    PersonKnowledge(Person*);

private:
    PersonDescription mDescription;
    float mOpinion;
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

class ItemKnowledge {
public:
    ItemKnowledge(Item*);
    float getOpinion();

private:
    ItemDescription mDescription;
    float mOpinion;
};

class Location {
public:
    Location(std::string);
    std::string getName();

private:
    std::string mName;
};

class LocationKnowledge {
public:
    LocationKnowledge(Location*);
    float getOpinion();

private:
    Location* mLocation;
    std::map<ItemKnowledge*, float> mItems;
    std::map<PersonKnowledge*, float> mPeople;
    float mOpinion;
};

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

struct ActionArgs {

    enum Type { transaction, position, health } type;

    union {
        struct {
            Person* person;
            Item* item;
        } transaction;
        struct {
            Person* person;
            Position position;
        } position;
        struct {
            Person* person;
        } health;
    } args;
};

typedef void (*TActionFunction)(ActionArgs);

class Action {
public:
    Action(TActionFunction);
    void execute(ActionArgs);
    std::string getName();

private:
    TActionFunction mAction;
    std::string mName;
    std::string mDescription;
};

class Task {
public:
    Task(Action*, ActionArgs);
    void execute();

private:
    Action* mAction;
    ActionArgs mActionArgs;
};

class Person {
public:
    Person(std::string, PersonDescription);
    bool createTask();
    void consumeTask();
    bool trade(Item*, int);
    bool alter(HealthStatus*, float);
    float getHealthStatus(HealthStatus*);
    void applyAfflictions();
    bool addAffliction(Action*);
    bool delAffliction(Action*);
    std::string getName();
    PersonDescription getDescription();

private:
    PersonDescription mDescription;
    std::string mName;
    std::queue<Task> mTasks;
    std::map<Item*, int> mItems;
    std::map<HealthStatus*, float> mHealth;
    std::vector<Action*> mAfflictions;
};
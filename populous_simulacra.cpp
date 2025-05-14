#include "person.hpp"
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <thread>
#include <vector>

const Color SKIN_COLOR_0 = {1.0f, 0.8f, 0.75f};   // very light
const Color SKIN_COLOR_1 = {0.9f, 0.7f, 0.6f};    // light
const Color SKIN_COLOR_2 = {0.75f, 0.55f, 0.48f}; // tan

const Color HAIR_COLOR_0 = {0.0f, 0.0f, 0.0f};    // black
const Color HAIR_COLOR_1 = {1.0f, 1.0f, 1.0f};    // white
const Color HAIR_COLOR_2 = {0.25f, 0.18f, 0.16f}; // brown

const Color EYE_COLOR_0 = {0.25f, 0.18f, 0.16f}; // brown
const Color EYE_COLOR_1 = {0.05f, 0.66f, 0.95f}; // blue
const Color EYE_COLOR_2 = {0.35f, 0.5f, 0.3f};   // green

PersonKnowledge::PersonKnowledge(Person* person) {
    mDescription = person->getDescription();
}

Item::Item(std::string name, ItemDescription description) {
    mName = name;
    mDescription = description;
}

ItemDescription Item::getDescription() { return mDescription; }

std::string Item::getName() { return mName; }

ItemKnowledge::ItemKnowledge(Item* item) {
    mDescription = item->getDescription();
}

Location::Location(std::string name) { mName = name; }

std::string Location::getName() { return mName; }

Location home = Location("Home");
Location park = Location("Park");

LocationKnowledge::LocationKnowledge(Location* location) {
    mLocation = location;
}

float LocationKnowledge::getOpinion() { return mOpinion; }

ItemMemory::ItemMemory(ItemKnowledge* itemK, LocationKnowledge* locationK,
                       struct Position position) {
    mItemKnowledge = itemK;
    mLocationKnowledge = locationK;
    mPosition = position;
}

Position ItemMemory::getPosition() { return mPosition; }

HealthStatus::HealthStatus(std::string name) { mName = name; }

std::string HealthStatus::getName() { return mName; }

HealthStatus satation = HealthStatus("Satation");
HealthStatus energy = HealthStatus("Energy");

Action::Action(void (*action)(ActionArgs)) { mAction = action; }

void Action::execute(ActionArgs args) { mAction(args); }

std::string Action::getName() { return mName; }

Item hotdog = Item("Hotdog", {{0.7f, 0.45f, 0.30f}, 0.15f, 0.7f, 0.2f, 0.2f});
Item money = Item("Money", {{0.7f, 0.45f, 0.30f}, 0.1f, 0.1f, 0.7f, 0.01f});

Task::Task(Action* action, ActionArgs arguments) {
    mAction = action;
    mActionArgs = arguments;
}

void Task::execute() { mAction->execute(mActionArgs); }

Action buyItem = Action([](ActionArgs args) {
    assert(args.type == ActionArgs::Type::transaction);
    if (args.args.transaction.person->trade(&money, -10)) {
        args.args.transaction.person->trade(args.args.transaction.item, 1);
        std::cout << args.args.transaction.person->getName();
        std::cout << " bought ";
        std::cout << args.args.transaction.item->getName();
        std::cout << "\n";
    }
});

Action eatItem = Action([](ActionArgs args) {
    assert(args.type == ActionArgs::Type::transaction);
    if (args.args.transaction.person->trade(args.args.transaction.item, -1)) {
        args.args.transaction.person->alter(&satation, 0.4f);
        std::cout << args.args.transaction.person->getName();
        std::cout << " ate ";
        std::cout << args.args.transaction.item->getName();
        std::cout << "\n";
    }
});

Action afflictionSleep = Action(
    [](ActionArgs args) { args.args.health.person->alter(&energy, 0.05f); });

Action afflictionAwake = Action(
    [](ActionArgs args) { args.args.health.person->alter(&energy, -0.1f); });

Action afflictionDigest = Action([](ActionArgs args) {
    if (args.args.health.person->alter(&satation, -0.1f)) {
        args.args.health.person->alter(&energy, 0.05f);
    }
});

Person::Person(std::string name, PersonDescription description) {
    mName = name;
    mDescription = description;
    mHealth[&satation] = 1.0f;
    mItems[&money] = 100;
    this->addAffliction(&afflictionDigest);
    this->addAffliction(&afflictionAwake);
    std::cout << "Person ";
    std::cout << name;
    std::cout << " created\n";
}

PersonDescription Person::getDescription() { return mDescription; }

int Person::getItemCount(Item* item) { return mItems[item]; }

float Person::getHealthStatus(HealthStatus* status) { return mHealth[status]; }

std::string Person::getName() { return mName; }

bool Person::addAffliction(Action* affliction) {
    if (std::find(mAfflictions.begin(), mAfflictions.end(), affliction) !=
        mAfflictions.end()) {
        // Affliction is already applied.
        return false;
    }
    mAfflictions.push_back(affliction);
    return true;
}

bool Person::delAffliction(Action* affliction) {
    auto it = std::find(mAfflictions.begin(), mAfflictions.end(), affliction);
    if (it == mAfflictions.end()) {
        // Affliction not on person, nothing to remove.
        return false;
    }
    mAfflictions.erase(it);
    return true;
}

void Person::applyAfflictions() {
    for (int i = 0; i < mAfflictions.size(); i++) {
        mAfflictions.at(i)->execute({ActionArgs::Type::health, {this}});
    }
}

bool Person::addItemKnowledge(ItemKnowledge itemK) {
    mItemKnowledge.push_back(itemK);
    return true;
}

bool Person::addPersonKnowledge(PersonKnowledge personK) {
    mPersonKnowledge.push_back(personK);
    return true;
}

bool Person::addLocationKnowledge(LocationKnowledge locationK) {
    mLocationKnowledge.push_back(locationK);
    return true;
}

bool Person::createTask() {
    if (mHealth[&satation] < 0.2f) {
        if (1 <= mItems[&hotdog]) {
            mTasks.push(Task(&eatItem,
                             {ActionArgs::Type::transaction, {this, &hotdog}}));
            return true;
        }
        if (10 <= mItems[&money]) {
            mTasks.push(Task(&buyItem,
                             {ActionArgs::Type::transaction, {this, &hotdog}}));
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
    float newAmount = mHealth[item] + amount;
    if (0 < newAmount && newAmount < 1.0f) {
        mHealth[item] = newAmount;
        return true;
    }
    return false;
}

int main() {
    std::vector<ItemKnowledge> basicKnowledge = {ItemKnowledge(&hotdog),
                                                 ItemKnowledge(&money)};
    std::vector<Person> people = {
        Person("Bobby",
               {SKIN_COLOR_0, EYE_COLOR_0, HAIR_COLOR_0, 0.5f, 1.75f, 1.0f}),
        Person("Kenta",
               {SKIN_COLOR_0, EYE_COLOR_1, HAIR_COLOR_2, 0.6f, 1.80f, 1.5f})};

    for (Person person : people) {
        for (ItemKnowledge itemK : basicKnowledge) {
            person.addItemKnowledge(itemK);
        }
    }
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < people.size(); j++) {
            people.at(j).applyAfflictions();
            people.at(j).createTask();
            people.at(j).consumeTask();
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
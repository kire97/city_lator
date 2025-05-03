TL;DR:

Small scale city simulation where characters generate their day/life based on needs and wants.
The player possesses a random character everytime the current character loses consiousness.



Start everyday as a different person in a city starting with 100 ppl
Small scale self contained city
Every person starts out with certain goals, needs/wants, known locations, money, health conditions, contracts, ideologies, etc.
Contracts include home and job
Objects have owners bound to them
Limited wardrobe of people

Main loop is that you wake up as a different character everytime you lose consciounsness.
Your actions can alter the goals, needs, behaviour, etc. of the person.
You should be able to look into the mind of the person you are to see the current stats, memories, etc. of the person.

The game should mainly be a sandbox game where you can trigger different endings if you manage to fulfill the requirements.
One example could be to gather enough followers for a cult to bring forth an eldritch horror.
Another could be to overthrow the government into anarchy.
Maybe triggering a zombie apocalypse through a virus, and living through it.

Being drunk should add a delay to all inputs


```mermaid
graph TD
    P[Person]
    p1(hunger)-->P
    p2(thirst)-->P
    p3(tiredness)-->P
    p4(happiness)-->P
    p5(belief)-->P
    p6(curiousity)-->P
    p7(cautiousness)-->P
    p8(sanity)-->P

    P--low on parameter-->o1{has remedy on hand?}
    
    o1--yes -->a1[use remedy]
    
    o1--no -->o2{knows location of remedy?}
    o2--yes -->a2[go to location]
    o2--no -->a4[look for new location]
    a4-->o3

    a2-->o3{is remedy at location?}
    o3--yes -->a3[accuire remedy]
    o3--no -->a5[alter remedy memory connection to location]
    a5-->o2
    a3-->a1
```

```mermaid
classDiagram
        Person <|-- Task
        Person <|-- HealthStatus
        Person <|-- ItemInstance
        ItemInstance <|-- Item
        class Person {
            bool createTask()
            bool consumeTask()
            bool trade(Item*, int)
            bool alter(HealthStatus*, float)
            string getName()
        }
        class Task {
          int call(Person*)
        }
        class HealthStatus {
          string getName()
        }
        class Item {
          string getName()
          bool isStackable()
        }
        class ItemInstance {
            Item* getItem()
        }
```

```mermaid
---
config:
  theme: forest
---
erDiagram
    PERSON ||--o{ TASK : contains
    TASK ||--|| ACTION : contains
    PERSON ||..|| HEALTH-STATUS : contains
    PERSON ||..|| PERSON-IDENTITY: contains
    PERSON ||..o{ MEMORIES: contains
    MEMORIES ||--|| PERSON-MEMORY: contains
    MEMORIES ||--|| LOCATION-MEMORY: contains
    MEMORIES ||--|| ITEM-MEMORY: contains
    PERSON-MEMORY ||..|| PERSON-IDENTITY: points-at
    LOCATION-MEMORY ||..|| LOCATION: points-at
    ITEM-MEMORY ||..|| ITEM: points-at
```
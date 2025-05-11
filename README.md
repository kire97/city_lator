TL;DR:

Small scale city simulation where characters generate their day/life based on needs and wants.
The player possesses a random character everytime the current character loses consiousness.

I guess it is just a colony simulator, but with a vary mundane base.

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
    o1--no -->o2[scout vision for remedy]
    o2--yes -->a3

    o1--yes -->a1[use remedy]
    
    o2--no -->o3{knows location of remedy?}
    o3--yes -->a2[go to location]
    o3--no -->a4[look for new location]
    a4-->o4

    a2-->o4{is remedy at location?}
    o4--yes -->a3[accuire remedy]
    o4--no -->a5[alter remedy memory connection to location]
    a5-->o3
    a3-->a1
```

```mermaid
---
config:
  theme: redux
---
erDiagram
    PERSON ||--o{ TASK : contains
    TASK ||--|| ACTION : contains
    PERSON ||--o{ HEALTH-CONDITION: contains
    HEALTH-CONDITION ||..|{ HEALTH-STATUS: acts-upon
    PERSON ||--|{ HEALTH-STATUS : contains
    PERSON ||--|| PERSON-IDENTITY: contains
    PERSON ||--|| MEMORIES: contains
    MEMORIES ||--o{ ITEM-KNOWLEDGE: contains
    ITEM-KNOWLEDGE ||..|| ITEM: points-at
    MEMORIES ||--o{ PERSON-MEMORY: contains
    ITEM-MEMORY ||..|| LOCATION-KNOWLEDGE: points-at
    MEMORIES ||--o{ LOCATION-KNOWLEDGE: contains
    EVENT-MEMORY ||..o{ ITEM-MEMORY: points-at
    MEMORIES ||--o{ ITEM-MEMORY: contains
    ITEM-MEMORY ||..|| ITEM-KNOWLEDGE: points-at
    PERSON-MEMORY ||--|| PERSON-IDENTITY: contains
    LOCATION-KNOWLEDGE ||..|| LOCATION: points-at
    MEMORIES ||--o{ EVENT-MEMORY: contains
    EVENT-MEMORY ||..o{ PERSON-IDENTITY: points-at
    EVENT-MEMORY ||..|| ACTION: points-at
```

When an Item is observed; compare observational parameters with item-knowledge bank.
The persons threshold for recognizing the object is dependant on factors such as visual clarity and the persons personality.

When recollecting an item memory that has a known location; a mental representation of the objects known parameters is created at the known location.
If an object matching the representation is recognized then the object is found.
If the mental representation is occluded, try to get a vantage point where it can be observed before dismissing the memory.
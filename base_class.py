from typing import callable, any


class HealthCondition:
    name: str
    description: str
    symptom: callable[[Person], tuple]


class Contract:
    title: str
    description: str
    promises: list[Promise]


class Location:
    pass


class LocationMemory:
    real_location: Location
    name: str | None
    coordinate: tuple[int, int] | None
    opinion: float  # -1 = worst, 0 = neutral, 1 = best
    location_contains: list[Thing]


class PersonMemory:
    voice: VoiceIdentity
    appearance: Appearance  # Height, hair color, etc.
    clarity: float  # how convinced they are of the memory


class Person:
    name: str
    healthConditions: list[HealthCondition]
    contracts: list[Contract]
    locations: list[LocationMemory]
    people: list[PersonMemory]
    things: list[ThingMemory]
    memories: list[Memory]


from typing import Any

class AttributeInitType(type):
    def __call__(self, *args: Any, **kwds: Any) -> Any:
        """ Create a new instance """

        # first create the object in the normal default way
        obj = type.__call__(self, *args)

        # Additionally, set attributes on the new object.
        for name, value in kwds.items():
            setattr(obj, name, value)
        
        # return the new object
        return obj

class Car1: 
    def __init__(self, make: str, model: str, year: int, color: str) -> None:
        self.make = make
        self.model = model
        self.year = year
        self.color = color

    @property
    def description(self) -> str: 
        """ Return a description of this car."""
        return f"{self.color} {self.make} {self.model}"
    

class Car(object, metaclass=AttributeInitType):

    @property
    def description(self) -> str:
        """ Return a description of this car."""
        return " ".join(str(value) for value in self.__dict__.values())

if __name__ == "__main__":
    new_car1 = Car1(make="Toyota", model="Prius", year=2005, color="Green")
    print(new_car1.description)
    new_car = Car(make="Toyota", model="Prius", year=2005, color="Green", engine="Hybride")
    print(new_car.description)
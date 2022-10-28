"""
This file is just to confirm whether class method & object method can have 
same name or not. If they do this would mean that they are stored 
in different location i.e one container for class method and another for 
class method. But after test it appears that can't have same name which I
guess means they all stored in the same container.

Breakthrough: After doing some experimentations I found that the order
in which the methods are defined makes a difference.
the last one to be defined will erased the previous one whether they 
are object or class methods which again reinforces my thesis that they
are all stored in the same container. We will adopt the same method with lox. 
"""
class A: 
    def __init__(self, x) -> None:
        self.x = x
    def m():
        print("Class method")
    def m(self):
        print("object method")

if __name__ == "__main__":
    a = A(5)
    # A.m()
    a.m() # failed to execute.
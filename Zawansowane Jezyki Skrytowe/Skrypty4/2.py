#!/usr/bin/env python3

from typing import List

class Student:
    quantity = 0
    def __init__ (self):
        self.name = ""
        self.last_name = ""
        self.marks = []
        Student.quantity += 1
        self.index_num = 0
    def give_name(self, name: str, last_name: str) -> None:
        self.name = name
        self.last_name = last_name
    def give_mark(self, mark: int) -> None:
        self.marks.append(mark)
    def get_marks(self) -> List[int]:
        return self.marks
    def name_is_valid(self) -> bool:
        return bool(self.name and self.last_name)
    def say_hello(self) -> None:
        if self.name_is_valid():
            print("Hello! I'm " + self.name + " " + self.last_name + ". My index number is " + str(self.index_num) )
        else:
            print("Name is not valid")
    def get_avg_marks(self) -> List[int]:
        return sum(self.marks) / len(self.marks)
    def give_index_num(self, index_num: int) -> None:
        self.index_num = index_num
    def __lt__(self, other) -> bool:
        return self.index_num < other.index_num
    def __gt__(self, other) -> bool:
        return self.index_num > other.index_num
    def __eq__(self, other) -> bool:
        return self.index_num == other.index_num
    def __le__(self, other) -> bool:
        return self.index_num <= other.index_num
    def __ge__(self, other) -> bool:
        return self.index_num >= other.index_num
    def __ne__(self, other) -> bool:
        return self.index_num != other.index_num
    def __repr__(self) -> str:
        return (self.name + " " + self.last_name)
    def __str__(self) -> str:
        return (self.last_name + " " + self.name)


class Vehicle:
    def __init__(self,owner:str = "") -> None:
        print("Vehicle Start")
        self.owner = owner
        print("Vehicle End")
    def get_sound(self) -> None:
        print("vehicle's brum brum")
    def get_owner(self) -> str:
        return self.owner

class Vehicle2(Vehicle):
    def __init__(self,type:str="") -> str:
        print("Vehicle2 Start")
        self.type = type
        print(super().__init__(type))
        print("Vehicle2 End")



class Car(Vehicle):
    def __init__ (self, owner: str, table: str):
        self.owner = owner
        self.table = table
    def get_sound(self) -> None:
        print("car's brum brum")
    def get_owner(self) -> str:
        return self.owner

class Item:
    def get_sound(self) -> None:
        print("item's sound")
class Element:
    def get_sound(self) -> None:
        print("element's sound")
class Thing(Element, Item):
    def say_hello(self) -> None:
        print("hello!")
class Thing_Rev(Item,Element):
    def say_hello(self) -> None:
        print("hello 2!")

def task_1():
    s = Student()
    s.give_name("Jane", "Doe")
    s.give_mark(5)  # wywołanie sposób 1
    s.give_index_num(123456)
    Student.give_mark(s, 3)  # wywołanie sposób 2
    print(s.get_marks())
    print(s.get_avg_marks())
    s.say_hello()

def task_2():
    v = Vehicle()
    c = Car("Jane","Table")

    v.get_sound()
    c.get_sound()
    print(v.get_owner())
    print(c.get_owner())

def task_3():
    s1 = Student()
    s2 = Student()
    s3 = Student()
    s4 = Student()
    print(s1.quantity)
    print(s2.quantity)
    print(s3.quantity)
    print(s4.quantity)
    print(Student.quantity)

def task_4():
    s1 = Student()
    s1.say_hello()
    s1.give_name("Jane", "Doe")
    s1.say_hello()
    print(s1.name_is_valid())

def task_5():
    x1 = Item()
    x2 = Element()
    x3 = Thing()
    x4 = Thing_Rev()

    x1.get_sound()
    x2.get_sound()
    x3.get_sound()
    x4.get_sound()

def task_6():
    s1 = Student()
    s1.give_index_num(202500)
    s2 = Student()
    s2.give_index_num(432432)

    print(s1>s2)
    print(s1<s2)
    print(s1!=s2)

def task_7():
    s1 = Student()
    s1.give_name("Jane", "Doe")
    print(repr(s1))
    print(str(s1))

def task_8():
    s1 = Student()
    s1.give_name("Jane", "Doe")
    s1.give_index_num(202500)
    s1.give_mark(5)

    print(s1.__dict__)

def task_9():
    v2=Vehicle2()

def main():
    task_9()


if __name__ == '__main__':
    main()
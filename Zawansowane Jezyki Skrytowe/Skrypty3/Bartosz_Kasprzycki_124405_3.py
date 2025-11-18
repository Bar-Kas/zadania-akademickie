#!/usr/bin/env python3

def task_1():
    list = []
    print("a.",list)

    list.extend([0,1,2,3,4])
    print("b.",list)

    print("c.",list[:2])

    print("d.",len(list))

    print("e.",list[::2])

    list.append(5)
    print("f.",list)

#   list.append("Cool_Word")
#   print("g.",list)

    list.sort()
    print("h.",list)

    list.pop(-1)
    print("i.",list)

    list.sort(reverse=True)
    print("j.",list)

    list.insert(2,67)
    print("k.",list)

    print("l.",list.count(13))

def task_2():
    list = []
    for i in range(10):
        list.append(int(input(f"Input number {i+1}: ")))
    list.sort()
    print(f"Biggest number is {list[-1]} and the smallest number is {list[0]}")

    not_negative=[]
    for i in list:
        if i >= 0:
            not_negative.append(i)
    print(f"The avrage of not negative number is {sum(not_negative)/len(not_negative)}")

def task_3():
    list1 = [1,2,3,4,5]
    list2 = [1,2,4,5,6]

    unique_list=[]
    for x in list1:
        if x not in list2:
            unique_list.append(x)

    print(unique_list)

def task_4():
    cool_list = [1,2,3,4,5,6,7,8,9,0]
    even_list = []
    for x in cool_list:
        if x % 2 == 0:
            even_list.append(x)

    print(even_list)
    even_list.sort()
    print(even_list[0])

def task_5():
    listA = [1,2,3,4,5]
    listB = ['a','b','c','d']

    conlistA=listA+listB
    conlistB=listB+listA

    print(conlistA)
    print(conlistB)

def task_6():

    temp_list=[]

    while True:
        x=int(input("Enter a number:"))
        if x == 0:
            break
        temp_list.append(x)

    the_set = set(temp_list)
    print(the_set)

def task_7():
    tuple = ("apple", "banana", "cherry")
    tuple_b = ("orange",)
    tuple += tuple_b  #dodawanie krotek
    multi_tuple  = tuple * 2  #mnożenie krotek
    print(len(tuple))  #długość krotki - liczba elementów
    for x in tuple:   #wypisanie wszystkich elementów krotki
        print(x)

def task_8():
    the_set = {"apple", "banana", "cherry", "papaya", "kiwi"}
    print("a.",the_set)
    the_set.remove("banana")
    the_set.discard("cherry")
#   the_set.remove("something")
    the_set.discard("something")
    print(the_set)
    popped = the_set.pop()
    print(f"Popped: {popped}")
    print(the_set)

def task_9():
    set1= {1,2,3,4,5}
    set2= {5,6,7,8,9}

    print(set1.isdisjoint(set2))
    print(set1.issubset(set2))
    print(set1.issuperset(set1))
    print(set1.union(set2))
    print(set1.difference(set2))
    print( set1.intersection(set2))

def task_10():
    person = {"name": "Alan", "lastname": "Wake", "age": 47, "city": "Night Springs", "status": "Active",
              "ocupation": "Writer"}

    person.update({
        "status": "Missing",
    })
    person.pop("status")
    person.popitem()

    print(person.items())

def task_11():
    students = {
        123456:{
            "name": "Alan",
            "lastname": "Wake",
            "grades": [5.0,4.5,3.0,4.0]
        },
        678903: {
            "name": "John",
            "lastname": "Helldiver",
            "grades": [5.0, 4.5, 2.0, 4.0]
        },
        234567:{
            "name": "Jane",
            "lastname": "Doe",
            "grades": [3.0,4.0,3.5,2.0]
        }


    }

    for index,data in students.items():
        grades=data["grades"]
        avg=sum(grades)/len(grades)

        print(f"Index = {index}, name = {data['name']} {data['lastname']}, Grades = {grades}, Avrage_Grade = {avg}")

def main():
    task_11()


if __name__ == '__main__':
    main()
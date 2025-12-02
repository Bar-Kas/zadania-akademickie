#!/usr/bin/env python3
import os
import shutil
import random
import struct

def task1():
    input_string = "2,5"
    try:
        # some_number = 3 / 0
        # some_number = float(input_string)
        some_number = int(input_string)
        print(f"This number is: {some_number}")
    except (ValueError, UnicodeError) as ex1:
        print("cannot do it :(")
        print(ex1)
    except NameError:
        print("i dont know this name :(")
    except:
        print("i dont know this error, sorry..")


def task2():
    try:
        x = -1
        if x < 0:
            raise Exception("Sorry, no numbers below zero")
        if x < 0:
            raise ValueError("Sorry, no numbers below zero")
    except ValueError as ex:
        print(f"Error cought: {ex}")
    except Exception as ex:
        print(f"Exception cought: {ex}")


def task3():
    try:
        a = int(input("Enter a number: "))
        b = int(input("Enter another number: "))

    except ValueError:
        print("You didn't enter a number")
    except Exception as ex:
        print(f"Something went wrong : {ex}")
    else:
        print(f"{a} + {b} = {a + b}")
    finally:
        print("End of Function")


def task4():
    try:
        file_name = "file.txt"

        with open(file_name, "r") as f:
            print(f.read())
            f = open(file_name, "r")
            print(f.readline())
            f = open(file_name, "r")
            print(f.read(10))
            f = open(file_name, "r")
            for x in f:
                print(x)

            f = open(file_name, "r")
            list = f.readlines()
            list_ten = f.readlines(10)
    except FileNotFoundError:
        print("File not found")
    except Exception as ex:
        print(f"Error : {ex}")


def task5():
    filename = "test_file_task5.txt"
    modes = ["w", "a", "x"]
    i=0

    try:
        with open(filename, modes[i]) as f:
            f.write("Test")
        print("Success")
    except FileExistsError:
        print(f"File already exists")
    except Exception as ex:
        print(f"Error {ex}")


def task6():
    dir_name = "C:\\Users\\Laptop\\Downloads\\aaa\\"
    try:
        for root, dir, file in os.walk(dir_name):
            level = root.replace(dir_name, '').count(os.sep)
            indent = ' ' * 4 * (level)
            print(f"{indent}{os.path.basename(root)}/")
            sub = '---- ' * (level + 1)
            for f in file:
                print(f"{sub}{f}")
    except Exception as ex:
        print(f"Error  :{ex}")


def task7():
    src = "file.txt"
    exists = "file_copy.txt"
    missing = "missing_dir/file_copy.txt"

    if not os.path.exists(src):
        with open(src, 'w') as f: f.write("Test content")

    try:
        shutil.copy(src, exists)
        print("File Created")
    except Exception as ex:
        print(f"Copy error: {ex}")

    try:
        shutil.copy(src, exists)
        print("File overwriten")
    except Exception as ex:
        print(f"Copy Error :{ex}")

    try:
        shutil.copy(src, missing)
    except FileNotFoundError:
        print("File does not exist")
    except Exception as ex:
        print(f"Error :{ex}")


def task8(n, a, b):
    try:
        with open("numbers.txt", "w") as f:
            for x in range(n):
                num = random.randint(a, b)
                f.write(f"{num}\n")
                print(f"{num}")
        print("All numbers saved")
    except Exception as ex:
        print(f"Error :{ex}")


class Student:
    def __init__(self, name, student_id, grades):
        self.name = name
        self.student_id = int(student_id)
        self.grades = grades

    def save_to_file(self, filename):
        try:
            with open(filename, "w") as f:
                f.write(f"{self.name}\n")
                f.write(f"{self.student_id}\n")
                f.write(",".join(map(str, self.grades)))
            print(f"{filename} | File created")
        except Exception as ex:
            print(f"Error saving student info : {ex}")

    def load_from_file(self, filename):
        try:
            with open(filename, "r") as f:
                self.name = f.readline().strip()
                self.student_id = int(f.readline().strip())
                grades_str = f.readline().strip()
                if grades_str:
                    self.grades = [float(g) for g in grades_str.split(",")]
            print(f"Student name: {self.name}, ID: {self.student_id}, Grades: {self.grades}")
        except ValueError as ex:
            print(f"Value Error : {ex}")
        except FileNotFoundError:
            print("File not found")
        except Exception as ex:
            print(f"Undefined Error while loading student infp : {ex}")


def task9():
    s1 = Student("Alan Wake", 765432, [3.5,4,5,3,2,3.5])
    s1.save_to_file("student_data.txt")

    s2 = Student("", 0, [])
    s2.load_from_file("student_data.txt")


def task10():
    try:
        with open("numbers.txt", "r") as f_input, open("numbers.bin", "wb") as f_output:
            for line in f_input:
                try:
                    number = int(line.strip())
                    f_output.write(number.to_bytes(4, byteorder='big', signed=True))
                except ValueError:
                    continue
        print("Conversion sucessful")
    except Exception as ex:
        print(f"Error : {ex}")


def main():
    task10()


if __name__ == '__main__':
    main()
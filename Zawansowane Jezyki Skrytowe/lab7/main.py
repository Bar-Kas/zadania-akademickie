#!/usr/bin/env python3
import re
import os











def task1():
    filename = "inwokacja.txt"
    try:
        with open(filename, "r", encoding="utf-8") as f:
            lines = f.readlines()

        print(f"Total rows: {len(lines)}")

        total_chars = 0
        total_words = 0
        line_number = 0

        for line in lines:
            line = line.strip()
            if not line:
                continue

            line_number += 1
            chars = len(line)

            words_list = line.split()
            words = len(words_list)

            total_chars += chars
            total_words += words

            print(f"Row {line_number}: chars={chars}, words={words}")

        print(f"Total summary: chars={total_chars}, words={total_words}")

    except FileNotFoundError:
        print("File not found")
    except Exception as ex:
        print(f"Error caught: {ex}")


def task2():
    try:
        with open("inwokacja.txt", "r", encoding="utf-8") as f:
            content = f.read()

        spaces = 0
        newlines = 0
        tabs = 0

        for char in content:
            if char == ' ':
                spaces += 1
            elif char == '\n':
                newlines += 1
            elif char == '\t':
                tabs += 1

        print(f"Spaces: {spaces}")
        print(f"Newlines: {newlines}")
        print(f"Tabs: {tabs}")

    except Exception as ex:
        print(f"Error caught: {ex}")


def task3():
    try:
        with open("inwokacja.txt", "r", encoding="utf-8") as f:
            content = f.read()

        temp = content.replace("...", "TEMP_MARKER")
        temp = temp.replace(".", "...")
        final = temp.replace("TEMP_MARKER", "...")

        print(final)
    except Exception as ex:
        print(f"Error caught: {ex}")


def task4():
    names = ["Anna", "Alan", "Ewa", "jane", "Iwona", "Katherine", "piotr", "Barnaba"]
    print(f"Input list: {names}")

    pattern = re.compile(r'^[A-Z][a-z]*a$')

    found_names = []
    for name in names:
        if pattern.match(name):
            found_names.append(name)

    print(f"Filtered names (Female, Capitalized): {found_names}")


def task5():
    try:
        with open("numery.txt", "r") as f:
            lines = f.readlines()

        print("Polish numbers found (+48 or 0048):")
        pattern = re.compile(r'^(\+48|0048)')

        for line in lines:
            num = line.strip()
            if pattern.match(num):
                print(num)

    except FileNotFoundError:
        print("File not found")


def task6():
    try:
        with open("numery.txt", "r") as f:
            lines = f.readlines()

        pat1 = r"^\d{3}-\d{3}-\d{3}$"
        pat2 = r"^\d{9}$"
        pat3 = r"^\+48\d{9}$"
        pat4 = r"^0048\d{9}$"
        pat5 = r"^\+48 \d{3} \d{3} \d{3}$"

        count1 = 0
        count2 = 0
        count3 = 0
        count4 = 0
        count5 = 0

        for line in lines:
            num = line.strip()
            if re.match(pat1, num):
                count1 += 1
            elif re.match(pat2, num):
                count2 += 1
            elif re.match(pat3, num):
                count3 += 1
            elif re.match(pat4, num):
                count4 += 1
            elif re.match(pat5, num):
                count5 += 1

        print(f"Format XXX-XXX-XXX: {count1}")
        print(f"Format XXXXXXXXX: {count2}")
        print(f"Format +48XXXXXXXXX: {count3}")
        print(f"Format 0048XXXXXXXXX: {count4}")
        print(f"Format +48 XXX XXX XXX: {count5}")

    except Exception as ex:
        print(f"Error caught: {ex}")

def validate_gmail(email):
    pattern = r"^[a-z][a-z0-9-]*@gmail\.com$"
    if re.match(pattern, email):
        return True
    else:
        return False

def task7():
    emails = ["alan.wake@gmail.com", "Jane.Doe@gmail.com", "user@wp.pl", "test@gmail.com"]
    print("Validating emails:")
    for email in emails:
        is_valid = validate_gmail(email)
        print(f"{email} -> {is_valid}")

def validate_date(date_str):
    pattern = r"^(0[1-9]|[12][0-9]|3[01])[-/](0[1-9]|1[0-2])[-/](\d{4})$"
    return re.match(pattern, date_str)

def get_month_name(month_num):
    months = {
        '01': 'January', '02': 'February', '03': 'March', '04': 'April',
        '05': 'May', '06': 'June', '07': 'July', '08': 'August',
        '09': 'September', '10': 'October', '11': 'November', '12': 'December'
    }
    if month_num in months:
        return months[month_num]
    else:
        return "Unknown"

def task8():
    date_input = input("Enter date (dd-mm-yyyy or dd/mm/yyyy): ")
    match = validate_date(date_input)

    if match:
        day = match.group(1)
        month = match.group(2)
        year = match.group(3)
        month_name = get_month_name(month)
        print(f"The month is: {month_name}")
    else:
        print("Invalid date format")


def task9():
    dirname = "."
    print(f"Searching for .txt files in: {dirname}")
    try:
        pattern = re.compile(r'.*\.txt$')
        all_files = os.listdir(dirname)

        for filename in all_files:
            if pattern.match(filename):
                print(f"Found: {filename}")

    except Exception as ex:
        print(f"Error caught: {ex}")


def task10():
    data = ["sky", "boy", "toy", "axe", "apple", "echo", "element", "item", "area", "yes"]
    print(f"Input data: {data}")

    list1 = []
    list2 = []
    list3 = []

    for word in data:
        if re.search(r'[xy]$', word):
            list1.append(word)

        if re.match(r'^a..$', word):
            list2.append(word)

        if re.match(r'^[aeiouyAEIOUY]', word):
            list3.append(word)

    print(f"Ends with x or y: {list1}")
    print(f"3 chars starting with a: {list2}")
    print(f"Starts with vowel: {list3}")


def main():
    task10()


if __name__ == '__main__':
    main()
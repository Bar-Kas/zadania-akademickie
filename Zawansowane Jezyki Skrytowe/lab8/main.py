#!/usr/bin/env python3
import re
import os


def task1():
        text = "aaaaa"
        patterns = [r'a+', r'a+?', r'a*', r'a*?', r'a?', r'a??']

        for p in patterns:
            print(f"{p}: {re.findall(p, text)}")


def task2():
    try:
        filename = "inwokacja.txt"

        with open(filename, "r", encoding="utf-8") as f:
            content = f.read()

        print(re.findall(r'\b\w+(?=!)', content))

        print(re.findall(r'\b\w*[ąęćłńóśźżĄĘĆŁŃÓŚŹŻ]\w*\b', content))

        count = len(re.findall(r'\bcie\b|\bcię\b|\bci\b', content, re.IGNORECASE))
        print(f"Liczba wystąpień 'cie', 'cię' lub 'ci': {count}")

    except Exception as ex:
        print(f"Error caught: {ex}")


def task3():
    try:
        with open("adresy.txt", "r", encoding="utf-8") as f:
            lines = f.readlines()

        pattern = r"ul\.\s+(.*?)\s+(\d+\S*)\s+(\d{2}-\d{3})"

        for line in lines:
            match = re.search(pattern, line.strip())
            if match:
                print(f"Street: {match.group(1)}, Num: {match.group(2)}, Zip: {match.group(3)}")

    except FileNotFoundError:
        print("File not found")
    except Exception as ex:
        print(f"Error caught: {ex}")


def task4():

    addr = "Al. prof. S. Kaliskiego 7 85-796 Bydgoszcz"
    pattern = r"^.*?(?=\s\d)"

    match = re.search(pattern, addr)
    if match:
        print(match.group(0))



def task5():
    text = "Ala ma kota a kot ma Ale"
    print(re.findall(r"ala", text, flags=re.I))

def task6():

    passwords = ["abc", "Slabe1", "Mocne1!"]

    for pwd in passwords:
        status = "Słabe"
        if len(pwd) >= 6 and re.search(r'\d', pwd):
            status = "Średnie"
            if re.search(r'(?=.*[A-Z])(?=.*[\W_])', pwd):
                status = "Mocne"
        print(f"{pwd}: {status}")


def fix_typo(match):
    word = match.group(0)
    if len(word) == 2:
        ans = input(f"Replace {word} with {word.capitalize()}? (y/n): ")
        if ans.lower() == 'y':
            return word.capitalize()
        return word
    return word.capitalize()


def task7():
    text = "Mieszkam w BYdgoszczy. To jest IT. Studiuję na poliTechnice."
    pattern = r'\b(?:[A-Z]{2}\w*|\w*[a-z][A-Z]\w*)\b'

    new_text = re.sub(pattern, fix_typo, text)
    print(new_text)


def main():
    task7()


if __name__ == '__main__':
    main()
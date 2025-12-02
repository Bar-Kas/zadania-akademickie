#!/usr/bin/env python3
import random,math,sys

def task1():
    p=True
    while True:
        a=int(input("Input a base number:"))
        if a in [2,8,10,16]:
            break

    b=int(input("Input a number:"),a)

    print(f"{bin(b)}\n{oct(b)}\n{b}\n{hex(b)}")

def task2():
    a=int(input("Input a number:"))
    b=int(input("Input a index of bit:"))
    b=len(bin(a))-1-b
    print(bin(a)[b])

def task3():
    random.seed()
    a=random.randint(3,10)
    b = random.randint(3, 10)
    c = random.randint(3, 10)

    print(f"{a}/{b}/{c}")

    if a + b > c and a + c > b and b + c > a:
        s = (a + b + c) / 2
        pole = math.sqrt(s * (s - a) * (s - b) * (s - c))
        print(f"Can create trianlge\n Area = {pole}")
    else:
        print("Can't create trianlge")

def task4():

    won = 0
    games=0
    inp=""
    while True:
        inp=input("Heads(h) or tails(t) ?\n(x) to quit\n")
        if inp=="x":
            break
        games+=1
        random.seed()
        if inp==random.choice(("h","t")):
            print("You won")
            won+=1
        else:
            print("You lost")
    print(f"Final results:\nGames: {games}\nWins: {won}")

def task5():
    won = 0
    games=0
    inp=""
    while True:
        inp=input("Rock(r),Paper(p) or Scissors(s) ?\n(x) to quit\n")
        if inp=="x":
            break
        games+=1
        random.seed()
        if inp==random.choice(("r","p","s")):
            print("You won")
            won+=1
        else:
            print("You lost")
    print(f"Final results:\nGames: {games}\nWins: {won}")


def task6():
    length = float(input("Input ladder length: "))
    deg = float(input("Input angle (degrees): "))

    rad = math.radians(deg)
    height = length * math.sin(rad)

    print(f"Height: {height}")


def task7():
    print("Checking trigonometry (sin^2 + cos^2 = 1):")
    all_correct = True
    for angle in range(91):
        rad = math.radians(angle)
        result = math.sin(rad) ** 2 + math.cos(rad) ** 2
        print(f"Angle {angle}: {result}")
        if not math.isclose(result, 1.0):
            all_correct = False

    if all_correct:
        print("Identity holds for all integer angles 0-90.")


def task8():
    text = input("Input text to encrypt: ")
    key = input("Input key (must be same length or longer): ")

    if len(key) < len(text):
        print("Error: Key is too short!")
        return

    encrypted = ""
    print("Encrypted chars:")
    for i in range(len(text)):
        # XOR na kodach znaków [cite: 86]
        val = ord(text[i]) ^ ord(key[i])
        encrypted += chr(val)

    print(f"Result string: {encrypted}")

    decrypted = ""
    for i in range(len(text)):
        val = ord(encrypted[i]) ^ ord(key[i])
        decrypted += chr(val)
    print(f"Decrypted back: {decrypted}")


def task9():
    p = int(input("Input exponent p: "))
    result = 1 << p
    print(f"2^{p} = {result}")


def task10():
    val = float(input("Input a float number: "))

    print(f"Trunc: {math.trunc(val)}")
    print(f"Floor: {math.floor(val)}")
    print(f"Ceil: {math.ceil(val)}")
    print(f"Abs (math.fabs): {math.fabs(val)}")  # [cite: 92]

    print(f"Python version: {sys.version}")
    if sys.version_info >= (3, 9):
        print("Python >= 3.9 detected. Testing LCM and GCD.")
        a = int(input("Input int a: "))
        b = int(input("Input int b: "))
        print(f"LCM({a},{b}) = {math.lcm(a, b)}")
        print(f"GCD({a},{b}) = {math.gcd(a, b)}")


def main():
    task7()

if __name__ == '__main__':
    main()
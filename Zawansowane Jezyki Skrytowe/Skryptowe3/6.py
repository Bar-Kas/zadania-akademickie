from math import trunc


def test():

    temp_list=[]

    while True:
        x=int(input("Enter a number:"))
        if x == 0:
            break
        temp_list.append(x)

    the_set = set(temp_list)
    print(the_set)


def main():
    test()

if __name__ == '__main__':
    main()
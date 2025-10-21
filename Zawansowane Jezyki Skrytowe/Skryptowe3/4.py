def not_odd():
    cool_list = [1,2,3,4,5,6,7,8,9,0]
    even_list = []
    for x in cool_list:
        if x % 2 == 0:
            even_list.append(x)

    print(even_list)
    even_list.sort()
    print(even_list[0])

def main():
    not_odd()


if __name__ == '__main__':
    main()
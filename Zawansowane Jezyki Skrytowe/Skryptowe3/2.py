def user_insert_elements():
    list1 = []
    for i in range(10):
        num = float(input(f"Podaj liczbę {i + 1}: "))
        list1.append(num)

    print(list1)
    list1.sort()
    print(f"Najmniejszy {list1[0]}")
    print(f"Największy {list1[-1]}")

    not_negative = [x for x in list1 if x >= 0]

    if len(not_negative) > 0:
        print(f"Średnia {sum(not_negative)/len(not_negative)}")
    else:
        print("Wszystkie liczby są ujemne")





def main():
    user_insert_elements()

if __name__ == '__main__':
    main()


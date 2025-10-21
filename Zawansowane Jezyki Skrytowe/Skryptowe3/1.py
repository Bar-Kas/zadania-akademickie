def list_op():
    list1 = []
    print('a. =',list1)
    list1.extend([5,11,67,1,420])
    print('b. =', list1)
    print('c. =', list1[:2],list1[-2:])
    print('d. =', len(list1))
    print('e. =', list1[1::2])
    list1.append(2137)
    print('f. =', list1)
    #list1.append("Fajne słowo")

    #Nie da się posortować listy z typami int i str

    print('g. =', list1)
    list1.sort()
    print('h. =', list1)
    list1.pop(-1)
    print('i. =', list1)
    list1.sort(reverse=True)
    print('j. =', list1)
    list1.insert(2,234)
    print('k. =', list1)
    print('l. =', list1.count(13))




def main():
    list_op()



if __name__ == '__main__':
    main()
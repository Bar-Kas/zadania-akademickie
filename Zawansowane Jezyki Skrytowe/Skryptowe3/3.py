def unique():
    list1 = [1,2,3,4,5]
    list2 = [1,2,4,5,6]

    unique_list=[]
    for x in list1:
        if x not in list2:
            unique_list.append(x)

    print(unique_list)



def main():
    unique()


if __name__ == '__main__':
    main()

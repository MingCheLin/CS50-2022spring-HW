# TODO
def main():
    num = input("Number: ")
    # Check valid or not
    if (check_valid(num)):
        # If number is valid than check which type it is
        if (len(num) == 15) & (num[0] == '3') & ((num[1] == '4') | (num[1] == '7')):
            print("AMEX")
        elif (len(num) == 16) & (num[0] == '5') & (int(num[1]) <= 5) & (int(num[1]) >= 1):
            print("MASTERCARD")
        elif ((len(num) == 16) | (len(num) == 13)) & (num[0] == '4'):
            print("VISA")
        else:
            print("INVALID")


def check_valid(num):       # Check if the num is valid
    total1 = 0
    total2 = 0
    # run the number respectively
    for i in range(len(num)):
        if (((len(num) - i) % 2) == 0):
            tmp = int(num[i]) * 2
            if tmp > 9:
                total1 += 1
                total1 += (tmp - 10)
            else:
                total1 += tmp
        else:
            total2 += int(num[i])
    if ((total1 + total2) % 10) != 0:
        print("INVALID")
        return False
    else:
        return True


# start main
if __name__ == "__main__":
    main()

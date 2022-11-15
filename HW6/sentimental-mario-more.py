# TODO
def main():
    # get int input without using getint, if enter a non int or not between 1 to 8 the scrip will ask again
    while True:
        height = input("Height: ")
        try:
            height = int(height)
            if (height > 0) & (height < 9):
                break
        except:
            pass
    # Print #
    for i in range(1, height+1):
        print((" " * (height - i)) + ("#" * i) + "  " + ("#" * i))

        
# make sure the py file run as scrip
if __name__ == "__main__":
    main()

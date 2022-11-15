# TODO
def main():
    name = input("What is your name?")
    try:
        print(f"hello, {name}")
    # if the input error print something warn user
    except:
        print("Weird name")

# With this line the py file will only run when run it as scrip
if __name__ == "__main__":
    main()

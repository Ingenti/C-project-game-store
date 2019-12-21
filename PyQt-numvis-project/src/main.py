from GUI import *
import sys

def main():

    path = input("Welcome to the numerical data visualization tool. Enter filename:")

    application = QApplication(sys.argv)

    window = GUI(path)

    sys.exit(application.exec_())

if __name__ == '__main__':
    main()

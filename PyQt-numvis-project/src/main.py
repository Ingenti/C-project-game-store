"""Starts the visualization program, imports from file that creates GUI"""
import sys
from PyQt5.QtWidgets import QApplication
from gui import GUI

def main():
    """main starts the execute loop opening a window that handles paintEvents of PyQt"""

    path = input("Welcome to the numerical data visualization tool. Enter filename:")
    application = QApplication(sys.argv)
    client = GUI(path)

    sys.exit(application.exec_())

if __name__ == '__main__':
    main()

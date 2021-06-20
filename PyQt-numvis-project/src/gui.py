"""Module for main application Graphical User Interface (GUI)"""

import math
from PyQt5.QtCore import Qt, QPoint, QDir
from PyQt5.QtGui import QIcon, QPixmap, QPainter, QFont, QPen
from PyQt5.QtWidgets import QInputDialog, QMainWindow, QAction, QFileDialog, qApp
from data_visualization_parser import GUIreader

class GUI(QMainWindow):
    """(PyQt5) QMainWindow-based Graphical user interface with customized interactions"""

    def __init__(self, filename):
        """Initializes attributes necessary for
         generating both the window and the visualizations"""
        super().__init__()
        self.filename = filename
        self.data = None

        self.title = ""             # Title for visualization
        self.x_labels = ""          # Label for x-axis
        self.y_labels = ""          # Label for y-axis
        self.x_range = None         # Range for x-axis
        self.y_range = None         # Range for y-axis
        self.x_bin = None           # Contains bin threshold values for x-axis
        self.y_bin = None           # Contains bin threshold values for y-axis
        self.x_window_scale = None  # upper limit of x-axis in pixels
        self.y_window_scale = None  # upper limit of y-axis in pixels

        self.grid_bins = None
        self.grid_o = True
        self.legend = True
        self.legends = None         # Storage for legends if multiple visualizations are needed
        self.menu = True
        self.menubar = None         # Menubar if that is added
        self.statusbar = None       # Statusbar if that is added
        self.point = "Point"
        self.line = "Line"
        self.pic = None             # If a picture is wanted to be displayed
        self.image = None           # If the screen wants to be saved

        self.setMinimumSize(int(self.width() / 2), int(self.height() / 2))
        self.adjustSize()
        self.initialize_ui(filename)


    def initialize_ui(self,filename):
        """Initializes the window on which the visualization is generated"""
        self.filename = filename
        reader = GUIreader(self.filename)
        self.data = reader.read_data()     # Contains Graph with x & y values

        self.grid_bins = 5
        self.find_axis_scales()
        self.build_menu()
        self.setGeometry(0, 0, 300, 200)

        #self.pic = QLabel(self)
        #self.pic.setPixmap(QPixmap("%USERPROFILE%/pic.png"))
        #self.pic.setGeometry(0,-100,1000,1000)

        # User prompted for info
        self.title, title_given = QInputDialog.getText(self, 'Title', 'Enter Title:')
        if not title_given or self.title == "":
            self.title = "Placeholder Title"

        self.x_labels, xlabel_given = QInputDialog.getText(self, 'X-Label', 'Enter X-Label:')
        if not xlabel_given or self.x_labels == "":
            self.x_labels = "Placeholder X-label"

        self.y_labels, ylabel_given = QInputDialog.getText(self, 'Y-Label', 'Enter Y-Label:')
        if not ylabel_given or self.y_labels == "":
            self.y_labels = "Placeholder Y-label"

        self.setWindowTitle(self.title)
        self.show()

    def paintEvent(self, event):
        """Override QWidget function: execution control
        goes through this function every time window is updated"""
        if self.data.graph_type == "plot":
            self.draw_lines()
            self.draw_axis()
            if self.grid_o:
                self.draw_grid()

            if self.legend:
                self.draw_legend()


    def find_axis_scales(self):
        """Find scales of axes based on window dimensions and data"""
        min_y = 0
        max_y = 0

        # Find largest and smallest value

        for number in self.data.ylist:
            if number < min_y:
                min_y = number
            elif number > max_y:
                max_y = number

        max_y = round(max_y)

        min_x = 0
        max_x = 0
        for number in self.data.xlist:
            if number < min_x:
                min_x = number
            elif number > max_x:
                max_x = number

        max_x = round(max_x)

        self.x_range=(int(math.ceil(max_x/10.0))*10)     # Round x- and y-ranges to tens
        self.y_range=(int(math.ceil(max_y/10.0))*10)

        self.x_bin = []
        self.y_bin = []

        k = 0
        addable = 0
        while addable < self.x_range:
            addable = k * int(math.ceil((self.x_range / self.grid_bins) / 10.0) * 10)
            self.x_bin.append(addable)
            k = k + 1

        k = 0
        addable = 0
        while addable < self.y_range:
            addable = k * int(math.ceil((self.y_range / self.grid_bins) / 10.0) * 10)
            self.y_bin.append(addable)
            k = k + 1

        print("Bins:")
        print(self.x_bin)
        print(self.y_bin)

    def draw_axis(self):
        """Draw axes and labels to the graph"""

        painter = QPainter(self)
        painter.setFont(QFont('Decorative', 10))

        painter.setPen(QPen(Qt.black, 4, Qt.SolidLine))
        painter.drawLine(0, self.height(), 0, 0)
        painter.setPen(QPen(Qt.black, 4, Qt.SolidLine))
        painter.drawLine(0, self.height(), self.width(), self.height())

        # Find x-axis bin thresholds
        painter.setPen(QPen(Qt.black, 1, Qt.SolidLine))

        for i in range(len(self.x_bin)):
            painter.drawLine(self.x_bin[i]*self.width()/self.x_bin[-1], self.height(),
                             self.x_bin[i]*self.width()/self.x_bin[-1], self.height()*19.3/20)
            painter.drawText(QPoint(self.x_bin[i]*self.width()/self.x_bin[-1] + 5,
                                    self.height() - 5), str(round(self.x_bin[i])))

        # Find y-axis bin thresholds
        for i in range(len(self.y_bin)):
            painter.drawLine(0, self.y_bin[i]*self.height()/self.y_bin[-1], self.width()/62,
                             self.y_bin[i]*self.height()/self.y_bin[-1])
            painter.drawText(QPoint(5, self.height()-self.y_bin[i]*self.height()/self.y_bin[-1] - 5)
                             ,str(round(self.y_bin[i])))

        painter.setPen(QPen(Qt.black, 4, Qt.SolidLine))
        painter.drawText(self.width()*18/20, self.height() - 5,
                         self.x_labels)
        painter.drawText(3, self.height()*1/20, self.y_labels)

    def draw_lines(self):
        """Visualize plot of data points with simple connected lines and dots"""
        painter = QPainter(self)
        pen = QPen(Qt.red, 3, Qt.SolidLine)

        self.x_window_scale = self.width() / self.x_bin[-1]
        self.y_window_scale = self.height() / self.y_bin[-1]

        k=0
        for i in range(len(self.data.xlist) - 1):
            painter.setPen(pen)
            painter.drawLine(self.data.xlist[i] * self.x_window_scale,
                            self.height() - self.data.ylist[i] * self.y_window_scale,
                            self.data.xlist[i + 1] * self.x_window_scale,
                            self.height() - self.data.ylist[i + 1] * self.y_window_scale)
            painter.setPen(QPen(Qt.black,8))
            painter.drawPoint(self.data.xlist[i] * self.x_window_scale,
                             self.height() - self.data.ylist[i] * self.y_window_scale)
            k+=1

        # Draw last data point separately here for ease of implementation
        painter.drawPoint(self.data.xlist[k] * self.x_window_scale,
                         self.height() - self.data.ylist[k] * self.y_window_scale)


    def draw_grid(self):
        """Generates slightly opaque grey grid lines to the graph"""
        painter = QPainter(self)
        painter.setPen(QPen(Qt.gray,1,Qt.DashLine))
        painter.setOpacity(0.4)

        for i in range(len(self.x_bin)):
            painter.drawLine(self.x_bin[i]*self.width()/self.x_bin[-1], self.height(),
                            self.x_bin[i]*self.width()/self.x_bin[-1], 0)

        for i in range(len(self.y_bin)):
            painter.drawLine(0, self.y_bin[i]*self.height()  / self.y_bin[-1],
                            self.width(), self.y_bin[i]*self.height()  / self.y_bin[-1])


    def draw_legend(self):
        """Generates legend on the side of the graph"""
        painter = QPainter(self)
        pen = QPen(Qt.darkBlue,1,Qt.SolidLine)
        painter.setPen(pen)
        painter.drawText(self.width() - 100, self.height()*1/8, 'Legend:')

        painter.setFont(QFont('Helvetica', 10))
        painter.setBrush(Qt.black)
        painter.drawText(self.width() - 85, self.height()*2/8, self.point)
        painter.drawRect(self.width() - 100, self.height()*2/8 - 10, 10, 10)
        painter.setBrush(Qt.red)
        painter.drawText(self.width() - 85, self.height() * 2.5/8, self.line)
        painter.drawRect(self.width() - 100, self.height()*2.5/8 - 10, 10, 10)


    def build_menu(self):
        """Function for adding menu bar containing nice-to-have features"""
        self.menubar = self.menuBar()
        self.statusbar = self.statusBar()

        #file_action = QAction(QIcon(), '&File', self)
        #file_action.setShortcut('Ctrl+F')
        #file_action.triggered.connect(self.change_file)

        exit_action = QAction(QIcon(), '&Exit', self)
        exit_action.setShortcut('Ctrl+Q')
        exit_action.setStatusTip('Exit application')
        exit_action.triggered.connect(qApp.quit)

        grid_action = QAction(QIcon(), '&Grid', self)
        grid_action.setShortcut('Ctrl+G')
        grid_action.setStatusTip('Toggle grid on/off')
        grid_action.setCheckable(True)
        grid_action.setChecked(True)
        grid_action.triggered.connect(self.toggle_grid)

        #grid_set_action = QAction(QIcon(), '&Grid', self)
        #grid_set_action.triggered.connect(self.set_grid)

        save_action = QAction(QIcon(), '&Save', self)
        save_action.setShortcut('Ctrl+S')
        save_action.triggered.connect(self.save_image)
        save_action.setStatusTip('Save current image')

        hide_action = QAction(QIcon(), '&Hide', self)
        hide_action.setShortcut('Ctrl+H')
        hide_action.triggered.connect(self.toggle_menu)

        label_action = QAction(QIcon(), '&Label', self)
        label_action.setShortcut('Ctrl+L')
        label_action.triggered.connect(self.label_check)

        legend_action = QAction(QIcon(), '&Legend', self)
        legend_action.setShortcut('Ctrl+E')
        legend_action.setCheckable(True)
        legend_action.triggered.connect(self.set_legend)

        legend_swap_action = QAction(QIcon(), '&Legend swap', self)
        legend_swap_action.triggered.connect(self.legend_swap)


        file_menu = self.menubar.addMenu('&File')
        #file_menu.addAction(file_action)            # For changing file without having to restart
        file_menu.addAction(exit_action)
        action_menu = self.menubar.addMenu('&Action')
        action_menu.addAction(grid_action)
        #action_menu.addAction(grid_set_action)
        action_menu.addAction(label_action)
        action_menu.addAction(legend_action)
        action_menu.addAction(legend_swap_action)
        save_menu = self.menubar.addMenu('&Save')
        save_menu.addAction(save_action)
        hide_menu = self.menubar.addMenu('&Hide')
        hide_menu.addAction(hide_action)


    def toggle_grid(self, state):
        """Toggle grid lines visible/not visible"""
        if state:
            self.grid_o = True
        else:
            self.grid_o = False
        self.update()

    def save_image(self):
        """Feature: Snap a picture and save as .png image of the graph window"""
        self.toggle_menu()
        self.filename, __tuplee = QFileDialog.getSaveFileName(self, "Save image", QDir.homePath())
        self.image = QPixmap(self.grab())
        self.filename = self.filename + ".png"
        self.image.save(self.filename)

    def toggle_menu(self):
        """Menu feature: Toggle menu visible/not visible"""
        self.menubar.clear()
        self.update()

    def label_check(self):
        """Menu feature: Change labels"""
        self.x_labels, xlabel_given = QInputDialog.getText(self, 'X-Label', 'Enter X-Label:')
        if not xlabel_given or self.x_labels == "":
            pass
        self.y_labels, ylabel_given = QInputDialog.getText(self, 'Y-Label', 'Enter Y-Label:')
        if not ylabel_given or self.y_labels == "":
            pass
        self.update()

    def set_legend(self, state):
        """Menu feature: setting legends visible/not visible"""
        if state:
            self.legend = False
        else:
            self.legend = True
        self.update()

    def legend_swap(self):
        """Menu feature: swapping legends"""
        self.point, point_legend = QInputDialog.getText(self, 'Legend', 'Enter Legend for Point')
        if not point_legend or self.line == "":
            pass
        self.line, line_legend = QInputDialog.getText(self, 'Legend', 'Enter Legend for Line')
        if not line_legend or self.line == "":
            pass

'''
    def set_grid(self):
        grr, e = QInputDialog.getText(self, 'Grid Resolution', 'Enter a value for grid resolution (default: 5)')
        if grr.isdigit():
            self.grid = grr
        else:
            print("Input not numeric!")
        self.update()

    def change_file(self):
        filename = QFileDialog.getOpenFileName(self, 'Open file', '.txt')
        file = open(filename,'r')
        with file:
            self.initialize_ui(filename)

        #self.menubar.clear()
        #self.initialize_ui()
        self.update()
    
'''

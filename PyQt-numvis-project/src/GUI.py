from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from guireader import *
import math
import os

class GUI(QMainWindow):

    def __init__(self, filename):
        super().__init__()
        self.filename = filename
        self.data = None            # Luetun tiedoston data Graph-oliona myöhemmin

        # Alustetaan myöhemmin esiintyviä muuttujia
        self.gridO = True
        self.legend = True
        self.menu = True
        self.point = "Point"
        self.line = "Line"
        self.setMinimumSize(self.width() / 2, self.height() / 2)

        self.initialize_ui(filename)


    def initialize_ui(self,filename):

        # Tiedoston luku
        self.filename = filename
        reader = GUIreader(self.filename)
        self.data = reader.read_data()     # Palauttaa Graph-olion, joka sisältää listat xlist ja ylist

        # Koordinaatiston rakennus, lisätoiminnot ja muu säätö
        self.grid = 5                    # Kuvaus siitä, kuinka monta jakoväliä luodaan koordinaatistoon
        self.find_axis_scales()
        self.build_menu()
        self.setGeometry(0, 0, 300, 200)

        self.pic = QLabel(self)
        self.pic.setPixmap(QPixmap("C:/Users/Omistaja/Y2-projekti/python2-project/doc/blue-eye.png"))   # Blue eye suddenly
        self.pic.setGeometry(0,-100,1000,1000)


        # Käyttäjän antamat tiedot
        self.title, b = QInputDialog.getText(self, 'Title', 'Enter Title:')
        self.x_labels, c = QInputDialog.getText(self, 'X-Label', 'Enter X-Label:')
        self.y_labels, d = QInputDialog.getText(self, 'Y-Label', 'Enter Y-Label:')

        self.setWindowTitle(self.title)
        self.show()

    def paintEvent(self, event):

        if self.data.graph_type == "plot":
            self.draw_lines()
            self.draw_axis()
            if self.gridO:
                self.draw_grid()

            self.legends = []
            #for graph in self.data.graphs:         # Myöhempiä lisäyksiä varten; tässä luodaan legendit graafin nimen mukaan
            #self.legends.append([graph.name])

            if self.legend:
                self.draw_legend()


    def find_axis_scales(self):

        min_y = 0
        max_y = 0

        for number in self.data.ylist:  # Etsitään suurin ja pienin arvo Graph-olion datasta
            if number < min_y:
                min_y = number
            elif number > max_y:
                max_y = number

        max_y = round(max_y)

        # Sama x-akselille

        min_x = 0
        max_x = 0
        for number in self.data.xlist:
            if number < min_x:
                min_x = number
            elif number > max_x:
                max_x = number

        max_x = round(max_x)

        # Sovitaan skaalaus koordinaatistoon self.grid -muuttujan avulla.
        # jakovälit ovat pienimmästä suurimpaan listoissa x_bin ja y_bin

        self.x_range=(int(math.ceil(max_x/10.0))*10)           # Pyöristetään x- ja y-lukuvälit kymmenen tarkkuudella
        self.y_range=(int(math.ceil(max_y/10.0))*10)

        self.x_bin = []
        self.y_bin = []

        k = 0
        addable = 0
        while addable < self.x_range:
            addable = k * int(math.ceil((self.x_range / self.grid) / 10.0) * 10)
            self.x_bin.append(addable)
            k = k + 1

        k = 0
        addable = 0
        while addable < self.y_range:
            addable = k * int(math.ceil((self.y_range / self.grid) / 10.0) * 10)
            self.y_bin.append(addable)
            k = k + 1

        print("Bins:")
        print(self.x_bin)
        print(self.y_bin)

    def draw_axis(self):
        # Draw axis and labels
        qp = QPainter(self)
        qp.setFont(QFont('Decorative', 10))

        # X-Akseli ja Y-Akseli
        qp.setPen(QPen(Qt.black, 4, Qt.SolidLine))
        qp.drawLine(0, self.height(), 0, 0)
        qp.setPen(QPen(Qt.black, 4, Qt.SolidLine))
        qp.drawLine(0, self.height(), self.width(), self.height())

        # X-akselin jakovälit
        qp.setPen(QPen(Qt.black, 1, Qt.SolidLine))

        for i in range(len(self.x_bin)):
            qp.drawLine(self.x_bin[i]*self.width()/self.x_bin[-1], self.height(), self.x_bin[i]*self.width()/self.x_bin[-1], self.height()*19.3/20)
            qp.drawText(QPoint(self.x_bin[i]*self.width()/self.x_bin[-1] + 5, self.height() - 5), str(round(self.x_bin[i])))

        # Y-akselin jakovälit
        for i in range(len(self.y_bin)):
            qp.drawLine(0, self.y_bin[i]*self.height()/self.y_bin[-1], self.width()/62, self.y_bin[i]*self.height()/self.y_bin[-1])
            qp.drawText(QPoint(5, self.height()-self.y_bin[i]*self.height()/self.y_bin[-1] - 5), str(round(self.y_bin[i])))

        qp.setPen(QPen(Qt.black, 4, Qt.SolidLine))
        qp.drawText(self.width()*18/20, self.height() - 5, self.x_labels)
        qp.drawText(3, self.height()*1/20, self.y_labels)

    def draw_lines(self):
        qpaint = QPainter(self)
        pen = QPen(Qt.red, 3, Qt.SolidLine)

        # Tehdään oletus siitä, että käyttäjä on järjestänyt datapisteet haluamaansa järjestykseen (esim. x-arvon suhteen)

        self.xlabel = self.data.xlist
        self.ylabel = self.data.ylist
        self.x_scale = self.width()/self.x_bin[-1]        # Skaala x-akselille ikkunan suhteen
        self.y_scale = self.height()/self.y_bin[-1]       # Skaala y-akselille ikkunan suhteen
        k=0
        for i in range(len(self.xlabel)-1):
            qpaint.setPen(pen)
            qpaint.drawLine(self.xlabel[i]*self.x_scale, self.height()-self.ylabel[i]*self.y_scale, self.xlabel[i+1]*self.x_scale, self.height()-self.ylabel[i+1]*self.y_scale)
            qpaint.setPen(QPen(Qt.black,8))
            qpaint.drawPoint(self.xlabel[i]*self.x_scale, self.height()-self.ylabel[i]*self.y_scale)
            k+=1

        qpaint.drawPoint(self.xlabel[k]*self.x_scale, self.height()-self.ylabel[k]*self.y_scale)


    def draw_grid(self):

        qpaint = QPainter(self)
        qpaint.setPen(QPen(Qt.gray,1,Qt.DashLine))
        qpaint.setOpacity(0.4)


        for i in range(len(self.x_bin)):
            qpaint.drawLine(self.x_bin[i]*self.width()/self.x_bin[-1], self.height(), self.x_bin[i]*self.width()/self.x_bin[-1], 0)
        for i in range(len(self.y_bin)):
            qpaint.drawLine(0, self.y_bin[i]*self.height()  / self.y_bin[-1], self.width(), self.y_bin[i]*self.height()  / self.y_bin[-1])


    def draw_legend(self):
        qpaint = QPainter(self)
        pen = QPen(Qt.darkBlue,1,Qt.SolidLine)
        qpaint.setPen(pen)
        qpaint.drawText(self.width() - 100, self.height()*1/8, 'Legend:')


        # Myöhempää varten: tähän for-loop, joka käy läpi jokaisen self.legendsiin lisätyn graafityypin, ja lisää sen nimen
        qpaint.setFont(QFont('Helvetica', 10))
        qpaint.setBrush(Qt.black)
        qpaint.drawText(self.width() - 85, self.height()*2/8, self.point)
        qpaint.drawRect(self.width() - 100, self.height()*2/8 - 10, 10, 10)
        qpaint.setBrush(Qt.red)
        qpaint.drawText(self.width() - 85, self.height() * 2.5/8, self.line)
        qpaint.drawRect(self.width() - 100, self.height()*2.5/8 - 10, 10, 10)


    def build_menu(self):

        # Luodaan yksinkertaisia lisäominaisuuksia

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


        fileMenu = self.menubar.addMenu('&File')
        #fileMenu.addAction(file_action)            # Tiedoston vaihtamista varten
        fileMenu.addAction(exit_action)
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
        if state:
            self.gridO = True
        else:
            self.gridO = False
        self.update()

    def save_image(self):
        self.toggle_menu()
        self.filename, __tuplee = QFileDialog.getSaveFileName(self, "Save image", QDir.homePath())
        self.image = QPixmap(self.grab())
        self.filename = self.filename + ".png"
        self.image.save(self.filename)

    def toggle_menu(self):
        self.menubar.clear()
        self.update()

    def label_check(self):
        self.x_labels, c = QInputDialog.getText(self, 'X-Label', 'Enter X-Label:')
        self.y_labels, d = QInputDialog.getText(self, 'Y-Label', 'Enter Y-Label:')
        self.update()

    def set_legend(self, state):
        if state:
            self.legend = False
        else:
            self.legend = True
        self.update()

    def legend_swap(self):
        self.point, f = QInputDialog.getText(self, 'Legend', 'Enter Legend for Point')
        self.line, g = QInputDialog.getText(self, 'Legend', 'Enter Legend for Line')
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
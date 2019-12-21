class GUIreader:

    def __init__(self, path):
        self.filename = path
        self.graph = None

    def read_data(self):
        ''' Reads the first identifier word (#column, #pie or #graph) '''

        # Tänne tallennetaan raakadata (pisteet)
        self.graph = Graph()
        self.graphs = GraphCollection()
        self.graphs.add_graph(self.graph)

        data_file = None

        try:
            data_file = open(self.filename, 'r')
            line = data_file.readline()
            first_line = line.split(";")				# We split the first data line from the text file into the keyword and raw data itself separated by ';

            if first_line[0] == "column":			    # Meitä kiinnostaa vain rivin ensimmäinen sana, joka toimii graafityypin tunnistimena
                self.graph.set_type("column")
                self.graph.set_name("Column")
                self.read_column(data_file, first_line[1])

            elif first_line[0] == "pie":
                self.graph.set_type("pie")
                self.graph.set_name("Pie")
                self.read_pie(data_file, first_line[1])

            elif first_line[0] == "plot":
                self.graph.set_type("plot")
                self.graph.set_name("Plot")
                self.read_plot(data_file, first_line[1])
        except:
            raise OSError("Corrupted file {}.".format(self.filename))

        finally:
            if data_file:
                data_file.close()

        return self.graph

    ''' Luetaan tunnistetusta graafityypistä riippuen data hieman eri tavalla.'''
    def read_column(self, path, first_line):
        pass

    def read_pie(self, path, first_line):
        pass

    def read_plot(self, path, first_line_data):
        xlist = []
        ylist = []

        line = first_line_data

        while line != "":
            line = line.replace(" ", "")
            line = line.replace("\t", "")
            line = line.strip("\n")
            csv_data = line.split(",")

            for i in range(len(csv_data)):
                if i % 2 == 0:
                    xlist.append(csv_data[i])
                if i % 2 == 1:
                    ylist.append(csv_data[i])

            line = path.readline()

            # Luetaan joka toinen arvo y- ja joka toinen x-listaan, ilman whitespacea


            datalist = csv_data				# Kokoaa kaikki arvot
            self.graph.values = datalist

        # Testataan datan numeerisuus
        print("Y-arvot")
        print(ylist)
        print("X-arvot")
        print(xlist)

        flag = 1
        for number in ylist:
            if self.is_number(number):
                self.graph.ylist.append(abs(float(number)))
            else:
                flag = 0
                break
        if flag == 0:
            raise OSError("Data isn't numerical.")

        for number in xlist:
            if self.is_number(number):
                self.graph.xlist.append(abs(float(number)))
            else:
                flag = 0
                break
        if flag == 0:
            raise OSError("Data isn't numerical. Please make sure the datafile doesn't contain lines starting with commas.")

        if len(self.graph.xlist) != len(self.graph.ylist):
            raise OSError("Some data not paired.")


    def is_number(self, number):
        try:
            float(number)
            return True
        except ValueError:
            print("Is not float.")
            return False


class Graph(object):

    def __init__(self):
        self.graph_type = None
        self.name = None
        self.xlist = []
        self.ylist = []
        self.values = []

    def get_type(self):
        return self.graph_type

    def set_type(self, type):
        self.graph_type = type
        print("Current graph type set to %s" %type)

    def get_data(self):
        return self.values

    def set_name(self, name):
        self.name = name


class Point:

    def __init__(self):
        self.x = None
        self.y = None

    def set_x(self,x):
        self.x = x
    def set_y(self,y):
        self.y = y

    # Pie- ja Column grapheja varten
    def set_label(self, label):
        self.x = label


class GraphCollection:

    def __init__(self):
        self.graph_array = []

    def add_graph(self, graph):
        self.graph_array.append(graph)
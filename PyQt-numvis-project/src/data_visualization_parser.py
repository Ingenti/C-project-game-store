"""Module for parsing and structuring data for different kinds of visualizations"""

def is_number(number):
    try:
        float(number)
        return True
    except ValueError:
        print("Is not float.")
        return False

class GUIreader:
    """Class for reading user-prompted .csv file"""
    def __init__(self, path):
        self.filename = path
        self.graph = None
        self.graphs = None

    def read_data(self):
        """ Reads the first identifier word (#column, #pie or #graph) """

        # Raw data collection
        self.graph = Graph()
        self.graphs = GraphCollection()
        self.graphs.add_graph(self.graph)

        # Split the first data line from the text file into the keyword & raw data, separated by ';'
        with open(self.filename, 'r') as data_file:
            try:
                line = data_file.readline()
                first_line = line.split(";")

                # Only interested in the first word that serves as identifier for visualization type
                if first_line[0] == "column":
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
            except Exception as error:
                raise OSError("Corrupted file {}.".format(self.filename)) from error

            finally:
                if data_file:
                    data_file.close()

        return self.graph


    def read_column(self, path, first_line):
        """Read data in a way that is easy to visualize as columns (histogram)"""
        raise NotImplementedError()

    def read_pie(self, path, first_line):
        """Read data in a way that is easy to visualize as pie chart"""
        raise NotImplementedError()

    def read_plot(self, path, first_line_data):
        """Read data in a way that is easy to visualize as plot"""
        xlist = []
        ylist = []

        line = first_line_data

        while line != "":
            line = line.replace(" ", "")
            line = line.replace("\t", "")
            line = line.strip("\n")
            csv_data = line.split(",")

            # Read every other value to y- and every other to x-list
            for i in range(len(csv_data)):
                if i % 2 == 0:
                    xlist.append(csv_data[i])
                else:
                    ylist.append(csv_data[i])

            line = path.readline()

            datalist = csv_data				# Assembles all values
            self.graph.values = datalist

        # Test if data is numerical
        print("Y-values")
        print(ylist)
        print("X-values")
        print(xlist)

        flag = 1
        for number in ylist:
            if is_number(number):
                self.graph.ylist.append(abs(float(number)))
            else:
                flag = 0
                break
        if flag == 0:
            raise OSError("Data isn't numerical.")

        for number in xlist:
            if is_number(number):
                self.graph.xlist.append(abs(float(number)))
            else:
                flag = 0
                break
        if flag == 0:
            raise OSError("Data isn't numerical. "
                          "Please make sure file doesn't contain lines starting with commas.")

        if len(self.graph.xlist) != len(self.graph.ylist):
            raise OSError("Some data not paired.")


class Graph():

    def __init__(self):
        self.graph_type = None
        self.name = None
        self.xlist = []
        self.ylist = []
        self.values = []

    def get_type(self):
        return self.graph_type

    def set_type(self, graph_type):
        self.graph_type = graph_type
        print("Current graph type set to %s" % graph_type)

    def get_data(self):
        return self.values

    def set_name(self, name):
        self.name = name


class Point:

    def __init__(self):
        self.x_point = None
        self.y_point = None

    def set_x(self, x_point):
        self.x_point = x_point
    def set_y(self, y_point):
        self.y_point = y_point

    # Pie- ja Column grapheja varten
    def set_label(self, label):
        self.x_point = label


class GraphCollection:
    """Simple class for saving different graphs"""
    def __init__(self):
        """Consists pretty much only of a list"""
        self.graph_array = []

    def add_graph(self, graph):
        """Add graphs to be stored in a list inside this class"""
        self.graph_array.append(graph)

    def get_graph(self):
        """Get access to array of graphs added to an instance of this class"""
        return self.graph_array

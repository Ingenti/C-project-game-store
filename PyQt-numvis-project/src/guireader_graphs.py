class GUIreader():

    def __init__(self, path):
        self.filename = path

    def read_data(self):
        ''' Reads the first identifier word (#column, #pie or #graph) '''

        # Tänne tallennetaan raakadata (pisteet)
        self.graph = Graph()

        data_file = None

        try:
            data_file = open(self.filename, 'r')			# File read
            line = data_file.readline()
            first_line = line.split(";")				# We split the first data from the text file into the keyword and raw data itself separated by ';

            if first_line[0] == "column":			# We are only interested in the first element, i.e. the keyword
                self.graph.set_type("column")
                self.read_column(data_file, first_line[1])

            elif first_line[0] == "pie":
                self.graph.set_type("pie")
                self.read_pie(data_file, first_line[1])

            elif first_line[0] == "plot":
                self.graph.set_type("plot")
                self.read_plot(data_file, first_line[1])
        except:
            raise OSError("Corrupted file {}.".format(self.filename))

        finally:
            if data_file:
                data_file.close()

        return self.graph

    ''' Same code as before, now we just read the whole file and send the data read to the GUI class.'''
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
            line = line.strip("\n")
            csv_data = line.split(",")

            for i in range(len(csv_data)):
                if i % 2 == 0:
                    xlist.append(csv_data[i])
                if i % 2 == 1:
                    ylist.append(csv_data[i])

            datalist = csv_data				# Combines all data

            # Read every other value of csv_data to ylist and xlist without whitespace
            line = path.readline()

        # Test whether all data is numerical and add points in Graph-list
        print("Y-arvot")
        print(ylist)
        print("X-arvot")
        print(xlist)
        flag = 1

        for number in ylist:
            if self.is_number(number):
                self.graph.ylist.append(float(number))
            else:
                flag = 0
                break
        if flag == 0:
            raise OSError("Data isn't numerical.")

        for number in xlist:
            if self.is_number(number):
                self.graph.xlist.append(float(number))
            else:
                flag = 0
                break
        if flag == 0:
            raise OSError("Data isn't numerical. Please make sure the datafile doesn't contain lines starting with commas.")


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
        self.xlist = []
        self.ylist = []
        self.data = []

    def get_type(self):
        return self.graph_type

    def set_type(self, type):
        self.graph_type = type
        print("Current graph type set to %s" %type)

    def get_data(self):
        return self.data

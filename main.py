import csv
import matplotlib.pyplot as plt
filename = '/home/kevgen/CLionProjects/TestCase/cmake-build-debug/csv_arg'

with open(filename, newline='') as csvfile:
    data = list(csv.reader(csvfile))
print(data)

headers = data[0]
data = data[1:]


for i in range(len(headers)):
    column_data = [float(row[i]) for row in data]
    plt.plot(column_data, label=headers[i])

plt.legend()
plt.xlabel('Номер строки')
plt.ylabel('Значение')
plt.title('График данных из CSV-файла')
plt.savefig('график.png')
plt.show()
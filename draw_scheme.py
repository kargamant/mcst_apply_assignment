import matplotlib.pyplot as plt
import re

directory="unconnected_figures.hkp"
coordinates=[]

with open(directory, "r") as file:
    for line in file:
        result=re.search("(-[0-9]+, [0-9]+, (0|30))", line)
        if result!=None:
            coordinates.append(result.group(1))
print(len(coordinates))

dots=[]
for i in range(len(coordinates)):
    new_coordinates=coordinates[i].split(", ")[0:2]
    new_coordinates=list(map(int, new_coordinates))
    print(new_coordinates)
    dots.append(new_coordinates)
plt.plot([dots[i][0] for i in range(7)], [dots[i][1] for i in range(7)], 'r+')
plt.plot([dots[i][0] for i in range(7, len(dots))], [dots[i][1] for i in range(7, len(dots))], 'b+')
#plt.plot([i[0] for i in dots], [i[1] for i in dots], 'r+')
plt.show()

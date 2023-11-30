import numpy as np
import matplotlib.pyplot as plt

def centerPoints():
    matrix = np.random.randint(0, 1999, (6, 3))
    return matrix

def readData(fileName):
    data = np.loadtxt(fileName)
    return data

def print3D(data):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(data[:,0], data[:,1], data[:,2], c='r')
    plt.show() 

def num_rows(fileName):
    with open(fileName, 'r') as file:
        num_rows = sum(1 for line in file)
    return num_rows

def calculateDistance(p1,p2):
    return np.sqrt(np.sum((p1-p2)**2))

def teachingRound():
    return 5

def datansiirto(points,fileName):
    print("tallennettava data .h tiedostoon:\n",points)
    np.savetxt(fileName,points,fmt="%d")
    with open(fileName,'r') as file:
        content = file.read().replace(' ', ',')

    lines = content.split('\n')
    lines = ['{' + line + '}' for line in lines if line]
    content = ',\n'.join(lines)
 
    content = content.replace('[', '{').replace(']', '}')
    with open("keskipisteet.h",'w') as output_file:
        output_file.write(f"int center_points[6][3] = {{\n{content}\n}};\n")
    
if (__name__ == "__main__"):
    print(calculateDistance(np.array([1,1,1]),np.array([2,2,2])))
    print(calculateDistance(readData("vastaanotettu_data.txt")[0,0],centerPoints()[0,0]))
    print(readData("vastaanotettu_data.txt"))
    centerPoints()
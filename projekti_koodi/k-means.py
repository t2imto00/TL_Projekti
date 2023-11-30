import utilities as ut
import numpy as np

fileName = "vastaanotettu_data.txt"
center_points = ut.centerPoints()
print("alkupisteet: \n",center_points)
data = ut.readData(fileName)
distances = np.zeros((6,1))
centerPointCumulativeSum = np.zeros((6,3))
count = np.zeros((6,1))

for i in range(ut.teachingRound()):
    
    for i in range(len(data)):
        dataPoints = data[i,:]

        for j in range(len(center_points)):
            distances[j,0] = ut.calculateDistance(dataPoints,center_points[j,:])

        winner = np.argmin(distances)
        centerPointCumulativeSum[winner,0:3] += dataPoints
        count[winner,0] += 1
        
        for i in range(6):
            if count[i,0] == 0:
                center_points[i,0:3] = np.random.randint(1200,1999,3)
            else:
                center_points[i,0:3] = centerPointCumulativeSum[i,0:3]/count[i,0]

        print("count:\n", count)
ut.print3D(data)
ut.print3D(center_points)

ut.datansiirto(center_points,"center_points.txt")
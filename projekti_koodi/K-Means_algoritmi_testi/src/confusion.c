#include <zephyr/kernel.h>
#include <math.h>
#include "confusion.h"
#include "adc.h"
#include "keskipisteet.h"

int CP[6][3]={
	               {1,0,0},
						{2,0,0},
						{0,1,0},
						{0,2,0},
						{0,0,1},
						{0,0,2}
};

int measurements[6][3]={
	               {1,0,0},
						{2,0,0},
						{0,1,0},
						{0,2,0},
						{0,0,1},
						{0,0,2}
};

int CM[6][6]= {0};


void printConfusionMatrix(void)
{
	printk("Confusion matrix = \n");
	printk("   cp1 cp2 cp3 cp4 cp5 cp6\n");
	for(int i = 0;i<6;i++)
	{
		printk("cp%d %d   %d   %d   %d   %d   %d\n",i+1,CM[i][0],CM[i][1],CM[i][2],CM[i][3],CM[i][4],CM[i][5]);
	}
}

void makeHundredFakeClassifications(void)
{
   for (int iteration = 0; iteration < 10; iteration++) {
      for(int i = 0; i < 6; i++) {
         double minDistance = INFINITY;
         int closestCenterPointIndex = -1;
         for(int j = 0; j < 6; j++) {
               double distance = sqrt(pow((measurements[i][0] - CP[j][0]), 2) +
                                    pow((measurements[i][1] - CP[j][1]), 2) +
                                    pow((measurements[i][2] - CP[j][2]), 2));
               if (distance < minDistance) {
                  minDistance = distance;
                  closestCenterPointIndex = j;
               }
         }
         printk("Measurement %d is closest to Center Point %d\n", i+1, closestCenterPointIndex+1);

         CM[i][closestCenterPointIndex]++;
      }
   }
}

void makeOneClassificationAndUpdateConfusionMatrix(int direction,int winner)
{
   CM[direction][winner]++;
}

int calculateDistanceToAllCentrePointsAndSelectWinner(int x,int y,int z)
{
   int winner = -1;
   double minDistance = INFINITY;
   for (int j = 0; j < 6; j++) {
      int distance = (pow((x - center_points[j][0]), 2) +
                        pow((y - center_points[j][1]), 2) +
                        pow((z - center_points[j][2]), 2));
      printk("Distance to center point %d is %d\n", j + 1, distance);
      if (distance < minDistance) {
         minDistance = distance;
         winner = j;
      }
   }
   printk("Voittaja = %d\n",winner);
   return winner;
}

void resetConfusionMatrix(void)
{
	for(int i=0;i<6;i++)
	{ 
		for(int j = 0;j<6;j++)
		{
			CM[i][j]=0;
		}
	}
}


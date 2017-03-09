//
//  main.c
//  greedy TSP
//
//  Created by Chen Xi on 15-4-23.
//  Copyright (c) 2015年 Chen Xi. All rights reserved.
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define cityNum 48  // Number of cities

struct CityPosition
{
    int index;
    int latitude;
    int longtitude;
}position[cityNum];

int column[cityNum];// If the column is visited, set to 0
int row[cityNum];// If the row is visited, set to 0
int distance[cityNum][cityNum];
int totalDistance[cityNum];


void TxTsp(){
  
    double rij;
    int i;
    int j;
    int tij;
    
    // Read data from att48.tsp
    FILE *fp = fopen("/Users/chenxi/Documents/att48.tsp","r");
        for(i = 0; i< cityNum; i++)
        {
            fscanf(fp,"%d",&(position[i].index));
            fscanf(fp,"%d",&(position[i].latitude));
            fscanf(fp,"%d",&(position[i].longtitude));

        }
   
    // The the distance arraylist holds Pseudo-Eucliean distances between cities
    for(i = 0; i< cityNum-1;i++){
        distance[i][i]=0;
    
    for(j = i+1; j< cityNum;j++){
        
        double temp = (position[i].latitude - position[j].latitude)*(position[i].latitude - position[j].latitude) +
        (position[i].longtitude - position[j].longtitude)*(position[i].longtitude - position[j].longtitude);
       rij = sqrt(temp/10.0);
        
    
    // Rounded to the nearest integer
    tij = (int)(round(rij));
    
    // to correct characteristic roots
    if (tij < rij) {
        distance[i][j] = tij + 1;
        distance[j][i] = distance[i][j];
    }
    else {
        distance[i][j] = tij;
        distance[j][i] = distance[i][j];
    }
    }
}
    distance[cityNum - 1][cityNum - 1] = 0;
}


// Nearest-Neighbor Algorithm (NNA)
void solve(a){
    
    column[0] = 0;
    for (int m = 1; m < cityNum; m++) {
        column[m] = 1;
    }
    
    for (int m = 0; m< cityNum; m++) {
        row[m] = 1;
    }
    
    int temp[cityNum];
    int s=0;// Calculate the distance
    int i=0;// Present node
    int j=0;// Next node
    // Initialize these variables from 0
    
        while(row[i]==1){
        // Copy one row
        for (int k = 0; k < cityNum; k++) {
            temp[k] = distance[i][k];
        }
        
        // Choose the next node that is unvisited and not i
        j = selectmin(temp);
        // Find the next node
        row[i] = 0;// Set to 0 if visited
        column[j] = 0;// Set to o if visited
       
    // Output the path
       if(j+a<=cityNum)
        printf( "%d-->",j+a+1);
            else
                printf("%d-->",j-cityNum+a+1);
       
        s = s + distance[i][j];
        i = j;// Mark this node as visited
        }
    
    printf("\nThe total distance :%d\n\n", s);
    
    //An array representing all tour found
    totalDistance[a]=s;
    
}


  // Find the minimum value in a row of the distance arraylist
int selectmin(int p[]){
    int j = 0, m = p[0], k = 0;
    // Find the first unvisited node
    while (column[j] == 0) {
        j++;
        if(j>=cityNum){
            //No unvisited node, ends. The last node is the starting node
            m = p[0];
            break;
            // or return 0;
        }
        else{
            m = p[j];
        }
    }
    // Find the minimun distance from the node j
    for (; j < cityNum; j++) {
        if (column[j] == 1) {
            if (m >= p[j]) {
                m = p[j];
                k = j;
            }
        }
    }
    
    return k;
}

void swap(){
    // Swap the rows of the distance arraylist
    int i,j,t;
    
    for(j=0;j<cityNum-1;j++){
        for(i=0;i<cityNum;i++){
            t=distance[j][i];
            distance[j][i]= distance[j+1][i];
            distance[j+1][i]=t;
        }
    }
    // Swap the column of the distance arraylists
    for(j=0;j<cityNum-1;j++){
        for(i=0;i<cityNum;i++){
            t=distance[i][j];
            distance[i][j]= distance[i][j+1];
            distance[i][j+1]=t;
        }
    }
}


// Find the best solution and the average solution
void optimal(int dis[]){
    int min=dis[0],i,a=0;
    int average, sumdistance;
    for(i=1;i<cityNum;i++){
        sumdistance+=dis[i];
        if(min>=dis[i]){
            a=i;
            min=dis[i];
        }
    }
    average=sumdistance/cityNum;
    printf("Path %d is the optimal solution: %d\n",a+1, min);
    printf("The average solution is %d.\n", average);
}


int main(){
    int i;
    printf("Start...\n\n");
    TxTsp();
    
    for(i=0;i<cityNum;i++){
        printf("Path %d: ", i+1);
        solve(i);
        swap();
    }
    
    optimal(totalDistance);
    return 0;
}

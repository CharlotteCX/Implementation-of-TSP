#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define cities 48  
#define MAXX 500 //genetation number
#define pc 0.8    
#define pm 0.05   
#define num 200 //number of chromosome 
int bestsolution;	
int distance[cities][cities];	
double SpicesGeneMatrixCopy[num][cities]; 

struct group  //the structure of the chromosome
{
	int city[cities];//the subsequence of the cities
	int pathLength;//length of the solution
	double p;//the survival possibility in the population
}group[num],grouptemp[num];

struct CityPosition
{
	int index;
	int latitude;
	int longtitude;
}position[cities];

//calculate the distances of each pair of cities
void init()
{
	int i,j;
	FILE *fp = fopen("./att48.tsp","r");
	for(i = 0; i< cities;i++)
	{
		fscanf(fp,"%d",&(position[i].index));
		fscanf(fp,"%d",&(position[i].latitude));
		fscanf(fp,"%d",&(position[i].longtitude));
	}

	for(i = 0; i< cities;i++)
		for(j = 0; j< cities;j++)
		{
			double temp = (position[i].latitude - position[j].latitude)*(position[i].latitude - position[j].latitude) +
				      (position[i].longtitude - position[j].longtitude)*(position[i].longtitude - position[j].longtitude);
			double tempdistance = sqrt(temp/10.0);
			int tempdistance_int = tempdistance;
			if(tempdistance_int < tempdistance) distance[i][j] = tempdistance_int + 1;
			else distance[i][j] = tempdistance_int;
		}
}

///output the optimal solution ever
void printGroundTruth()
{
	int i,gtLength = 0;
	FILE *fp = fopen("GT","r");
	int gt[cities];
	for(i = 0; i< cities;i++)
		fscanf(fp,"%d",&(gt[i]));
	for(i = 1;i<cities;i++)
		gtLength += distance[gt[i - 1] - 1][gt[i] - 1];
	printf("GroundTruth of path:%6d\n",gtLength);

}

//randomly generate the original population
void groupproduce()
{
	int i,j,t,k,flag;
	for(i=0;i<num;i++)  //initialization
	for(j=0;j<cities;j++)
	group[i].city[j]=-1;
	srand((unsigned)time(NULL));
    for(i=0;i<num;i++)
	{
		//generate 10 different numbers
		for(j=0;j<cities;)
		{
			t=rand()%cities;
			flag=1;
			for(k=0;k<j;k++)
			{
				if(group[i].city[k]==t)
				{
					flag=0;
					break;
				}
			}
			if(flag)
			{
				group[i].city[j]=t;
				j++;
			}
		}
	}
}
//evaluate the function and find the optimal one
void pingjia()
{
	int i,j;
	int n1,n2;
	int sumdistance,biggestsum=0;
	double biggestp=0;


	///sum the length of the chromosomes
	for(i=0;i<num;i++)
	{
		sumdistance=0;
		for(j=1;j<cities;j++)
		{
			n1=group[i].city[j-1];
			n2=group[i].city[j];
			sumdistance+=distance[n1][n2];
		}
		group[i].pathLength=sumdistance; //每条染色体的路径总和
		biggestsum = (sumdistance > biggestsum)?sumdistance:biggestsum; ///找到最大的路径之和的长度
	}


	//calculate the survival capacity of the chromosome
	for(i=0;i<num;i++)
	{
		group[i].p=  (double)(biggestsum - group[i].pathLength + 1);
		biggestp += group[i].p;
	}

	for(i=0;i<num;i++)
	group[i].p=group[i].p/biggestp;  //sum of the survival possibility should be 1


	//find the best one
	bestsolution=0;
	for(i=0;i<num;i++)
		if(group[i].p>group[bestsolution].p)
			bestsolution=i;

	///Mid-process Reslult
	for(i = 0; i< num;i++)
		printf("The path Length of group[%d] is %d and pro is %6f \n",i,group[i].pathLength,group[i].p);
	printf("The shortest path Length of the group is group[%d] is %6d \n\n\n",bestsolution,group[bestsolution].pathLength);
}


// selection using the route wheel selection algorithm
void xuanze()
{
	////backup all the gene in this generation
	int i,j;
	for(i = 0; i< num;i++)
		for(j = 0; j< cities;j++)
			SpicesGeneMatrixCopy[i][j] = group[i].city[j];
	
	double tin_upperBound[num];
	double pre = 0;
	for(i = 0; i< num;i++)
	{
		tin_upperBound[i] = group[i].p + pre;
		pre =  tin_upperBound[i];
	}
	
	srand((unsigned)time(NULL));
	for(i = 0; i< num;i++) ///
	{
		int RandomValue_Int = rand()%10000;
		double RandomValue = RandomValue_Int;
		RandomValue = RandomValue/10000;

		int foundIndex;
		for(foundIndex = 0; foundIndex< num;foundIndex++)
			if(RandomValue < tin_upperBound[foundIndex])
				break;	

		for(j = 0; j< cities;j++)
			group[i].city[j] = SpicesGeneMatrixCopy[foundIndex][j];
	}
   
}

//crossover, generate the crossover possibility of every parents
void  jiaopei()  
{  
    int i,j,k,kk;  
    int t;//the number of chromosome in the crossover 
    int point1,point2,temp;//crossover point  
    int pointnum;  
    int temp1,temp2;  
    int map1[cities],map2[cities];  
    double jiaopeip[num];//the Pc 
    int jiaopeiflag[num];//the availability of the crossover.  
    int kkk,flag=0;  
    //initialization  
    for(i=0;i<num;i++)  
    {  
        jiaopeiflag[i]=0;  
    }  
    //generate Pc randomly  
    srand((unsigned)time(NULL));  
    for(i=0;i<num;i++)  
    {  
        jiaopeip[i]=(rand()%100);  
        jiaopeip[i]/=100;  
    }  
    //ensure the chromosome to crossover  
    t=0;  
    for(i=0;i<num;i++)  
    {  
        if(jiaopeip[i]<pc)  
        {  
            jiaopeiflag[i]=1;  
            t++;  
        }  
    }  
    t=t/2*2;//tmust be even  
    //产生t/2 of 0-9 crossover point 
    srand((unsigned)time(NULL));  
    temp1=0;  
    //temp1 crossover with temp2 
    for(i=0;i<t/2;i++)  
    {  
        point1=rand()%cities;//point 1  
        point2=rand()%cities;//point 2  
        //select one chromosome to crossover 
        for(j=temp1;j<num;j++)  
        {  
            if(jiaopeiflag[j]==1)  
            {  
                temp1=j;  
                break;  
            }  
        }  
        //select another one to crossover with the previous one  
        for(j=temp1+1;j<num;j++)  
        {  
            if(jiaopeiflag[j]==1)  
            {  
                temp2=j;  
                break;  
            }  
        }  
        //operate the crossover  
        if(point1>point2) //保证point1<=point2  
        {  
            temp=point1;  
            point1=point2;  
            point2=temp;  
        }  
        //initialization  
        memset(map1,-1,sizeof(map1));  
        memset(map2,-1,sizeof(map2));  
        //the mapping relation between gene points 
        for(k=point1;k<=point2;k++)  
        {  
            map1[group[temp1].city[k]]=group[temp2].city[k];  
            map2[group[temp2].city[k]]=group[temp1].city[k];  
        }  
        //exchange the gene on two sides of the point 
        for(k=0;k<point1;k++)  
        {  
            temp=group[temp1].city[k];  
            group[temp1].city[k]=group[temp2].city[k];  
            group[temp2].city[k]=temp;  
        }  
        for(k=point2+1;k<cities;k++)  
        {  
            temp=group[temp1].city[k];  
            group[temp1].city[k]=group[temp2].city[k];  
            group[temp2].city[k]=temp;  
        }   
        //deal with the crossover confliction of chromosome 1  
        for(k=0;k<point1;k++)  
        {  
            for(kk=point1;kk<=point2;kk++)  
            {  
                if(group[temp1].city[k]==group[temp1].city[kk])  
                {  
                    group[temp1].city[k]=map1[group[temp1].city[k]];  
                    //find  
                    for(kkk=point1;kkk<=point2;kkk++)  
                    {  
                        if(group[temp1].city[k]==group[temp1].city[kkk])  
                        {  
                            flag=1;  
                            break;  
                        }  
                    }  
                    if(flag==1)  
                    {  
                        kk=point1-1;  
                        flag=0;  
                    }  
                    else  
                    {  
                        flag=0;  
                        break;  
                    }  
                }  
            }  
              
        }  
        for(k=point2+1;k<cities;k++)  
        {  
            for(kk=point1;kk<=point2;kk++)  
            {  
                if(group[temp1].city[k]==group[temp1].city[kk])  
                {  
                    group[temp1].city[k]=map1[group[temp1].city[k]];  
                    //find  
                    for(kkk=point1;kkk<=point2;kkk++)  
                    {  
                        if(group[temp1].city[k]==group[temp1].city[kkk])  
                        {  
                            flag=1;  
                            break;  
                        }  
                    }  
                    if(flag==1)  
                    {  
                        kk=point1-1;  
                        flag=0;  
                    }  
                    else  
                    {  
                        flag=0;  
                        break;  
                    }  
                }  
            }  
        }  
        //deal with the gene conflict of chromosome 2  
        for(k=0;k<point1;k++)  
        {  
            for(kk=point1;kk<=point2;kk++)  
            {  
                if(group[temp2].city[k]==group[temp2].city[kk])  
                {  
                    group[temp2].city[k]=map2[group[temp2].city[k]];  
                    //find  
                    for(kkk=point1;kkk<=point2;kkk++)  
                    {  
                        if(group[temp2].city[k]==group[temp2].city[kkk])  
                        {  
                            flag=1;  
                            break;  
                        }  
                    }  
                    if(flag==1)  
                    {  
                        kk=point1-1;  
                        flag=0;  
                    }  
                    else  
                    {  
                        flag=0;  
                        break;  
                    }  
                }  
            }  
        }  
        for(k=point2+1;k<cities;k++)  
        {  
            for(kk=point1;kk<=point2;kk++)  
            {  
                if(group[temp2].city[k]==group[temp2].city[kk])  
                {  
                    group[temp2].city[k]=map2[group[temp2].city[k]];  
                    //find  
                    for(kkk=point1;kkk<=point2;kkk++)  
                    {  
                        if(group[temp2].city[k]==group[temp2].city[kkk])  
                        {  
                            flag=1;  
                            break;  
                        }  
                    }  
                    if(flag==1)  
                    {  
                        kk=point1-1;  
                        flag=0;  
                    }  
                    else  
                    {  
                        flag=0;  
                        break;  
                    }  
                }  
            }  
        }  
        temp1=temp2+1;  
    }  
}
//Mutation
void bianyi()
{
	int i,j;
	int t;
	int temp1,temp2,point;
	double bianyip[num]; //Pm
	int bianyiflag[num];
	for(i=0;i<num;i++)//initialization
	bianyiflag[i]=0;
	//randomly generate the Pm
	srand((unsigned)time(NULL));
	for(i=0;i<num;i++)
	{
		bianyip[i]=(rand()%100);
		bianyip[i]/=100;
	}
	//ensure the chromosome that is available to mutate
	t=0;
	for(i=0;i<num;i++)
	{
		if(bianyip[i]<pm)
		{
			bianyiflag[i]=1;
			t++;
		}
	}
	//mutaion operation
	srand((unsigned)time(NULL));
	for(i=0;i<num;i++)
	{
		if(bianyiflag[i]==1)
		{
	        temp1=rand()%10;
			temp2=rand()%10;
			point=group[i].city[temp1];
            group[i].city[temp1]=group[i].city[temp2];
			group[i].city[temp2]=point;
		}
	}
}

int main()
{
	int i,j,t;
 	init();
	groupproduce();
	//evaluation of the original population
	printf("The initiali result:\n");
	pingjia();
	t=0;
    while(t++<MAXX)
	{
	         xuanze();
		 jiaopei();
		 bianyi();
		 printf("The result of Circle %4d :\n",t);
		 pingjia();
	}
	
	printf("The result Path Can be seen below:\n");
    	for(j=0;j<cities;j++)
	{
			printf("%4d",group[bestsolution].city[j] + 1);
			if(j%10 == 9) printf("\n");
	}
	printf("\n");
	printf("Sum of path:%6d\n",group[bestsolution].pathLength);
	printGroundTruth();
	return 0;
}


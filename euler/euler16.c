#include<stdio.h>
#define N 4
int n[N][N]=
{
{3},
{7,4},
{2,4,6},
{8,5,9,3}
};
int main()
{
	int i,j;
	int max=0;
	for(i=1;i<N;i++)
	{
		printf("line %d:",i);
		for(j=0;j<=i;j++) 
			printf("%d ",n[i][j]);
		printf("\n");
	}
	for(i=1;i<N;i++)
	{
		n[i][0]+=n[i-1][0];
		n[i][i]+=n[i-1][i-1];
		for(j=1;j<i;j++) 
			n[i][j]+=(n[i-1][j-1]>n[i-1][j]?n[i-1][j-1]:n[i-1][j]);
	}


	for(i=1;i<N;i++)
	{
		printf("line %d:",i);
		for(j=0;j<=i;j++) 
			printf("%d ",n[i][j]);
		printf("\n");
	}

	return 0;
}

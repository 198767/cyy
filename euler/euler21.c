#include<stdio.h>
#define N 10000
int n[N+1];
int main()
{
	int i,j;
	int sum=0;
	for(i=1;i<=N;i++)
	{
		j=i*2;
		while(j<=N)
		{
			n[j]+=i;
			j+=i;
		}
	}

	


	for(i=1;i<N;i++)
	{
		j=n[i];
		if(j<=N&&n[j]==i && j>i)
			sum+=j+i;
	}
	printf("%d",sum);
	return 0;
}

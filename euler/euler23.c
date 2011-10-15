#include<stdio.h>
#define N 28123
int n[N+1];
int m[N+1];
int a[N+1];
int main()
{
	int i,j,num=0;
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

	for(i=12;i<=N;i++)
	{
		if(n[i]>i)
			m[num++]=i;
	}


	for(i=0;i<num;i++)
	{
		for(j=i;j<num;j++)
		{
			if(m[i]+m[j]<=N)
				a[m[i]+m[j]]=1;
			else 
				break;
		}
	}




	for(i=1;i<=N;i++)
	{
		if(!a[i])
			sum+=i;
	}
	printf("%d",sum);


}

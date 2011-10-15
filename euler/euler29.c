#include<stdio.h>
#define N 100
void insert(int *n,int a);
int main()
{
	int a[N+1][N*N];
	int i,j,k,l;
	int sum=0;
	for(i=2;i<=N;i++)
	{
		a[i][0]=i;
		a[i][1]=N;
		for(j=2;j<=N;j++)
			a[i][j]=j;
	}
	for(i=2;i<=N;i++)
	{
		if(a[i][0] !=i)
			continue;
		for(j=i*i,k=2;j<=N;j*=i,k++)
		{
			for(l=2;l<=N;l++)
				insert(a[i],l*k);
			a[j][0]=i;
		}
	}



	for(i=2;i<=N;i++)
	{
		if(a[i][0] !=i)
			continue;
		sum+=a[i][1]-1;
	}

	printf("%d",sum);

	return 0;
}
void insert(int *n,int a)
{
	int i,j;

	if(a>n[n[1]])
	{
		n[1]++;
		n[n[1]]=a;
		return;
	}

	i=2;
	while(i<=n[1])
	{
		if(a>n[i])
			i++;
		else if(a==n[i])
			return;
		else
		{
			for(j=n[1];j>=i;j--)
				n[j+1]=n[j];
			n[i]=a;
			n[1]++;
			return;
		}
	}
}

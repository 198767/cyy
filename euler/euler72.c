#include<stdio.h>
#define N 1000000
long long eu[N+1][2];
int main()
{
	long long i,j,k;
	long long res=0;
	for(i=2;i<=N;i++)
	{
		if(eu[i][0]==0) //ÖÊÊý
		{
			eu[i][1]=i-1;
			j=2*i;
			k=i*i;
			while(j<=N)
			{
				eu[j][0]=1;
				if(j==k)
				{
					eu[j][1]=j-j/i;
					k*=i;
				}
				else
				{
					if(eu[j][1]==0)
						eu[j][1]=(j*(i-1))/i;
					else
						eu[j][1]=(eu[j][1]*(i-1))/i;
				}
				j+=i;
			}
		}
	}
	for(i=2;i<=N;i++)
		res+=eu[i][1];
	printf("%lld",res);
	return 0;

}

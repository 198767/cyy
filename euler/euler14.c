#include<stdio.h>
#define N 1000000
long long int n[N+1];
long long int sequence(long long int n);
long long int main()
{
	long long int i;
	long long int max=0;
	long long int maxi=1;
	for(i=1;i<=N;i++)
	{
		n[i]=sequence(i);
		if(n[i]>max)
		{
			max=n[i];
			maxi=i;
		}
	}
	printf("%lld",maxi);
	return 0;
}
long long int sequence(long long int a)
{
	long long int i=1;
	while(a !=1)
	{
		if(a%2==0)
			a/=2;
		else
			a=a*3+1;
		if(a<=N&&n[a]!=0)
			return i+n[a];
		i++;
	}
	return i;
}

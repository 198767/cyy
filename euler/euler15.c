#include<stdio.h>
#define N 20
long long int route(long long int m,long long int n);
int main()
{
	printf("%lld",route(N,N));
	return 0;
}

long long int route(long long int m,long long int n)
{
	long long int i;
	long long int sum=0;
	long long int a,b;
	a=m<n?m:n;
	b=m+n-a;
	if(a==0)
		return 1;
	else if(a==1)
		return b+1;
	else if(a==2)
		return (b+1)*(b+2)/2;
	else
	{
		for(i=0;i<=b;i++)
			sum+=(route(a/2-1,i))*route(a-a/2,b-i);
		return sum;
	}
}

#include<stdio.h>
#define N 500
int count (int  n);
int main()
{
	int i=1,sum=1;
	while(1)
	{
		i++;
		sum+=i;
		if(sum>=N*2&&count(sum)>N)
		{
			printf("%d",sum);
			break;
		}
	}


}
int count (int  n)
{
	int i=2,m=2;
	int len=n;
	while(i<len)
	{
		if(n%i==0)
		{
			len=n/i;
			m+=2;
		}
		else
		{
			if(m+(len-i-2)/2<=N)
				return -1;
		}
		i++;

	}
	if(i*i==n)
		m--;
	return m;
}

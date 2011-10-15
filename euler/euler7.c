#include<stdio.h>
#include<stdlib.h>

#define N  99999999
long long numbers[N+1];
long long primes[9999999];
int main()
{
	long long i;
	long long j;
	long long k;
	long long l;
	long long d1;
	long long *p=primes;
	primes[0]=4;
	primes[1]=2;
	primes[2]=3;
	primes[3]=5;
	primes[4]=7;
	
	for(i=9;i<N;i+=6)
		numbers[i]=1;
	for(i=21;i<N;i+=14)
		numbers[i]=1;
	i=11;
	while(i<N)
	{
		j=i; //1
		if(numbers[j]==0)
		{
			primes[++*p]=j;
			k=j*j;
			if(j%3==1)
				d1=4;
			else
				d1=2;
			while(k<N)
			{
				numbers[k]=1;
				k+=d1*j;
				if(d1==2)
					d1=4;
				else
					d1=2;
			}	
		}
		j=i+2; //3
		if(numbers[j]==0)
		{
			primes[++*p]=j;
			k=j*j;
			if(j%3==1)
				d1=4;
			else
				d1=2;
			while(k<N)
			{
				numbers[k]=1;
				k+=d1*j;
				if(d1==2)
					d1=4;
				else
					d1=2;
			}	
		}

		j=i+6; //7
		if(numbers[j]==0)
		{
			primes[++*p]=j;
			k=j*j;
			if(j%3==1)
				d1=4;
			else
				d1=2;
			while(k<N)
			{
				numbers[k]=1;
				k+=d1*j;
				if(d1==2)
					d1=4;
				else
					d1=2;
			}	
		}
		j=i+8; //9
		if(numbers[j]==0)
		{
			primes[++*p]=j;
			k=j*j;
			if(j%3==1)
				d1=4;
			else
				d1=2;
			while(k<N)
			{
				numbers[k]=1;
				k+=d1*j;
				if(d1==2)
					d1=4;
				else
					d1=2;
			}	
		}
		i+=10;
	}
	
	printf("%lld",primes[10001]);
}

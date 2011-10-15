#include<stdio.h>
#include<stdlib.h>

#define N 1000000
long long numbers[N+1];
long long is_circular(long long n);
long long num;
int main()
{
	long long i;
	long long j;
	
	for(i=4;i<N;i+=2)
		numbers[i]=1;
	for(i=9;i<N;i+=6)
		numbers[i]=1;
	for(i=15;i<N;i+=10)
		numbers[i]=1;
	for(i=21;i<N;i+=14)
		numbers[i]=1;
	i=11;
	while(i<N)
	{
		if(numbers[i]==0)
		{
			j=i*i; //1
			while(j<N)
			{
				numbers[j]=1;
				j+=i;
			}	
		}
		i++;
	}
	for(i=2;i<N;i++)
	{
		if(!numbers[i])
		 	is_circular(i);
	}
			printf("\n%lld ",num);
}
long long is_circular(long long n)
{
	int digit[150];
	int temp;
	int digitnum=0;
	int i=0;
	long long a=n;
	const long long b=n;
	int flag=1;
	long long num2=num;
	while(n)
	{
		digit[i]=n%10;
		digitnum++;
		i++;
		n/=10;
	}

	for(i=1;i<digitnum;i++)
	{
		if(!((digit[i])&1))
			return 0;
		if(digit[i]==5)
			return 0;
	}
	
	flag=1;
	while(1)
	{
		temp=digit[0];
		for(i=0;i<digitnum-1;i++)
		{
			digit[i]=digit[i+1];
		}
		digit[i]=temp;
		
		a=0;
		for(i=digitnum-1;i>=0;i--)
		{
			a=a*10+digit[i];
		}
		
		num++;
		if(a==b)
			break;
		if(numbers[a]==1)
			flag=0;
		else
			numbers[a]=1;
	}
	
	if(flag==0)
	{
		num=num2;
	}

	return flag;
}

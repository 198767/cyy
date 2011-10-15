#include<stdio.h>
#define N 5
int get_digit_exp(int n);
int myexp(int base,int power);
int main()
{
	int i,j=2,n=1;
	int sum=0;
	n=myexp(9,N);
	while(myexp(10,j-1) < j*n)
		j++;

	j=myexp(10,j);
	for(i=2;i<j;i++)
	{
		if(i==get_digit_exp(i))
		{
			sum+=i;
		}
	}
	printf("%d",sum);
	return 0;
	
}

int myexp(int base,int power)
{
	int n=1;
	int i;
	for(i=0;i<power;i++)
	{
		n*=base;
	}
	return n;
}
int get_digit_exp(int n)
{
	int sum=0;
	int digit;
	while(n)
	{
		digit=n%10;
		n/=10;
		sum+=myexp(digit,N);
	}
	return sum;
}

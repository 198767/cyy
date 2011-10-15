#include<stdio.h>
#define N 1000000
int is_palindromic(int n,int base);
int main()
{
	int i,sum=0;
	for(i=1;i<N;i+=2)
	{
		if(is_palindromic(i,10) && is_palindromic(i,2))
			sum+=i;
	}
	printf("%d ",sum);
}

int is_palindromic(int n,int base)
{
	int *p,*q;
	int a[30];
	p=a;
	q=a;
	while(n)
	{
		*p=n%base;
		p++;
		n/=base;
	}
	p--;
	while(q<p)
	{	
		if(*q != *p)
			return 0;
		p--;
		q++;
	}
	return 1;
}

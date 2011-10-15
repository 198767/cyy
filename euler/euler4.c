#include<stdio.h>

int start=100;
int end=999;
int radindex=100000;
struct 
{
	int i;
	int j;
	int ij;
}palindrome;
int is_palindromic(int n,int base);
int main()
{

	int i,j,k;
	int start2=start;
	int flag=9;
	for(i=end;i>=start;i--)
	{
		if(flag==0)
		{
			flag=9;
			continue;
		}
		flag--;
		if(i<start2)
			break;
		for(j=i;j>=start2;j--)
		{
			k=i*j;
			if(is_palindromic(k,10))
			{
				if(k>palindrome.ij)
				{
					palindrome.i=i;
					palindrome.j=j;
					palindrome.ij=k;
					start2=j;
				}
				break;
			}
		}
	}
	printf("%d",palindrome.ij);
	return -1;
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


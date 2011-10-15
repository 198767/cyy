#include<stdio.h>
#define N  29999999
long long int square[N+1];
long long int  is_square(long long int  n);
int main()
{
	long long int i,j,k,l;
	for(i=1;i<=N;i++)
	{
		square[i]=i*i;
	}
	for(i=1;i<=100;i++)
	{
		if(!is_square(i))
		{
			j=1;
			while(1)
			{
				k=1+i*square[j];

				if(l=is_square(k))
				{
					printf("%lld^2-%lld*%lld^2=1\n",l,i,j);
					break;
				}
				else
					j++;
			}
		}
	}
}
long long int  is_square(long long int  n)
{
	int i=1,j=N;
	while(i<=j)
	{
		if(n==square[(i+j)/2])
			return (i+j)/2;
		else if(n<square[(i+j)/2])
			j=(i+j)/2-1;
		else
			i=(i+j)/2+1;
	}
	return 0;
}

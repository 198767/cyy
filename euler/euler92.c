#include<stdio.h>
#define N 567
int num[N+1];
int trans(int a);
int main()
{
	int i,j;
	for(i=N;i>=1;i--)
	{
		j=i;
		while(1)
		{
			j=trans(j);
			if( j==1 || j==89)
			{
				num[i]=j;
				if(j==89)
					++num[0];
				break;
			}
		}
	}
	for(i=N+1;i<10000000;i++)
	{
		j=num[trans(i)];
		if(j==89)
			++num[0];
	}
	printf("%d",num[0]);
}
int trans(int a)
{
	int res=0;
	int b;
	while(a)
	{

		b=a%10;
		res+=b*b;
		a/=10;
	}
	return res;
}


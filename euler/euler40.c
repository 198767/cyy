#include<stdio.h>
int digit(int a);
int cyy_exp(int a,int b);
int main()
{
	printf("%d\n",digit(1)*digit(10)*digit(100)*digit(1000)*digit(10000)*digit(100000)*digit(1000000));
	return 0;
}
int digit(int index)
{
	int fac[10]={0,1,21,321,4321,54321,654321,7654321,87654321,987654321};
	int min_index;
	int max_index;
	int i;
	int digit=-1;
	int temp_index;
	min_index=1;
	max_index=9;
	for(i=1;i<=9;i++)
	{
		min_index=fac[i-1]*9+1;
		max_index=fac[i]*9;
		if(index==min_index)
			return 1;
		if(index==max_index)
			return 9;
		if(index>min_index && index<max_index)
		{
			temp_index=min_index;
			digit=cyy_exp(10,i-1);
			while(temp_index<index)
			{
				temp_index+=i;
				digit++;
			}
			if(temp_index==index)
				return digit/cyy_exp(10,i-1);
			digit--;
			temp_index--;
			while(temp_index >index)
			{
				digit/=10;
				temp_index--;
			}
			return digit%10;
		}
	}
	return digit;
}
int cyy_exp(int a,int b)
{
	int i;
	int c=a;
	if(b==0)
		return 1;
	if(b==1)
		return a;
	for(i=0;i<b-1;i++)
		c*=a;
	return c;
}

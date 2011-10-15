#include<stdio.h>
int n[3]={6008,5147,5143};
int m[3];
int k=3;
int main()
{
	int factors[1000];
	int i=0,j=0;
	for(i=7;i<90000;i++)
	{
		if(divide(i))
		{
			factors[j++]=i;
			if(n[0]==0&&n[1]==0&&n[2]==1)
			{
				break;
			}
		}
	}	

	for(i=7;i<90000;i++)
	{
		for(k=0;k<j;k++)
		{
			if(factors[k]%i==0)
			{
				factors[j++]=i;
				break;
			}
		}
	}	

		for(k=0;k<j;k++)
		{
			printf("%d\n",factors[k]);
		}
	return 0;
}
int divide(int num)
{
	int i;
	int j=n[0];
	for(i=0;i<k;i++)
	{
		m[i]=j/num;
		if(i+1<k)
			j=(j%num)*10000+n[i+1];
	}
	if(j%num==0)
	{
		for(i=0;i<k;i++)
			n[i]=m[i];
		return 1;
	}
	else
		return 0;
}

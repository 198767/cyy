#include<stdio.h>
int main()
{
	int a,b;
	int res[1010];
	int i,j,cycle_len;
	int flag;
	int d=0,maxlen=0;
	for(b=1000;b>=maxlen;b--)
	{
		j=0;
		a=1;
		flag=0;
		cycle_len=0;
		while(1)
		{
			a*=10;
			if(a>b)
				flag=1;
			if(flag)
			{
				for(i=0;i<j;i++)
				{
					if(res[i]==a)
					{
						cycle_len=j-i;
						break;
					}
				}
				res[j++]=a;
			}
			a=a%b;
			if(cycle_len || a==0)
				break;
		}
		if(cycle_len>maxlen)
		{
			maxlen=cycle_len;
			d=b;
		}
	}
	printf("%d %d",d,maxlen);
	return 0;

}

#include<stdio.h>
int main()
{
	int i=1,j=2,sum=2,k=0;
	while(j<4000000)
	{
		k=i+j;
		i=j;
		j=k;
		if(j%2==0)
			sum+=j;
		
	}
	printf("%d",sum);
}

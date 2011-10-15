#include <stdio.h>
int main()
{
	int a,b,c;
	for(a=1;a<=333;a++)
	{
		for(b=a+1;b<500;b++)
		{
			c=1000-a-b;
			if(a*a+b*b==c*c)
			{
				printf("%d %d %d",a,b,c);
			}
		}
	}
	return 0;
}

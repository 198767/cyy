#include<stdio.h>
#include "ln.h"
int main()
{
	int i=2;
	lN a=str2ln(NULL,"1");
	if(!a)
	{
		printf("创建失败");
		return -1;
	}
	lN b=str2ln(NULL,"1");
	if(!b)
	{
		printf("创建失败");
		return -1;
	}
	while(1)
	{
		i++;
		a=add_lns(a,b,firstln);
		if(ln_get_digitnum(a,a->highestdigit)==1000)
		{
			
			printf("%d",i);
			break;
		}
		b=add_lns(b,a,firstln);
		i++;
		if(ln_get_digitnum(b,b->highestdigit)==1000)
		{
			printf("%d",i);
			break;
		}
	}
	free_ln(a);
	free_ln(b);
}

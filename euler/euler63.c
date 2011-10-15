#include "largenum.h"
#include <stdio.h>
#include <limits.h>
int main()
{
	int i;
	int n=1;
	int count=0;
	int flag=0;
	largeNum num=NULL;
	while(1)
	{
		flag=0;
		for(i=9;i>=1;i--)
		{
			num=largenum_setval(num,i);

			largenum_exp_int(num,n,firstnumber);
			if(get_digitnum(num)==n)
			{
				flag=1;
				show_largenum(num);
				printf("\n");
				count++;
			}
			else
				break;	
		}
		if(flag==0)
			break;
		n++;
	}
	printf("总数=%d",count);
	return 0;
}

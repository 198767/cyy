#include <stdio.h>
#include "largenum.h"
/*
 *	溢出直接截断掉 这样快很多
 */

int main()
{
	int i,b;
	largeNum sum=init_largenum(5);
	largeNum k=NULL;
	largeNum temp=NULL;
	largeNum j=init_largenum(1);
	for(i=3;i<=1000;i++)
	{
		if(i%10==0)
			continue;
		j->lowestdigit->digit=i;
		j->highestdigit=j->lowestdigit;
		b=i;
		if(b%2==0)
			k=init_largenum(1);
		else
		{
			k=init_largenum(i);
			b--;	
		}
		while(1)
		{
			temp=copy_largenums(temp,j);
			multiply_largenums(j,temp,firstnumber);
			if(get_digitnum(j)>10)
				ln_truncate(j,3);
			b/=2;
			if(b==1)
				break;
			if(b%2!=0)
			{
				multiply_largenums(k,j,firstnumber);
				b--;
			}
		}
		multiply_largenums(k,j,firstnumber);
		add_largenums(sum,k,firstnumber);
		if(get_digitnum(sum)>10)
			ln_truncate(sum,3);
	}
	free_largenum(temp);
	show_largenum(sum);
	return 0;
}


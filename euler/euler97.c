#include <stdio.h>
#include "largenum.h"
/*
 *  跟euler48类似
 */

int main()
{
	int i,b;
	largeNum sum=init_largenum(1);
	largeNum k=NULL;
	largeNum temp=NULL;
	largeNum j=init_largenum(1);
	i=2;
	b=7830457;
	j->lowestdigit->digit=i;
	j->highestdigit=j->lowestdigit;
	k=init_largenum(i);
	b--;	
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
	free_largenum(temp);
	multiply_largenums(k,j,firstnumber);
	j=largenum_setval(j,28433);
	multiply_largenums(k,j,firstnumber);


	add_largenums(sum,k,firstnumber);

	show_largenum(sum);
	return 0;
}


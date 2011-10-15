#include<stdio.h>
#define N 100
#include "ln.h"
int main()
{


	lN n=NULL;
	lN a=NULL;
	lN b=NULL;
	lN limit=NULL;
	int i,j;
	int max=0;
	limit=init_ln(1);
	for(i=N-1;i>=2;i--)
	{
		if(i%10==0)
			continue;
		
		a=init_ln(i);
		for(j=N-1;j>=2;j--)
		{
			n=ln_exp_int(a,j,newln);
			if(ln_cmp(n,limit)<0)
			{
				free_ln(n);
				break;
			}
                        if(ln_digitsum(n)>max)
                        {  
                                max=ln_digitsum(n);
				ln_setval(limit,10);
				ln_exp_int(limit,max/9,firstln);
                        }  
                        free_ln(n);
                }  
                free_ln(a);
        }  
        printf("%d",max);
        return 0; 
}

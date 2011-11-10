#include<stdio.h>
#include "ln_helper.h"
int main(int argc, char** argv)
{
	/*
	ln a;
	ln b;
	a=ln_creat(5);
	if(a)
	{
		ln_info(a);
		ln_free(&a);
	}
	a=ln_setval(a,100);
	printf("%d\n",ln_cellnum(a));
	printf("%d\n",ln_untilcellnum(a,a->lsd->lcell));
	ln_addcell(a,1020);
	printf("%d\n",ln_untilcellnum(a,a->lsd->lcell));
	ln_info(a);
	b=ln_copy(NULL,a);
		ln_info(b);

	a=ln_setval(a,23);	
		ln_info(a);
		ln_copy(b,a);
		ln_info(b);
		ln_copy(NULL,NULL);
		*/
	ln a=str2ln(NULL,"00000123456.00000");
	printf(ln2str(a));
	return 0;
}

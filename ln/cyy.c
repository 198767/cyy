#include<stdio.h>
#include<limits.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init(199999);
	ln_info(a);
			//	2147483647
		ln_divide_int(a,200000,20,round_res,firstln);
	puts(ln2str(a));
	//todo  那个除法

	return 0;
}

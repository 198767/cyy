#include<stdio.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init_str("9999.989");
	ln_divide_int(a,3,2,round_res,firstln);
	puts(ln2str(a));
todo  那个除法

	return 0;
}

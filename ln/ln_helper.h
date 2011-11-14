/*
 *	该库里面包含ln的构造及释放的函数，以及转换函数等辅助函数
 */
#ifndef LN_HELPER_H
#define LN_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include "ln.h"


/*
 * 作用:创建ln
 * 参数:
 * 	cellnum:ln的初始节点数
 * 返回值:
 * 	成功:ln
 * 	失败:NULL
 */
ln ln_creat(int cellnum);

/*
 * 作用:释放ln
 * 参数:
 *	n:要释放的ln的指针
 * 返回值:
 * 	无
 */
void ln_free(ln* n);
/*
 * 作用:获取10的乘方,这个功能很常用
 * 参数:
 *	power:乘方的指数，必须>=0
 * 返回值:
 * 	成功:乘方
 * 	失败:返回-1
 */
int power10(int power);
/*
 * 作用:检查函数传入的ln参数是否合法(不包含NULL)
 * 参数:
 *	n:要检查的ln
 * 返回值:
 * 	成功:返回n
 * 	失败:返回NULL
 */
int ln_checknull(ln n);

/*
 * 作用:查看ln的内存结构
 * 参数:
 *	n:要查看的ln
 * 返回值:
 * 	无
 */
void ln_info(ln n);

/*
 * 作用:设置ln的值
 * 参数:
 *	ln:要赋值的ln,如果为NULL则构造一个新ln
 *	new_value:ln的值
 * 返回值:
 * 	成功:返回ln
 * 	失败:NULL
 */
ln ln_setval(ln n,int new_value);
#define ln_init(init_value) ln_setval(NULL,init_value)

/*
 * 作用:增加ln的节点,新节点在lsd于msd之间
 * 参数:
 *	ln:要处理的ln
 *	inc_num:增加的节点数
 * 返回值:
 * 	成功:返回增加后的ln
 * 	失败:返回NULL
 */
ln ln_addcell(ln n,int inc_num);


/*
 * 作用:获取ln整数部分从lsd节点截止到q节点处的节点个数(包括q)
 * 参数:
 *	n:要计算的ln
 *	q:截止节点
 * 返回值:
 * 	成功:返回节点数
 * 	失败:返回-1
 */
int ln_untilcellnum(ln n,cell q);
// 作用:获取ln从lsd到msd的节点总数
#define ln_cellnum(n) ln_untilcellnum(n,(n)->msd) 


/*
 * 作用:获取ln整数部分的十进制位数
 * 副作用:使用ln_stripleadingzero()把ln整数部分前置0去掉
 * 参数:
 *	n:要计算的ln
 *	q:截止节点
 * 返回值:
 * 	成功:返回十进制位数
 * 	失败:返回-1
 */
int ln_digitnum(ln n);

/*
 * 参数要求:n必须去掉前置0和后置0,否则会出错
 * 作用:获取节点的小数点位数,如果是在小数点前面,则取负值
 * 参数:
 *	n:要计算的ln
 *	q:节点
 * 返回值:返回小数点位数
 */
int ln_pointnum(ln n,cell q);

/*
 * 作用:复制b的值给a
 * 参数:
 *	b:原ln
 *	a:目标ln,如果传入NULL则返回新创建的b的副本
 * 返回值:
 * 	成功:如果a非NULL,返回a,否则返回新创建的b的副本
 * 	失败:返回NULL
 */
ln ln_copy(ln a,ln b);

/*
 * 作用:比较ln a和ln b的大小
 * 副作用:使用ln_stripleadingzero()把a,b整数部分前置0去掉
 * 参数:
 * 	a,b:待比较的ln
 * 返回值:
 * 	0: a=b
 * 	1: a>b
 * 	-1: a<b
 * 	-2: 出错
 */
int ln_cmp(ln a,ln b);

/*
 * 作用:比较ln a和ln b的绝对值大小
 * 副作用:使用ln_stripleadingzero()把a,b整数部分前置0去掉
 * 参数:
 * 	a,b:待比较的ln
 * 返回值:
 * 	0: |a|=|b|
 * 	1: |a|>|b|
 * 	-1: |a|<|b|
 * 	-2: 出错
 */
int ln_cmp_abs(ln a,ln b);

/*
 * 作用:比较ln a和int b的大小
 * 副作用:使用ln_stripleadingzero()把a整数部分前置0去掉
 * 参数:
 * 	a:待比较的ln
 * 	b:待比较的int
 * 返回值:
 * 	0: a=b
 * 	1: a>b
 * 	-1: a<b
 * 	-2: 出错
 */
int ln_cmp_int(ln a,int b);

/*
 * 作用:检查str的格式是否为(+-)?\d+(.\d+)? 该格式的数字可以用ln表示
 * 参数:
 *	str:要检查的字符串
 * 返回值:
 * 	成功:0
 * 	失败:-1
 */
int ln_checkstr(const char* str);

/*
 * 作用:把ln的前置0去掉,不影响ln的数值,只是方便一些操作
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	无
 */
void ln_stripleadingzero(ln n);

/*
 * 作用:把ln的后置0节点去掉,增加指数
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	无
 */
void ln_stripendingzero(ln n);

/*
 * 作用:获取ln的整数部分的结尾0的个数
 * 副作用:使用ln_stripleadingzero()把ln整数部分前置0去掉
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	成功:返回个数
 * 	失败:-1
 */
int ln_endingzeronum(ln n);

/*
 * 作用:把ln取精度
 * 副作用:把ln前置0和后置0去掉
 * 参数:
 *	n:要处理的ln
 *	precision:所需精度(保留的小数位数)
 *	mode:指定截断或者四舍五入
 * 返回值:
 * 	成功:返回ln
 * 	失败:NULL
 */
ln ln_fix(ln n,int precision,divide_mode mode);

/*
 * 作用:把ln的有效节点的num转负
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	成功:处理后的ln
 * 	失败:返回NULL
 */
ln ln_invcell(ln n);


/*
 * 作用:调整ln的有效节点为正常形式
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	成功:处理后的ln
 * 	失败:返回NULL
 */
ln ln_adjustcell(ln n);

/*
 * 作用:调整指数部分
 * 参数:
 *	n:要处理的ln
 * 	inc_power:指数增量 >0 增加指数,把整数部分结尾的0去掉 <0 减少指数,增加整数部分结尾的0
 * 返回值:
 * 	成功:ln
 * 	失败:返回NULL
 */
ln ln_adjustpower(ln n,int inc_power);

/*
 * 作用:把字符串转换为ln
 * 参数:
 *	n:要赋值的ln,如果为NULL则构造一个新ln
 *	str:要转换的字符串
 * 返回值:
 * 	成功:返回ln
 * 	失败:NULL
 */
ln str2ln(ln n,const char* str);
#define ln_init_str(str) str2ln(NULL,str)

/*
 * 作用:把ln转换为字符串
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	成功:返回ln的字符串表示(需要用free释放)
 * 	失败:NULL
 */
char* ln2str(ln n);

/*
 * 作用:打印ln的值
 * 参数:
 *	n:要输出的ln
 * 返回值:
 * 	无
 */
void ln_output(ln n);

#endif

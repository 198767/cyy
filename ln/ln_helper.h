#ifndef LN_HELPER_H
#define LN_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
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
 * 作用:检查函数传入的ln参数是否合法(不包含NULL)
 * 参数:
 *	n:要检查的ln
 * 返回值:
 * 	成功:返回n
 * 	失败:返回NULL
 */
ln ln_checknull(ln n);

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
 * 作用:获取ln从lsd截止到q节点处的节点个数(包括q)
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
 * 作用:复制b的值给a
 * 参数:
 *	b:原ln
 *	a:目标ln,如果传入NULL则返回新创建的b的副本
 * 返回值:
 * 	成功:如果a非NULL,返回a,否则返回新创建的b的副本
 * 	失败:返回NULL
 */
ln ln_copy(ln a,ln b);

#endif

/*
 *	该文件里面包含ln的基本运算函数
 */
#ifndef LN_ARITHMETIC_H
#define LN_ARITHMETIC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "ln.h"
#include "ln_helper.h"

/*
 * 作用:把a加上b
 * 副作用:调用ln_stripleadingzero()消除a,b前置0并且用ln_adjustpower()调整a,b的指数
 * 副作用:如果是减法,调用ln_stripleadingzero()消除差的前置0
 * 参数:
 *	a,b:要相加的ln
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_add(ln a,ln b,res_type restype);
/*
 * 作用:把ln a加上int b
 * 副作用:调用ln_stripleadingzero()消除a前置0并且用ln_adjustpower()调整a的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 * 	a:待相加的ln
 * 	b:待相加的int
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_add_int(ln a,int b,res_type restype);
/*
 * 作用:把a减去b
 * 副作用:调用ln_stripleadingzero()消除a,b前置0并且用ln_adjustpower()调整a,b的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 *	a,b:要相减的ln
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_minus(ln a,ln b,res_type restype);

/*
 * 作用:把i乘以j
 * 副作用:调用ln_stripleadingzero()消除a,b前置0并且用ln_adjustpower()调整a,b的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 *	a,b:要相减的ln
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_multiply(ln i,ln j,res_type restype);
/*
 * 作用:把ln a乘以int b
 * 副作用:调用ln_stripleadingzero()消除a前置0并且用ln_adjustpower()调整a的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 * 	a:待相乘的ln
 * 	b:待相乘的int
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_multiply_int(ln a,int b,res_type restype);

/*
 * 作用:把ln a除以int b
 * 副作用:调用ln_stripleadingzero()消除a前置0并且用ln_adjustpower()调整a的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 * 	a:除数
 * 	b:被除数
 *	precision:所需精度(保留的小数位数)
 *	mode:指定截断或者四舍五入
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_divide_int(ln a,int b,int precision,divide_mode mode,res_type restype);

/*
 * 作用:把ln相除
 * 副作用:使用ln_stripleadingzero()把a,b整数部分前置0去掉
 * 参数:
 * 	a:除数
 * 	b:被除数
 *	precision:所需精度(保留的小数位数)
 *	mode:指定截断或者四舍五入
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_divide(ln a,ln b,int precision,divide_mode mode,res_type restype);
#endif

/*
 *	该文件里面包含ln构成的分数的基本操作函数
 */
#ifndef LN_FRAC_H
#define LN_FRAC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "ln.h"
#include "ln_helper.h"

struct _frac
{
	ln fenzi;	//分子
	ln fenmu;	//分母
};
typedef struct _frac* frac;

#endif

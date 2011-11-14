/*
 *	该文件里面包含ln的基本运算函数
 */
#include "ln_arithmetic.h"

/*
 * 作用:把a加上b
 * 副作用:调用ln_stripleadingzero()消除a,b前置0并且用ln_adjustpower()调整a,b的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 *	a,b:要相加的ln
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_add(ln a,ln b,res_type restype)
{
	ln c;
	int carry;
	cell x,y,z;
	int cmp_res;

	//验证参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//去除前置0
	ln_stripleadingzero(a);
	ln_stripleadingzero(b);

	//把指数调整为一致
	if(a->power > b->power)
		ln_adjustpower(a,b->power-a->power);
	if(a->power < b->power)
		ln_adjustpower(b,a->power-b->power);

	//根据存放方式设置c
	if(restype==firstln) 
		c=a;
	else
	{
		c=ln_creat(ln_cellnum(a));
		if(c==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}
	}

	c->power=a->power;
	//a,b符号相同,加法
	if(a->sign==b->sign)	
	{
		//有可能会进位c多增加一个节点
		if(c->msd->hcell==c->lsd)
		{
			c=ln_addcell(c,1);
			if(c==NULL)
			{
				fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
				return NULL;	
			}
		}
		//调整指数部分一致
		c->sign=a->sign;
		x=a->lsd;
		y=b->lsd;
		z=c->lsd;
		carry=0;
		while(1)
		{
			if(x)
				carry+=x->num;
			if(y)
				carry+=y->num;
			if(x==a->msd)
				x=NULL;
			if(y==b->msd)
				y=NULL;
			z->num=carry%UNIT;
			carry=carry/UNIT;
			if(x==NULL&&y==NULL&&carry==0)
			{
				c->msd=z;
				break;
			}
			if(x)
				x=x->hcell;
			if(y)
				y=y->hcell;
			z=z->hcell;
		}
		return c;
	}
	else //减法
	{
		cmp_res=ln_cmp_abs(a,b);
		if(cmp_res==2)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_cmp_abs fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}

		if(cmp_res==0) //绝对值相等 为0
		{
			ln_setval(c,0);
			return c;
		}
		//确定符号
		if(cmp_res==1)
		{
			x=a->lsd;
			y=b->lsd;
			c->sign=a->sign;
		}
		else	
		{
			x=b->lsd;
			y=a->lsd;
			c->sign=b->sign;
		}

		z=c->lsd;
		carry=0;
		while(1)
		{
			if(y)
				z->num=x->num-carry-y->num;
			else
				z->num=x->num-carry;
			if(z->num <0)
			{
				carry=1;
				z->num+=UNIT;	
			}
			else
				carry=0;
			if(y==a->msd || y==b->msd)
				y=NULL;
			if(x==a->msd || x==b->msd)
			{
				c->msd=z;
				break;
			}
			x=x->hcell;
			if(y)
				y=y->hcell;
			z=z->hcell;
		}	
		//消除前置0
		ln_stripleadingzero(c);
		return c;
	}	
}

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
ln ln_add_int(ln a,int b,res_type restype)
{
	ln c,d;
	//验证参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	c=ln_init(b);
	if(c==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	

	}
	d=ln_add(a,c,restype);
	ln_free(&c);
	return d;
}


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
ln ln_minus(ln a,ln b,res_type restype)
{
	ln c;
	//验证参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	//把b转负
	b->sign=-b->sign;
	//相减即相加
	c=ln_add(a,b,restype);
	//把b转负
	b->sign=-b->sign;
	return c;
}

/*
 * 作用:把i乘以j
 * 副作用:调用ln_stripleadingzero()消除i,j前置0并且用ln_stripendingzero()调整a,b的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 *	a,b:要相减的ln
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_multiply(ln i,ln j,res_type restype)
{
	ln k;
	cell a,b,c,x,y,z;
	int res;
	
	//验证参数
	if(ln_checknull(i)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(j)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//去除前置0
	ln_stripleadingzero(i);
	ln_stripleadingzero(j);

	//简单地去掉后置0
	ln_stripendingzero(i);
	ln_stripendingzero(j);

	//分配结果节点,节点数可以直接算出来
	k=ln_creat(ln_cellnum(i)+ln_cellnum(j));
	if(k==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	k->msd=k->lsd->lcell;

	//确定正负号
	if(i->sign==j->sign)
		k->sign=1;
	else
		k->sign=-1;

	//确定指数
	k->power=i->power+j->power;

	a=i->lsd;
	b=j->lsd;
	c=k->lsd;
	do
	{
		x=a;
		y=b;
		z=c;
		do
		{
			res=(x->num*y->num)+z->num;
			if(res>=UNIT)
			{
				z->num=res%UNIT;
				z->hcell->num+=res/UNIT;
			}
			else
				z->num=res;
			y=y->hcell;
			z=z->hcell;
		}
		while(y != j->msd->hcell);
		a=a->hcell;
		c=c->hcell;
	}
	while(a != i->msd->hcell);
	
	//消除前置0
	ln_stripleadingzero(k);

	if(restype==newln)
		return  k;
	else
	{
		ln_copy(i,k);
		ln_free(&k);
		return i;
	}
}

/*
 * 作用:把ln a乘以int b
 * 副作用:调用ln_stripleadingzero()消除a前置0并且用ln_stripendingzero()调整a的指数
 * 副作用:调用ln_stripleadingzero()消除结果的前置0
 * 参数:
 * 	a:待相乘的ln
 * 	b:待相乘的int
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_multiply_int(ln a,int b,res_type restype)
{
	ln c,d;

	//验证参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//去除前置0
	ln_stripleadingzero(a);
	//简单地去掉后置0
	ln_stripendingzero(a);

	//把int转换为ln
	c=ln_init(b);
	if(c==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//相乘
	d=ln_multiply(a,c,restype);
	if(d==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_multiply fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	
	ln_free(&c);
	return d;
}


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
ln ln_divide_int(ln a,int b,int precision,divide_mode mode,res_type restype)
{
	//TODO:如果a或者b=MIN_INT 这边会溢出要转换成ln除法
	int res=0;
	int carry=0;
	int inc_prec=0; //累积精度
	ln c,d;
	cell x,z;

	//验证参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//精度参数有误
	if(precision<0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: precision error\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}
	//除数不能为0
	if(b==0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: b=0\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//除数必须在-200000-200000之间否则下面计算有可能溢出,因此必须把b转化成ln再操作
	//这个范围是根据INT_MAX/UNIT即2147483647/10000得出
	if(b>200000 || b<-200000)
	{
		d=ln_init(b);
		if(d==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		c=ln_divide(a,d,precision,mode,restype);
		if(c==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		return c;
	}
	

	//去除前置0
	ln_stripleadingzero(a);

	//没指定商的精度 那就使用默认精度
	if(precision<0)
		precision=DIV_PREC;

	if(restype==newln)
	{
		c=ln_creat(ln_cellnum(a));
		if(c==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		c->msd=c->lsd->lcell;
	}
	else
		c=a;

	//确定符号
	if(b>0)
		c->sign=a->sign;
	else
	{
		c->sign=-a->sign;
		b=-b;
	}

	//算出初始精度
	inc_prec=ln_pointnum(a,a->msd)-DIGIT_NUM;

	//开始计算
	carry=0;
	x=a->msd;
	z=c->msd;
	res=x->num;
	while(1)
	{
		res+=carry*UNIT;
		z->num=res/b;
		carry=res%b;
		//开始计算当前精度
		inc_prec+=DIGIT_NUM;
		if(inc_prec>precision) //已经达到需要的精度
		{
			c->lsd=z;
			//确定指数
			c->power=-inc_prec;
			break;
		}
		//必须这样标识，否则如果a和c一样，则下面会修改lsd
		if(x==a->lsd)
			x=NULL;
		if(x==NULL)
		{
			if(carry==0) //除得尽
			{
				c->lsd=z;
				//确定指数
				c->power=-inc_prec;
				break;
			}
			res=0;
		}
		else
		{
			x=x->lcell;
			res=x->num;
		}

		if(z==c->lsd) //增加节点
		{
			if(ln_addcell(c,INIT_SIZE) ==NULL)
			{
				fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
				return NULL;
			}
			c->lsd=c->msd->hcell;
		}
		z=z->lcell;
	}

	//获取精度
	ln_fix(c,precision,mode);
	return c;
}

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
ln ln_divide(ln a,ln b,int precision,divide_mode mode,res_type restype)
{
	ln c;
	ln x,y,z;
	int x_cellnum;
	int y_cellnum;
	int inc_prec=0; //累积精度
	cell i,j,k;

	//验证参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	
	//去除前置0
	ln_stripleadingzero(a);
	ln_stripleadingzero(b);

	//复制ln
	x=ln_copy(NULL,a);
	if(x==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	y=ln_copy(NULL,b);
	if(y==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_copy fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	//分配结果空间
	z=ln_init(0);
	if(z==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//确定符号
	if(x->sign==y->sign)
		z->sign=1;
	else
		z->sign=-1;

	//把x,y整数部分节点数调整一致
	x_cellnum=ln_cellnum(x);
	if(x_cellnum==-1)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_cellnum fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	y_cellnum=ln_cellnum(y);
	if(y_cellnum==-1)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_cellnum fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	
	if(x_cellnum<y_cellnum)
		ln_adjustpower(x,(x_cellnum-y_cellnum)*DIGIT_NUM);
	if(y_cellnum<x_cellnum)
		ln_adjustpower(y,(y_cellnum-x_cellnum)*DIGIT_NUM);
	
	//算出初始精度
	inc_prec=ln_pointnum(x,x->lsd)-DIGIT_NUM+y->power;

	x->sign=1;
	x->power=0;
	y->power=0;
	z->power=0;
	i=x->msd;
	j=y->msd;
	k=z->msd;

	while(1)
	{
		if(i!=x->msd) 
			k->num=(i->num+i->hcell->num*UNIT)/j->num; //求出z位数
		else
			k->num=i->num/j->num; //求出z位数
		printf("addr=%d num=%d\n",(int)k,k->num);
		if(k->num !=0)
		{
			y->sign=-1; //y变成负数
			c=ln_multiply_int(y,k->num,newln);
			ln_add(x,c,firstln);
			ln_free(&c);
			while(x->sign==1) //少减了，再减去
			{
				y->sign=-1;
				ln_add(x,y,firstln);
				k->num++;
			}
			while(x->sign==-1) //多减了，补上
			{
				y->sign=1;
				ln_add(x,y,firstln);
				k->num--;
			}
		}

		//开始计算当前精度
		inc_prec+=DIGIT_NUM;
		if(inc_prec>precision) //已经达到需要的精度
		{
			z->lsd=k;
			//确定指数
			z->power=-inc_prec;
			break;
		}
		

		if(ln_cmp_int(x,0)==0) //除得尽
		{
			z->lsd=k;
			//确定指数
			z->power=-inc_prec;
			break;
		}

		//去掉前置0
		if(x->msd->num==0 && x->msd !=x->lsd)
			x->msd=x->msd->lcell;

		//增加节点
		if(x->lsd->lcell ==x->msd)
		{
			if(ln_addcell(x,INIT_SIZE) ==NULL)
			{
				fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
				return NULL;
			}
		}
		x->lsd=x->lsd->lcell;
		x->lsd->num=0;
		i=i->lcell;

		//增加节点
		if(k==z->lsd)
		{
			if(ln_addcell(z,INIT_SIZE) ==NULL)
			{
				fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
				return NULL;
			}
		}
		z->lsd=z->msd->hcell;
		k=k->lcell;
	}

	//释放临时ln
	ln_free(&x);
	ln_free(&y);
	//去除前置0
	ln_stripleadingzero(z);
	//获取精度
	ln_fix(z,precision,mode);
	//复制给a
	if(restype==firstln)
	{
		ln_copy(a,z);
		ln_free(&z);
	}
	return z;
}
#ifdef dsa
ln ln_exp_int(ln i,int b,res_type restype)
{

	ln j=NULL,k=NULL;
	assert(b>=0);
	if(b==0)
	{
		if(restype==newln)
			return str2ln(NULL,"1");
		else
			return ln_setval(i,1);
	}
	else if(b==1)
	{
		if(restype==newln)
			return copy_lns(NULL,i);
		else
			return i;
	}
	else if(b==2)
	{
		if(restype==newln)
			return ln_square(i,newln);
		else
			return ln_square(i,firstln);
	}
	else
	{
		j=copy_lns(NULL,i);
		if(b%2==0)
			k=init_ln(1);
		else
		{
			k=copy_lns(NULL,i);
			b--;	
		}
		while(1)
		{
			ln_square(j,firstln);
			b/=2;
			if(b==1)
				break;
			if(b%2!=0)
			{
				ln_multiply(k,j,firstln);
				b--;
			}
		}
		ln_multiply(k,j,firstln);
		ln_free(j);
		if(restype==newln)
			return k;
		else
		{
			copy_lns(&i,k);
			ln_free(k);
			return i;
		}
	}
}

int ln_divideable_num(const ln i,int b)
{
	cell a;
	int res=0;
	int carry=0;
	ln j;
	assert(b!=0);
	assert(i->power==0);
	if(b==1|| b==-1 )
		return 1;
	j=copy_lns(NULL,i);
	a=j->msd;
	res=a->num;
	while(1)
	{
		res=res+carry*UNIT;
		carry=res%b;
		if(a==j->lsd)
		{
			ln_free(j);
			if(carry==0) //除得尽
				return 1;
			else
				return 0;
		}
		a=a->lcell;
		res=a->num;
	}
	return 0;
}





//求平方

ln ln_square(ln a,res_type restype)
{
	ln b;
	cell i,j,k,l;
	int res;
	if(a->lsd==a->msd)
	{
		if(restype==newln)
			return  ln_multiply(a,a,newln);
		else
			return  ln_multiply(a,a,firstln);
	}
	b=ln_creat(ln_nodenum(a)*2);
	b->sign=1;
	b->power=a->power*2;
	i=a->lsd;
	j=b->lsd;
	do
	{
		k=i;
		l=j;
		if(i->num !=0)
		{
			do
			{
				if(i==k)
					res=i->num*k->num+l->num;
				else
					res=(i->num*k->num*2)+l->num;
				if(res>=UNIT)
				{
					l->num=res%UNIT;
					l->hcell->num+=res/UNIT;
				}
				else
					l->num=res;
				k=k->hcell;
				l=l->hcell;
			}
			while(k != a->msd->hcell);
		}
		i=i->hcell;
		j=j->hcell->hcell;
	}
	while(i != a->msd->hcell);
	b->msd=j->lcell;

	if(restype==newln)
		return  b;
	else
	{
		copy_lns(&a,b);
		ln_free(b);
		return a;
	}
}



//获取ln的十进制位数的和
/*
   int ln_digitsum(ln n)
   {
   cell p=n->lsd->lcell;
   int sum=0;
   int i;
   int j;
   do
   {
   p=p->hcell;
   j=1;
   for(i=0;i<DIGIT_NUM;i++)
   {
   sum+=(p->num/j)%10;
   j*=10;
   }

   }
   while(p !=n->msd);
   return sum;
   }
   */

#endif

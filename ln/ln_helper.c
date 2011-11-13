/*
 *	该库里面包含ln的构造及释放的函数，以及转换函数等辅助函数
 */
#include "ln_helper.h"

/*
 * 作用:创建ln,初始节点值为0
 * 参数:
 * 	cellnum:ln的初始节点数
 * 返回值:
 * 	成功:ln
 * 	失败:NULL
 */
ln ln_creat(int cellnum)
{
	int i;
	cell p;

	ln n=(ln)malloc(sizeof(struct _ln));
	if(!n)
	{
		fprintf(stderr,"[%s %d] %s malloc error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
		return NULL;			
	}

	n->lsd=NULL;
	n->msd=NULL;
	n->power=0;
	n->sign=1;

	for(i=0;i<cellnum;i++)
	{
		p=(cell)malloc(sizeof(struct _cell));
		if(!p)
		{
			fprintf(stderr,"[%s %d] %s malloc error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
			ln_free(&n);
			return NULL;			
		}
		p->num=0;
		if(!n->lsd)
		{
			n->lsd=p;
			p->hcell=p;
			p->lcell=p;
		}
		else
		{
			p->hcell=n->lsd;
			p->lcell=n->lsd->lcell;
			n->lsd->lcell->hcell=p;
			n->lsd->lcell=p;
			n->lsd=p;
		}
	}
	n->msd=n->lsd;
	return n;
}

/*
 * 作用:释放ln
 * 参数:
 *	n:要释放的ln的指针
 * 返回值:
 * 	无
 */
void ln_free(ln* n)
{
	cell p;
	if(n==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: n=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}

	if(ln_checknull(*n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}


	p=(*n)->lsd;
	if(p)
	{
		while(p!=(*n)->lsd->lcell)
		{
			p=p->hcell;
			free(p->lcell);
		}
		free(p);
	}
	free(*n);
	*n=NULL;
	return;
}

/*
 * 作用:获取10的乘方,这个功能很常用
 * 参数:
 *	power:乘方的指数，必须>=0
 * 返回值:
 * 	成功:乘方
 * 	失败:返回-1
 */
int power10(int power)
{
	int i;
	int base=10;
	int res=1;
	if(power<0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: power <0\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;
	}
	else if(power==0)
		return 1;
	else if(power==1)
		return base;
	else
	{
		for(i=0;i<power;i++)
			res*=base;
		return res;
	}
}

/*
 * 作用:检查函数传入的ln参数是否合法(不包含NULL)
 * 参数:
 *	n:要检查的ln
 * 返回值:
 * 	成功:返回0
 * 	失败:返回-1
 */
int ln_checknull(ln n)
{
	if(n==NULL) //空指针
	{
		fprintf(stderr,"[%s %d] %s error,reason:n=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}

	if(n->sign !=-1 && n->sign !=0 && n->sign !=1) //非法符号
	{
		fprintf(stderr,"[%s %d] %s error,reason:invalid sign\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}

	if(n->msd==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason:n->msd=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}

	if(n->lsd==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason:n->lsd=NULL\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	return 0;
}


/*
 * 作用:查看ln的内存结构
 * 参数:
 *	n:要查看的ln
 * 返回值:
 * 	无
 */
void ln_info(ln n)
{
	cell p=n->lsd;
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason:ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	printf("ln_addr=%d sign=%d \n",(int)n,n->sign);
	printf("ln_addr=%d power=%d \n",(int)n,n->power);
	if(p)
	{
		while(p!=n->msd)
		{
			printf("num_addr=%d num=%d \n",(int)p,p->num);
			p=p->hcell;
		}
		printf("num_addr=%d num=%d \n",(int)p,p->num);
	}
	else
	       	printf("ln_addr=%d not num",(int)n);
	return;
}

/*
 * 作用:设置ln的值
 * 参数:
 *	n:要赋值的ln,如果为NULL则构造一个新ln
 *	new_value:ln的值
 * 返回值:
 * 	成功:返回ln
 * 	失败:NULL
 */
ln ln_setval(ln n,int new_value)
{
	int flag; 
	cell p=NULL;
	if(!n)
	{
		n=ln_creat(INIT_SIZE);
		if(!n)
		{
			fprintf(stderr,"[%s %d] %s error,reason:ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
	}

	flag=0;
	if(new_value==INT_MIN) //最小的int,需要特殊处理
	{
		new_value=INT_MAX;
		flag=1;
	}
	if(new_value<0)
	{
		n->sign=-1;
		new_value=-new_value;
	}
	else
		n->sign=1;
	n->power=0;
	p=n->lsd;
	while(1)
	{
		p->num=new_value%UNIT;
		new_value=new_value/UNIT;
		if(new_value !=0) //往前移动一步
		{
			if(p->hcell==n->lsd)//节点已经用完
			{
				if(ln_addcell(n,INIT_SIZE)==NULL)
				{
					fprintf(stderr,"[%s %d] %s error,reason:ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
					return NULL;
				}
			}
			p=p->hcell;
		}
		else
		{
			n->msd=p;
			break;
		}
	}
	
	if(flag==1)
	{
		n->lsd->num++;
		n->sign=-1;
	}
	return n;
}

/*
 * 作用:增加ln的节点,新节点在lsd于msd之间,并且num初始化为0
 * 参数:
 *	n:要处理的ln
 *	inc_num:增加的节点数
 * 返回值:
 * 	成功:返回增加后的ln
 * 	失败:返回NULL
 */
ln ln_addcell(ln n,int inc_num)
{
	cell p;

	//验证参数
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason:ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}
	if(inc_num<=0) //无须增加,非法参数
	{
		fprintf(stderr,"[%s %d] %s error,reason:inc_num<=0\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//开始增加节点
	while(inc_num)
	{
		p=(cell)malloc(sizeof(struct _cell));
		if(!p)
		{
			fprintf(stderr,"[%s %d] ln_addcell malloc error,reason: %s\n",__FILE__,__LINE__,strerror(errno));
			return NULL;
		}
		p->hcell=n->lsd;
		p->lcell=n->lsd->lcell;
		n->lsd->lcell->hcell=p;
		n->lsd->lcell=p;
		p->num=0;
		inc_num--;
	}
	return n;
}

/*
 * 作用:获取ln整数部分从lsd节点截止到q节点处的节点个数(包括q)
 * 参数:
 *	n:要计算的ln
 *	q:截止节点
 * 返回值:
 * 	成功:返回节点数
 * 	失败:返回-1
 */
int ln_untilcellnum(ln n,cell q)
{
	int i=1;
	cell p=n->lsd;
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	while(p!=q)
	{
		p=p->hcell;
		if(p==n->lsd) //指定的节点在n中不存在 导致又碰到lsd了
		{
			fprintf(stderr,"[%s %d] %s error,reason: invalid cell\n",__FILE__,__LINE__,__FUNCTION__);
			return -1;
		}
		i++;
	}
	return i;
}


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
int ln_digitnum(ln n)
{
	int i,j;
	cell p;

	//检查参数
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}

	//去掉前置0,避免计算出错
	ln_stripleadingzero(n);

	//计算有效节点总数
	i=ln_cellnum(n);
	if(i==-1)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_cellnum fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	i=(i-1)*4;

	//计算最高节点的位数
	p=n->msd;
	j=1;
	while(j<=p->num)
	{
		i++;
		j*=10;
	}
	return i;
}


/*
 * 参数要求:n必须去掉前置0
 * 作用:获取节点的小数点位数,如果是在小数点前面,则取负值
 * 参数:
 *	n:要计算的ln
 *	q:节点
 * 返回值:返回小数点位数
 */
int ln_pointnum(ln n,cell q)
{
	int pointnum;
	cell p;
	//检查参数
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	//检查n的格式
	if(n->msd->num==0)
	{
		if(n->msd==n->lsd)  //这个ln是0
			return 0;
		else //前置0没有去掉,报错
		{
			fprintf(stderr,"[%s %d] %s error,reason: n format error,n has leading zeros\n",__FILE__,__LINE__,__FUNCTION__);
			return -1;	
		}
	}
	pointnum=-n->power;
	p=n->lsd;
	while(p!=q)
	{
		p=p->hcell;
		pointnum-=DIGIT_NUM;
	}
	return pointnum;
}





/*
 * 作用:复制b的值给a
 * 参数:
 *	b:原ln
 *	a:目标ln,如果传入NULL则返回新创建的b的副本
 * 返回值:
 * 	成功:如果a非NULL,返回a,否则返回新创建的b的副本
 * 	失败:返回NULL
 */
ln ln_copy(ln a,ln b)
{
	cell p,q;
	int cellnum_a;
	int cellnum_b;

	//验证b
	if(ln_checknull(b) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	cellnum_b=ln_cellnum(b); //获取b的有效节点
	if(cellnum_b==-1)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_cellnum fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	if(!a)
	{
		a=ln_creat(cellnum_b);  //初始创建和b一样的有效节点数 省得再分配
		if(a==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
	}
	else
	{
		if(ln_checknull(a) !=0)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}

		cellnum_a=ln_cellnum(a);
		if(cellnum_a==-1)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_cellnum fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}

		if(cellnum_b>cellnum_a) //如果b的有效节点数比a多 
		{
			if(ln_addcell(a,cellnum_b-cellnum_a)==NULL) //把a的长度设置成和b一样
			{
				fprintf(stderr,"[%s %d] %s error,reason: ln_addsize fail\n",__FILE__,__LINE__,__FUNCTION__);
				return NULL;
			}
		}
	}
	a->sign=b->sign;
	a->power=b->power;
	p=a->lsd;
	q=b->lsd;
	while(cellnum_b>0)
	{
		p->num=q->num;
		p=p->hcell;
		q=q->hcell;
		cellnum_b--;
	}
	a->msd=p->lcell;
	return a;
}


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
int ln_cmp(ln a,ln b)
{
	int res;
	cell p,q;
	
	//检查参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	
	ln_stripleadingzero(a);
	ln_stripleadingzero(b);

	if(a->msd->num==0 && b->msd->num==0) //a,b都是0
		return 0;
	if(a->msd->num==0) //a是0
	{
		if(b->sign==1) //正数
			return -1;
		else
			return 1;
	}
	if(b->msd->num==0) //b是0
	{
		if(a->sign==1) //正数
			return 1;
		else
			return -1;
	}

	//a,b一正一负
	if(a->sign > b->sign)
		return 1;
	else if(a->sign < b->sign)
		return -1;

	//到这边a,b符号相等
	//把指数调整为一致
	if(a->power > b->power)
		ln_adjustpower(a,b->power-a->power);
	if(a->power < b->power)
		ln_adjustpower(b,a->power-b->power);
	
	p=a->lsd;
	q=b->lsd;
	res=0;
	while(1)
	{
		if(p->num>q->num)
			res=1;
		else if(p->num<q->num)
			res=-1;
		if(p==a->msd)
		{
			if(q!=b->msd)
				res=-1;
			break;
		}
		if(q==b->msd)
		{
			if(p!= a->msd)
				res=1;
			break;
		}
		p=p->hcell;
		q=q->hcell;
	}
	if(a->sign==-1) //负数
		res*=-1;
	return res;
}

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
int ln_cmp_abs(ln a,ln b)
{
	int res;
	int sign_a,sign_b;
	
	//检查参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	
	//保存正负号
	sign_a=a->sign;
	sign_b=b->sign;
	//设置为正数
	a->sign=1;
	b->sign=1;
	res=ln_cmp(a,b);
	//恢复
	a->sign=sign_a;
	b->sign=sign_b;
	return res;
}

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
int ln_cmp_int(ln a,int b)
{
	int res;
	ln c;
	//检查参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	c=ln_init(b);
	if(c==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	res=ln_cmp(a,c);
	ln_free(&c);
	return res;
}

/*
 * 作用:检查str的格式是否为(+-)?\d+(.\d+)? 该格式的数字可以用ln表示
 * 参数:
 *	str:要检查的字符串
 * 返回值:
 * 	成功:0
 * 	失败:-1
 */
int ln_checkstr(const char* str)
{
	int haspoint=0;	//0-无小数点 1-有小数点

	if(str==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: str=NULL",__FILE__,__LINE__,__FUNCTION__);
		return -1;

	}
	//以合法字符开头
	if(*str!='+' && *str!='-' && (*str<'0' || *str>'9'))
	{
		fprintf(stderr,"[%s %d] %s error,reason: invalid first char (%c)\n",__FILE__,__LINE__,__FUNCTION__,*str);
		return -1;
	}
	str++;
	while(*str)
	{
		if(*str !='.' && (*str<'0' || *str>'9'))
		{
			fprintf(stderr,"[%s %d] %s error,reason: invalid char (%c)\n",__FILE__,__LINE__,__FUNCTION__,*str);
			return -1;
		}
		if(*str=='.')
		{
			if(haspoint==0)
				haspoint=1;
			else //已经有了
			{
				fprintf(stderr,"[%s %d] %s error,reason: mutiple points\n",__FILE__,__LINE__,__FUNCTION__);
				return -1;
			}
		}
		str++;
	}
	if(haspoint==1)
	{
		if(str[-1]<'0' || str[-1]>'9')
		{
			fprintf(stderr,"[%s %d] %s error,reason: point is the last char\n",__FILE__,__LINE__,__FUNCTION__);
			return -1;
		}
	}
	return 0;
}

/*
 * 作用:把ln的前置0去掉,不影响ln的数值,只是方便一些操作
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	无
 */
void ln_stripleadingzero(ln n)
{
	cell p;
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;
	}
	p=n->msd;
	while(p->num==0 && p != n->lsd)
		p=p->lcell;
	n->msd=p;
	return;
}

/*
 * 作用:把ln的后置0节点去掉,增加指数
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	无
 */
void ln_stripendingzero(ln n)
{
	cell p;
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;
	}
	p=n->lsd;
	while(p->num==0 && p != n->msd)
	{
		p=p->hcell;
		n->power+=DIGIT_NUM;
	}
	n->lsd=p;
	return;
}

/*
 * 作用:获取ln的整数部分的结尾0的个数
 * 副作用:使用ln_stripleadingzero()把ln整数部分前置0去掉
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	成功:返回个数
 * 	失败:-1
 */
int ln_endingzeronum(ln n)
{
	cell p;
	int i,j;
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;
	}
	
	//去除前置0
	ln_stripleadingzero(n);
	if(n->msd->num==0)  //整数部分是0
		return 0;

	i=0;
	p=n->lsd;
	while(1)
	{
		if(p->num==0)
			i+=DIGIT_NUM;
		else
		{
			j=10;
			while(p->num%j==0)
			{
				i++;
				j*=10;
			}
			return i;
		}
		p=p->hcell;
	}
	return i;
}

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
ln ln_fix(ln n,int precision,divide_mode mode)
{
	int pointnum;
	int power;
	cell p;
	//验证参数
	if(ln_checknull(n)!=0)
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
	//去除前置0
	ln_stripleadingzero(n);
	//去除后置0
	ln_adjustpower(n,ln_endingzeronum(n));

	//获取最低节点的小数点位数
	pointnum=ln_pointnum(n,n->lsd);

	
	//大于pointnum,直接返回
	if(precision >=pointnum)
		return n;
	
	//只存在一个节点
	if(n->lsd==n->msd)
	{
		p=n->lsd;
		power=power10(pointnum-precision);
		if(mode==trunc_res) //截断
			p->num-=p->num%power;
		else
		{
			if(p->num%power>=5*power/10)
				p->num=(p->num/power+1)*power;
			else
				p->num-=p->num%power;
		}

	}
	else
	{
		p=n->lsd;
		while(pointnum>precision)
		{
			p=p->hcell;
			pointnum-=DIGIT_NUM;
		}
		if(pointnum==precision) //现在p所指的节点精度就是precision
		{
			if(mode==round_res && p->lcell->num>=UNIT/2) //四舍五入
				p->num++;
		}
		else
		{
			p=p->lcell;
			power=power10(DIGIT_NUM+pointnum-precision);
			if(mode==trunc_res) //截断
			{
				p->num-=p->num%power;
			}
			else
			{
				if(p->num%power>=5*power/10)
					p->num=(p->num/power+1)*power;
				else
					p->num-=p->num%power;
			}
		}
		//p后面有效节点被舍去,因此重置为0
		if(p!=n->lsd)	
		{
			do	
			{
				p=p->lcell;
				p->num=0;
			}
			while(p!=n->lsd);
		}
	}
	//有可能四舍五入导致溢出,需要做调整
	p=n->lsd;
	while(1)
	{
		if(p->num>=UNIT)
		{
			p->num-=UNIT;
			if(p==n->msd)
			{
				if(ln_addcell(n,INIT_SIZE) !=NULL)
				{
					fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
					return NULL;
				}
				n->msd=p->hcell;
				n->msd->num=1;
				break;
			}
			else
			{
				p=p->hcell;
				p->num+=1;
			}

		}
		else
			break;
	}
	return n;
}

/*
 * 作用:把ln的有效节点的num转负
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	成功:处理后的ln
 * 	失败:返回NULL
 */
ln ln_invcell(ln n)
{
	cell p;
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}
	p=n->msd;
	while(p!=n->lsd)
	{
		p->num=-(p->num);
		p=p->lcell;
	}
	p->num=-(p->num);
	return n;
}

/*
 * 作用:调整指数部分
 * 参数:
 *	n:要处理的ln
 * 	inc_power:指数增量 >0 增加指数,把整数部分结尾的0去掉 <0 减少指数,增加整数部分结尾的0
 * 返回值:
 * 	成功:ln
 * 	失败:返回NULL
 */
ln ln_adjustpower(ln n,int inc_power)
{
	int a;
	int res,carry;
	int zeronum;
	cell p;

	//验证参数
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//如果n是0,让它随便设置指数,反正都一样
	if(ln_cmp_int(n,0)==0)
	{
		//设置新的指数
		n->power+=inc_power;  
		return n;
	}

	if(inc_power==0) //指数不变
		return n;
	if(inc_power<0) //减少指数
	{
		//设置新的指数
		n->power+=inc_power;  
		//转正
		inc_power*=-1;  
		
		//先乘上剩余部分的0
		a=power10(inc_power%DIGIT_NUM);
		p=n->lsd;
		res=0;
		carry=0;
		while(1)
		{
			res=a*p->num+carry;
			p->num=res%UNIT;
			carry=res/UNIT;
			if(p==n->msd)
			{
				if(carry>0) //存在进位
				{
					if(n->lsd->lcell ==n->msd) //节点不够
					{
						if(ln_addcell(n,INIT_SIZE) ==NULL) //分配失败
						{
							fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
							return NULL;
						}
					}
					p->hcell->num=carry;
					n->msd=p->hcell;
				}
				break;
			}
			p=p->hcell;
		}

		inc_power-=inc_power%DIGIT_NUM;
		while(inc_power>0)
		{
			if(n->lsd->lcell !=n->msd) //还有多余的节点，利用它
			{
				n->lsd=n->lsd->lcell;
				n->lsd->num=0;
				inc_power-=DIGIT_NUM;
			}
			else //一次性分配剩下的节点
			{
				if(ln_addcell(n,inc_power/DIGIT_NUM)==NULL)
				{
					fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
					return NULL;
				}
				n->lsd=n->msd->hcell;
				break;
			}
		}
	}
	else
	{
		//获取结尾0的个数
		zeronum=ln_endingzeronum(n);
		if(zeronum==-1)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_endingzeronum fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		//不能去掉这么多0
		if(inc_power>zeronum)
		{
			fprintf(stderr,"[%s %d] %s error,reason: inc_power too large (%d)\n",__FILE__,__LINE__,__FUNCTION__,inc_power);
			return NULL;
		}
		//设置新的指数
		n->power+=inc_power;  
		
		//先处理结尾的0节点
		p=n->lsd;
		while(p->num==0 && inc_power>=DIGIT_NUM)
		{
			inc_power-=DIGIT_NUM;
			p=p->hcell;
		}
		n->lsd=p;

		//除以剩下的0
		if(inc_power==0)
			return n;

		a=power10(inc_power);
		p=n->msd;
		res=0;
		carry=0;
		while(1)
		{
			res=p->num+carry*UNIT;
			p->num=res/a;
			carry=res%a;
			if(p==n->lsd)
				break;
			p=p->lcell;
		}
	}
	return n;
}

/*
 * 作用:把字符串转换为ln
 * 参数:
 *	n:要赋值的ln,如果为NULL则构造一个新ln
 *	str:要转换的字符串
 * 返回值:
 * 	成功:返回ln
 * 	失败:NULL
 */
ln str2ln(ln n,const char* str)
{
	int i;
	const char *lastdigit,*point;
	cell p;

	//创建一个ln
	if(n==NULL)
	{
		n=ln_creat(INIT_SIZE);
		if(n==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
	}
	else
	{
		if(ln_checknull(n) !=0)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
	}

	//str格式验证
	if(ln_checkstr(str) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checkstr fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//设置符号
	if(*str=='-')
		n->sign=-1;
	else
		n->sign=1;

	//设置变量
	lastdigit=str+strlen(str)-1;	
	point=strstr(str,".");

	//清除符号
	if(*str=='+' || *str=='-')
		str++;
	//清除起始的0
	while(str != lastdigit && *str=='0')
		str++;
	//碰到0.XXX的情况,回退
	if(*str=='.')
		str--;

	//清除小数点后终结的0
	if(point !=NULL) //存在小数点	
	{
		while(*lastdigit=='0')
			lastdigit--;
		if(*lastdigit=='.') //小数点后全是0 其实是整数
		{
			lastdigit--;
			point=NULL;
		}
	}

	//求出指数
	n->power=0;
	if(point !=NULL) //存在小数点	
		n->power=point-lastdigit;
	else if(*lastdigit=='0' && lastdigit !=str) //以0结尾的整数 (不是0本身)
	{
		while(*lastdigit=='0')  
		{
			lastdigit--;
			n->power++;
		}
	}

	i=1;
	p=n->lsd;
	p->num=0;

	if(point !=NULL) //存在小数点	
	{
		//跳过初始0,直奔有效数字
		if(*str=='0' && *(str+1)=='.')
			str+=2;
		while(*str =='0')
			str++;
	}

	while(lastdigit>=str) //从最低位向上构造digit部分
	{
		if(*lastdigit!='.')
		{
			p->num+=(*lastdigit-'0')*i;
			lastdigit--;
			if(i*10==UNIT)
			{
				if(lastdigit>=str)
				{
					i=1;
					if(p->hcell==n->lsd) //节点已经用完,分配新节点
					{
						if(ln_addcell(n,(lastdigit-str+1)/DIGIT_NUM+1)==NULL)
						{
							fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
							return NULL;
						}
					}
					p=p->hcell;
					p->num=0;
				}
				else
					break;
			}
			else 
				i*=10;
		}
		else
			lastdigit--;
	}
	n->msd=p;
	return n;
}

/*
 * 作用:把ln转换为字符串
 * 副作用:使用ln_stripleadingzero()把ln整数部分前置0去掉,使用ln_adjustpower()把ln整数部分后置0放入指数
 * 参数:
 *	n:要处理的ln
 * 返回值:
 * 	成功:返回ln的字符串表示(需要用free释放)
 * 	失败:NULL
 */
char* ln2str(ln n)
{
	extern int errno;
	cell a;
	char *p,*str;
	int size;
	int i,power;
	int cellnum;
	int zeronum;
	int digitnum;

	//检查参数
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//剔除整数部分前置0
	ln_stripleadingzero(n);
	zeronum=ln_endingzeronum(n);
	if(zeronum==-1)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_endingzeronum fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//剔除整数部分结尾0
	if(ln_adjustpower(n,zeronum)==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_adjustpower fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//如果n为0
	if(ln_cmp_int(n,0)==0)
	{
		str=strdup("0");
		if(str==NULL)
		{
			fprintf(stderr,"[%s %d] %s strdup error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
			return NULL;			
		}
		return str;
	}

	//获取整数部分结尾的0个数
	if(n->power>=0) //整数的显示
	{
		cellnum=ln_cellnum(n);
		//分配空间
		str=(char*)malloc(cellnum*DIGIT_NUM+n->power+10);
		if(str==NULL)
		{
			fprintf(stderr,"[%s %d] %s malloc error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
			return NULL;			
		}

		p=str;
		//打印符号
		if(n->sign==-1)
			*p++='-';

		//显示最高位节点
		a=n->msd;
		i=UNIT/10;
		while(i)
		{
			while(a->num<i)
				i/=10;
			*p++=(a->num/i)%10+'0';
			i/=10;
		}

		while(a!=n->lsd)
		{
			a=a->lcell;
			i=UNIT/10;
			while(i)
			{
				*p++=(a->num/i)%10+'0';
				i/=10;
			}
		}
		for(i=0;i<n->power;i++)
			*p++='0';
		*p='\0';
	}
	else //小数的显示
	{
		 //先计算需要的空间
		digitnum=ln_digitnum(n);
		if(digitnum==-1)
		{
			fprintf(stderr,"[%s %d] %s ln_digitnum error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
			return NULL;			
		}

		power=-n->power;
		if(digitnum>power)
			size=digitnum+1+1;
		else
			size=power+2+1;
		
		if(n->sign==-1) //多一个'-'字符空间
			size++;

		//分配空间
		str=(char*)malloc(size);
		if(!str)
		{
			fprintf(stderr,"[%s %d] %s malloc error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
			return NULL;			
		}
		p=str+size-1;  //p指向字符串尾部
		*p--='\0';

		a=n->lsd;
		i=1;
		while(a!=n->msd)
		{
			*p--=(a->num/i)%10+'0';
			i*=10;
			if(i==UNIT)
			{
				i=1;
				a=a->hcell;
			}
			power--;
			if(power==0)
				*p--='.';
		}
		
		//处理最高节点
		i=1;
		while(i<=a->num)
		{
			*p--=(a->num/i)%10+'0';
			power--;
			if(power==0)
				*p--='.';
			i*=10;
		}
		
		//剩下的指数用0填充
		while(power>0)
		{
			*p--='0';
			power--;
			if(power==0)
				*p--='.';
		}


		if(*(p+1)=='.')
			*p--='0';
		if(n->sign==-1)
			*str='-';
	}
	return str;
}

/*
 * 作用:打印ln的值
 * 参数:
 *	n:要输出的ln
 * 返回值:
 * 	无
 */
void ln_output(ln n)
{
	char* p;
	p=ln2str(n);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;			
	}
	puts(p);
	free(p);
	return;			
}


/*
 *	该库里面包含ln的构造及释放的函数，以及转换函数等辅助函数
 *	
 */
#include "ln_helper.h"

/*
 * 作用:创建ln
 * 参数:
 * 	cellnum:ln的初始节点数
 * 返回值:
 * 	成功:ln
 * 	失败:NULL
 */
ln ln_creat(int cellnum) 
{
	extern int errno;
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
	n->zero=0;
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

	if(n->sign !=0 && n->sign !=1) //非法符号
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
	printf("ln_addr=%d zero=%d \n",(int)n,n->zero);
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
	if(new_value<0)
	{
		n->sign=0;
		new_value=-new_value;
	}
	else
		n->sign=1;
	n->zero=0;
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
	return n;
}

/*
 * 作用:增加ln的节点,新节点在lsd于msd之间
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
 * 作用:获取ln从lsd截止到q节点处的节点个数(包括q)
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
	a->zero=b->zero;
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
		n->sign=0;
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
	n->zero=0;
	if(point !=NULL) //存在小数点	
		n->zero=point-lastdigit;
	else if(*lastdigit=='0' && lastdigit !=str) //以0结尾的整数 (不是0本身)
	{
		while(*lastdigit=='0')  
		{
			lastdigit--;
			n->zero++;
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
	char *p,*str,*head,*tail,*tail2;
	size_t size=0,digitnum=0;
	int i,zero;
	int cellnum;

	//检查参数
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	ln_stripleadingzero(n);
	if(n->zero>=0) //整数的显示
	{
		cellnum=ln_cellnum(n);
		//分配空间
		str=(char*)malloc(cellnum*DIGIT_NUM+n->zero+10);
		if(!str)
		{
			fprintf(stderr,"[%s %d] %s malloc error,reason: %s\n",__FILE__,__LINE__,__FUNCTION__,strerror(errno));
			return NULL;			
		}

		a=n->msd;
		if(a->num==0)
		{
			str[0]='0';
			str[1]='\0';
			return str;
		}

		p=str;
		//打印符号
		if(n->sign==0)
			*p++='-';

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
			if(a->num==0)
			{
				for(i=0;i<DIGIT_NUM;i++)
					*p++='0';
			}
			else
			{
				i=UNIT/10;
				while(i)
				{
					*p++=(a->num/i)%10+'0';
					i/=10;
				}
			}
		}
		for(i=0;i<n->zero;i++)
			*p++='0';
		*p='\0';
		return str;
	}
	/*
	else
	{
		//先计算需要的空间
		zero=-n->zero;
		p=n->lcsd;
		i=UNIT/10;
		while(i)
		{
			while(p->num<i)
				i/=10;
			digitnum++;
			i/=10;
		}
		i=ln_nodenum(n);
		digitnum+=(i-1)*DIGIT_NUM;
		if(zero<digitnum)
			size=digitnum+1;
		else 
			size=2+zero;
		if(n->sign==0)
			size++;

		str=(char*)malloc(size+1);
		assert(str);
		tail=str+size;
		tail2=tail-1;
		head=str;
		*tail--='\0';
		if(n->sign==0)
		{
			*head++='-';
			size--;
		}
		p=n->lcsd;
		i=1;
		if(zero<digitnum) //小数点在中间
		{
			size-=zero+1;
			while(zero) //输出小数点后半部分
			{
				*tail--=(p->num/i)%10+'0';
				i*=10;
				if(i==UNIT)
				{
					i=1;
					p=p->hcell;
				}
				zero--;
			}
			*tail--='.';
			while(size) //输出小数点前半部分
			{
				*tail--=(p->num/i)%10+'0';
				i*=10;
				if(i==UNIT)
				{
					i=1;
					p=p->hcell;
				}
				size--;
			}
		}
		else //小数点在外面
		{
			*head++='0';
			*head++='.';
			zero-=digitnum;
			while(digitnum)
			{
				*tail--=(p->num/i)%10+'0';
				i*=10;
				if(i==UNIT)
				{
					i=1;
					p=p->hcell;
				}
				digitnum--;
			}
			while(zero)
			{
				*head++='0';
				zero--;
			}
		}
		//去除结尾0
		while(*tail2=='0')
			tail2--;
		if(*tail2=='.')
			*tail2='\0';
		else
			*(tail2+1)='\0';
		
		return str;
	}
*/
	return str;
}
#ifdef cyy


int ln_get_cellnum(ln n,cell q)
{
	int i;
	cell p=n->lcsd;
	if(p->num<10)
		i=1;
	else if(p->num<100)
		i=2;
	else if(p->num<1000)
		i=3;
	else
		i=4;
	if(p!=q)
	{
		while(p!=q)
		{
			p=p->hcell;
			i+=4;
		}
		i-=4;
		p=q;
		if(p->num<10)
			i+=1;
		else if(p->num<100)
			i+=2;
		else if(p->num<1000)
			i+=3;
		else
			i+=4;
	}
	return i;
}









//打印ln
void print_ln(ln n)
{
	char *p=ln2str(n);
	puts(p);
	free(p);
}




//截取n的digit部分m个节点,如果m过大则n不变
void ln_trunc(ln n,int m)
{
	assert(m>0);
	int i=1;
	cell p=n->lcsd;
	while(i!=m&&p!=n->lcsd)
	{
		p=p->hcell;
		i++;
	}
	n->lcsd=p;
}
//减少指数部分,多余的0放到整数部分
ln ln_setzero(ln n,int zero)
{
	assert(n);
	assert(n->zero>=zero);
	int a,b;
	int i,j;
	cell d=NULL;
	a=n->zero-zero;

	n->zero=zero;
	if(a==0)
		return n;
	b=a%DIGIT_NUM;
	j=1;
	for(i=0;i<b;i++)
		j*=10;
	if(j>1)
	{
		ln_multiply_num(n,j,firstln);
	}
	while(a>=DIGIT_NUM)
	{
		d=(cell)malloc(sizeof(struct _cell));
		assert(d);
		d->num=0;
		d->hcell=n->lcsd;
		d->lcell=n->lcsd->lcell;
		n->lcsd->lcell->hcell=d;
		n->lcsd->lcell=d;
		n->lcsd=d;
		a-=DIGIT_NUM;
	}
	return n;
}

#endif

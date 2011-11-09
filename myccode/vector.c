/*
 * 该程序为实现C语言的向量有关函数的定义
 */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "vector.h"

vector vect_create(ctypes type)
{
	vector vector = malloc(sizeof(*vector));//分配新空间
	if(vector == NULL) //空间分配失败
		return NULL;
	//根据类型获取元素字节数
	if(type==CHAR)
		vector->element_size=sizeof(char);
	else if(type==SHORT)
		vector->element_size=sizeof(short);
	else if(type==INT)
		vector->element_size=sizeof(int);
	else if(type==LONG)
		vector->element_size=sizeof(long);
	else if(type==FLOAT)
		vector->element_size=sizeof(float);
	else if(type==DOUBLE)
		vector->element_size=sizeof(double);
	else 
	{
		free(vector);
		return NULL;
	}
	vector->type=type;
	return _vect_create(vector);
}


vector vect_create_str(const char* fmt,...)
{
	int i;
	char str[10001]; //必须保证输出的字符串不超过这个长度，一般情况下是足够大的
	va_list ap;
	va_start(ap, fmt);
	i = vsnprintf(str,10000,fmt,ap);//n是成功输入的字符数
	va_end(ap);
	if (i < 0) //解析失败
		return NULL;
	vector vector = malloc(sizeof(*vector));//分配新空间
	if(vector == NULL) //空间分配失败
		return NULL;
	//根据类型获取元素字节数
	vector->element_size=sizeof(char);
	vector->type=CHAR;
	if(! _vect_create(vector))
		return NULL;
	i=0;
	//设置向量值
	while(str[i])
		vect_pushelm(vector,str[i++]);
	return vector;
}

char*	vect2str(vector vector)
{
	int i;
	if(vector->type !=CHAR)
		return NULL;
	char *p=(char*)malloc(vector->size+1);
	if(!p)
		return NULL;
	//复制
	for(i=0;i<vector->size ;i++)
		p[i]=vect_getcharelmat(vector,i);
	p[i]='\0';
	return p;
}

vector	vect_strcat(vector vector,const char* fmt,...)
{
	int i;
	char str[10001]; //必须保证输出的字符串不超过这个长度，一般情况下是足够大的
	va_list ap;
	va_start(ap, fmt);
	i = vsnprintf(str,10000,fmt,ap);//n是成功输入的字符数
	va_end(ap);
	if (i < 0) //解析失败
		return vector;

	char *p=str;
	if(vector->type !=CHAR)
		return vector;

	//找出已经保存的空字节
	for(i=0;i<vector->size;i++)
	{
		if(vect_getcharelmat(vector,i)=='\0')
			break;
	}

	//开始拼接字符串
	for(;i<vector->size && *p ;i++)
		vect_setelmat(vector,i,*p++);
	while(*p)
		vect_pushelm(vector,*p++);
	return vector;
}

void   vect_uniqstr(vector vector)
{
	int i,j;	//迭代变量
	if(!vector)
		return;
	if(vector->type !=STRING)
		return;
	for(i=0;i<vect_getlength(vector)-1;i++)
	{
		j=i+1;
		while(j<vect_getlength(vector))
		{
			if(strcmp(vect_getstrelmat(vector,i),vect_getstrelmat(vector,j))==0 )
			{
				free(vect_getstrelmat(vector,j));
				vect_delelmat(vector,j);
			}
			else 
				j++;
		}
	}
	return;
}


vector vect_getsamestr(vector vector1)
{
	int i,j,k;	//迭代变量
	int flag;	//非空串
	vector vector2;	//保存vector1中重复的非空串

	if(!vector1)
		return NULL;
	if(vector1->type !=STRING)
		return NULL;
	vector2=vect_create_strpointer(NULL);
	if(!vector2)
		return NULL;

	for(i=0;i<vect_getlength(vector1);i++)
	{
		for(j=i+1;j<vect_getlength(vector1);j++)
		{
			//重复的非空串
			if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector1,j))==0 && vect_getstrelmat(vector1,i)[0])
			{
				flag=1;
				for(k=0;k<vect_getlength(vector2);k++) //是否已经放进去了？
				{
					if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector2,k))==0) //已经放进去了
					{
						flag=0;
						break;
					}
				}
				if(flag)
					vect_pushelm(vector2,vect_getstrelmat(vector1,i));
			}
		}
	}
	return vector2;
}




vector vect_getuniqstr(vector vector1)
{
	int i,j,k;	//迭代变量
	int flag;	//非空串
	vector vector2;	//保存vector1中重复的非空串

	if(!vector1)
		return NULL;
	if(vector1->type !=STRING)
		return NULL;
	vector2=vect_create_strpointer(NULL);
	if(!vector2)
		return NULL;

	for(i=0;i<vect_getlength(vector1);i++)
	{
		for(j=i+1;j<vect_getlength(vector1);j++)
		{
			//重复的非空串
			if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector1,j))==0 && vect_getstrelmat(vector1,i)[0])
			{
				flag=1;
				for(k=0;k<vect_getlength(vector2);k++) //是否已经放进去了？
				{
					if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector2,k))==0) //已经放进去了
					{
						flag=0;
						break;
					}
				}
				if(flag)
					vect_pushelm(vector2,vect_getstrelmat(vector1,i));
			}
		}
	}
	return vector2;
}


int vect_hassamestr(vector vector)
{
	int i,j;	//迭代变量

	if(!vector)
		return -1;
	if(vector->type !=STRING)
		return -1;

	for(i=0;i<vect_getlength(vector);i++)
	{
		for(j=i+1;j<vect_getlength(vector);j++)
		{
			//重复的非空串
			if(strcmp(vect_getstrelmat(vector,i),vect_getstrelmat(vector,j))==0 && vect_getstrelmat(vector,i)[0])
			{
				return 1;
			}
		}
	}
	return 0;
}

int vect_hasstr(vector vector,const char* s)
{
	int i;	//迭代变量

	if(!vector)
		return -1;
	if(vector->type !=STRING)
		return -1;

	for(i=0;i<vect_getlength(vector);i++)
	{
		if(strcmp(vect_getstrelmat(vector,i),s)==0)
			return 1;
	}
	return 0;
}


int vect_delstr(vector vector, const char* s)
{

	int i;	//迭代变量

	if(!vector)
		return -1;
	if(vector->type !=STRING)
		return -1;

	for(i=0;i<vect_getlength(vector);i++)
	{
		if(strcmp(vect_getstrelmat(vector,i),s)==0)
		{
			free(vect_delstrelmat(vector,i));
			i--;
		}
	}
	return 0;
}







char* vect_joinstr(vector vector1,const char* limit)
{
	int i;			//迭代变量
	vector vector2;		//保存连接后的字符串
	char* p;		//返回的结果
	if(!vector1)
		return NULL;
	if(vector1->type !=STRING)
		return NULL;
	vector2=vect_create_str("");
	if(!vector2)
		return NULL;

	for(i=0;i<vect_getlength(vector1);i++)
	{
		if(vect_getlength(vector2) ==0)
			vect_strcat(vector2,"%s",vect_getstrelmat(vector1,i));
		else
			vect_strcat(vector2,"%s%s",limit,vect_getstrelmat(vector1,i));
	}
	p=vect2str(vector2);
	free(vector2);
	return p;
}


void   vect_sortstr(vector vector)
{
	int i,j;	//迭代变量
	char *p,*q;
	if(!vector)
		return;
	if(vector->type !=STRING)
		return;
	for(i=0;i<vect_getlength(vector)-1;i++)
	{
		p=vect_getstrelmat(vector,i);
		for(j=i+1;j<vect_getlength(vector);j++)
		{
			q=vect_getstrelmat(vector,j);
			if(strcmp(p,q)>0)	//交换位置
			{
				vect_setelmat(vector,i,q);
				vect_setelmat(vector,j,p);
				p=vect_getstrelmat(vector,i);
			}
		}
	}
	return;
}


int vect_strcmp(const vector vector1,const vector vector2)
{
	int ret = 0;
	vector vector_temp1;
	vector vector_temp2;
	if(!vector1 || !vector2) //空指针 出错
		return -1;
	if(vect_getelmtype(vector1) !=STRING || vect_getelmtype(vector2) !=STRING) //保存的不是字符串 出错
		return -1;
	if(vect_getlength(vector1) !=vect_getlength(vector2))	//保存的字符串个数不一致 肯定不相等
		return 1;	
	//拷贝副本
	vector_temp1=vect_copy(vector1);
	if(!vector_temp1)
		return -1;
	vector_temp2=vect_copy(vector2);
	if(!vector_temp2)
		return -1;
	vect_sortstr(vector_temp1);
	vect_sortstr(vector_temp2);

	ret = vect_strcmp_order(vector_temp1, vector_temp2);
	vect_free(vector_temp1);
	vect_free(vector_temp2);
	return (ret);
}


int vect_strcmp_order(const vector vector1,const vector vector2)
{
	int i;
	if(!vector1 || !vector2) //空指针 出错
		return -1;
	if(vect_getelmtype(vector1) !=STRING || vect_getelmtype(vector2) !=STRING) //保存的不是字符串 出错
		return -1;
	if(vect_getlength(vector1) !=vect_getlength(vector2))	//保存的字符串个数不一致 肯定不相等
		return 1;	

	for(i=0;i<vect_getlength(vector1);i++)
	{
		if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector2,i)) !=0)
			return 1;
	}
	return 0;
}








vector	vect_strcpy(vector vector,const char* fmt,...)
{
	int i;
	char str[10001]; //必须保证输出的字符串不超过这个长度，一般情况下是足够大的
	char *p=str;
	va_list ap;
	va_start(ap, fmt);
	i = vsnprintf(str,10000,fmt,ap);//n是成功输入的字符数
	va_end(ap);
	if (i < 0) //解析失败
		return vector;
	if(!vector || vector->type !=CHAR)
		return vector;
	vector->size=0;
	while(*p)
		vect_pushelm(vector,*p++);
	return vector;
}

vector vect_create_struct(int element_size)
{
	vector vector = malloc(sizeof(*vector));//分配新空间
	if(vector == NULL) //空间分配失败
		return NULL;
	vector->type=STRUCT;
	vector->element_size=element_size;
	return _vect_create(vector);
}

vector vect_create_pointer()
{
	vector vector = malloc(sizeof(*vector));//分配新空间
	if(vector == NULL) //空间分配失败
		return NULL;
	vector->type=POINTER;
	vector->element_size=sizeof(char*);
	return _vect_create(vector);
}

vector vect_create_strpointer(char* str)
{
	vector vector = malloc(sizeof(*vector));//分配新空间
	if(vector == NULL) //空间分配失败
		return NULL;
	vector->type=STRING;
	vector->element_size=sizeof(char*);
	_vect_create(vector);

	if(str)	//如果有传入字符串 
		return vect_pushelm(vector,str);
	else
		return vector;
}





vector _vect_create(vector vector)
{
	vector->size=0;
	vector->capacity = INIT_CAPACITY;
	vector->data=malloc(vector->element_size * vector->capacity);//分配新空间
	if(vector->data ==NULL)
	{
		free(vector);
		return NULL;
	}
	memset(vector->data,0,vector->element_size * vector->capacity); //初始化
	vector->del_data=malloc(vector->element_size);//分配新空间
	if(vector->del_data==NULL)
	{
		free(vector->data);
		free(vector);
		return NULL;
	}
	memset(vector->del_data,0,vector->element_size); //初始化
	return vector;
}

vector vect_copy(const vector vector1)
{
	int i;
	char *p,*q;
	vector vector2;
	if(vector1==NULL)
		return NULL;

	vector2 = malloc(sizeof(*vector2));//分配新空间
	if(vector2 == NULL) //空间分配失败
		return NULL;
	vector2->type=vector1->type;
	vector2->capacity=vector1->capacity;
	vector2->element_size=vector1->element_size;
	vector2->size=vector1->size;
	vector2->data=malloc(vector1->element_size*vector1->capacity);//分配新空间 如果内存不够会失败
	if(vector2->data==NULL)
	{
		free(vector2);
		return NULL;

	}

	vector2->del_data=malloc(vector1->element_size);//分配新空间 如果内存不够会失败
	if(vector2->del_data==NULL)
	{
		free(vector2->data);
		free(vector2);
		return NULL;
	}
	if(vect_getelmtype(vector1) !=STRING)
	{
		memcpy(vector2->data,vector1->data,vector1->element_size*vector1->capacity);
		memcpy(vector2->del_data,vector1->del_data,vector1->element_size);
	}
	else	//字符串进行深层次复制
	{
		for(i=0;i<vect_getlength(vector1);i++)
		{
			p=vect_getstrelmat(vector1,i);
			q=strdup(p);
			if(!q)
			{
				free(vector2->del_data);
				free(vector2->data);
				free(vector2);
			}
			vect_setelmat(vector2,i,q);
		}
		p=*((char**) (vector2->del_data));
		if(p)
		{
			q=strdup(p);
			if(!q)
			{
				free(vector2->del_data);
				free(vector2->data);
				free(vector2);
			}
			memcpy(vector2->del_data,&q,vector1->element_size);
		}
	}
	return vector2;
}


void vect_free(vector vector)
{
	int i;
	if (vector == NULL) 
		return;

	if(vector->type==STRING)	//释放每个字符串的空间
	{
		for(i=0;i<vect_getlength(vector);i++)
			free(vect_getstrelmat(vector,i));
	}
	free(vector->del_data);
	free(vector->data);
	free(vector);
	vector=NULL;
	return;
}


void vect_clear(vector vector)
{
	int i;
	if (vector == NULL) 
		return;
	
	if(vector->type==STRING)
	{
		for(i=0;i<vect_getlength(vector);i++)
			free(vect_getstrelmat(vector,i));
	}
	memset(vector->data,0,vector->capacity*vector->element_size);
	vector->size=0;
	return;
}

int vect_getlength(vector vector)
{
	if(vector==NULL)
		return 0;
	return vector->size;
}

int vect_getelmtype(vector vector)
{
	if(vector==NULL)
		return -1;
	return vector->type;
}



vector vect_setelmat(vector vector,int index,...)
{ 
	int real_index;
	char *str;
	if(vector->size==0)
		return vector;


	//确定下标
	if(index>=0)
	{
		if(index>=vector->size)
			real_index=vector->size-1;
		else
			real_index=index;
	}
	else
	{
		real_index=vector->size+index;
		if(real_index<0)
			real_index=0;
	}

	//获取参数值存入data[real_index]
	if(vector->type ==STRUCT || vector->type ==POINTER) //结构体或指针
		memcpy(vector->data+real_index*vector->element_size,((char*)&index)+sizeof(int), vector->element_size); 
	else if(vector->type ==STRING)		//字符串
	{
		str=strdup(*(char**)(((char*)&index)+sizeof(int)));
		if(!str)	//失败
			return vector;
		memcpy(vector->data+real_index*vector->element_size,&str, vector->element_size); 
	}
	else //基本数据类型
	{
		va_list ap;
		va_start(ap,index);
		vect_getarg(vector,(vector->data+real_index*vector->element_size),ap); 
		va_end(ap);
	}
	return vector;
}

void* vect_getelmat(vector vector,int index)
{
	int real_index;
	if(vector->size==0)
		return NULL;
	if(index>=0)
	{
		if(index>=vector->size)
			real_index=vector->size-1;
		else
			real_index=index;
	}
	else
	{
		real_index=vector->size+index;
		if(real_index<0)
			real_index=0;
	}
	return (vector->data+real_index*vector->element_size);
}

vector vect_addelmat(vector vector,int index,...)
{ 
	int real_index;
	int i;
	int oldsize=vector->size;
	char* str=NULL;
	if(vector->type ==STRING)		//字符串,先复制一份副本
	{

		str=strdup(*(char**)(((char*)&index)+sizeof(int)));
		if(!str)	//失败
			return vector;
	}

	if(index>=0)
	{
		if(index>=vector->size) //添加新元素
		{
			real_index=index;
			vector->size=index+1; //增加在尾部
		}
		else  //在原位子上插入新元素
		{
			real_index=index;
			vector->size++;
		}
	}
	else
	{
		real_index=vector->size+index;
		if(real_index<0) //增加在头部
			real_index=0;
		vector->size++;
	}
	vect_checkcap(vector); //检查向量容量是否足够
	//把空隙用零字节填充
	memset(vector->data+oldsize*vector->element_size,0,vector->element_size*(vector->size-oldsize));
	//元素往后移动
	for(i=oldsize-1;i>=index;i--)
		memcpy(vector->data+(i+1)*vector->element_size, vector->data+i*vector->element_size, vector->element_size); 
	//获取参数值存入data[real_index]
	if(vector->type ==STRUCT || vector->type ==POINTER) //结构体或指针
		memcpy(vector->data+real_index*vector->element_size,((char*)&index)+sizeof(int), vector->element_size); 
	else if(vector->type ==STRING)		//字符串
		memcpy(vector->data+real_index*vector->element_size,&str, vector->element_size); 
	else //基本数据类型
	{
		va_list ap;
		va_start(ap,index);
		vect_getarg(vector,(vector->data+real_index*vector->element_size),ap);
		va_end(ap);
	}
	return vector;
}


void* vect_delelmat(vector vector,int index)
{ 
	int real_index;
	int i;
	if(vector->size==0)
		return NULL;
	//确定下标
	if(index>=0)
	{
		if(index>=vector->size)
			real_index=vector->size-1;
		else
			real_index=index;
	}
	else
	{
		real_index=vector->size+index;
		if(real_index<0)
			real_index=0;
	}

	//把要删除的元素放在del_data
	memcpy(vector->del_data, vector->data+real_index*vector->element_size, vector->element_size); 
	//元素往前移动
	for(i=real_index;i<vector->size-1;i++)
		memcpy(vector->data+i*vector->element_size, vector->data+(i+1)*vector->element_size, vector->element_size); 

	vector->size--;
	return vector->del_data;
}

vector vect_checkcap(vector vector)
{
	int newCapacity;
	if( vector->size>=vector->capacity  //超过总容量
	|| (vector->capacity - vector->size)*5 < vector->capacity)  //剩余容量/总容量< 1/5
	{
		newCapacity=(vector->capacity*3)/2;  //旧容量/新容量=2/3
		return vect_setcap(vector,newCapacity);
	}
	else
		return vector;
}

vector vect_setcap(vector vector, int capacity)
{
	if(vector == NULL) 
	{
		return vector;
	}
	if(capacity <= vector->size) //小于等于向量长度
	{
		return vector;
	}
	if (vector->capacity >= capacity)//容量足够用
	{
		vector->capacity = capacity;
		return vector;
	}
	else     //分配新空间
	{	
		vector->capacity = capacity;
		vector->data=realloc(vector->data,vector->element_size*vector->capacity);//分配新空间 如果内存不够会失败
		return vector;
	}
}

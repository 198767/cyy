/***************************
 * 实现向量，该向量可以存储几种基本类型，结构体和指针
 * by：陈源源
 **************************/


#ifndef _VECTOR_H_
#define _VECTOR_H_

#include<stddef.h>

#define INIT_CAPACITY 10  //向量初始容量


//C类型 可用来设置变参类型
typedef enum 
{
	CHAR=1, //char
	SHORT=2, //short 
	INT=3,  //int 
	LONG=4,  //long
	FLOAT=5,  //float
	DOUBLE=6,  //double
	STRUCT=7,  //结构体
	POINTER=8, //指针
	STRING=9, //字符串
}ctypes;





typedef struct 
{
	ctypes 	type; //向量保存的类型
	int 	capacity;  //向量的容量 
	int 	element_size;  //向量元素的字节数
	int 	size;      //向量已保存的元素个数
	void*   data;      //向量的数据
	void*   del_data;      //删除的元素
}* vector;

/*
 *  创建向量，用于保存基本数据类型	
 *  @param type：向量存放的元素类型，必须设置为CHAR，INT，LONG，SHORT，FLOAT，DOUBLE之一，否则函数失败返回NULL
 *  @return：返回新建的向量，出错返回NULL
 */
vector vect_create (ctypes type);

/*
 *  创建向量，用于保存结构体
 *  @return：返回新建的向量，出错返回NULL
 */
vector vect_create_struct(int element_size);

/*
 *  创建向量，用于保存char,要保存的char由格式字符串和后面的参数构成的字符串指定
 *  @param element_size：请用sizeof获取结构体大小传进来
 *  @return：返回新建的向量，出错返回NULL
 */
vector vect_create_str(const char* fmt,...);


/*
 *  创建向量，用于保存指针
 *  @return：返回新建的向量，出错返回NULL
 */
vector vect_create_pointer();

/*
 *  创建向量，用于保存字符串，str非NULL，则把str复制一份保存起来
 *  @return：返回新建的向量，出错返回NULL
 */
vector vect_create_strpointer(char* str);

/*
 *  如果type为char 则把向量中保存的字符添加在vector尾部
 *  否则不做任何操作直接返回vector
 *  @return：返回vector
 */
vector	vect_strcat(vector vector,const char* fmt,...);


/*
 *  如果type为char 则把向量中保存的字符覆盖到vector
 *  否则不做任何操作直接返回vector
 *  @return：返回vector
 */
vector	vect_strcpy(vector vector,const char* fmt,...);





/*
 *  如果type为char 则把向量中保存的字符以字符串的形式返回
 *  否则返回NULL
 *  @return：返回向量保存的字符串，记得free释放
 */

char*	vect2str(vector vector);


/*
 *  如果向量保存的是STRING 则去除重复的字符串
 *  @param vector：要处理的向量
 */
void   vect_uniqstr(vector vector);


/*
 *  判断vector中是否有重复的非空字符串
 *  @param vector：要处理的向量
 *  @return：-1-出错  0-不存在  1-存在
 */
int vect_hassamestr(vector vector);

/*
 *  判断vector中是否存在字符串s，用strcmp进行比较
 *  @param vector：要处理的向量
 *  @return：-1-出错  0-不存在  1-存在
 */
int vect_hasstr(vector vector, const char* s);


/*
 *  返回vector1中的重复的非空字符串
 *  @param vector：要处理的向量
 *  @return：NULL-出错 其它-返回重复字符串，如果无重复，则向量长度为0
 */
vector vect_getsamestr(vector vector1);


/*
 *  判断vector中如果存在字符串s，则删除
 *  @param vector：要处理的向量
 *  @return：-1-出错  0-成功
 */
int vect_delstr(vector vector, const char* s);


/*
 *  如果向量保存的是STRING 把字符串按照limit作为分隔符连接，返回一个新的字符串
 *  @param vector：要处理的向量
 *  @param limit：字符串分隔符
 *  @return：NULL-出错 其它-返回新的字符串 要用free()释放
 */
char* vect_joinstr(vector vector1,const char* limit);

/*
 *  如果向量保存的是STRING 则用strcmp排序 从小到大排
 *  @param vector：要处理的向量
 *  @return：0-相等 1-不相等 -1-出错 参数为NULL或者非STRING就出错
 */
void	vect_sortstr(vector vector);


/*
 *  如果这两个向量保存的都是STRING 比较这两个向量的字符串是否完全相等，无视顺序
 *  @param vector1 vector2：要比较的向量
 *  @return：0-相等 1-不相等 -1-出错 参数为NULL或者非STRING就出错
 */
int 	vect_strcmp(const vector vector1,const vector vector2);

/*
 *  如果这两个向量保存的都是STRING 按顺序比较这两个向量的字符串是否完全相等
 *  @param vector1 vector2：要比较的向量
 *  @return：0-相等 1-不相等 -1-出错 参数为NULL或者非STRING就出错
 */
int 	vect_strcmp_order(const vector vector1,const vector vector2);





/*
 *  如果向量保存类型是char,则把它们以字符串的形式返回
 *  @return：返回新建的向量，出错返回NULL
 */
vector vect_createstr(char* str);


/*
 *  复制向量 vector1 
 *  @param vector  ：需要复制的向量，如果为NULL则返回NULL
 *  @return：返回新的向量，出错返回NULL
 *  注意：如果向量存放的是STRING，则连指向的字符串都自动复制一份，即进行深层复制
 *
 */
vector vect_copy(const vector vector1);

/*
 *  清空向量保存的元素，调用完向量什么都没保存 长度为0 ,以前保存的STRING会free掉
 *  @param vector  ：需要清空的向量，如果为NULL则直接返回
 *  @remark        ：把向量内部分配的空间重置为零字节
 */
void vect_clear(vector vector);


/*
 *  释放向量vector分配的空间和里面每个元素分配的空间，并把vector重置为NULL 
 *  @param vector  ：需要释放的向量，如果为NULL则直接返回
 *  @remark        ：把向量内部分配的空间释放
 */
void vect_free(vector vector);

/*
 *  获取向量长度
 *  @param vector：向量，如果为NULL则返回0
 *  @return：向量长度
 */
int vect_getlength(vector vector);


/*
 *  获取元素类型
 *  @param vector：向量，如果为NULL则返回-1
 *  @return：元素类型，参考ctypes枚举
 */
int vect_getelmtype(vector vector);


/*
 *  获取向量在第index位的值，存放在返回的指针指向的地址；若是当前向量的长度为0，那就直接返回空指针
 *  @param vector ：要获取的向量
 *  @param index：要取值的下标。向量的下标从0开始计算
 *	当index大于0时：
 *  		如果index超过最大下标,则获取最后一个元素的值
 *	当index小于0时：
 *  		如果index为-1,则获取最后一个元素的值，-2则获取倒数第二个元素的值。。以此类推，如果index过小,则获取第一个元素的值
 *  
 *  @return ：返回的值是void*类型，要再转为相应的类型指针并解引用 或者使用下面的宏
 *  		注意：如果当前向量的长度为0，那就直接返回空指针，对其解引用会使程序挂掉
 */
void* vect_getelmat(vector vector,int index);
//设置一些宏定义函数自动转换类型
#define vect_getcharelmat(vector,index)  (*((char*)vect_getelmat(vector,index)))
#define vect_getshortelmat(vector,index)  (*((short*)vect_getelmat(vector,index)))
#define vect_getintelmat(vector,index)  (*((int*)vect_getelmat(vector,index)))
#define vect_getlongelmat(vector,index)  (*((long*)vect_getelmat(vector,index)))
#define vect_getfloatelmat(vector,index)  (*((float*)vect_getelmat(vector,index)))
#define vect_getdoubleelmat(vector,index)  (*((double*)vect_getelmat(vector,index)))
#define vect_getstrelmat(vector,index)  (*((char**)vect_getelmat(vector,index)))
#define vect_getpointerelmat(vector,index)  (*((void**)vect_getelmat(vector,index)))


/*
 *  设置向量在第index位的值为elem,即vector[index]=elem,如果当前向量长度为0，就不设置直接返回
 *  @param vector ：要设置的向量
 *  @param index：要设置的下标。向量的下标从0开始计算
 *	当index大于0时：
 *  		如果index超过最大下标,则设置最后一个元素的值
 *	当index小于0时：
 *  		如果index为-1,则设置最后一个元素的值，-2则设置倒数第二个元素的值。。以此类推，如果index小于最小下标,则设置第一个元素的值
 *  
 *  @param 可变参数：该下标的值，虽然是可变参数，但主要是为了处理通用类型，程序只获取第一个参数，如果是指针，则动态分配空间
 *  @return ：返回传入的向量
 */
vector vect_setelmat(vector vector,int index,...);


/*
 *  设置在第index位插入elem,当前的值往后移
 *  @param vector ：要设置的向量
 *  @param index：要插入的下标。向量的下标从0开始计算
 *	当index大于0时：
 *		如果index>最大下标a则在向量尾部增加新元素
 *		否则就在向量内部的坐标增加新元素，原来的元素往后移动
 *	当index小于0时：
 *  		如果index为-1,则在最后一个元素插值，-2则在倒数第二个元素插值。。以此类推，如果index小于最小下标,则在index=0位置增加一个元素
 *  
 *  @param 可变参数：该下标的值，虽然是可变参数，但主要是为了处理通用类型，程序只获取第一个参数，如果是指针，则动态分配空间
 *  @return ：返回传入的向量
 */
vector vect_addelmat(vector vector,int index,...);


/*
 *  把elem压栈到向量尾部
 *  @param vector ：向量
 *  @param 可变参数：要入栈的元素，虽然是可变参数，但主要是为了处理通用类型，程序只获取第一个参数
 *  @return ：返回传入的向量
 */
#define vect_pushelm(vector,...)  vect_addelmat((vector),(vector)->size,##__VA_ARGS__)



/*
 *  设置在第index位删除值,后面的值往前移，删除的值存放在返回的指针指向的地址；若是当前向量的长度为0，那就直接返回空指针
 *  @param vector ：要设置的向量
 *  @param index：要插入的下标。向量的下标从0开始计算
 *	当index大于0时：
 *  		如果index超过最大下标,则删除最后一个元素的值
 *	当index小于0时：
 *  		如果index为-1,则删除最后一个元素的值，-2则删除倒数第二个元素的值。。以此类推，如果index过小,则删除第一个元素的值
 *  @return ：返回的值是void*类型，要再转为相应的类型指针并解引用，或者使用下面的宏
 *  		注意：如果当前向量的长度为0，那就直接返回空指针，对其解引用会使程序挂掉
 */
void* vect_delelmat(vector vector,int index);
//设置一些宏定义函数自动转换类型
#define vect_delcharelmat(vector,index)  (*((char*)vect_delelmat(vector,index)))
#define vect_delshortelmat(vector,index)  (*((short*)vect_delelmat(vector,index)))
#define vect_delintelmat(vector,index)  (*((int*)vect_delelmat(vector,index)))
#define vect_dellongelmat(vector,index)  (*((long*)vect_delelmat(vector,index)))
#define vect_delfloatelmat(vector,index)  (*((float*)vect_delelmat(vector,index)))
#define vect_deldoubleelmat(vector,index)  (*((double*)vect_delelmat(vector,index)))
#define vect_delstrelmat(vector,index)  (*((char**)vect_delelmat(vector,index)))

/*
 *  把弹出向量最后一个元素
 *  @param vector ：向量
 *  @return ：返回的值是void*类型，要再转为相应的类型指针并解引用，或者使用下面的宏
 *  		注意：如果当前向量的长度为0，那就直接返回空指针，对其解引用会使程序挂掉
 */
#define vect_popelm(vector)  vect_delelmat((vector),(vector)->size-1)
//设置一些宏定义函数自动转换类型
#define vect_popcharelm(vector)  (*((char*)vect_delelmat( (vector),(vector)->size-1     )))
#define vect_popshortelm(vector)  (*((short*)vect_delelmat( (vector),(vector)->size-1    )))
#define vect_popintelm(vector)  (*((int*)vect_delelmat( (vector),(vector)->size-1  )))
#define vect_poplongelm(vector)  (*((long*)vect_delelmat(  (vector),(vector)->size-1  )))
#define vect_popfloatelm(vector)  (*((float*)vect_delelmat( (vector),(vector)->size-1  )))
#define vect_popdoubleelm(vector)  (*((double*)vect_delelmat( (vector),(vector)->size-1  )))
#define vect_popstrelm(vector)  (*((char**)vect_delelmat( (vector),(vector)->size-1  )))





/************************以下为内部函数，用户不应该调用********************/
/*
 *  修改向量的容量
 *
 *  @param vector ：被修改的向量，必须非空，否则直接返回
 *  @param capacity：新容量,如果新容量<=当前向量的长度，则直接返回
 *  @return：返回被修改的向量
 */
vector vect_setcap (vector vector, int capacity);

/*
 *  检查向量的容量是否已经快用完，是的话增加容量
 *  @param vector ：被修改的向量
 *  @return：返回被修改的向量
 *
 */
vector vect_checkcap (vector vector);

/*
 *	分配data,del_data等，设置向量的初始容量为INIT_CAPACITY，是创建向量的辅助函数
 *  @param vector ：被设置的向量
 *  @return：返回被修改的向量
 */
vector _vect_create(vector vector);


//根据vector的类型把参数赋值给p,p是void*指针
#define vect_getarg(vector,p,ap)  				\
{								\
	if(vector->type==CHAR)					\
		*( (char*) (p))=(char)va_arg(ap,int);		\
	else if(vector->type==SHORT)				\
		*( (short*)(p))=(short)va_arg(ap,int);          \
	else if(vector->type==INT)				\
		*( (int*)(p))=(int)va_arg(ap,int);           	\
	else if(vector->type==LONG)				\
		*( (long*)(p))=(long)va_arg(ap,long);           \
	else if(vector->type==FLOAT)				\
		*( (float*)(p))=(float)va_arg(ap,double);       \
	else if(vector->type==DOUBLE)				\
		*( (double*)(p))=(double)va_arg(ap,double);     \
}




#endif  // !_VECTOR_H_


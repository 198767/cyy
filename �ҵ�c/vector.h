/***************************
 * ʵ�����������������Դ洢���ֻ������ͣ��ṹ���ָ��
 * by����ԴԴ
 **************************/


#ifndef _VECTOR_H_
#define _VECTOR_H_

#include<stddef.h>

#define INIT_CAPACITY 10  //������ʼ����


//C���� ���������ñ������
typedef enum 
{
	CHAR=1, //char
	SHORT=2, //short 
	INT=3,  //int 
	LONG=4,  //long
	FLOAT=5,  //float
	DOUBLE=6,  //double
	STRUCT=7,  //�ṹ��
	POINTER=8, //ָ��
	STRING=9, //�ַ���
}ctypes;





typedef struct 
{
	ctypes 	type; //�������������
	int 	capacity;  //���������� 
	int 	element_size;  //����Ԫ�ص��ֽ���
	int 	size;      //�����ѱ����Ԫ�ظ���
	void*   data;      //����������
	void*   del_data;      //ɾ����Ԫ��
}* vector;

/*
 *  �������������ڱ��������������	
 *  @param type��������ŵ�Ԫ�����ͣ���������ΪCHAR��INT��LONG��SHORT��FLOAT��DOUBLE֮һ��������ʧ�ܷ���NULL
 *  @return�������½���������������NULL
 */
vector vect_create (ctypes type);

/*
 *  �������������ڱ���ṹ��
 *  @return�������½���������������NULL
 */
vector vect_create_struct(int element_size);

/*
 *  �������������ڱ���char,Ҫ�����char�ɸ�ʽ�ַ����ͺ���Ĳ������ɵ��ַ���ָ��
 *  @param element_size������sizeof��ȡ�ṹ���С������
 *  @return�������½���������������NULL
 */
vector vect_create_str(const char* fmt,...);


/*
 *  �������������ڱ���ָ��
 *  @return�������½���������������NULL
 */
vector vect_create_pointer();

/*
 *  �������������ڱ����ַ�����str��NULL�����str����һ�ݱ�������
 *  @return�������½���������������NULL
 */
vector vect_create_strpointer(char* str);

/*
 *  ���typeΪchar ��������б�����ַ������vectorβ��
 *  �������κβ���ֱ�ӷ���vector
 *  @return������vector
 */
vector	vect_strcat(vector vector,const char* fmt,...);


/*
 *  ���typeΪchar ��������б�����ַ����ǵ�vector
 *  �������κβ���ֱ�ӷ���vector
 *  @return������vector
 */
vector	vect_strcpy(vector vector,const char* fmt,...);





/*
 *  ���typeΪchar ��������б�����ַ����ַ�������ʽ����
 *  ���򷵻�NULL
 *  @return����������������ַ������ǵ�free�ͷ�
 */

char*	vect2str(vector vector);


/*
 *  ��������������STRING ��ȥ���ظ����ַ���
 *  @param vector��Ҫ���������
 */
void   vect_uniqstr(vector vector);


/*
 *  �ж�vector���Ƿ����ظ��ķǿ��ַ���
 *  @param vector��Ҫ���������
 *  @return��-1-����  0-������  1-����
 */
int vect_hassamestr(vector vector);

/*
 *  �ж�vector���Ƿ�����ַ���s����strcmp���бȽ�
 *  @param vector��Ҫ���������
 *  @return��-1-����  0-������  1-����
 */
int vect_hasstr(vector vector, const char* s);


/*
 *  ����vector1�е��ظ��ķǿ��ַ���
 *  @param vector��Ҫ���������
 *  @return��NULL-���� ����-�����ظ��ַ�����������ظ�������������Ϊ0
 */
vector vect_getsamestr(vector vector1);


/*
 *  �ж�vector����������ַ���s����ɾ��
 *  @param vector��Ҫ���������
 *  @return��-1-����  0-�ɹ�
 */
int vect_delstr(vector vector, const char* s);


/*
 *  ��������������STRING ���ַ�������limit��Ϊ�ָ������ӣ�����һ���µ��ַ���
 *  @param vector��Ҫ���������
 *  @param limit���ַ����ָ���
 *  @return��NULL-���� ����-�����µ��ַ��� Ҫ��free()�ͷ�
 */
char* vect_joinstr(vector vector1,const char* limit);

/*
 *  ��������������STRING ����strcmp���� ��С������
 *  @param vector��Ҫ���������
 *  @return��0-��� 1-����� -1-���� ����ΪNULL���߷�STRING�ͳ���
 */
void	vect_sortstr(vector vector);


/*
 *  �����������������Ķ���STRING �Ƚ��������������ַ����Ƿ���ȫ��ȣ�����˳��
 *  @param vector1 vector2��Ҫ�Ƚϵ�����
 *  @return��0-��� 1-����� -1-���� ����ΪNULL���߷�STRING�ͳ���
 */
int 	vect_strcmp(const vector vector1,const vector vector2);

/*
 *  �����������������Ķ���STRING ��˳��Ƚ��������������ַ����Ƿ���ȫ���
 *  @param vector1 vector2��Ҫ�Ƚϵ�����
 *  @return��0-��� 1-����� -1-���� ����ΪNULL���߷�STRING�ͳ���
 */
int 	vect_strcmp_order(const vector vector1,const vector vector2);





/*
 *  �����������������char,����������ַ�������ʽ����
 *  @return�������½���������������NULL
 */
vector vect_createstr(char* str);


/*
 *  �������� vector1 
 *  @param vector  ����Ҫ���Ƶ����������ΪNULL�򷵻�NULL
 *  @return�������µ�������������NULL
 *  ע�⣺���������ŵ���STRING������ָ����ַ������Զ�����һ�ݣ���������㸴��
 *
 */
vector vect_copy(const vector vector1);

/*
 *  ������������Ԫ�أ�����������ʲô��û���� ����Ϊ0 ,��ǰ�����STRING��free��
 *  @param vector  ����Ҫ��յ����������ΪNULL��ֱ�ӷ���
 *  @remark        ���������ڲ�����Ŀռ�����Ϊ���ֽ�
 */
void vect_clear(vector vector);


/*
 *  �ͷ�����vector����Ŀռ������ÿ��Ԫ�ط���Ŀռ䣬����vector����ΪNULL 
 *  @param vector  ����Ҫ�ͷŵ����������ΪNULL��ֱ�ӷ���
 *  @remark        ���������ڲ�����Ŀռ��ͷ�
 */
void vect_free(vector vector);

/*
 *  ��ȡ��������
 *  @param vector�����������ΪNULL�򷵻�0
 *  @return����������
 */
int vect_getlength(vector vector);


/*
 *  ��ȡԪ������
 *  @param vector�����������ΪNULL�򷵻�-1
 *  @return��Ԫ�����ͣ��ο�ctypesö��
 */
int vect_getelmtype(vector vector);


/*
 *  ��ȡ�����ڵ�indexλ��ֵ������ڷ��ص�ָ��ָ��ĵ�ַ�����ǵ�ǰ�����ĳ���Ϊ0���Ǿ�ֱ�ӷ��ؿ�ָ��
 *  @param vector ��Ҫ��ȡ������
 *  @param index��Ҫȡֵ���±ꡣ�������±��0��ʼ����
 *	��index����0ʱ��
 *  		���index��������±�,���ȡ���һ��Ԫ�ص�ֵ
 *	��indexС��0ʱ��
 *  		���indexΪ-1,���ȡ���һ��Ԫ�ص�ֵ��-2���ȡ�����ڶ���Ԫ�ص�ֵ�����Դ����ƣ����index��С,���ȡ��һ��Ԫ�ص�ֵ
 *  
 *  @return �����ص�ֵ��void*���ͣ�Ҫ��תΪ��Ӧ������ָ�벢������ ����ʹ������ĺ�
 *  		ע�⣺�����ǰ�����ĳ���Ϊ0���Ǿ�ֱ�ӷ��ؿ�ָ�룬��������û�ʹ����ҵ�
 */
void* vect_getelmat(vector vector,int index);
//����һЩ�궨�庯���Զ�ת������
#define vect_getcharelmat(vector,index)  (*((char*)vect_getelmat(vector,index)))
#define vect_getshortelmat(vector,index)  (*((short*)vect_getelmat(vector,index)))
#define vect_getintelmat(vector,index)  (*((int*)vect_getelmat(vector,index)))
#define vect_getlongelmat(vector,index)  (*((long*)vect_getelmat(vector,index)))
#define vect_getfloatelmat(vector,index)  (*((float*)vect_getelmat(vector,index)))
#define vect_getdoubleelmat(vector,index)  (*((double*)vect_getelmat(vector,index)))
#define vect_getstrelmat(vector,index)  (*((char**)vect_getelmat(vector,index)))
#define vect_getpointerelmat(vector,index)  (*((void**)vect_getelmat(vector,index)))


/*
 *  ���������ڵ�indexλ��ֵΪelem,��vector[index]=elem,�����ǰ��������Ϊ0���Ͳ�����ֱ�ӷ���
 *  @param vector ��Ҫ���õ�����
 *  @param index��Ҫ���õ��±ꡣ�������±��0��ʼ����
 *	��index����0ʱ��
 *  		���index��������±�,���������һ��Ԫ�ص�ֵ
 *	��indexС��0ʱ��
 *  		���indexΪ-1,���������һ��Ԫ�ص�ֵ��-2�����õ����ڶ���Ԫ�ص�ֵ�����Դ����ƣ����indexС����С�±�,�����õ�һ��Ԫ�ص�ֵ
 *  
 *  @param �ɱ���������±��ֵ����Ȼ�ǿɱ����������Ҫ��Ϊ�˴���ͨ�����ͣ�����ֻ��ȡ��һ�������������ָ�룬��̬����ռ�
 *  @return �����ش��������
 */
vector vect_setelmat(vector vector,int index,...);


/*
 *  �����ڵ�indexλ����elem,��ǰ��ֵ������
 *  @param vector ��Ҫ���õ�����
 *  @param index��Ҫ������±ꡣ�������±��0��ʼ����
 *	��index����0ʱ��
 *		���index>����±�a��������β��������Ԫ��
 *		������������ڲ�������������Ԫ�أ�ԭ����Ԫ�������ƶ�
 *	��indexС��0ʱ��
 *  		���indexΪ-1,�������һ��Ԫ�ز�ֵ��-2���ڵ����ڶ���Ԫ�ز�ֵ�����Դ����ƣ����indexС����С�±�,����index=0λ������һ��Ԫ��
 *  
 *  @param �ɱ���������±��ֵ����Ȼ�ǿɱ����������Ҫ��Ϊ�˴���ͨ�����ͣ�����ֻ��ȡ��һ�������������ָ�룬��̬����ռ�
 *  @return �����ش��������
 */
vector vect_addelmat(vector vector,int index,...);


/*
 *  ��elemѹջ������β��
 *  @param vector ������
 *  @param �ɱ������Ҫ��ջ��Ԫ�أ���Ȼ�ǿɱ����������Ҫ��Ϊ�˴���ͨ�����ͣ�����ֻ��ȡ��һ������
 *  @return �����ش��������
 */
#define vect_pushelm(vector,...)  vect_addelmat((vector),(vector)->size,##__VA_ARGS__)



/*
 *  �����ڵ�indexλɾ��ֵ,�����ֵ��ǰ�ƣ�ɾ����ֵ����ڷ��ص�ָ��ָ��ĵ�ַ�����ǵ�ǰ�����ĳ���Ϊ0���Ǿ�ֱ�ӷ��ؿ�ָ��
 *  @param vector ��Ҫ���õ�����
 *  @param index��Ҫ������±ꡣ�������±��0��ʼ����
 *	��index����0ʱ��
 *  		���index��������±�,��ɾ�����һ��Ԫ�ص�ֵ
 *	��indexС��0ʱ��
 *  		���indexΪ-1,��ɾ�����һ��Ԫ�ص�ֵ��-2��ɾ�������ڶ���Ԫ�ص�ֵ�����Դ����ƣ����index��С,��ɾ����һ��Ԫ�ص�ֵ
 *  @return �����ص�ֵ��void*���ͣ�Ҫ��תΪ��Ӧ������ָ�벢�����ã�����ʹ������ĺ�
 *  		ע�⣺�����ǰ�����ĳ���Ϊ0���Ǿ�ֱ�ӷ��ؿ�ָ�룬��������û�ʹ����ҵ�
 */
void* vect_delelmat(vector vector,int index);
//����һЩ�궨�庯���Զ�ת������
#define vect_delcharelmat(vector,index)  (*((char*)vect_delelmat(vector,index)))
#define vect_delshortelmat(vector,index)  (*((short*)vect_delelmat(vector,index)))
#define vect_delintelmat(vector,index)  (*((int*)vect_delelmat(vector,index)))
#define vect_dellongelmat(vector,index)  (*((long*)vect_delelmat(vector,index)))
#define vect_delfloatelmat(vector,index)  (*((float*)vect_delelmat(vector,index)))
#define vect_deldoubleelmat(vector,index)  (*((double*)vect_delelmat(vector,index)))
#define vect_delstrelmat(vector,index)  (*((char**)vect_delelmat(vector,index)))

/*
 *  �ѵ����������һ��Ԫ��
 *  @param vector ������
 *  @return �����ص�ֵ��void*���ͣ�Ҫ��תΪ��Ӧ������ָ�벢�����ã�����ʹ������ĺ�
 *  		ע�⣺�����ǰ�����ĳ���Ϊ0���Ǿ�ֱ�ӷ��ؿ�ָ�룬��������û�ʹ����ҵ�
 */
#define vect_popelm(vector)  vect_delelmat((vector),(vector)->size-1)
//����һЩ�궨�庯���Զ�ת������
#define vect_popcharelm(vector)  (*((char*)vect_delelmat( (vector),(vector)->size-1     )))
#define vect_popshortelm(vector)  (*((short*)vect_delelmat( (vector),(vector)->size-1    )))
#define vect_popintelm(vector)  (*((int*)vect_delelmat( (vector),(vector)->size-1  )))
#define vect_poplongelm(vector)  (*((long*)vect_delelmat(  (vector),(vector)->size-1  )))
#define vect_popfloatelm(vector)  (*((float*)vect_delelmat( (vector),(vector)->size-1  )))
#define vect_popdoubleelm(vector)  (*((double*)vect_delelmat( (vector),(vector)->size-1  )))
#define vect_popstrelm(vector)  (*((char**)vect_delelmat( (vector),(vector)->size-1  )))





/************************����Ϊ�ڲ��������û���Ӧ�õ���********************/
/*
 *  �޸�����������
 *
 *  @param vector �����޸ĵ�����������ǿգ�����ֱ�ӷ���
 *  @param capacity��������,���������<=��ǰ�����ĳ��ȣ���ֱ�ӷ���
 *  @return�����ر��޸ĵ�����
 */
vector vect_setcap (vector vector, int capacity);

/*
 *  ��������������Ƿ��Ѿ������꣬�ǵĻ���������
 *  @param vector �����޸ĵ�����
 *  @return�����ر��޸ĵ�����
 *
 */
vector vect_checkcap (vector vector);

/*
 *	����data,del_data�ȣ����������ĳ�ʼ����ΪINIT_CAPACITY���Ǵ��������ĸ�������
 *  @param vector �������õ�����
 *  @return�����ر��޸ĵ�����
 */
vector _vect_create(vector vector);


//����vector�����ͰѲ�����ֵ��p,p��void*ָ��
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


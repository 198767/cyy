/*
 * �ó���Ϊʵ��C���Ե������йغ����Ķ���
 */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "vector.h"

vector vect_create(ctypes type)
{
	vector vector = malloc(sizeof(*vector));//�����¿ռ�
	if(vector == NULL) //�ռ����ʧ��
		return NULL;
	//�������ͻ�ȡԪ���ֽ���
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
	char str[10001]; //���뱣֤������ַ���������������ȣ�һ����������㹻���
	va_list ap;
	va_start(ap, fmt);
	i = vsnprintf(str,10000,fmt,ap);//n�ǳɹ�������ַ���
	va_end(ap);
	if (i < 0) //����ʧ��
		return NULL;
	vector vector = malloc(sizeof(*vector));//�����¿ռ�
	if(vector == NULL) //�ռ����ʧ��
		return NULL;
	//�������ͻ�ȡԪ���ֽ���
	vector->element_size=sizeof(char);
	vector->type=CHAR;
	if(! _vect_create(vector))
		return NULL;
	i=0;
	//��������ֵ
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
	//����
	for(i=0;i<vector->size ;i++)
		p[i]=vect_getcharelmat(vector,i);
	p[i]='\0';
	return p;
}

vector	vect_strcat(vector vector,const char* fmt,...)
{
	int i;
	char str[10001]; //���뱣֤������ַ���������������ȣ�һ����������㹻���
	va_list ap;
	va_start(ap, fmt);
	i = vsnprintf(str,10000,fmt,ap);//n�ǳɹ�������ַ���
	va_end(ap);
	if (i < 0) //����ʧ��
		return vector;

	char *p=str;
	if(vector->type !=CHAR)
		return vector;

	//�ҳ��Ѿ�����Ŀ��ֽ�
	for(i=0;i<vector->size;i++)
	{
		if(vect_getcharelmat(vector,i)=='\0')
			break;
	}

	//��ʼƴ���ַ���
	for(;i<vector->size && *p ;i++)
		vect_setelmat(vector,i,*p++);
	while(*p)
		vect_pushelm(vector,*p++);
	return vector;
}

void   vect_uniqstr(vector vector)
{
	int i,j;	//��������
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
	int i,j,k;	//��������
	int flag;	//�ǿմ�
	vector vector2;	//����vector1���ظ��ķǿմ�

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
			//�ظ��ķǿմ�
			if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector1,j))==0 && vect_getstrelmat(vector1,i)[0])
			{
				flag=1;
				for(k=0;k<vect_getlength(vector2);k++) //�Ƿ��Ѿ��Ž�ȥ�ˣ�
				{
					if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector2,k))==0) //�Ѿ��Ž�ȥ��
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
	int i,j,k;	//��������
	int flag;	//�ǿմ�
	vector vector2;	//����vector1���ظ��ķǿմ�

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
			//�ظ��ķǿմ�
			if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector1,j))==0 && vect_getstrelmat(vector1,i)[0])
			{
				flag=1;
				for(k=0;k<vect_getlength(vector2);k++) //�Ƿ��Ѿ��Ž�ȥ�ˣ�
				{
					if(strcmp(vect_getstrelmat(vector1,i),vect_getstrelmat(vector2,k))==0) //�Ѿ��Ž�ȥ��
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
	int i,j;	//��������

	if(!vector)
		return -1;
	if(vector->type !=STRING)
		return -1;

	for(i=0;i<vect_getlength(vector);i++)
	{
		for(j=i+1;j<vect_getlength(vector);j++)
		{
			//�ظ��ķǿմ�
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
	int i;	//��������

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

	int i;	//��������

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
	int i;			//��������
	vector vector2;		//�������Ӻ���ַ���
	char* p;		//���صĽ��
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
	int i,j;	//��������
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
			if(strcmp(p,q)>0)	//����λ��
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
	if(!vector1 || !vector2) //��ָ�� ����
		return -1;
	if(vect_getelmtype(vector1) !=STRING || vect_getelmtype(vector2) !=STRING) //����Ĳ����ַ��� ����
		return -1;
	if(vect_getlength(vector1) !=vect_getlength(vector2))	//������ַ���������һ�� �϶������
		return 1;	
	//��������
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
	if(!vector1 || !vector2) //��ָ�� ����
		return -1;
	if(vect_getelmtype(vector1) !=STRING || vect_getelmtype(vector2) !=STRING) //����Ĳ����ַ��� ����
		return -1;
	if(vect_getlength(vector1) !=vect_getlength(vector2))	//������ַ���������һ�� �϶������
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
	char str[10001]; //���뱣֤������ַ���������������ȣ�һ����������㹻���
	char *p=str;
	va_list ap;
	va_start(ap, fmt);
	i = vsnprintf(str,10000,fmt,ap);//n�ǳɹ�������ַ���
	va_end(ap);
	if (i < 0) //����ʧ��
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
	vector vector = malloc(sizeof(*vector));//�����¿ռ�
	if(vector == NULL) //�ռ����ʧ��
		return NULL;
	vector->type=STRUCT;
	vector->element_size=element_size;
	return _vect_create(vector);
}

vector vect_create_pointer()
{
	vector vector = malloc(sizeof(*vector));//�����¿ռ�
	if(vector == NULL) //�ռ����ʧ��
		return NULL;
	vector->type=POINTER;
	vector->element_size=sizeof(char*);
	return _vect_create(vector);
}

vector vect_create_strpointer(char* str)
{
	vector vector = malloc(sizeof(*vector));//�����¿ռ�
	if(vector == NULL) //�ռ����ʧ��
		return NULL;
	vector->type=STRING;
	vector->element_size=sizeof(char*);
	_vect_create(vector);

	if(str)	//����д����ַ��� 
		return vect_pushelm(vector,str);
	else
		return vector;
}





vector _vect_create(vector vector)
{
	vector->size=0;
	vector->capacity = INIT_CAPACITY;
	vector->data=malloc(vector->element_size * vector->capacity);//�����¿ռ�
	if(vector->data ==NULL)
	{
		free(vector);
		return NULL;
	}
	memset(vector->data,0,vector->element_size * vector->capacity); //��ʼ��
	vector->del_data=malloc(vector->element_size);//�����¿ռ�
	if(vector->del_data==NULL)
	{
		free(vector->data);
		free(vector);
		return NULL;
	}
	memset(vector->del_data,0,vector->element_size); //��ʼ��
	return vector;
}

vector vect_copy(const vector vector1)
{
	int i;
	char *p,*q;
	vector vector2;
	if(vector1==NULL)
		return NULL;

	vector2 = malloc(sizeof(*vector2));//�����¿ռ�
	if(vector2 == NULL) //�ռ����ʧ��
		return NULL;
	vector2->type=vector1->type;
	vector2->capacity=vector1->capacity;
	vector2->element_size=vector1->element_size;
	vector2->size=vector1->size;
	vector2->data=malloc(vector1->element_size*vector1->capacity);//�����¿ռ� ����ڴ治����ʧ��
	if(vector2->data==NULL)
	{
		free(vector2);
		return NULL;

	}

	vector2->del_data=malloc(vector1->element_size);//�����¿ռ� ����ڴ治����ʧ��
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
	else	//�ַ����������θ���
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

	if(vector->type==STRING)	//�ͷ�ÿ���ַ����Ŀռ�
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


	//ȷ���±�
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

	//��ȡ����ֵ����data[real_index]
	if(vector->type ==STRUCT || vector->type ==POINTER) //�ṹ���ָ��
		memcpy(vector->data+real_index*vector->element_size,((char*)&index)+sizeof(int), vector->element_size); 
	else if(vector->type ==STRING)		//�ַ���
	{
		str=strdup(*(char**)(((char*)&index)+sizeof(int)));
		if(!str)	//ʧ��
			return vector;
		memcpy(vector->data+real_index*vector->element_size,&str, vector->element_size); 
	}
	else //������������
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
	if(vector->type ==STRING)		//�ַ���,�ȸ���һ�ݸ���
	{

		str=strdup(*(char**)(((char*)&index)+sizeof(int)));
		if(!str)	//ʧ��
			return vector;
	}

	if(index>=0)
	{
		if(index>=vector->size) //�����Ԫ��
		{
			real_index=index;
			vector->size=index+1; //������β��
		}
		else  //��ԭλ���ϲ�����Ԫ��
		{
			real_index=index;
			vector->size++;
		}
	}
	else
	{
		real_index=vector->size+index;
		if(real_index<0) //������ͷ��
			real_index=0;
		vector->size++;
	}
	vect_checkcap(vector); //������������Ƿ��㹻
	//�ѿ�϶�����ֽ����
	memset(vector->data+oldsize*vector->element_size,0,vector->element_size*(vector->size-oldsize));
	//Ԫ�������ƶ�
	for(i=oldsize-1;i>=index;i--)
		memcpy(vector->data+(i+1)*vector->element_size, vector->data+i*vector->element_size, vector->element_size); 
	//��ȡ����ֵ����data[real_index]
	if(vector->type ==STRUCT || vector->type ==POINTER) //�ṹ���ָ��
		memcpy(vector->data+real_index*vector->element_size,((char*)&index)+sizeof(int), vector->element_size); 
	else if(vector->type ==STRING)		//�ַ���
		memcpy(vector->data+real_index*vector->element_size,&str, vector->element_size); 
	else //������������
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
	//ȷ���±�
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

	//��Ҫɾ����Ԫ�ط���del_data
	memcpy(vector->del_data, vector->data+real_index*vector->element_size, vector->element_size); 
	//Ԫ����ǰ�ƶ�
	for(i=real_index;i<vector->size-1;i++)
		memcpy(vector->data+i*vector->element_size, vector->data+(i+1)*vector->element_size, vector->element_size); 

	vector->size--;
	return vector->del_data;
}

vector vect_checkcap(vector vector)
{
	int newCapacity;
	if( vector->size>=vector->capacity  //����������
	|| (vector->capacity - vector->size)*5 < vector->capacity)  //ʣ������/������< 1/5
	{
		newCapacity=(vector->capacity*3)/2;  //������/������=2/3
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
	if(capacity <= vector->size) //С�ڵ�����������
	{
		return vector;
	}
	if (vector->capacity >= capacity)//�����㹻��
	{
		vector->capacity = capacity;
		return vector;
	}
	else     //�����¿ռ�
	{	
		vector->capacity = capacity;
		vector->data=realloc(vector->data,vector->element_size*vector->capacity);//�����¿ռ� ����ڴ治����ʧ��
		return vector;
	}
}

#include<stdio.h>
#define size 1000

typedef struct
{
	int *head;
	int length;
}link;

int initlink(link &p) //����˳�������ʼ����
{
	p.head=new int[size];
	if(!p.head) return 0;
	p.length=0;
	return 1;
}
void input(link &p)  //����ֵ
{
	int i;
	printf("�����");
	scanf("%d",&p.length);
	printf("˳�����룺");
	for(i=0;i<p.length;i++)
		scanf("%d",&p.head[i]);
}
int get(link p,int i)//ȡֵ
{
	int x;
	if(i<1||i>p.length)  return 0;
	x=p.head[i-1];
	return x;
}
int locate(link p,int e)//����
{
	int i;
	for(i=0;i<p.length;i++)
	if(p.head[i]==e) return i+1;
	return 0;
}
int insert(link &p,int i,int e)//����
{
	int j;
	if(i<1||i>p.length+1) return 0;
	if(p.length==size) return 0;
	for(j=p.length-1;j>=i-1;j--)
		p.head[j+1]=p.head[j];
	p.head[i-1]=e;
	++p.length;
	return 1;
}
int Delete(link &p,int i) //ɾ��
{
	int j;
	if((i<1)||(i>p.length+1)) return 0;
	for(j=i;j<=p.length-1;j++)
		p.head[j-1]=p.head[j];
	--p.length;
	return 1;
}
void output(link &p) //���
{
	int k;
	for(k=0;k<p.length;k++)
		printf("p.head[%d]=%d\n",k,p.head[k]);
}
void killbig(link &p) //ɾ�����
{
	int i,k=0;
	for(i=1;i<p.length;i++)
		if (p.head[k]<p.head[i]) k=i; 
	Delete(p,k+1);
}

void chaxiao(link &p) //����СֵԪ�غ������һ�������СֵԪ��
{
	int i,j,k=0;
	for(i=1;i<p.length;i++)
		if (p.head[k]>p.head[i]) k=i;
	j=p.head[k];
	insert(p,k+1,j);
}
void hebing(link &a,link &b)
{
	int j,e,h;
	for(j=1;j<=b.length;j++)
	{
		e=get(b,j);
		h=locate(a,e);
		if(h==0)
			insert(a,a.length+1,e);
	}
}

void main()
{
	link x;
	int a;
	a=initlink(x);
	if(a==1)
	printf("ok");
	else
		printf("no");
	input(x);
	printf("�����һ:\n");
	output(x);

	link y;
	int b;
	b=initlink(y);
	if(b==1)
	printf("ok");
	else
		printf("no");
	input(y);
	printf("������:\n");
	output(y);
	hebing(x,y);
	printf("����ϲ�:\n");
	output(x);

}


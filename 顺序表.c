#include<stdio.h>
#define size 1000

typedef struct
{
	int *head;
	int length;
}link;

int initlink(link &p) //建立顺序表，并初始化。
{
	p.head=new int[size];
	if(!p.head) return 0;
	p.length=0;
	return 1;
}
void input(link &p)  //输入值
{
	int i;
	printf("输入表长");
	scanf("%d",&p.length);
	printf("顺序输入：");
	for(i=0;i<p.length;i++)
		scanf("%d",&p.head[i]);
}
int get(link p,int i)//取值
{
	int x;
	if(i<1||i>p.length)  return 0;
	x=p.head[i-1];
	return x;
}
int locate(link p,int e)//查找
{
	int i;
	for(i=0;i<p.length;i++)
	if(p.head[i]==e) return i+1;
	return 0;
}
int insert(link &p,int i,int e)//插入
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
int Delete(link &p,int i) //删除
{
	int j;
	if((i<1)||(i>p.length+1)) return 0;
	for(j=i;j<=p.length-1;j++)
		p.head[j-1]=p.head[j];
	--p.length;
	return 1;
}
void output(link &p) //输出
{
	int k;
	for(k=0;k<p.length;k++)
		printf("p.head[%d]=%d\n",k,p.head[k]);
}
void killbig(link &p) //删除最大
{
	int i,k=0;
	for(i=1;i<p.length;i++)
		if (p.head[k]<p.head[i]) k=i; 
	Delete(p,k+1);
}

void chaxiao(link &p) //在最小值元素后面插入一个这个最小值元素
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
	printf("输出表一:\n");
	output(x);

	link y;
	int b;
	b=initlink(y);
	if(b==1)
	printf("ok");
	else
		printf("no");
	input(y);
	printf("输出表二:\n");
	output(y);
	hebing(x,y);
	printf("输出合并:\n");
	output(x);

}


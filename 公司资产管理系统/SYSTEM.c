#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct { 
int ThingNum;			//记录事件个数 
int thing[50];		    //记录事件事件状态  借(0)或还(1)
int Staff[50];		    //收录员工编号 
int Time[301];		    //时间 
int MobiRef[100];		//手机编号 
char MobiType[50][10];	    //手机序列号 
}data;



void AddStaff()  //录入员工 
{
	AddStaff:
	printf("现在开始录入员工编号，输入完毕后请按回车键。\n");
	fflush(stdin);   //缓存区清除函数 
	scanf("%d",&data.Staff[data.ThingNum]);
	if(data.Staff[data.ThingNum]/10000000==0||data.Staff[data.ThingNum]/10000000>10||data.Staff[data.ThingNum]<0)  //判断输入编号格式 
	{
		printf("	*请按照正确格式输入，该次输入无效。*\n");
		goto AddStaff;
	}
	else
	{
		printf("员工编号已录入系统。");
	}
} 

void GetTime()   //添加系统时间
{
	time_t t;//将t声明为时间变量
    struct tm *p;//struct tm是一个结构体，声明一个结构体指针
    time(&t);
    p=localtime(&t);//获得当地的时间
    data.Time[data.ThingNum*6+0]=1900+p->tm_year;
    data.Time[data.ThingNum*6+1]=1+p->tm_mon;
    data.Time[data.ThingNum*6+2]=p->tm_mday;
    data.Time[data.ThingNum*6+3]=p->tm_hour;
    data.Time[data.ThingNum*6+4]=p->tm_min;
    data.Time[data.ThingNum*6+5]=p->tm_sec;
    printf("成功添加时间！\n"); 
} 

void error()
{
	printf("\n"); 
	printf("	*数据错误，请重新输入!*\n");
	printf("\n"); 

}

void AddTime()  //录入时间 
{
	int a,i,b,c;
	again:
	printf("您可以选择手动添加时间或者自动添加本地时间，自动添加输入1，逐个添加输入其它任意数\n");
	fflush(stdin);   //缓存区清除函数 
	scanf("%d",&a);
	if(a==1) {GetTime();}
	else
	{
		Year: 
		printf("请输入年：\n");
		fflush(stdin);   //缓存区清除函数 
		scanf("%d",&data.Time[data.ThingNum*6+0]);
		if(data.Time[data.ThingNum*6+0]<=0) {error();  goto Year;}
		if(data.Time[data.ThingNum*6+0]%4==0&&data.Time[data.ThingNum*6+0]%100||data.Time[data.ThingNum*6+0]%400==0) b=1; else{b=0;}
		Moon:
		printf("请输入月：\n");
		fflush(stdin);   //缓存区清除函数 
		scanf("%d",&data.Time[data.ThingNum*6+1]);
		if(data.Time[data.ThingNum*6+1]<=0||data.Time[data.ThingNum*6+1]>12) {error();  goto Moon;}
		Day:
		printf("请输入日：\n");
		fflush(stdin);   //缓存区清除函数 
		scanf("%d",&data.Time[data.ThingNum*6+2]);
		if(data.Time[data.ThingNum*6+2]<=0||data.Time[data.ThingNum*6+2]>31) {error();  goto Moon;}
			else 
			{
				if(data.Time[data.ThingNum*6+2]>28)
				{
					if(b==1&&data.Time[data.ThingNum*6+2]>29&&data.Time[data.ThingNum*6+2]==2){error();  goto Day;} //闰年二月 
					if(b==0&&data.Time[data.ThingNum*6+2]>28&&data.Time[data.ThingNum*6+2]==2){error();  goto Day;} //非闰年二月 
					if(data.Time[data.ThingNum*6+2]>30&&(data.Time[data.ThingNum*6+2]==4||data.Time[data.ThingNum*6+2]==6||data.Time[data.ThingNum*6+2]==9||data.Time[data.ThingNum*6+2]==11)){error();  goto Day;} //大悦小月判断 
				}
			}
		hour:
		printf("请输入时：\n");	
		fflush(stdin);   //缓存区清除函数 
		scanf("%d",&data.Time[data.ThingNum*6+3]);
		if(data.Time[data.ThingNum*6+3]<0||data.Time[data.ThingNum*6+3]>23) {error();  goto hour;}
		min:
		printf("请输入分：\n");	
		fflush(stdin);   //缓存区清除函数 
		scanf("%d",&data.Time[data.ThingNum*6+4]);
		if(data.Time[data.ThingNum*6+4]<0||data.Time[data.ThingNum*6+4]>60) {error();  goto min;}
		sec:
		printf("请输入秒：\n");	
		fflush(stdin);   //缓存区清除函数 
		scanf("%d",&data.Time[data.ThingNum*6+5]);
		if(data.Time[data.ThingNum*6+5]<0||data.Time[data.ThingNum*6+5]>60) {error();  goto sec;}
	}
	printf("您录入的时间为： %d年%02d月%02d日 %02d:%02d:%02d\n",data.Time[data.ThingNum*6+0],data.Time[data.ThingNum*6+1],data.Time[data.ThingNum*6+2],data.Time[data.ThingNum*6+3],data.Time[data.ThingNum*6+4],data.Time[data.ThingNum*6+5]);
	printf("请确认录入时间，确认请输入“1”，重新则输入“0”\n");
	fflush(stdin);   //缓存区清除函数 
	scanf("%d",&c);
	if(c==0) {goto again;}
		else
		{
			printf("时间录入成功。"); 
		}
}





void AddMobiType()    //录入手机型号 

{
	int a; 
	AddMobiType:
	printf("请输入手机型号:\n");
	scanf("%s",data.MobiType[data.ThingNum]);
	char ab[10];
	strcpy(ab, data.MobiType[data.ThingNum]);
	a=strlen(ab);
	if(a==8)
	{ 
		printf("成功录入手机型号！\n");
	} 
	else
	{
		error();
		a=0;
		goto AddMobiType;
	}
}


void AddMobiRef() //添加手机编号 
{
	int a;
	AddMobiRef: 
	printf("请输入手机编号：\n");
	fflush(stdin);   //缓存区清除函数 
	scanf("%d",&data.MobiRef[data.ThingNum]);
	a=data.MobiRef[data.ThingNum]/10000;
	if(a>0&&a<10)
	{ 
		printf("成功录入手机编号！\n");
	} 
	else
	{
		error();
		a=0;
		goto AddMobiRef; 
	}
} 

void ThingState()  //记录事件状态，借或还。
{
	ThingState:
	printf("借手机请输入0或任意字符，还手机请输入1。输出后请按回车\n");
	fflush(stdin);   //缓存区清除函数 
	scanf("%d",&data.thing[data.ThingNum]);
	if(data.thing[data.ThingNum]!=0&&data.thing[data.ThingNum]!=1)
	{
		error();
		goto ThingState;
	}
	printf("已成功录入\n");
} 
void AddThing()  //添加事件 
{
	data.ThingNum++;
	ThingState();	    	//录入事件 
	AddStaff(); 		   //录入员工 
	AddMobiType();			//录入手机型号
	AddMobiRef();			//录入手机编号
	AddTime();              //录入时间 
	printf("事件录入完成。\n"); 
}
void LookThing()  //查看事件
{
	int i;
	printf("*************************************************************************************************\n");
	printf("*         员工         借/还    手机型号       手机编号                      日期               *\n");
	printf("                                                                                                 \n"); 
	for(i=1;i<=data.ThingNum;i++)
	{
		if(data.thing[i]==1)
		{
			printf("*	%d	归还	%s	%d		%d年%d月%d日	%02d时%02d分%02d秒	*\n",data.Staff[i],data.MobiType[i],data.MobiRef[i],data.Time[i*6+0],data.Time[i*6+1],data.Time[i*6+2],data.Time[i*6+3],data.Time[i*6+4],data.Time[i*6+5]);
		}
		if(data.thing[i]==0)
		{
			printf("*	%d	借出	%s	%d		%d年%d月%d日	%02d时%02d分%02d秒	*\n",data.Staff[i],data.MobiType[i],data.MobiRef[i],data.Time[i*6+0],data.Time[i*6+1],data.Time[i*6+2],data.Time[i*6+3],data.Time[i*6+4],data.Time[i*6+5]);
		} 
	}
	if(data.ThingNum==0) 
	printf("*                            这里空空如也，您可以添加事件后再来查看                             *\n");
	printf("                                                                                                 \n"); 
	printf("*************************************************************************************************\n");
}

// 将结构体数组写入txt文件  WriteDataFile
void WriteDataFile() {
    FILE* fp;
    int l;
    if ((fp = fopen("data.txt", "w")) == NULL) {
        printf("无法读取本地文件！\n");
        return;
    }
     printf("警告！此操作将覆盖本地文件，是否继续？是输入1，否输入0\n");
     scanf("%d",&l);
     if(l==1)
     {
     	fwrite(&data, sizeof(data), 1, fp);
	    fclose(fp);
	    printf("数据已存储至本地文件：data.txt\n");
	 }
    
}

// 从txt文件读取结构体
void ReadDataFile() {
    FILE* fp;
    if ((fp = fopen("data.txt", "r")) == NULL) {
        printf("无法读取本地文件！\n");
        return;
    }
    fread(&data, sizeof(data), 1, fp);
    fclose(fp);
    printf("数据已从本地文件“data.txt”成功导入，您可以在系统操作查看命令。\n");
} 
 

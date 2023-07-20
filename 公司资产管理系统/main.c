#include <stdio.h>
#include <stdlib.h>
#include "SYSTEM.h"
#include <windows.h>

extern struct { 
		int ThingNum;			//记录事件个数 
		int thing[50];		    //记录事件事件状态  借(0)或还(1)
		int Staff[50];		    //收录员工编号 
		int Time[301];		    //时间 
		int MobiRef[100];		//手机编号 
		char MobiType[50][8];	    //手机序列号 
}data;


int a,size = sizeof(int);;
FILE *fp;
char ch;
int main() 
{

	while(1)
	{
		printf(
		"*******************************\n" 
		"                               \n"
		"欢迎登录公司资产管理系统       \n"
		"添加事件                (输入1)\n"
		"查看事件                (输入2)\n"
		"清屏                    (输入3)\n"
		"存储事件至本地          (输入4)\n"
		"从本地导入事件          (输入5)\n"
		"                               \n" 
		"输入完成请按回车键             \n"
		"*******************************\n"
		);
		scanf("%d",&a);
		switch(a)
		{
			case 1:  AddThing();  	break;	//添加事件 
			case 2:  LookThing();	break;	//查看事件 
			case 3:  system("cls");	break;	//清屏
			case 4:	 WriteDataFile();  break;
			case 5:	 ReadDataFile();   break;
		}		
	}	
}







/*
原先的想法是做一个完整的系统，先录入员工数据，再录入数据时，先与系统中的员工校验，比对通过后才允许添加事件，后来觉得太麻烦了。 还是先交作业比较方便。
完整的系统应该有两部分，一部分用于管理员输入密码后，然后执行添加员工，手机型号，删除员工，删除事件等操作。 
另一部分由员工自行输入，输入的数据（除了时间和手机序列号）与系统比对，只有正确才能继续写入事件。
最后printf员工表，型号表，事件表等功能，只能由管理员操作。 
*/


#include <stdio.h>
#include <math.h>
#include <windows.h>

struct {
int StaffNum;	        //员工数量 
int ThingNum;			//记录事件个数 
int state[50];		    //事件状态  借(0)或还(1)
int thing[50];		    //记录事件的员工和编号。 
int Staff[50];		    //收录员工编号 
int Time[301];		    //时间 
int MobiRef[100];		//手机编号 
char MobiType[100];	    //手机序列号 
}data;

void addthing()  //添加事件 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Windows颜色 
	int a,b,c,i,man,flag=0;
	data.ThingNum++;
	printf("请录入员工编号：");
	scanf("%d",&man);
	for(i=1;i<=data.StaffNum;i++)
	{
		if(data.Staff[i]==man)  
		{
			flag=1;       //标志位 
			break;
		}
	} 
	if(flag==0)
	{	
		SetConsoleTextAttribute(hConsole, 0xC );  //变红输出警告 
		printf("该员工未录入系统，请先录入。");
		SetConsoleTextAttribute(hConsole, 0x7 );  //输出后又变白 
	} 
		else
		{
			data.ThingNum++;
			data.thing[data.ThingNum]=man;
			//到这就不想写了。	
		} 
		
}



void AddStaff()  //添加员工 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Windows颜色 
	int i,j,a;
	printf("您需要录入几位员工的数据，请输入：");
	scanf("%d",&a);
	if(a<=0) {printf("ERROR！请输入正确的格式");}
	else
	{
		printf("开始录入员工编号\n");
		for(i=1;i<=a;i++)
		{
			printf("您正在录入第%d位员工的编号，输入完毕后请按回车键。\n",data.StaffNum+i);
			scanf("%d",&data.Staff[data.StaffNum+i]);
			
			for(j=1;j<data.StaffNum+i;j++)      //判断输入数据是否和之前的一样。 
			{
				if(data.Staff[data.StaffNum+i]==data.Staff[j])
				{
					SetConsoleTextAttribute(hConsole, 0xC );  //变红输出警告 
					printf("系统已收录该员工编号，请重新输入。\n"); 
					SetConsoleTextAttribute(hConsole, 0x7 );  //输出后又变白 
					i--;	
				}	
			} 
		
			if(data.Staff[data.StaffNum+i]/10000000==0||data.Staff[data.StaffNum+i]/10000000>10||data.Staff[data.StaffNum+i]<0)  //判断输入编号格式 
			{
				SetConsoleTextAttribute(hConsole, 0xC );  //变红输出警告 
				printf("请按照正确格式输入，该次输入无效。\n");
				SetConsoleTextAttribute(hConsole, 0x7 );  //输出后又变白 
				i--;
			}
		} 
		printf("输入成功，已录入系统。");
		data.StaffNum+=a; 	
	}
} 

void LookStaff(void)   //查看录入的员工编号 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Windows颜色 
	int i;
	if(data.StaffNum==0)
	{
		SetConsoleTextAttribute(hConsole, 0xC );  //变红输出警告 
		printf("您还未录入员工编号，请先录入。");
		SetConsoleTextAttribute(hConsole, 0x7 );  //输出后又变白 
	 } 
	 else
	 {
		 printf("以下是系统已录入的员工编号：\n");
		for(i=1;i<=data.StaffNum;i++)
		{
			printf("第%d位员工：",i);
			printf("%d\n",data.Staff[i]);
		}
	  } 
}


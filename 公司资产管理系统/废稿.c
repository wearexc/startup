/*
ԭ�ȵ��뷨����һ��������ϵͳ����¼��Ա�����ݣ���¼������ʱ������ϵͳ�е�Ա��У�飬�ȶ�ͨ�������������¼�����������̫�鷳�ˡ� �����Ƚ���ҵ�ȽϷ��㡣
������ϵͳӦ���������֣�һ�������ڹ���Ա���������Ȼ��ִ�����Ա�����ֻ��ͺţ�ɾ��Ա����ɾ���¼��Ȳ����� 
��һ������Ա���������룬��������ݣ�����ʱ����ֻ����кţ���ϵͳ�ȶԣ�ֻ����ȷ���ܼ���д���¼���
���printfԱ�����ͺű��¼���ȹ��ܣ�ֻ���ɹ���Ա������ 
*/


#include <stdio.h>
#include <math.h>
#include <windows.h>

struct {
int StaffNum;	        //Ա������ 
int ThingNum;			//��¼�¼����� 
int state[50];		    //�¼�״̬  ��(0)��(1)
int thing[50];		    //��¼�¼���Ա���ͱ�š� 
int Staff[50];		    //��¼Ա����� 
int Time[301];		    //ʱ�� 
int MobiRef[100];		//�ֻ���� 
char MobiType[100];	    //�ֻ����к� 
}data;

void addthing()  //����¼� 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Windows��ɫ 
	int a,b,c,i,man,flag=0;
	data.ThingNum++;
	printf("��¼��Ա����ţ�");
	scanf("%d",&man);
	for(i=1;i<=data.StaffNum;i++)
	{
		if(data.Staff[i]==man)  
		{
			flag=1;       //��־λ 
			break;
		}
	} 
	if(flag==0)
	{	
		SetConsoleTextAttribute(hConsole, 0xC );  //���������� 
		printf("��Ա��δ¼��ϵͳ������¼�롣");
		SetConsoleTextAttribute(hConsole, 0x7 );  //������ֱ�� 
	} 
		else
		{
			data.ThingNum++;
			data.thing[data.ThingNum]=man;
			//����Ͳ���д�ˡ�	
		} 
		
}



void AddStaff()  //���Ա�� 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Windows��ɫ 
	int i,j,a;
	printf("����Ҫ¼�뼸λԱ�������ݣ������룺");
	scanf("%d",&a);
	if(a<=0) {printf("ERROR����������ȷ�ĸ�ʽ");}
	else
	{
		printf("��ʼ¼��Ա�����\n");
		for(i=1;i<=a;i++)
		{
			printf("������¼���%dλԱ���ı�ţ�������Ϻ��밴�س�����\n",data.StaffNum+i);
			scanf("%d",&data.Staff[data.StaffNum+i]);
			
			for(j=1;j<data.StaffNum+i;j++)      //�ж����������Ƿ��֮ǰ��һ���� 
			{
				if(data.Staff[data.StaffNum+i]==data.Staff[j])
				{
					SetConsoleTextAttribute(hConsole, 0xC );  //���������� 
					printf("ϵͳ����¼��Ա����ţ����������롣\n"); 
					SetConsoleTextAttribute(hConsole, 0x7 );  //������ֱ�� 
					i--;	
				}	
			} 
		
			if(data.Staff[data.StaffNum+i]/10000000==0||data.Staff[data.StaffNum+i]/10000000>10||data.Staff[data.StaffNum+i]<0)  //�ж������Ÿ�ʽ 
			{
				SetConsoleTextAttribute(hConsole, 0xC );  //���������� 
				printf("�밴����ȷ��ʽ���룬�ô�������Ч��\n");
				SetConsoleTextAttribute(hConsole, 0x7 );  //������ֱ�� 
				i--;
			}
		} 
		printf("����ɹ�����¼��ϵͳ��");
		data.StaffNum+=a; 	
	}
} 

void LookStaff(void)   //�鿴¼���Ա����� 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Windows��ɫ 
	int i;
	if(data.StaffNum==0)
	{
		SetConsoleTextAttribute(hConsole, 0xC );  //���������� 
		printf("����δ¼��Ա����ţ�����¼�롣");
		SetConsoleTextAttribute(hConsole, 0x7 );  //������ֱ�� 
	 } 
	 else
	 {
		 printf("������ϵͳ��¼���Ա����ţ�\n");
		for(i=1;i<=data.StaffNum;i++)
		{
			printf("��%dλԱ����",i);
			printf("%d\n",data.Staff[i]);
		}
	  } 
}


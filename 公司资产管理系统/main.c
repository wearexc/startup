#include <stdio.h>
#include <stdlib.h>
#include "SYSTEM.h"
#include <windows.h>

extern struct { 
		int ThingNum;			//��¼�¼����� 
		int thing[50];		    //��¼�¼��¼�״̬  ��(0)��(1)
		int Staff[50];		    //��¼Ա����� 
		int Time[301];		    //ʱ�� 
		int MobiRef[100];		//�ֻ���� 
		char MobiType[50][8];	    //�ֻ����к� 
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
		"��ӭ��¼��˾�ʲ�����ϵͳ       \n"
		"����¼�                (����1)\n"
		"�鿴�¼�                (����2)\n"
		"����                    (����3)\n"
		"�洢�¼�������          (����4)\n"
		"�ӱ��ص����¼�          (����5)\n"
		"                               \n" 
		"��������밴�س���             \n"
		"*******************************\n"
		);
		scanf("%d",&a);
		switch(a)
		{
			case 1:  AddThing();  	break;	//����¼� 
			case 2:  LookThing();	break;	//�鿴�¼� 
			case 3:  system("cls");	break;	//����
			case 4:	 WriteDataFile();  break;
			case 5:	 ReadDataFile();   break;
		}		
	}	
}







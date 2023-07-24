#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct { 
int ThingNum;			//��¼�¼����� 
int thing[50];		    //��¼�¼��¼�״̬  ��(0)��(1)
int Staff[50];		    //��¼Ա����� 
int Time[301];		    //ʱ�� 
int MobiRef[100];		//�ֻ���� 
char MobiType[50][10];	    //�ֻ����к� 
}data;



void AddStaff()  //¼��Ա�� 
{
	AddStaff:
	printf("���ڿ�ʼ¼��Ա����ţ�������Ϻ��밴�س�����\n");
	fflush(stdin);   //������������� 
	scanf("%d",&data.Staff[data.ThingNum]);
	if(data.Staff[data.ThingNum]/10000000==0||data.Staff[data.ThingNum]/10000000>10||data.Staff[data.ThingNum]<0)  //�ж������Ÿ�ʽ 
	{
		printf("	*�밴����ȷ��ʽ���룬�ô�������Ч��*\n");
		goto AddStaff;
	}
	else
	{
		printf("Ա�������¼��ϵͳ��");
	}
} 

void GetTime()   //���ϵͳʱ��
{
	time_t t;//��t����Ϊʱ�����
    struct tm *p;//struct tm��һ���ṹ�壬����һ���ṹ��ָ��
    time(&t);
    p=localtime(&t);//��õ��ص�ʱ��
    data.Time[data.ThingNum*6+0]=1900+p->tm_year;
    data.Time[data.ThingNum*6+1]=1+p->tm_mon;
    data.Time[data.ThingNum*6+2]=p->tm_mday;
    data.Time[data.ThingNum*6+3]=p->tm_hour;
    data.Time[data.ThingNum*6+4]=p->tm_min;
    data.Time[data.ThingNum*6+5]=p->tm_sec;
    printf("�ɹ����ʱ�䣡\n"); 
} 

void error()
{
	printf("\n"); 
	printf("	*���ݴ�������������!*\n");
	printf("\n"); 

}

void AddTime()  //¼��ʱ�� 
{
	int a,i,b,c;
	again:
	printf("������ѡ���ֶ����ʱ������Զ���ӱ���ʱ�䣬�Զ��������1����������������������\n");
	fflush(stdin);   //������������� 
	scanf("%d",&a);
	if(a==1) {GetTime();}
	else
	{
		Year: 
		printf("�������꣺\n");
		fflush(stdin);   //������������� 
		scanf("%d",&data.Time[data.ThingNum*6+0]);
		if(data.Time[data.ThingNum*6+0]<=0) {error();  goto Year;}
		if(data.Time[data.ThingNum*6+0]%4==0&&data.Time[data.ThingNum*6+0]%100||data.Time[data.ThingNum*6+0]%400==0) b=1; else{b=0;}
		Moon:
		printf("�������£�\n");
		fflush(stdin);   //������������� 
		scanf("%d",&data.Time[data.ThingNum*6+1]);
		if(data.Time[data.ThingNum*6+1]<=0||data.Time[data.ThingNum*6+1]>12) {error();  goto Moon;}
		Day:
		printf("�������գ�\n");
		fflush(stdin);   //������������� 
		scanf("%d",&data.Time[data.ThingNum*6+2]);
		if(data.Time[data.ThingNum*6+2]<=0||data.Time[data.ThingNum*6+2]>31) {error();  goto Moon;}
			else 
			{
				if(data.Time[data.ThingNum*6+2]>28)
				{
					if(b==1&&data.Time[data.ThingNum*6+2]>29&&data.Time[data.ThingNum*6+2]==2){error();  goto Day;} //������� 
					if(b==0&&data.Time[data.ThingNum*6+2]>28&&data.Time[data.ThingNum*6+2]==2){error();  goto Day;} //��������� 
					if(data.Time[data.ThingNum*6+2]>30&&(data.Time[data.ThingNum*6+2]==4||data.Time[data.ThingNum*6+2]==6||data.Time[data.ThingNum*6+2]==9||data.Time[data.ThingNum*6+2]==11)){error();  goto Day;} //����С���ж� 
				}
			}
		hour:
		printf("������ʱ��\n");	
		fflush(stdin);   //������������� 
		scanf("%d",&data.Time[data.ThingNum*6+3]);
		if(data.Time[data.ThingNum*6+3]<0||data.Time[data.ThingNum*6+3]>23) {error();  goto hour;}
		min:
		printf("������֣�\n");	
		fflush(stdin);   //������������� 
		scanf("%d",&data.Time[data.ThingNum*6+4]);
		if(data.Time[data.ThingNum*6+4]<0||data.Time[data.ThingNum*6+4]>60) {error();  goto min;}
		sec:
		printf("�������룺\n");	
		fflush(stdin);   //������������� 
		scanf("%d",&data.Time[data.ThingNum*6+5]);
		if(data.Time[data.ThingNum*6+5]<0||data.Time[data.ThingNum*6+5]>60) {error();  goto sec;}
	}
	printf("��¼���ʱ��Ϊ�� %d��%02d��%02d�� %02d:%02d:%02d\n",data.Time[data.ThingNum*6+0],data.Time[data.ThingNum*6+1],data.Time[data.ThingNum*6+2],data.Time[data.ThingNum*6+3],data.Time[data.ThingNum*6+4],data.Time[data.ThingNum*6+5]);
	printf("��ȷ��¼��ʱ�䣬ȷ�������롰1�������������롰0��\n");
	fflush(stdin);   //������������� 
	scanf("%d",&c);
	if(c==0) {goto again;}
		else
		{
			printf("ʱ��¼��ɹ���"); 
		}
}





void AddMobiType()    //¼���ֻ��ͺ� 

{
	int a; 
	AddMobiType:
	printf("�������ֻ��ͺ�:\n");
	scanf("%s",data.MobiType[data.ThingNum]);
	char ab[10];
	strcpy(ab, data.MobiType[data.ThingNum]);
	a=strlen(ab);
	if(a==8)
	{ 
		printf("�ɹ�¼���ֻ��ͺţ�\n");
	} 
	else
	{
		error();
		a=0;
		goto AddMobiType;
	}
}


void AddMobiRef() //����ֻ���� 
{
	int a;
	AddMobiRef: 
	printf("�������ֻ���ţ�\n");
	fflush(stdin);   //������������� 
	scanf("%d",&data.MobiRef[data.ThingNum]);
	a=data.MobiRef[data.ThingNum]/10000;
	if(a>0&&a<10)
	{ 
		printf("�ɹ�¼���ֻ���ţ�\n");
	} 
	else
	{
		error();
		a=0;
		goto AddMobiRef; 
	}
} 

void ThingState()  //��¼�¼�״̬����򻹡�
{
	ThingState:
	printf("���ֻ�������0�������ַ������ֻ�������1��������밴�س�\n");
	fflush(stdin);   //������������� 
	scanf("%d",&data.thing[data.ThingNum]);
	if(data.thing[data.ThingNum]!=0&&data.thing[data.ThingNum]!=1)
	{
		error();
		goto ThingState;
	}
	printf("�ѳɹ�¼��\n");
} 
void AddThing()  //����¼� 
{
	data.ThingNum++;
	ThingState();	    	//¼���¼� 
	AddStaff(); 		   //¼��Ա�� 
	AddMobiType();			//¼���ֻ��ͺ�
	AddMobiRef();			//¼���ֻ����
	AddTime();              //¼��ʱ�� 
	printf("�¼�¼����ɡ�\n"); 
}
void LookThing()  //�鿴�¼�
{
	int i;
	printf("*************************************************************************************************\n");
	printf("*         Ա��         ��/��    �ֻ��ͺ�       �ֻ����                      ����               *\n");
	printf("                                                                                                 \n"); 
	for(i=1;i<=data.ThingNum;i++)
	{
		if(data.thing[i]==1)
		{
			printf("*	%d	�黹	%s	%d		%d��%d��%d��	%02dʱ%02d��%02d��	*\n",data.Staff[i],data.MobiType[i],data.MobiRef[i],data.Time[i*6+0],data.Time[i*6+1],data.Time[i*6+2],data.Time[i*6+3],data.Time[i*6+4],data.Time[i*6+5]);
		}
		if(data.thing[i]==0)
		{
			printf("*	%d	���	%s	%d		%d��%d��%d��	%02dʱ%02d��%02d��	*\n",data.Staff[i],data.MobiType[i],data.MobiRef[i],data.Time[i*6+0],data.Time[i*6+1],data.Time[i*6+2],data.Time[i*6+3],data.Time[i*6+4],data.Time[i*6+5]);
		} 
	}
	if(data.ThingNum==0) 
	printf("*                            ����տ���Ҳ������������¼��������鿴                             *\n");
	printf("                                                                                                 \n"); 
	printf("*************************************************************************************************\n");
}

// ���ṹ������д��txt�ļ�  WriteDataFile
void WriteDataFile() {
    FILE* fp;
    int l;
    if ((fp = fopen("data.txt", "w")) == NULL) {
        printf("�޷���ȡ�����ļ���\n");
        return;
    }
     printf("���棡�˲��������Ǳ����ļ����Ƿ������������1��������0\n");
     scanf("%d",&l);
     if(l==1)
     {
     	fwrite(&data, sizeof(data), 1, fp);
	    fclose(fp);
	    printf("�����Ѵ洢�������ļ���data.txt\n");
	 }
    
}

// ��txt�ļ���ȡ�ṹ��
void ReadDataFile() {
    FILE* fp;
    if ((fp = fopen("data.txt", "r")) == NULL) {
        printf("�޷���ȡ�����ļ���\n");
        return;
    }
    fread(&data, sizeof(data), 1, fp);
    fclose(fp);
    printf("�����Ѵӱ����ļ���data.txt���ɹ����룬��������ϵͳ�����鿴���\n");
} 
 

#include<stdio.h>
int getnum(int n)
{
	if(n==1||n==2)
		return 1;
	return getnum(n-1)+getnum(n-2);
} 
 
int main()
{
	int x,i;
	scanf("%d",&x);
	for(i=1;i<100;i++)
	{
		getnum(i);
		if(x<getnum(i)) break;
		printf("%d\n",getnum(i));
	}
	return 0;
}
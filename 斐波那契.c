#include <stdio.h>
#include <stdlib.h>
void main()
{
	int a=1,b=0,c=0,x,i;
	scanf("%d",&x);
	for(i=0;i<100;i++)
	{
	c=a+b;
	if (c>x) break;
	printf(" %d",c);
	a=b+c;
	if (a>x) break;
	printf(" %d",a);
	b=a+c;
	if (b>x) break;
    printf(" %d",b);
	}
}

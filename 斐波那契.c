#include<stdio.h>
int getNum(int n){	
	if (n == 1 || n==2 )
		return 1;
	return getNum(n - 2) + getNum(n - 1);

}
 
int main(){
	printf("f(12)=%d",getNum(12));
	return 0;
}
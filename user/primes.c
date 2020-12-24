#include "kernel/types.h"
#include "user/user.h"

void filter(int *a,int number)
{
	if(number==1)
	{
		printf("prime %d\n",*a);
		return;
	}
	int pip[2],i,temp;
	int divisor = *a;
	printf("prime %d\n",divisor);
	pipe(pip);
	if(fork() == 0) //如果是子进程
	{
		for(i = 0;i < number; i++)  //通过管道将数组往下传
		{
			temp = *(a+i);
			write(pip[1],(char *)(&temp),4);
		}
		exit();
	}
	close(pip[1]); //关闭写功能
	if(fork() == 0) //如果是子进程
	{   
		int counter = 0;
		char buffer[4];
		while(read(pip[0],buffer,4)!=0)
		{//读取管道数组并筛选
			temp = *((int *)buffer);
			if(temp % divisor != 0){
				*a = temp;
				a = a+1;
				counter = counter +1;			
			}
		}
		filter(a-counter,counter);//递归调用
		exit();
	}
	wait();
	wait();	
}
int main(){
	int a[34];
	int x = 0;
	for(x = 0; x < 34;x++){
		a[x] = x+2;	
	}
	filter(a,34);
	exit();
}
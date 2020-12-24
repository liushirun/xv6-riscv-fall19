#include "kernel/types.h"
#include "user/user.h"

int main(int argnum,char *argv[])
{
	if(argnum != 2)  //判断参数是否为空
    {
		fprintf(2, "At least 1 argument!\n");  //固定格式
		exit();
	}
	int sleeptime = atoi(argv[1]);//获得sleep时长，单位是0.1s
	printf("(nothing happens for a little while)\n");
	sleep(sleeptime);
	exit();
}
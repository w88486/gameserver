#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <zinx.h>
#include"GameChannel.h"
#include"GameWorld.h"
#include"ZInxTimer.h"
using namespace std;
void daemonlize() {
	//fork,父进程退出
	int pid = fork();
	if (pid < 0) {
		exit(-1);
	}
	if (pid > 0) {
		exit(0);
	}
	//设置会话id
	setsid();
	//重定向标准输出
	int nullfd = open("/dev/null", O_RDWR);
	if (nullfd > 0) {
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		dup2(nullfd, 2);
		close(nullfd);
	}
	//进程监控
	while (1) {
		int ppid = fork();
		//等待子进程，出错就循环重启
		if (ppid < 0) {
			exit(-1);
		}
		if (ppid > 0) {
			int status = 0;
			wait(&status);
			if (0 == status) {
				exit(0);
			}
		}
		else {//子进程跳出循环执行后续程序
			cout << getpid() << endl;
			break;
		}
	}
}
int main() {
	//将服务开启为守候进程
	daemonlize();
	//1初始化框架
	ZinxKernel::ZinxKernelInit();
	//4将对象添加到框架
	ZinxKernel::Zinx_Add_Channel(*(new ZInxTimerChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8888, new GameTcpChannelFact())));
	//5运行框架
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
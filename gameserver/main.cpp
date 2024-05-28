#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <zinx.h>
#include"GameChannel.h"
#include"GameWorld.h"
#include"ZInxTimer.h"
using namespace std;
void daemonlize() {
	//fork,�������˳�
	int pid = fork();
	if (pid < 0) {
		exit(-1);
	}
	if (pid > 0) {
		exit(0);
	}
	//���ûỰid
	setsid();
	//�ض����׼���
	int nullfd = open("/dev/null", O_RDWR);
	if (nullfd > 0) {
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		dup2(nullfd, 2);
		close(nullfd);
	}
	//���̼��
	while (1) {
		int ppid = fork();
		//�ȴ��ӽ��̣������ѭ������
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
		else {//�ӽ�������ѭ��ִ�к�������
			cout << getpid() << endl;
			break;
		}
	}
}
int main() {
	//��������Ϊ�غ����
	daemonlize();
	//1��ʼ�����
	ZinxKernel::ZinxKernelInit();
	//4��������ӵ����
	ZinxKernel::Zinx_Add_Channel(*(new ZInxTimerChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8888, new GameTcpChannelFact())));
	//5���п��
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
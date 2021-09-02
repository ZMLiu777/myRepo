#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <fork.h>
#include <stdlib.h>

void call_back(int signo)
{
	int wpid = 0;

	while((wpid = wait(NULL)) != -1)
	{
		printf("catch SIGCHILD, child pid = %d\n", wpid);
	}

	return ;
}

int main()
{
	pid_t pid;

	// 设置屏蔽 SIGCHLD 信号，防止信号事件为注册成功，子进程就结束了的情况发生
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, NULL);  

	int i = 0;
	for(i = 0; i < 5; ++i)
	{
		pid = fork();
		if(pid == 0)   // 子进程
			break;
	}

	if(i == 5)         // 父进程
	{
		printf("I am parent pid = %d\n", getpid());
		struct sigaction act;
		act.sa_handler = call_back;
		sigemptyset(act.sa_mask);
		act.flags = 0;

		sigaction(SIGCHLD, act, NULL);

		sigprocmask(SIG_UNBLOCK, &set, NULL); 
	}
	else
	{
		printf("I am child pid = %d, parent pid = %d\n", getpid(),getppid());
	}

	return 0;
}
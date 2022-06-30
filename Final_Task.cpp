#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;
struct FCB
{
	int pid;	   //进程号
	int comeTime;  //到达时间
	int runTime;   //运行所需时间
	int beginTime; //开始运行的时间
	int endTime;   //结束运行的时间
	int order;	   //调度顺序
	int priority;  //优先级
	int slot;	   //时间片
	int finish;	   //结束标志
} que[1000];
int num = 0;		 //输入的总进程数
void FCFS();		 //先来先服务
void SJF();			 //不可剥夺的短作业优先算法
void SRTF();		 //可剥夺式的短作业优先算法
void RR();			 //时间片轮转
void DPSA();		 //优先级调度算法
bool cmp1(FCB, FCB); // FCFS、RR
bool cmp2(FCB, FCB); // SJF、SRTF
bool cmp3(FCB, FCB); // DPSA
const int IsEnd = 1; //进程已经结束
const int NoEnd = 0; //进程还未结束
void FCFS()
{
	cout << "您当前选取的先来先服务(FCFS)调度方式！" << endl;
	sort(que, que + num, cmp1); //先来后到排好序
	queue<FCB> q;				//就绪队列
	int endTime = 0;
	for (int i = 0; i < num; i++)
	{
		que[i].order = i + 1;
		if (i == 0) //第一个进程特判一下
		{
			que[i].beginTime = que[i].comeTime;
		}
		else
		{
			que[i].beginTime = max(que[i].comeTime, que[i - 1].endTime);
		}
		que[i].endTime = que[i].beginTime + que[i].runTime;

		for (int j = i + 1; j < num; j++)
		{
			if (que[j].beginTime < que[i].endTime)
				q.push(que[j]);
		}

		//输出
		cout << "当前处理进程信息(处理顺序/PID/处理开始时间/处理结束时间/优先级): ";
		printf("%d/%d/%d/%d/%d\n", que[i].order, que[i].pid, que[i].beginTime, que[i].endTime, que[i].priority);

		//就绪队列处理
		cout << "当前就绪队列的进程号为: ";
		for (int i = 0; i < q.size(); i++)
		{
			cout << q.front().pid;
			q.push(q.front());
			q.pop();
		}
		cout << endl;
		while (!q.empty())
			q.pop();
	}

	endTime = que[num - 1].endTime;
	// cpu调度可视化
	for (int i = 0; i < num; i++)
	{
		printf("\n");
		cout << "0 ";
		for (int k = 0; k < endTime; k++)
		{
			if (que[i].beginTime <= k && k < que[i].endTime)
			{
				cout << "--- ";
			}
			else
				cout << "    ";
		}
		cout << endTime;
		cout << "  PID:" << que[i].pid;
	}
	cout << endl;
	cout << "0 ";
	for (int i = 0; i < endTime; i++)
	{
		cout << "--- ";
	}
	cout << endTime;
	cout << "  CPU-Total";
}
void SJF()
{
	cout << "您当前选取的不可剥夺短作业优先服务(SJF)调度方式！" << endl;
	queue<FCB> q; //就绪队列
	int endTime = 0;
	sort(que, que + num, cmp2);
	int lastTime = que[0].comeTime; //当前时间
	int tmp = 0;					//每次选中的短作业
	for (int i = 0; i < num; i++)	//筛选num次，每次选出最佳的短作业
	{
		bool isBool = false; //判断当前时间是否有短作业在就绪队列

		while (1)
		{
			if (i == 0)
			{
				//排序后的第一个肯定满足条件，特判
				break;
			}
			else
			{
				for (int j = 1; j < num; j++) // for一遍的复杂度为O(n),比排序O(nlogn)快
				{
					if (que[j].finish == IsEnd) //当前短作业已经结束就跳过
						continue;
					if (que[j].comeTime <= lastTime) //当前短作业在就绪队列中
					{
						q.push(que[j]);
						if (isBool == false) //没有标记最优短作业
						{
							isBool = true;
							tmp = j;
						}
						else //已经标记了最优短作业
						{
							//比较，更新最优短作业
							//当短作业运行时间相等时，优先调度进程号小的短作业执行
							if (que[j].runTime < que[tmp].runTime)
							{
								tmp = j;
							}
							else if (que[j].runTime == que[tmp].runTime && que[j].pid < que[tmp].pid)
								tmp = j;
						}
					}
				}
				if (isBool) //如果存在短进程满足条件就输出
					break;
				else //不存在就把时间后移再寻找
					lastTime++;
				cout << "下一个等待处理的进程是: " << que[tmp].pid;
			}
		}

		que[tmp].order = i + 1;
		que[tmp].beginTime = max(que[tmp].beginTime, lastTime);
		que[tmp].endTime = que[tmp].beginTime + que[tmp].runTime;

		// for(int j = tmp + 1 ; j < num; j++ )
		// {
		// 	cout << que[tmp].endTime << endl;
		// 	cout << que[j].beginTime << endl;
		// 	if(que[j].beginTime < que[tmp].endTime )
		// 	q.push(que[j]);
		// }

		//输出
		cout << "当前处理进程信息(处理顺序/PID/处理开始时间/处理结束时间/优先级): ";
		printf("%d/%d/%d/%d/%d\n", que[tmp].order, que[tmp].pid, que[tmp].beginTime, que[tmp].endTime, que[tmp].priority);
		lastTime = que[tmp].endTime; //更新当前时间
		que[tmp].finish = IsEnd;	 //标记短作业结束

		//就绪队列处理
		cout << "当前就绪队列的进程号为: ";
		for (int i = 0; i < q.size(); i++)
		{
			cout << q.front().pid;
			q.push(q.front());
			q.pop();
		}
		cout << endl;
		while (!q.empty())
			q.pop();
	}
	endTime = que[num - 1].beginTime + que[num - 1].runTime;
	// cpu调度可视化
	for (int i = 0; i < num; i++)
	{
		printf("\n");
		cout << "0 ";
		for (int k = 0; k < endTime; k++)
		{
			if (que[i].beginTime <= k && k < que[i].endTime)
			{
				cout << "--- ";
			}
			else
				cout << "    ";
		}
		cout << endTime;
		cout << "  PID:" << que[i].pid;
	}
	cout << endl;
	cout << "0 ";
	for (int i = 0; i < endTime; i++)
	{
		cout << "--- ";
	}
	cout << endTime;
	cout << "  CPU-Total";
}
void SRTF()
{
	cout << "您当前选取的可剥夺短作业优先服务(SRTF)调度方式！" << endl;
	int endTime = 0;
	sort(que, que + num, cmp2);
	int lastTime = que[0].comeTime;
	int ID = 1; //输出顺序
	int key = 0;
	int tmp = 0, counts = 0;   //当前进程，输出次数
	int isRun = -1, start = 0; //当前是否有进程运行,运行开始时间
	while (counts < num)
	{
		while (que[tmp].comeTime <= lastTime && tmp < num) //当前时间内的进程
		{
			tmp++;
		}
		int minx = 0x3f3f3f, minId = -1; //最短时间和下标
		for (int i = 0; i < tmp; i++)	 //寻找当前进程中剩余运行时间最短的进程
		{
			if (que[i].runTime < minx && que[i].finish == NoEnd)
			{
				minx = que[i].runTime;
				minId = i;
			}
		}
		if (minId == -1) //如果当前时间进程都结束了就等待下一个进程
		{
			lastTime = que[tmp].comeTime;
			continue;
		}
		if (isRun == -1) //当前没有进程在运行
		{
			isRun = minId;
			start = max(que[isRun].comeTime, lastTime); //运行刚找到的进程
		}
		//如果找到进程的剩余运行时间小于当前进程的剩余运行时间
		if (que[minId].runTime < que[isRun].runTime)
		{
			que[isRun].order = ID++;
			que[isRun].beginTime = start;
			que[isRun].endTime = lastTime;
			printf("%d/%d/%d/%d/%d\n", que[isRun].order, que[isRun].pid, que[isRun].beginTime, que[isRun].endTime, que[isRun].priority);
			key = que[isRun].endTime;
			que[isRun + num] = que[isRun];
			isRun = minId;
			start = lastTime;
		}
		//运行进程
		int run = que[tmp].comeTime - lastTime;
		//进程能运行完就运行完
		if (run >= que[isRun].runTime || run <= 0)
		{
			lastTime += que[isRun].runTime;
			que[isRun].order = ID++;
			que[isRun].beginTime = start;
			que[isRun].endTime = lastTime;
			printf("%d/%d/%d/%d/%d\n", que[isRun].order, que[isRun].pid, que[isRun].beginTime, que[isRun].endTime, que[isRun].priority);
			key = que[isRun].endTime;
			que[isRun].runTime = 0;
			que[isRun].finish = IsEnd;
			counts++;
			isRun = -1;
			start = lastTime;
		}
		//进程不能运行完就运行一个时间片
		else
		{
			lastTime += run;
			que[isRun].runTime -= run;
		}
	}
	endTime = key;
	// cpu调度可视化
	for (int i = 0; i < ID - 1; i++)
	{
		printf("\n");
		cout << "0 ";
		for (int k = 0; k < endTime; k++)
		{
			if (que[i].beginTime <= k && k < que[i].endTime)
			{
				cout << "--- ";
			}
			else
				cout << "    ";
		}
		cout << endTime;
		cout << "  PID:" << que[i].pid;
	}

	cout << endl;
	cout << "0 ";
	for (int i = 0; i < endTime; i++)
	{
		cout << "--- ";
	}
	cout << endTime;
	cout << "  CPU-Total";
}
void RR()
{	
	cout << "您当前选取的时间片轮转(RR)调度方式！" << endl;
	sort(que, que + num, cmp1);
	queue<FCB> q; //就绪队列
	struct FCB v[1000];
	int lastTime = 0; //当前时间
	int ID = 1;		  //输出顺序
	int endTime = 0;
	int tmp = 0, counts = 0; //当前进程，输出次数
	while (counts < num)
	{
		if (q.empty()) //队列为空则等待进程到来
		{
			lastTime = que[tmp].comeTime;
			while (que[tmp].comeTime <= lastTime && tmp < num) //当前时间内的进程
			{
				if (que[tmp].finish == NoEnd) //还没完成的进程入队
				{
					q.push(que[tmp]);
				}
				tmp++;
			}
		}
		else
		{
			FCB ans = q.front(); //取出处于队列头的进程
			q.pop();
			//进程可以运行完
			if (ans.runTime <= ans.slot)
			{
				//输出当前进程调度
				ans.order = ID++;
				ans.beginTime = lastTime;
				ans.endTime = ans.beginTime + ans.runTime;
				cout << "当前处理进程信息(处理顺序/PID/处理开始时间/处理结束时间/优先级): ";
				printf("%d/%d/%d/%d/%d\n", ans.order, ans.pid, ans.beginTime, ans.endTime, ans.priority);
				v[ID - 2] = ans;
				endTime = ans.endTime;
				lastTime = ans.endTime; //更新当前时间
				ans.finish = IsEnd;		//标记短作业结束
				counts++;

				while (que[tmp].comeTime <= lastTime && tmp < num) //当前时间内的进程
				{
					if (que[tmp].finish == NoEnd) //还没完成的进程入队
					{
						q.push(que[tmp]);
					}
					tmp++;
				}

				//就绪队列处理
				cout << "当前就绪队列的进程号为: ";
				for (int i = 0; i < q.size(); i++)
				{
					cout << q.front().pid;
					q.push(q.front());
					q.pop();
				}
				cout << endl;
			}
			//进程不能运行完
			else
			{
				//输出当前进程调度
				ans.order = ID++;
				ans.beginTime = lastTime;
				ans.endTime = ans.beginTime + ans.slot;
				cout << "当前处理进程信息(处理顺序/PID/处理开始时间/处理结束时间/优先级): ";
				printf("%d/%d/%d/%d/%d\n", ans.order, ans.pid, ans.beginTime, ans.endTime, ans.priority);
				v[ID - 2] = ans;
				endTime = ans.endTime;
				lastTime = ans.endTime;
				ans.runTime -= ans.slot;

				while (que[tmp].comeTime <= lastTime && tmp < num) //当前时间内的进程
				{
					if (que[tmp].finish == NoEnd) //还没完成的进程入队
					{
						q.push(que[tmp]);
					}
					tmp++;
				}
				q.push(ans); //先让新进程入队再让当前进程入队

				//就绪队列处理
				cout << "当前就绪队列的进程号为: ";
				for (int i = 0; i < q.size(); i++)
				{

					cout << q.front().pid;
					q.push(q.front());
					q.pop();
				}
				cout << endl;
			}
		}
	}

	// cpu调度可视化
	for (int i = 0; i < ID - 1; i++)
	{
		printf("\n");
		cout << "0 ";
		for (int k = 0; k < endTime; k++)
		{
			if (v[i].beginTime <= k && k < v[i].endTime)
			{
				cout << "--- ";
			}
			else
				cout << "    ";
		}
		cout << endTime;
		cout << "  PID:" << v[i].pid;
	}
	cout << endl;
	cout << "0 ";
	for (int i = 0; i < endTime; i++)
	{
		cout << "--- ";
	}
	cout << endTime;
	cout << "  CPU-Total";
}
void DPSA()
{
	sort(que, que + num, cmp3);
	queue<FCB> q; //就绪队列
	struct FCB v[1000];
	int lastTime = que[0].comeTime; //当前时间
	int ID = 1;						//输出顺序
	int tmp = 0, counts = 0;		//当前进程，输出次数
	int start = lastTime;			//当前进程开始时间
	int maxId, Smax;
	int endTime = 0;
	while (counts < num)
	{
		while (que[tmp].comeTime <= lastTime && tmp < num) //当前时间内的进程
		{
			if (que[tmp].comeTime > start && que[tmp].comeTime < lastTime)
			{
				que[tmp].priority = max(que[tmp].priority + 1, 0); //等待时优先级加1
			}
			tmp++;
		}

		//寻找最高优先级进程
		maxId = -1;
		Smax = 0;
		for (int i = 0; i < tmp; i++)
		{
			if (que[i].priority > Smax && que[i].finish == NoEnd)
			{
				Smax = que[i].priority;
				maxId = i;
			}
		}
		if (maxId == -1) //如果当前时间进程都结束了就等待下一个进程
		{
			lastTime = que[tmp].comeTime;
			continue;
		}
		//运行进程
		start = lastTime;
		//进程能运行完
		if (que[maxId].runTime <= que[maxId].slot)
		{
			que[maxId].order = ID++;
			que[maxId].beginTime = lastTime;
			que[maxId].endTime = lastTime + que[maxId].runTime;
			cout << "当前处理进程信息(处理顺序/PID/处理开始时间/处理结束时间/优先级): ";
			printf("%d/%d/%d/%d/%d\n", que[maxId].order, que[maxId].pid, que[maxId].beginTime, que[maxId].endTime, que[maxId].priority - 1);
			v[ID - 2] = que[maxId];
			counts++;
			lastTime += que[maxId].runTime;
			que[maxId].finish = IsEnd;
		}
		//进程不能运行完
		else
		{
			que[maxId].order = ID++;
			que[maxId].beginTime = lastTime;
			que[maxId].endTime = lastTime + que[maxId].slot;
			cout << "当前处理进程信息(处理顺序/PID/处理开始时间/处理结束时间/优先级): ";
			printf("%d/%d/%d/%d/%d\n", que[maxId].order, que[maxId].pid, que[maxId].beginTime, que[maxId].endTime, que[maxId].priority - 1);
			v[ID - 2] = que[maxId];
			lastTime += que[maxId].slot;
			que[maxId].runTime -= que[maxId].slot;
		}

		//改变进程优先级
		for (int i = 0; i < tmp; i++)
		{
			if (que[i].finish == NoEnd)
			{
				if (i != maxId) //运行优先级减3
				{
					que[i].priority = max(que[i].priority + 1, 0);
					q.push(que[i]);
				}
				else //等待优先级加1
				{
					que[i].priority -= 1;
				}
			}
		}

		for (int i = 0; i < tmp; i++)
		{
			if (que[i].finish == NoEnd)
			{
				if (i == maxId) //运行优先级减3
				{
					q.push(que[i]);
				}
			}
		}

		//就绪队列处理
		cout << "当前就绪队列的进程号为: ";
		for (int i = 0; i < q.size(); i++)
		{
			cout << q.front().pid;
			q.push(q.front());
			q.pop();
		}
		cout << endl;

		while (!q.empty())
			q.pop();
	}

	endTime = v[ID - 2].endTime;

	// cpu调度可视化
	for (int i = 0; i < ID - 1; i++)
	{
		printf("\n");
		cout << "0 ";
		for (int k = 0; k < endTime; k++)
		{
			if (v[i].beginTime <= k && k < v[i].endTime)
			{
				cout << "--- ";
			}
			else
				cout << "    ";
		}
		cout << endTime;
		cout << "  PID:" << v[i].pid;
	}
	cout << endl;
	cout << "0 ";
	for (int i = 0; i < endTime; i++)
	{
		cout << "--- ";
	}
	cout << endTime;
	cout << "  CPU-Total";
}
bool cmp1(FCB a, FCB b)
{
	if (a.comeTime != b.comeTime)
		return a.comeTime < b.comeTime;
	else
		return a.pid < b.pid;
}
bool cmp2(FCB a, FCB b)
{
	if (a.comeTime != b.comeTime)
		return a.comeTime < b.comeTime;
	else if (a.runTime != b.runTime)
		return a.runTime > b.runTime;
	else
		return a.pid < b.pid;
}
bool cmp3(FCB a, FCB b)
{
	if (a.comeTime != b.comeTime)
		return a.comeTime < b.comeTime;
	else if (a.priority != b.priority)
		return a.priority > b.priority;
	else
		return a.pid < b.pid;
}
int main()
{
	int key = 0;
	int sig = 0;
	int endTime = 0;
	cout << "请选择您想要的调度程序: 1:'先来先服务' 2:'不可剥夺的短作业优先' 3:'可剥夺的短作业优先' 4:'时间片轮转' 5:'优先级调度'  : ";
	cin >> sig; //算法选择标志
	//读入数据
	cout << "请输入当前进程数量 : ";
	cin >> key;
	cout << "请依次输入进程信息(分别为:PID/到达时间/所需运行时间/优先级/时间片) " << endl;
	while (num < key && ~scanf("%d/%d/%d/%d/%d", &que[num].pid, &que[num].comeTime, &que[num].runTime, &que[num].priority, &que[num].slot))
	{
		que[num].beginTime = que[num].endTime = que[num].order = 0;
		que[num].finish = NoEnd;
		num++;
	}
	//选择算法
	switch (sig)
	{
	case 1:
		FCFS();
		break;
	case 2:
		SJF();
		break;
	case 3:
		SRTF();
		break;
	case 4:
		RR();
		break;
	case 5:
		DPSA();
		break;
	}
	return 0;
}
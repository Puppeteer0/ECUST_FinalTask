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
	int pid;	   //���̺�
	int comeTime;  //����ʱ��
	int runTime;   //��������ʱ��
	int beginTime; //��ʼ���е�ʱ��
	int endTime;   //�������е�ʱ��
	int order;	   //����˳��
	int priority;  //���ȼ�
	int slot;	   //ʱ��Ƭ
	int finish;	   //������־
} que[1000];
int num = 0;		 //������ܽ�����
void FCFS();		 //�����ȷ���
void SJF();			 //���ɰ���Ķ���ҵ�����㷨
void SRTF();		 //�ɰ���ʽ�Ķ���ҵ�����㷨
void RR();			 //ʱ��Ƭ��ת
void DPSA();		 //���ȼ������㷨
bool cmp1(FCB, FCB); // FCFS��RR
bool cmp2(FCB, FCB); // SJF��SRTF
bool cmp3(FCB, FCB); // DPSA
const int IsEnd = 1; //�����Ѿ�����
const int NoEnd = 0; //���̻�δ����
void FCFS()
{
	cout << "����ǰѡȡ�������ȷ���(FCFS)���ȷ�ʽ��" << endl;
	sort(que, que + num, cmp1); //�������ź���
	queue<FCB> q;				//��������
	int endTime = 0;
	for (int i = 0; i < num; i++)
	{
		que[i].order = i + 1;
		if (i == 0) //��һ����������һ��
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

		//���
		cout << "��ǰ���������Ϣ(����˳��/PID/����ʼʱ��/�������ʱ��/���ȼ�): ";
		printf("%d/%d/%d/%d/%d\n", que[i].order, que[i].pid, que[i].beginTime, que[i].endTime, que[i].priority);

		//�������д���
		cout << "��ǰ�������еĽ��̺�Ϊ: ";
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
	// cpu���ȿ��ӻ�
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
	cout << "����ǰѡȡ�Ĳ��ɰ������ҵ���ȷ���(SJF)���ȷ�ʽ��" << endl;
	queue<FCB> q; //��������
	int endTime = 0;
	sort(que, que + num, cmp2);
	int lastTime = que[0].comeTime; //��ǰʱ��
	int tmp = 0;					//ÿ��ѡ�еĶ���ҵ
	for (int i = 0; i < num; i++)	//ɸѡnum�Σ�ÿ��ѡ����ѵĶ���ҵ
	{
		bool isBool = false; //�жϵ�ǰʱ���Ƿ��ж���ҵ�ھ�������

		while (1)
		{
			if (i == 0)
			{
				//�����ĵ�һ���϶���������������
				break;
			}
			else
			{
				for (int j = 1; j < num; j++) // forһ��ĸ��Ӷ�ΪO(n),������O(nlogn)��
				{
					if (que[j].finish == IsEnd) //��ǰ����ҵ�Ѿ�����������
						continue;
					if (que[j].comeTime <= lastTime) //��ǰ����ҵ�ھ���������
					{
						q.push(que[j]);
						if (isBool == false) //û�б�����Ŷ���ҵ
						{
							isBool = true;
							tmp = j;
						}
						else //�Ѿ���������Ŷ���ҵ
						{
							//�Ƚϣ��������Ŷ���ҵ
							//������ҵ����ʱ�����ʱ�����ȵ��Ƚ��̺�С�Ķ���ҵִ��
							if (que[j].runTime < que[tmp].runTime)
							{
								tmp = j;
							}
							else if (que[j].runTime == que[tmp].runTime && que[j].pid < que[tmp].pid)
								tmp = j;
						}
					}
				}
				if (isBool) //������ڶ̽����������������
					break;
				else //�����ھͰ�ʱ�������Ѱ��
					lastTime++;
				cout << "��һ���ȴ�����Ľ�����: " << que[tmp].pid;
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

		//���
		cout << "��ǰ���������Ϣ(����˳��/PID/����ʼʱ��/�������ʱ��/���ȼ�): ";
		printf("%d/%d/%d/%d/%d\n", que[tmp].order, que[tmp].pid, que[tmp].beginTime, que[tmp].endTime, que[tmp].priority);
		lastTime = que[tmp].endTime; //���µ�ǰʱ��
		que[tmp].finish = IsEnd;	 //��Ƕ���ҵ����

		//�������д���
		cout << "��ǰ�������еĽ��̺�Ϊ: ";
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
	// cpu���ȿ��ӻ�
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
	cout << "����ǰѡȡ�Ŀɰ������ҵ���ȷ���(SRTF)���ȷ�ʽ��" << endl;
	int endTime = 0;
	sort(que, que + num, cmp2);
	int lastTime = que[0].comeTime;
	int ID = 1; //���˳��
	int key = 0;
	int tmp = 0, counts = 0;   //��ǰ���̣��������
	int isRun = -1, start = 0; //��ǰ�Ƿ��н�������,���п�ʼʱ��
	while (counts < num)
	{
		while (que[tmp].comeTime <= lastTime && tmp < num) //��ǰʱ���ڵĽ���
		{
			tmp++;
		}
		int minx = 0x3f3f3f, minId = -1; //���ʱ����±�
		for (int i = 0; i < tmp; i++)	 //Ѱ�ҵ�ǰ������ʣ������ʱ����̵Ľ���
		{
			if (que[i].runTime < minx && que[i].finish == NoEnd)
			{
				minx = que[i].runTime;
				minId = i;
			}
		}
		if (minId == -1) //�����ǰʱ����̶������˾͵ȴ���һ������
		{
			lastTime = que[tmp].comeTime;
			continue;
		}
		if (isRun == -1) //��ǰû�н���������
		{
			isRun = minId;
			start = max(que[isRun].comeTime, lastTime); //���и��ҵ��Ľ���
		}
		//����ҵ����̵�ʣ������ʱ��С�ڵ�ǰ���̵�ʣ������ʱ��
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
		//���н���
		int run = que[tmp].comeTime - lastTime;
		//�������������������
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
		//���̲��������������һ��ʱ��Ƭ
		else
		{
			lastTime += run;
			que[isRun].runTime -= run;
		}
	}
	endTime = key;
	// cpu���ȿ��ӻ�
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
	cout << "����ǰѡȡ��ʱ��Ƭ��ת(RR)���ȷ�ʽ��" << endl;
	sort(que, que + num, cmp1);
	queue<FCB> q; //��������
	struct FCB v[1000];
	int lastTime = 0; //��ǰʱ��
	int ID = 1;		  //���˳��
	int endTime = 0;
	int tmp = 0, counts = 0; //��ǰ���̣��������
	while (counts < num)
	{
		if (q.empty()) //����Ϊ����ȴ����̵���
		{
			lastTime = que[tmp].comeTime;
			while (que[tmp].comeTime <= lastTime && tmp < num) //��ǰʱ���ڵĽ���
			{
				if (que[tmp].finish == NoEnd) //��û��ɵĽ������
				{
					q.push(que[tmp]);
				}
				tmp++;
			}
		}
		else
		{
			FCB ans = q.front(); //ȡ�����ڶ���ͷ�Ľ���
			q.pop();
			//���̿���������
			if (ans.runTime <= ans.slot)
			{
				//�����ǰ���̵���
				ans.order = ID++;
				ans.beginTime = lastTime;
				ans.endTime = ans.beginTime + ans.runTime;
				cout << "��ǰ���������Ϣ(����˳��/PID/����ʼʱ��/�������ʱ��/���ȼ�): ";
				printf("%d/%d/%d/%d/%d\n", ans.order, ans.pid, ans.beginTime, ans.endTime, ans.priority);
				v[ID - 2] = ans;
				endTime = ans.endTime;
				lastTime = ans.endTime; //���µ�ǰʱ��
				ans.finish = IsEnd;		//��Ƕ���ҵ����
				counts++;

				while (que[tmp].comeTime <= lastTime && tmp < num) //��ǰʱ���ڵĽ���
				{
					if (que[tmp].finish == NoEnd) //��û��ɵĽ������
					{
						q.push(que[tmp]);
					}
					tmp++;
				}

				//�������д���
				cout << "��ǰ�������еĽ��̺�Ϊ: ";
				for (int i = 0; i < q.size(); i++)
				{
					cout << q.front().pid;
					q.push(q.front());
					q.pop();
				}
				cout << endl;
			}
			//���̲���������
			else
			{
				//�����ǰ���̵���
				ans.order = ID++;
				ans.beginTime = lastTime;
				ans.endTime = ans.beginTime + ans.slot;
				cout << "��ǰ���������Ϣ(����˳��/PID/����ʼʱ��/�������ʱ��/���ȼ�): ";
				printf("%d/%d/%d/%d/%d\n", ans.order, ans.pid, ans.beginTime, ans.endTime, ans.priority);
				v[ID - 2] = ans;
				endTime = ans.endTime;
				lastTime = ans.endTime;
				ans.runTime -= ans.slot;

				while (que[tmp].comeTime <= lastTime && tmp < num) //��ǰʱ���ڵĽ���
				{
					if (que[tmp].finish == NoEnd) //��û��ɵĽ������
					{
						q.push(que[tmp]);
					}
					tmp++;
				}
				q.push(ans); //�����½���������õ�ǰ�������

				//�������д���
				cout << "��ǰ�������еĽ��̺�Ϊ: ";
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

	// cpu���ȿ��ӻ�
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
	queue<FCB> q; //��������
	struct FCB v[1000];
	int lastTime = que[0].comeTime; //��ǰʱ��
	int ID = 1;						//���˳��
	int tmp = 0, counts = 0;		//��ǰ���̣��������
	int start = lastTime;			//��ǰ���̿�ʼʱ��
	int maxId, Smax;
	int endTime = 0;
	while (counts < num)
	{
		while (que[tmp].comeTime <= lastTime && tmp < num) //��ǰʱ���ڵĽ���
		{
			if (que[tmp].comeTime > start && que[tmp].comeTime < lastTime)
			{
				que[tmp].priority = max(que[tmp].priority + 1, 0); //�ȴ�ʱ���ȼ���1
			}
			tmp++;
		}

		//Ѱ��������ȼ�����
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
		if (maxId == -1) //�����ǰʱ����̶������˾͵ȴ���һ������
		{
			lastTime = que[tmp].comeTime;
			continue;
		}
		//���н���
		start = lastTime;
		//������������
		if (que[maxId].runTime <= que[maxId].slot)
		{
			que[maxId].order = ID++;
			que[maxId].beginTime = lastTime;
			que[maxId].endTime = lastTime + que[maxId].runTime;
			cout << "��ǰ���������Ϣ(����˳��/PID/����ʼʱ��/�������ʱ��/���ȼ�): ";
			printf("%d/%d/%d/%d/%d\n", que[maxId].order, que[maxId].pid, que[maxId].beginTime, que[maxId].endTime, que[maxId].priority - 1);
			v[ID - 2] = que[maxId];
			counts++;
			lastTime += que[maxId].runTime;
			que[maxId].finish = IsEnd;
		}
		//���̲���������
		else
		{
			que[maxId].order = ID++;
			que[maxId].beginTime = lastTime;
			que[maxId].endTime = lastTime + que[maxId].slot;
			cout << "��ǰ���������Ϣ(����˳��/PID/����ʼʱ��/�������ʱ��/���ȼ�): ";
			printf("%d/%d/%d/%d/%d\n", que[maxId].order, que[maxId].pid, que[maxId].beginTime, que[maxId].endTime, que[maxId].priority - 1);
			v[ID - 2] = que[maxId];
			lastTime += que[maxId].slot;
			que[maxId].runTime -= que[maxId].slot;
		}

		//�ı�������ȼ�
		for (int i = 0; i < tmp; i++)
		{
			if (que[i].finish == NoEnd)
			{
				if (i != maxId) //�������ȼ���3
				{
					que[i].priority = max(que[i].priority + 1, 0);
					q.push(que[i]);
				}
				else //�ȴ����ȼ���1
				{
					que[i].priority -= 1;
				}
			}
		}

		for (int i = 0; i < tmp; i++)
		{
			if (que[i].finish == NoEnd)
			{
				if (i == maxId) //�������ȼ���3
				{
					q.push(que[i]);
				}
			}
		}

		//�������д���
		cout << "��ǰ�������еĽ��̺�Ϊ: ";
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

	// cpu���ȿ��ӻ�
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
	cout << "��ѡ������Ҫ�ĵ��ȳ���: 1:'�����ȷ���' 2:'���ɰ���Ķ���ҵ����' 3:'�ɰ���Ķ���ҵ����' 4:'ʱ��Ƭ��ת' 5:'���ȼ�����'  : ";
	cin >> sig; //�㷨ѡ���־
	//��������
	cout << "�����뵱ǰ�������� : ";
	cin >> key;
	cout << "���������������Ϣ(�ֱ�Ϊ:PID/����ʱ��/��������ʱ��/���ȼ�/ʱ��Ƭ) " << endl;
	while (num < key && ~scanf("%d/%d/%d/%d/%d", &que[num].pid, &que[num].comeTime, &que[num].runTime, &que[num].priority, &que[num].slot))
	{
		que[num].beginTime = que[num].endTime = que[num].order = 0;
		que[num].finish = NoEnd;
		num++;
	}
	//ѡ���㷨
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
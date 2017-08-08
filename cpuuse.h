#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct         //����һ��cpu occupy�Ľṹ��
{
	char name[20];      //����һ��char���͵�������name��20��Ԫ��
	unsigned int user; //����һ���޷��ŵ�int���͵�user
	unsigned int nice; //����һ���޷��ŵ�int���͵�nice
	unsigned int system;//����һ���޷��ŵ�int���͵�system
	unsigned int idle; //����һ���޷��ŵ�int���͵�idle
}CPU_OCCUPY;


double cal_cpuoccupy(CPU_OCCUPY *o, CPU_OCCUPY *n)
{
	unsigned int od, nd;
	unsigned int id, sd;
	double cpu_use = 0;
	double st,tol;

	od = (unsigned int)(o->user + o->nice + o->system + o->idle);//��һ��(�û�+���ȼ�+ϵͳ+����)��ʱ���ٸ���od
	nd = (unsigned int)(n->user + n->nice + n->system + n->idle);//�ڶ���(�û�+���ȼ�+ϵͳ+����)��ʱ���ٸ���od

	id = (unsigned int)(n->user - o->user);    //�û���һ�κ͵ڶ��ε�ʱ��֮���ٸ���id
	sd = (unsigned int)(n->system - o->system);//ϵͳ��һ�κ͵ڶ��ε�ʱ��֮���ٸ���sd
	st=sd+id;
	tol=nd-od;
    printf("%.2f,%.2f\n",st,tol);
	if (tol != 0)
		cpu_use = (st * 100) / tol; //((�û�+ϵͳ)��100)��(��һ�κ͵ڶ��ε�ʱ���)�ٸ���g_cpu_used
	else 
		cpu_use = 0;
	//printf("cpu: %.2f\%\n", cpu_use);
	return cpu_use;
}

void get_cpuoccupy(CPU_OCCUPY *cpust) //��������get��������һ���βνṹ����Ū��ָ��O
{
	FILE *fd;
	int n;
	char buff[256];
	CPU_OCCUPY *cpu_occupy;
	cpu_occupy = cpust;

	fd = fopen("/proc/stat", "r");
	fgets(buff, sizeof(buff), fd);

	sscanf(buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice, &cpu_occupy->system, &cpu_occupy->idle);
	//printf("name = %s, user = %u, nice = %u, system = %u , idle = %u \n", cpu_occupy->name, cpu_occupy->user, cpu_occupy->nice, cpu_occupy->system, cpu_occupy->idle);

	fclose(fd);
}

int cpuuse()
{
	CPU_OCCUPY cpu_stat1;
	CPU_OCCUPY cpu_stat2;
	double cpu;
	//��һ�λ�ȡcpuʹ�����
	//printf("===============================cpu use================================\n");
	get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
	sleep(5);

	//�ڶ��λ�ȡcpuʹ�����
	get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);

	//����cpuʹ����
	cpu = cal_cpuoccupy((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
	printf("cpu use = %.2f\%\n", cpu);
	//printf("======================================================================\n");

	return 0;
}

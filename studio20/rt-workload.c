#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define SUCCESS 0
#define FAILURE -1
#define LOOP_LIMIT 500000000 // five hundred million

enum args
{
	PROGRAM_NAME,
	CPU_NO,
	RT_PRIORITY,
	EXPECTED_ARGC
};

int main(int argc, char* argv[])
{
	int ret;
	int cpuno, rt_priority;
	int count = 0, accumulator = 0;
	cpu_set_t set;
	struct sched_param sparam;

	// validate input.
	if (argc != EXPECTED_ARGC) {
		fprintf(stderr, "usage: %s <cpuno, rt_priority>\n", argv[PROGRAM_NAME]);
		return FAILURE;
	}
	cpuno = atoi(argv[CPU_NO]);
	rt_priority = atoi(argv[RT_PRIORITY]);

	//check rt_priority range.
	if (rt_priority < sched_get_priority_min(SCHED_RR) || rt_priority > sched_get_priority_max(SCHED_RR)) {
		fprintf(stderr, "priority range is from %d to %d \n", sched_get_priority_min(SCHED_RR), sched_get_priority_max(SCHED_RR));
		return FAILURE;
	}

	sparam.sched_priority = rt_priority;

	//set scheduler
	if (sched_setscheduler(0, SCHED_RR, &sparam) == -1) {
		perror("set scheduler failed.");
		return FAILURE;
	}

	// set mask.
	CPU_ZERO(&set);
	CPU_SET(cpuno, &set);

	// set affinity.
	ret = sched_setaffinity(getpid(), sizeof(set), &set) ;
	if (ret < SUCCESS)
	{
		perror("failed to set affinity");
		return ret;
	}
	
	while (++count < LOOP_LIMIT)
	{
		accumulator = (accumulator + 1) * 3;
	}
	
	return SUCCESS;
}

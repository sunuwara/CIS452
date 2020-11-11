#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>

int main(int argc, char *argv[]) {

	struct rlimit limit;
	struct timespec resolution;
	
	// Get page size in bytes
	int pageSize = sysconf(_SC_PAGESIZE);
	printf("Page size (bytes): %d\n", pageSize);
	
	// get physical pages in system
	int physicalPages = sysconf(_SC_PHYS_PAGES);
	printf("Physical pages in system: %d\n", physicalPages);
	
	// get maximum # of processes per user
	int maxProcesses = sysconf(_SC_CHILD_MAX);
	printf("Maximum # of processes per user: %d\n", maxProcesses);
	
	// get maximum filesize (bytes)
	getrlimit(RLIMIT_FSIZE, &limit);
	printf("Maximum filesize (bytes): %lu\n", limit.rlim_cur);
	
	// get maximum # of open files (hard limit)
	getrlimit(RLIMIT_NOFILE, &limit);
	printf("Maximum # of open files (hard limit): %lu\n", limit.rlim_max);
	
	// get maximum # of open files (soft limit)
	getrlimit(RLIMIT_NOFILE, &limit);
	printf("Maximum # of open files (soft limit): %lu\n", limit.rlim_cur);
	
	// get number of clock ticks per second
	int numClockTicks = sysconf(_SC_CLK_TCK);
	printf("Number of clock ticks per second: %d\n", numClockTicks);
	
	// get clock resolution
	clock_getres(CLOCK_REALTIME, &resolution);
	printf("Clock resolution (nano seconds): %ld\n", resolution.tv_nsec);

	return 0;
}

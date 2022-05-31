//MODIFIED VERSION OF mem_test.c
//MODIFIED BY CAN CIFTCI
//ADDED ONE FUNCTION NAMED get_memory_usage

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <string.h>

int parseLine(char*);
int currentlyUsedMem();
int test1(int, int);
int test2(int, int);

struct sysinfo memInfo;

int main() 
{
	sysinfo(&memInfo);
	pid_t pid = fork();

	long long totalMem = memInfo.totalram;

	if(pid > 0) {
		
		//totalMem *= memInfo.mem_unit;
		//printf("Total RAM: %lld\n", totalMem);
		
		// Call your system call;
		printf("1) Currently used memory by Parent: %d\n",get_memory_usage(getppid()));
		//printf("1) Currently used memory by Parent: %d\n",currentlyUsedMem());
		
		// Call your system call;
		int prevUsage = get_memory_usage(getpid());
		//int prevUsage = currentlyUsedMem();
		
		int sizeP = 1024*1024;
	
		int a[sizeP];
		int i = 0;
		
		for(i=0; i<sizeP; i++)
			a[i] = 5;

		// Call your system call;
		int lastUsage = get_memory_usage(getpid());
		//int lastUsage = currentlyUsedMem();
		
		// Call your system call;
		printf("2) Currently used memory by Parent: %d\n",get_memory_usage(getpid()));
		//printf("2) Currently used memory by Parent: %d\n",currentlyUsedMem());


		// Run Test #1
		if(test1(currentlyUsedMem(),(lastUsage-prevUsage)))
			printf("PASS\n");
		else 
			printf("FAIL\n");
		// Run Test #2
		if(test2(currentlyUsedMem(),get_memory_usage(getpid())))
			printf("PASS\n");
		else 
			printf("FAIL\n");
	}

	return 0;
}

// -------------- UPDATED PART ------------------

int get_memory_usage(int pid)
{
	int retVal = 0;
	retVal = syscall( 548, pid);
	retVal = retVal / 1024;  	// I added this line because my system call returns bytes not kilobytes.
	return retVal;
}

// -------------- UPDATED PART ------------------

// parse file
int parseLine(char* line) {

	int i = strlen(line);
	const char* p = line;
	while(*p < '0' || *p > '9')p++;
	line[i-3] = '\0';
	i = atoi(p);
	return i;
}

// Another alternative function to find currently used memory
// However, this function is not a system call and only used for test.
int currentlyUsedMem() {
	FILE* file = fopen("/proc/self/status", "r");
	int res = -1;

	char line[128];

	while(fgets(line, 128, file) != NULL) {
		if(strncmp(line, "VmRSS:", 6) == 0) {
			res = parseLine(line);
			break;
		}
	}
	fclose(file);
	return res;
}

int test1(int currentMem, int size) {

	if(currentMem > size && currentMem < size + 900) 
		return 1;
	else 
		return 0;
}

int test2(int osSysCall, int mySysCall) {
	if((osSysCall % mySysCall) <= 32)
		return 1;
	else
		return 0;

}
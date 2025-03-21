/*
	Practice 3: Examine the process memory regions
	  In this section, we try to touch the different regions in memory layout of the process
*/

#include <stdio.h>

int global_init_data = 22;
int global_non_init_data;

void func(unsigned long number)
{
	unsigned long local_data = number;

	printf("Process ID = %d\n", getpid());
	printf("Addresses of the process :\n");
	printf(" 1. global_init data = %p\n", &global_init_data);
	printf(" 2. global_noninit data = %p\n", &global_non_init_data);
	printf(" 3. print func() = %p\n", &func);
	printf(" 4. local data = %p\n", &local_data);
}

int main(int argc, char *argv[])
{
	func(10);
	
	while (1)
	{
		usleep(0);
	}
	
}
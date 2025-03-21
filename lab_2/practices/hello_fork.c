/*
	Practice 2: Traverse the tree of processes
	  We create a process based on our previous example and add additional call of fork.
*/

#include <stdio.h>

int main(int argc, char * argv[]) {
	fork();

	printf("now it more sciene! bitch!\n");
	getc(stdin);

	return 0;
}
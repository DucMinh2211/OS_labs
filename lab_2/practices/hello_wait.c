/*
	Practice 1: Create process

	  Recall the experiment of creating a process with additional IO waiting
*/

# include<stdio.h>

int main(int argc, char* argv[]) {
	printf("science bitch/n");
	getc(stdin); // Dòng này nói với máy tính rằng "ayyo khi nào tao cho phép (ấn phím bất kỳ) thì mày mới được terminate process này"
	
	return 0;
}
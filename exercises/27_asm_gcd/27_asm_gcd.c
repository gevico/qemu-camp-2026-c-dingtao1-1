#include <stdio.h>

unsigned int gcd_asm(unsigned int a, unsigned int b)
{
	unsigned int result;

	__asm__ volatile(
		"movl %1, %%eax\n\t"
		"movl %2, %%ebx\n\t"
		"jmp 2f\n\t"
		"1:\n\t"
		"xorl %%edx, %%edx\n\t"
		"divl %%ebx\n\t"
		"movl %%ebx, %%eax\n\t"
		"movl %%edx, %%ebx\n\t"
		"2:\n\t"
		"testl %%ebx, %%ebx\n\t"
		"jne 1b\n\t"
		"movl %%eax, %0\n\t"
		: "=r"(result)
		: "r"(a), "r"(b)
		: "eax", "ebx", "edx", "cc");

	return result;
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf("%d\n", gcd_asm(12, 8));
	printf("%d\n", gcd_asm(7, 5));
	return 0;
}

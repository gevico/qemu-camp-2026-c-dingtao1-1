#include <stdio.h>

typedef int (*op_fn)(int, int);

static int add(int a, int b) { return a + b; }
static int subtract(int a, int b) { return a - b; }
static int multiply(int a, int b) { return a * b; }
static int divide(int a, int b) { return a / b; }

int main(void)
{
	int a, b;
	char op;

	op_fn operations[] = {add, subtract, multiply, divide};

	char operators[] = "+-*/";

	printf("输入两个整数和一个运算符 (+, -, *, /): ");
	scanf("%d %d %c", &a, &b, &op);

	int index = -1;
	for (int i = 0; i < 4; i++) {
		if (op == operators[i]) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		printf("无效的运算符\n");
		return 1;
	}

	if (index == 3 && b == 0) {
		printf("除零错误\n");
		return 0;
	}

	int result = operations[index](a, b);
	printf("%d %c %d = %d\n", a, op, b, result);

	return 0;
}

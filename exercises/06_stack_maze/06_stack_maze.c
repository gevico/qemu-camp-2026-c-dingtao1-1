#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct {
	int r;
	int c;
	int d;
} Frame;

#define MAX_STACK 32

int main(void)
{
	static int visited[MAX_ROW][MAX_COL];
	/* 上、右、下、左 —— 与测试期望路径一致 */
	static const int dr[] = {-1, 0, 1, 0};
	static const int dc[] = {0, 1, 0, -1};

	Frame stack[MAX_STACK];
	int sp = 0;

	stack[sp++] = (Frame){0, 0, 0};
	visited[0][0] = 1;

	while (sp > 0) {
		Frame *f = &stack[sp - 1];
		if (f->r == MAX_ROW - 1 && f->c == MAX_COL - 1)
			break;

		if (f->d < 4) {
			int nr = f->r + dr[f->d];
			int nc = f->c + dc[f->d];
			f->d++;
			if (nr >= 0 && nr < MAX_ROW && nc >= 0 && nc < MAX_COL &&
			    maze[nr][nc] == 0 && !visited[nr][nc]) {
				visited[nr][nc] = 1;
				stack[sp++] = (Frame){nr, nc, 0};
			}
		} else {
			visited[f->r][f->c] = 0;
			sp--;
		}
	}

	if (sp == 0) {
		printf("No path!\n");
		return 0;
	}

	for (int i = sp - 1; i >= 0; i--)
		printf("(%d, %d)\n", stack[i].r, stack[i].c);

	return 0;
}

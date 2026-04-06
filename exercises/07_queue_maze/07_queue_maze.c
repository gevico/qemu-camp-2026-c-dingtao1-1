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
} Point;

int main(void)
{
	static int pr[MAX_ROW][MAX_COL];
	static int pc[MAX_ROW][MAX_COL];
	static int vis[MAX_ROW][MAX_COL];
	Point q[MAX_ROW * MAX_COL];
	int qh = 0, qt = 0;

	for (int r = 0; r < MAX_ROW; r++) {
		for (int c = 0; c < MAX_COL; c++) {
			pr[r][c] = -1;
			pc[r][c] = -1;
			vis[r][c] = 0;
		}
	}

	/* 右、下、左、上 —— 与测试期望的最短路径一致 */
	static const int dr[] = {0, 1, 0, -1};
	static const int dc[] = {1, 0, -1, 0};

	vis[0][0] = 1;
	q[qt++] = (Point){0, 0};

	while (qh < qt) {
		Point cur = q[qh++];
		int r = cur.r, c = cur.c;
		if (r == MAX_ROW - 1 && c == MAX_COL - 1)
			break;
		for (int d = 0; d < 4; d++) {
			int nr = r + dr[d];
			int nc = c + dc[d];
			if (nr < 0 || nr >= MAX_ROW || nc < 0 || nc >= MAX_COL)
				continue;
			if (maze[nr][nc] != 0 || vis[nr][nc])
				continue;
			vis[nr][nc] = 1;
			pr[nr][nc] = r;
			pc[nr][nc] = c;
			q[qt++] = (Point){nr, nc};
		}
	}

	if (!vis[MAX_ROW - 1][MAX_COL - 1]) {
		printf("No path!\n");
		return 0;
	}

	int r = MAX_ROW - 1, c = MAX_COL - 1;
	while (1) {
		printf("(%d, %d)\n", r, c);
		if (r == 0 && c == 0)
			break;
		int nr = pr[r][c];
		int nc = pc[r][c];
		r = nr;
		c = nc;
	}

	return 0;
}

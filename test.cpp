#include <stdio.h>

#include "task.h"

int main() {
  Row rows[] = {
      {1000, 9},  {1000, 10},  {1000, 20}, {2000, 23}, {2000, 49}, {2000, 50}, {2000, 54},
      {2000, 66}, {2000, 100}, {3000, 9},  {3000, 11}, {3000, 33}, {3000, 66}, {4000, 44},
  };
  printf("task1\n");
  task1(rows, sizeof(rows) / sizeof(rows[0]));

  printf("task2\n");
  task2(rows, sizeof(rows) / sizeof(rows[0]));

  printf("task3\n");
  task3(rows, sizeof(rows) / sizeof(rows[0]));

  printf("task4\n");
  const int i_cnt = 99;
  const int j_cnt = 1000;
  const int row_cnt = i_cnt * j_cnt;
  Row* huge_rows = new Row[row_cnt];
  for (int i = 0; i < i_cnt; i++) {
    for (int j = 0; j < j_cnt; j++) {
      huge_rows[j_cnt * i + j].a = (i + 1) * 1000;
      huge_rows[j_cnt * i + j].b = j;
    }
  }
  task4(huge_rows, row_cnt);
  delete[] huge_rows;
  return 0;
}
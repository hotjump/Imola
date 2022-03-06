#include "task.h"

#include <stdio.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>

#include "min_heap.h"

using namespace std::chrono;
using namespace std;

// task1
void task1(const Row* rows, int nrows) {
  if (!rows || nrows <= 0) return;

  for (int i = 0; i < nrows; i++) {
    int a = rows[i].a;
    int b = rows[i].b;
    if ((b >= 10 && b < 50) && (a == 1000 || a == 2000 || a == 3000)) printf("%d,%d\n", a, b);
  }
}

bool less_func(const Row& lhs, const Row& rhs) { return lhs.a < rhs.a || (lhs.a == rhs.a && lhs.b < rhs.b); }
bool need_serach_next(const Row* row, int target_a, int target_b[]) {
  return row->a == target_a && row->b >= target_b[0] && row->b < target_b[1];
}

// task2
// 使用二分查找加快对目标值的搜索
void task2(const Row* rows, int nrows) {
  if (!rows || nrows <= 0) return;
  int target_a[] = {1000, 2000, 3000};
  const int target_a_cnt = sizeof(target_a) / sizeof(target_a[0]);
  int target_b[] = {10, 50};

  auto rows_begin = rows;
  auto rows_end = rows + nrows;
  // 1. 查找目标的行
  for (int i = 0; i < target_a_cnt; i++) {
    // 1.1 通过二分查找找到第一个大于等于目标值(targget_a, 10)，作为target_begin
    auto target_begin = rows_begin = std::lower_bound(rows_begin, rows_end, Row(target_a[i], target_b[0]), less_func);
    if (rows_begin >= rows_end) break;
    if (!need_serach_next(target_begin, target_a[i], target_b)) continue;
    // 1.2 继续通过二分查找找到第一个大于等于目标值(targget_a, 50)，作为target_end
    auto target_end = rows_begin = std::lower_bound(target_begin, rows_end, Row(target_a[i], target_b[1]), less_func);
    // 1.3 打印结果
    for (auto j = target_begin; j < target_end; j++) {
      printf("%d,%d\n", j->a, j->b);
    }
    if (rows_begin >= rows_end) break;
  }
}

typedef struct TargetRows {
  const Row* begin;
  const Row* end;
} TargetRows;

void merge_target_rows(TargetRows target_rows[], int channel_cnt) {
  while (channel_cnt > 0) {
    if (channel_cnt == 1) {
      for (auto i = target_rows[0].begin; i < target_rows[0].end; i++) {
        printf("%d,%d\n", i->a, i->b);
      }
      break;
    }
    // task3归并路数较少，就偷个懒没写个最小堆或者败者树
    int min_i = 0;
    int min_b = target_rows[0].begin->b;
    for (int i = 1; i < channel_cnt; i++) {
      if (target_rows[i].begin->b < min_b) {
        min_b = target_rows[i].begin->b;
        min_i = i;
      }
    }
    printf("%d,%d\n", target_rows[min_i].begin->a, min_b);
    // 如果某个channel到达尾部，则将它移除
    if (++(target_rows[min_i].begin) == target_rows[min_i].end) {
      while (min_i < channel_cnt - 1) {
        target_rows[min_i] = target_rows[++min_i];
      }
      --channel_cnt;
    }
  }
}

// task3
// 使用三路归并排序实现对b列的有序输出
void task3(const Row* rows, int nrows) {
  if (!rows || nrows <= 0) return;

  int target_a[] = {1000, 2000, 3000};
  const int target_a_cnt = sizeof(target_a) / sizeof(target_a[0]);
  int target_b[] = {10, 50};

  TargetRows target_rows[target_a_cnt];
  int find_target_row_cnt = 0;

  auto rows_begin = rows;
  auto rows_end = rows + nrows;
  // 1. 查找目标的行
  for (int i = 0; i < target_a_cnt; i++) {
    // 1.1 通过二分查找找到第一个大于等于目标值(targget_a, 10)，作为target_begin
    auto target_begin = rows_begin = std::lower_bound(rows_begin, rows_end, Row(target_a[i], target_b[0]), less_func);
    if (rows_begin >= rows_end) break;
    if (!need_serach_next(target_begin, target_a[i], target_b)) continue;
    // 1.2 继续通过二分查找找到第一个大于等于目标值(targget_a, 50)，作为target_end
    auto target_end = rows_begin = std::lower_bound(target_begin, rows_end, Row(target_a[i], target_b[1]), less_func);
    // 1.3 将结果暂存
    target_rows[find_target_row_cnt].begin = target_begin;
    target_rows[find_target_row_cnt++].end = target_end;
    if (rows_begin >= rows_end) break;
  }

  // 2. 打印结果
  if (find_target_row_cnt > 0) {
    merge_target_rows(target_rows, find_target_row_cnt);
  }
}

template <size_t S>
class TargetRowsCollector {
 public:
  TargetRows rows[S];
  int find_target_row_cnt{0};

  void collect(const Row* begin, const Row* end) {
    if (begin >= end) return;
    rows[find_target_row_cnt].begin = begin;
    rows[find_target_row_cnt++].end = end;
  }

  void binary_collect_target_rows(int* target_a, int target_num, int target_b[], const Row* rows_begin,
                                  const Row* rows_end) {
    if (target_num == 0 || rows_begin >= rows_end) {
      return;
    } else if (target_num == 1) {
      auto target_begin = std::lower_bound(rows_begin, rows_end, Row(target_a[0], target_b[0]), less_func);
      auto target_end = std::lower_bound(target_begin, rows_end, Row(target_a[0], target_b[1]), less_func);
      collect(target_begin, target_end);
      return;
    } else {
      int mid = target_num / 2;
      auto target_begin = std::lower_bound(rows_begin, rows_end, Row(target_a[mid], target_b[0]), less_func);
      auto target_end = std::lower_bound(target_begin, rows_end, Row(target_a[mid], target_b[1]), less_func);
      collect(target_begin, target_end);
      binary_collect_target_rows(target_a, mid, target_b, rows_begin, target_begin);
      binary_collect_target_rows(&target_a[mid + 1], target_num - mid - 1, target_b, target_end, rows_end);
    }
  }

  void merge_target_rows_by_min_heap() {
    // 模板的两个int分别对应b和这个值来自哪个队列
    MinHeap<int, int> min_heap(S);
    // init
    for (int i = 0; i < find_target_row_cnt; i++) {
      min_heap.insert(rows[i].begin->b, i);
    }
    while (!min_heap.isEmpty()) {
      int i;
      min_heap.getMin(i);
      min_heap.deleteMin();
      printf("%d,%d\n", rows[i].begin->a, rows[i].begin->b);
      if (++(rows[i].begin) != rows[i].end) {
        min_heap.insert(rows[i].begin->b, i);
      }
    }
  }
};

// task4
// 1. 对1000, 2000, 3000, ..., 98000, 99000的查找由顺序优化成二分的方式，可以进一步减少下一层二分查找的时间复杂度。
// 2. 将多路归并使用最小堆或者败者树的方式优化，可以减少比较次数。
// 3. TODO:
// 如果在多核CPU下，可以考虑使用与核数相同的线程进行归并排序。比如8核，则可以使用各12/13路先归并一轮，最后再归并一轮即可。
void task4(const Row* rows, int nrows) {
  if (!rows || nrows <= 0) return;
  int target_a[99];
  const int target_a_cnt = sizeof(target_a) / sizeof(target_a[0]);
  for (int i = 0; i < target_a_cnt; i++) {
    target_a[i] = (i + 1) * 1000;
  }
  int target_b[] = {10, 50};

  TargetRowsCollector<target_a_cnt> rows_coll;
  /*
   1. 查找目标的行
   假如a 列上的匹配条件不只是 1000,2000,3000, 而是扩充 1000, 2000, 3000, ..., 98000, 99000
   虽然查找对应的target时是二分查找，但是第一层循环依然是顺序的，如果数据包含以上所有值并且分布较为均衡
   查找的数据比例大约将 2 *（1+ 98/99 + 97/99 + ... + 1/99) = 100
   如果在第一层就按二分的方式，
   查找的数据比例大约为 2 *（1 + 2*1/2 + 4*1/4 + 8*1/8 + ... + 64*1/64）= 12
  */
  rows_coll.binary_collect_target_rows(target_a, target_a_cnt, target_b, rows, rows + nrows);

  /*
   2. 打印结果
   在task上，只有三路，于是用最简单的归并写法，每产生一个值需要做两次比较；当归并的路数有k时，则需要做k-1次比较。
   这里可以使用最小堆或者败者树进行优化，最小堆的比较次数大约为2*logk, 而败者树的比较次数大约是logk。
   TODO: 题目说按照b排序，没有说严格按照(b，a)，这里偷个懒没有对a进行处理
  */
  if (rows_coll.find_target_row_cnt > 0) {
    rows_coll.merge_target_rows_by_min_heap();
  }
}

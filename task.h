#pragma once

typedef struct Row {
  Row() {}
  Row(int _a, int _b) : a(_a), b(_b) {}
  int a;
  int b;
} Row;

/*!
 * Task 1.
 *
 * Find out all the rows that sastify below conditions:
 *
 * ((b >= 10 && b < 50) &&
 * (a == 1000 || a == 2000 || a == 3000))
 *
 * Print them to the terminal, one row per line, for example: *
 *
 * 1000,20
 * 1000,23
 * 2000,16
 *
 * @param nrows The total number of rows.
 * @param rows The rows, for example rows[0] is the first row.
 */
void task1(const Row* rows, int nrows);

/*!
 * Task 2.
 * 如果输入的参数 rows 已经按照 (a,b) 进行过排序, 想办法对 task1() 的执行性能进行优化
 */
void task2(const Row* rows, int nrows);

/*!
 * Task 3.
 * 在任务 2 的基础上, 我们期望你打印出的匹配行是按照 b 列进行排序的, 请尽量优化函数的执行性能.
 */
void task3(const Row* rows, int nrows);

/*!
 * Task 4.
 * 思考任务, 不需要实现. 基于任务 3, 如果 a 列上的匹配条件不只是 1000,2000,3000, 而是扩充成
 * 1000,2000,3000,...,98000,99000, 你在任务 3 中进行的实现是否足够优化, 是否有更好的优化手段?
 */
void task4(const Row* rows, int nrows);

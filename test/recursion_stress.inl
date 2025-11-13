/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

// Testing the speed of dynamic typing/garbage collection

#include <Commons/io/print.hh>
#include <Commons/dp/var.hh>


using namespace cm;


static var fib(var n)
{
    if (n < 2)
        return n;
    else
        return fib(n - 1) + fib(n - 2);
}

static int fibInt(int n)
{
    if (n < 2)
        return n;
    else
        return fibInt(n - 1) + fibInt(n - 2);
}

constexpr static auto Swap(auto* a, auto* b)
{
    auto tmp = *a;
    *a = *b;
    *b = tmp;
}

static void QSort(var* base, usize num, var (*cmp)(var*, var*))
{
    var* left = base;
    var* right = base + num - 1;
    var pivot = base[num / 2];

    do {
        for (; (*cmp)(left, &pivot) < 0; left++)
            ;
        for (; (*cmp)(right, &pivot) > 0; right--)
            ;
        if (left <= right)
            Swap(left++, right--);
    } while (left <= right);

    auto i = (usize)(right + 1 - base);

    if (1 < i && i < num)
        QSort(base, i, cmp);

    i = (usize)(base + num - left);

    if (1 < i && i < num)
        QSort(left, i, cmp);
}

static void QSortInt(int* base, usize num, int (*cmp)(int*, int*))
{
    int* left = base;
    int* right = base + num - 1;
    int pivot = base[num / 2];
    do {
        for (; (*cmp)(left, &pivot) < 0; left++)
            ;
        for (; (*cmp)(right, &pivot) > 0; right--)
            ;
        if (left <= right)
            Swap(left++, right--);
    } while (left <= right);

    auto i = (usize)(right + 1 - base);

    if (1 < i && i < num)
        QSortInt(base, i, cmp);

    i = (usize)(base + num - left);

    if (1 < i && i < num)
        QSortInt(left, i, cmp);
}

static void testFib()
{


    auto t1 = __builtin_readcyclecounter();
    io::println(fib(20));
    auto t2 = __builtin_readcyclecounter();
    io::println("cycles: ", t2 - t1);

    auto t3 = __builtin_readcyclecounter();
    io::println(fibInt(20));
    auto t4 = __builtin_readcyclecounter();
    io::println("cycles: ", t4 - t3);

    io::println("fibInt is ", 1.0 / (double(t4 - t3) / double(t2 - t1)), " times faster");
}

static void testQSort()
{


    Array<int> q1;
    srand(__builtin_readcyclecounter());
    for (int i = 0; i < 50000; i++)
        q1 += rand() * rand();

    Array<var> q2;
    srand(__builtin_readcyclecounter());
    for (int i = 0; i < 50000; i++)
        q2 += rand() * rand();


    auto t1 = __builtin_readcyclecounter();
    QSort(q2.data(), q2.length(), [](var* a, var* b) -> var { return a - b; });
    auto t2 = __builtin_readcyclecounter();
    io::println("cycles: ", t2 - t1);

    auto t3 = __builtin_readcyclecounter();
    QSortInt(q1.data(), q1.length(), [](int* a, int* b) -> int { return *a - *b; });
    auto t4 = __builtin_readcyclecounter();
    io::println("cycles: ", t4 - t3);

    io::println("qsortInt is ", 1.0 / (double(t4 - t3) / double(t2 - t1)), " times faster");
}

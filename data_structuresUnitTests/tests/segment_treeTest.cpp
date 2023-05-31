#include "my_testing_tools.hpp"
#include "segment_tree.hpp"

typedef long long ll;

using namespace std;

TEST(SegmentTree, Test1)
{// Дерево отрезков на сумму
// https://codeforces.com/edu/course/2/lesson/4/1/practice/contest/273169/problem/A

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() : m_sum(0) {}
      CalcData(const ll value) : m_sum(value) {}

      ll m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() : m_toAssign(numeric_limits<ll>::max()) {}
      ModifyOp(const ll toAssign) : m_toAssign(toAssign) {}

      ll m_toAssign;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs = rhs;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      nodeCalcData.m_sum = modifyOp.m_toAssign * (rx - lx);
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  const vector<ll> input = { 5, 4, 2, 3, 5 };

  MySegmentTree segTree;
  segTree.Build(input);

  ASSERT_EQUAL(segTree.Calculate(0,  3 ).m_sum, 11);
  segTree.Modify(1, { 1 });
  ASSERT_EQUAL(segTree.Calculate(0,  3 ).m_sum, 8);
  segTree.Modify(3, { 1 });
  ASSERT_EQUAL(segTree.Calculate(0,  5 ).m_sum, 14);
}

TEST(SegmentTree, Test2)
{// Дерево отрезков на минимум
// https://codeforces.com/edu/course/2/lesson/4/1/practice/contest/273169/problem/B

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() : m_min(numeric_limits<ll>::max()) {}
      CalcData(ll value) : m_min(value) {}

      ll m_min;
    };

    struct ModifyOp
    {
      ModifyOp() : m_toAssign(numeric_limits<ll>::min()) {}
      ModifyOp(const ll toAssign) : m_toAssign(toAssign) {}

      ll m_toAssign;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs = rhs;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t, const size_t) override
    {
      nodeCalcData.m_min = modifyOp.m_toAssign;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_min = min(leftChild.m_min, rightChild.m_min);
      return res;
    }
  };

  const vector<ll> input = { 5, 4, 2, 3, 5 };

  MySegmentTree segTree;
  segTree.Build(input);

  ASSERT_EQUAL(segTree.Calculate(0, 3).m_min, 2);
  segTree.Modify(2, { 6 });
  ASSERT_EQUAL(segTree.Calculate(0,  3 ).m_min, 4);
  segTree.Modify(3, { 1 });
  ASSERT_EQUAL(segTree.Calculate(0,  5 ).m_min, 1);
}

TEST(SegmentTree, Test3)
{// Число минимумов на отрезке
// https://codeforces.com/edu/course/2/lesson/4/1/practice/contest/273169/problem/C
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test4)
{// Отрезок с максимальной суммой
// https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/A
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test5)
{// Первый элемент не меньше X
// https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/C
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test6)
{// Прибавление к отрезку
// https://codeforces.com/edu/course/2/lesson/4/3/practice/contest/274545/problem/E
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test7)
{// Криптография (умножение матриц)
// https://codeforces.com/edu/course/2/lesson/4/4/practice/contest/274684/problem/B
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test8)
{// Прибавление к отрезку
// https://codeforces.com/edu/course/2/lesson/5/1/practice/contest/279634/problem/A
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test9)
{// Применение MAX к отрезку
// https://codeforces.com/edu/course/2/lesson/5/1/practice/contest/279634/problem/B
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test10)
{// Присваивание к отрезку
// https://codeforces.com/edu/course/2/lesson/5/1/practice/contest/279634/problem/C
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test11)
{// Прибавление и минимум
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/A
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test12)
{// Умножение и сумма
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/B
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test13)
{// Битовые OR и AND
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/C
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test14)
{// Прибавление и сумма
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/D
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test15)
{// Присваивание и минимум
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/E
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test16)
{// Присваивание и сумма
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/F
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test17)
{// Присваивание и максимальный отрезок
// https://codeforces.com/edu/course/2/lesson/5/3/practice/contest/280799/problem/A
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test18)
{// Прибавление и элемент не меньше X
// https://codeforces.com/edu/course/2/lesson/5/3/practice/contest/280799/problem/C
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test19)
{// Присваивание, прибавление и сумма
// https://codeforces.com/edu/course/2/lesson/5/4/practice/contest/280801/problem/A
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test20)
{// Сумма и умножение

  struct Node
  {
    typedef double T;

    struct CalcData
    {
      CalcData() : m_sum(-1) {} // wrong initial value shouldn`t affect the result
      CalcData(const double value) : m_sum(value) {}

      double m_sum;
    };

    struct ModifyOp
    {
      //ModifyOp() : m_k(1), m_x(0) {}
      ModifyOp() = default;
      ModifyOp(const double k, const double x) : m_k(k), m_x(x) {}

      double m_k;
      double m_x;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs.m_k *= rhs.m_k;
      lhs.m_x = lhs.m_x * rhs.m_k + rhs.m_x;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      nodeCalcData.m_sum = nodeCalcData.m_sum * modifyOp.m_k + modifyOp.m_x * (rx - lx);
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  const vector<double> input = { 5, 4, 2, 3, 5 };

  MySegmentTree segTree;
  segTree.Build(input);

  ASSERT(mathdef::is_eq(segTree.Calculate(1, 5).m_sum, 14));
  segTree.Modify(1, 2, { 1, 5 });
  ASSERT(mathdef::is_eq(segTree.Calculate(0, 3).m_sum, 16));
  segTree.Modify(1, 4, { 2, 0 });
  ASSERT(mathdef::is_eq(segTree.Calculate(0, 5).m_sum, 38));
  segTree.Modify(0, 3, { 1, 1 });
  ASSERT(mathdef::is_eq(segTree.Calculate(1, 3).m_sum, 24));
  segTree.Modify(1, 3, { 3, 0 });
  ASSERT(mathdef::is_eq(segTree.Calculate(0, 5).m_sum, 89));
  segTree.Modify(0, 4, { 0.5, 0 });
  ASSERT(mathdef::is_eq(segTree.Calculate(1, 5).m_sum, 44));
}
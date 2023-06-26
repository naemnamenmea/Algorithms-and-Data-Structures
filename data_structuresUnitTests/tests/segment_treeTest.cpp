#include "my_testing_tools.hpp"
#include "math_constants.hpp"
#include "segment_tree.hpp"
#include "matrixUtils.hpp"

typedef long long ll;

using namespace std;
using namespace std::placeholders;
using namespace Ankor;

TEST(SegmentTree, Test1)
{// Дерево отрезков на сумму
// https://codeforces.com/edu/course/2/lesson/4/1/practice/contest/273169/problem/A

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value) {}

      T m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() : m_toAssign(numeric_limits<T>::max()) {} // wrong initial value shouldn`t affect the result
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
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

  ASSERT_EQUAL(segTree.Calculate(0, 3).m_sum, 11);
  segTree.Modify(1, { 1 });
  ASSERT_EQUAL(segTree.Calculate(0, 3).m_sum, 8);
  segTree.Modify(3, { 1 });
  ASSERT_EQUAL(segTree.Calculate(0, 5).m_sum, 14);
}

TEST(SegmentTree, Test2)
{// Дерево отрезков на минимум
// https://codeforces.com/edu/course/2/lesson/4/1/practice/contest/273169/problem/B

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_min(value) {}

      T m_min;
    };

    struct ModifyOp
    {
      //ModifyOp() : m_toAssign(numeric_limits<ll>::min()) {}
      ModifyOp() = default;
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
  ASSERT_EQUAL(segTree.Calculate(0, 3).m_min, 4);
  segTree.Modify(3, { 1 });
  ASSERT_EQUAL(segTree.Calculate(0, 5).m_min, 1);
}

TEST(SegmentTree, Test3)
{// Число минимумов на отрезке
// https://codeforces.com/edu/course/2/lesson/4/1/practice/contest/273169/problem/C

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_min(value), m_cnt(1) {}

      T m_min;
      size_t m_cnt;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
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
      if (leftChild.m_min < rightChild.m_min)
      {
        res.m_min = leftChild.m_min;
        res.m_cnt = leftChild.m_cnt;
      }
      else if (leftChild.m_min > rightChild.m_min)
      {
        res.m_min = rightChild.m_min;
        res.m_cnt = rightChild.m_cnt;
      }
      else
      {
        res.m_min = leftChild.m_min;
        res.m_cnt = leftChild.m_cnt + rightChild.m_cnt;
      }

      return res;
    }
  };

  const vector<ll> input = { 3, 4, 3, 5, 2 };

  MySegmentTree segTree;
  segTree.Build(input);

  Node::CalcData tmp;
  tmp = segTree.Calculate(0, 3); ASSERT_EQUAL(tmp.m_min, 3); ASSERT_EQUAL(tmp.m_cnt, 2);
  segTree.Modify(1, { 2 });
  tmp = segTree.Calculate(0, 3); ASSERT_EQUAL(tmp.m_min, 2); ASSERT_EQUAL(tmp.m_cnt, 1);
  segTree.Modify(0, { 2 });
  tmp = segTree.Calculate(0, 5); ASSERT_EQUAL(tmp.m_min, 2); ASSERT_EQUAL(tmp.m_cnt, 3);
}

TEST(SegmentTree, Test4)
{// Отрезок с максимальной суммой
// https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/A

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value), m_maxPrefixSum(max(T(0), value)), m_maxSuffixSum(max(T(0), value)), m_maxSum(max(T(0), value)) {}

      T m_sum;
      T m_maxPrefixSum;
      T m_maxSuffixSum;
      T m_maxSum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
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
      nodeCalcData = CalcData(modifyOp.m_toAssign);
    }

    CalcData CombineCalcData(const CalcData& lhs, const CalcData& rhs) override
    {
      CalcData res;
      res.m_sum = lhs.m_sum + rhs.m_sum;
      res.m_maxPrefixSum = max(lhs.m_maxPrefixSum, lhs.m_sum + rhs.m_maxPrefixSum);
      res.m_maxSuffixSum = max(rhs.m_maxSuffixSum, lhs.m_maxSuffixSum + rhs.m_sum);
      res.m_maxSum = (std::max)({ lhs.m_maxSum, rhs.m_maxSum, lhs.m_maxSuffixSum + rhs.m_maxPrefixSum });

      return res;
    }
  };

  MySegmentTree segTree;
  {
    const vector<ll> input = { 5, -4, 4, 3, -5 };
    size_t n = input.size();
    segTree.Build(input);

    ASSERT_EQUAL(segTree.Calculate(0, n).m_maxSum, 8);
    segTree.Modify(4, { 3 });
    ASSERT_EQUAL(segTree.Calculate(0, n).m_maxSum, 11);
    segTree.Modify(3, { -1 });
    ASSERT_EQUAL(segTree.Calculate(0, n).m_maxSum, 7);
  }
  {
    const vector<ll> input = { -2, -1, -5, -4 };
    size_t n = input.size();
    segTree.Build(input);

    ASSERT_EQUAL(segTree.Calculate(0, n).m_maxSum, 0);
    segTree.Modify(1, { 3 });
    ASSERT_EQUAL(segTree.Calculate(0, n).m_maxSum, 3);
    segTree.Modify(3, { 2 });
    ASSERT_EQUAL(segTree.Calculate(0, n).m_maxSum, 3);
  }
}

TEST(SegmentTree, Test5)
{// Первый элемент не меньше X
// https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/C
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test7)
{// Криптография (умножение матриц)
// https://codeforces.com/edu/course/2/lesson/4/4/practice/contest/274684/problem/B

  using ValueType = int;
  using Mtrx = SquareMatrix<ValueType, 2>;
  struct Node
  {
    typedef Mtrx T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T& value) : m_mult(value) {}

      T m_mult;
    };

    struct ModifyOp
    {
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    MySegmentTree(ValueType mod) : m_mod(mod) {}

    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs; rhs;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      nodeCalcData; modifyOp; lx; rx;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_mult = MtrxUtils::Mult(leftChild.m_mult, rightChild.m_mult, m_mod);
      return res;
    }

  private:
    ValueType m_mod;
  };

  vector<Mtrx> mtrxArray = {
    {{0, 1}, {0, 0}},
    {{2, 1}, {1, 2}},
    {{0, 0}, {0, 2}},
    {{1, 0}, {0, 2}},
  };
  ValueType mod = 3;
  auto segTree = MySegmentTree(mod);
  segTree.Build(mtrxArray);
  ASSERT_EQUAL(segTree.Calculate(0, 4).m_mult, Mtrx({ {0, 2}, {0, 0} }));
  ASSERT_EQUAL(segTree.Calculate(1, 3).m_mult, Mtrx({ {0, 2}, {0, 1} }));
  ASSERT_EQUAL(segTree.Calculate(0, 3).m_mult, Mtrx({ {0, 1}, {0, 0} }));
  ASSERT_EQUAL(segTree.Calculate(1, 2).m_mult, Mtrx({ {2, 1}, {1, 2} }));
}

TEST(SegmentTree, Test8)
{// Прибавление к отрезку
// https://codeforces.com/edu/course/2/lesson/5/1/practice/contest/279634/problem/A

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value) {}

      T m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs.m_toAssign += rhs.m_toAssign;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      lx; rx;
      nodeCalcData.m_sum += modifyOp.m_toAssign;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  MySegmentTree segTree;
  int n = 5;
  segTree.Build(n, 0);

  segTree.Modify(0, 3, { 3 });
  ASSERT_EQUAL(segTree.Get(1).m_sum, 3);
  segTree.Modify(2, 4, { 4 });
  ASSERT_EQUAL(segTree.Get(3).m_sum, 4);
  ASSERT_EQUAL(segTree.Get(4).m_sum, 0);
}

TEST(SegmentTree, Test9)
{// Применение MAX к отрезку
// https://codeforces.com/edu/course/2/lesson/5/1/practice/contest/279634/problem/B

  struct Node
  {
    typedef int T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_value(value) {}

      T m_value;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
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
      lx; rx;
      nodeCalcData.m_value = max(nodeCalcData.m_value, modifyOp.m_toAssign);
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      leftChild; rightChild;
      return {};
    }
  };

  MySegmentTree segTree;
  int n = 5;
  segTree.Build(n, 0);

  segTree.Modify(0, 3, { 3 });
  ASSERT_EQUAL(segTree.Get(1).m_value, 3);
  segTree.Modify(2, 4, { 4 });
  ASSERT_EQUAL(segTree.Get(3).m_value, 4);
  ASSERT_EQUAL(segTree.Get(4).m_value, 0);
}

TEST(SegmentTree, Test10)
{// Присваивание к отрезку
// https://codeforces.com/edu/course/2/lesson/5/1/practice/contest/279634/problem/C

  struct Node
  {
    typedef int T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_value(value) {}

      T m_value;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
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
      lx; rx;
      nodeCalcData.m_value = modifyOp.m_toAssign;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      leftChild; rightChild;
      return {};
    }
  };

  MySegmentTree segTree;
  int n = 5;
  segTree.Build(n, 0);

  segTree.Modify(0, 3, { 3 });
  ASSERT_EQUAL(segTree.Get(1).m_value, 3);
  segTree.Modify(2, 4, { 4 });
  ASSERT_EQUAL(segTree.Get(3).m_value, 4);
  ASSERT_EQUAL(segTree.Get(4).m_value, 0);
}

TEST(SegmentTree, Test11)
{// Прибавление и минимум
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/A

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_min(value) {}

      T m_min;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAdd(toAssign) {}

      T m_toAdd;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs.m_toAdd += rhs.m_toAdd;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      lx; rx;
      nodeCalcData.m_min += modifyOp.m_toAdd;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_min = min(leftChild.m_min, rightChild.m_min);
      return res;
    }
  };

  MySegmentTree segTree;
  int n = 5;
  segTree.Build(n, 0);

  segTree.Modify(0, 3, { 3 });
  ASSERT_EQUAL(segTree.Calculate(1, 2).m_min, 3);
  segTree.Modify(1, 4, { 4 });
  ASSERT_EQUAL(segTree.Calculate(1, 3).m_min, 7);
  ASSERT_EQUAL(segTree.Calculate(1, 4).m_min, 4);
  ASSERT_EQUAL(segTree.Calculate(3, 5).m_min, 0);
}

TEST(SegmentTree, Test12)
{// Умножение и сумма
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/B

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value) {}

      T m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_multiplier(toAssign) {}

      T m_multiplier;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs.m_multiplier = (lhs.m_multiplier * rhs.m_multiplier) % MOD;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      lx; rx;
      nodeCalcData.m_sum = (nodeCalcData.m_sum * modifyOp.m_multiplier) % MOD;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = (leftChild.m_sum + rightChild.m_sum) % MOD;
      return res;
    }

  private:
    const ll MOD = (ll)1e9 + 7;
  };

  MySegmentTree segTree;
  int n = 5;
  segTree.Build(n, 1);

  segTree.Modify(0, 3, { 3 });
  ASSERT_EQUAL(segTree.Calculate(1, 2).m_sum, 3);
  segTree.Modify(1, 4, { 4 });
  ASSERT_EQUAL(segTree.Calculate(1, 3).m_sum, 24);
  ASSERT_EQUAL(segTree.Calculate(1, 4).m_sum, 28);
  ASSERT_EQUAL(segTree.Calculate(3, 5).m_sum, 5);
}

TEST(SegmentTree, Test13)
{// Битовые OR и AND
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/C

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_bitwiseAND(value) {}

      T m_bitwiseAND;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_bitwiseOR(toAssign) {}

      T m_bitwiseOR;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs.m_bitwiseOR |= rhs.m_bitwiseOR;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      lx; rx;
      nodeCalcData.m_bitwiseAND |= modifyOp.m_bitwiseOR;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_bitwiseAND = leftChild.m_bitwiseAND & rightChild.m_bitwiseAND;
      return res;
    }
  };

  MySegmentTree segTree;
  int n = 5;
  segTree.Build(n, 0);

  segTree.Modify(0, 3, { 3 });
  ASSERT_EQUAL(segTree.Calculate(1, 2).m_bitwiseAND, 3);
  segTree.Modify(1, 4, { 4 });
  ASSERT_EQUAL(segTree.Calculate(1, 3).m_bitwiseAND, 7);
  ASSERT_EQUAL(segTree.Calculate(1, 4).m_bitwiseAND, 4);
  ASSERT_EQUAL(segTree.Calculate(3, 5).m_bitwiseAND, 0);
}

TEST(SegmentTree, Test14)
{// Прибавление и сумма
// https://codeforces.com/edu/course/2/lesson/5/2/practice/contest/279653/problem/D

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value) {}

      T m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAdd(toAssign) {}

      T m_toAdd;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs.m_toAdd += rhs.m_toAdd;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      nodeCalcData.m_sum += modifyOp.m_toAdd * (rx - lx);
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  MySegmentTree segTree;
  int n = 5;
  segTree.Build(n, 0);

  segTree.Modify(0, 3, { 3 });
  ASSERT_EQUAL(segTree.Calculate(1, 2).m_sum, 3);
  segTree.Modify(1, 4, { 4 });
  ASSERT_EQUAL(segTree.Calculate(1, 3).m_sum, 14);
  ASSERT_EQUAL(segTree.Calculate(1, 4).m_sum, 18);
  ASSERT_EQUAL(segTree.Calculate(3, 5).m_sum, 4);
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

TEST(SegmentTree, Test21)
{// K-я единица
// https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/B
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test22)
{// Первый элемент не меньше X - 2
// https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/D
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test23)
{// Инверсии
// https://codeforces.com/edu/course/2/lesson/4/3/practice/contest/274545/problem/A

  struct Node
  {
    typedef int T;

    struct CalcData
    {
      CalcData() : m_sum(0) {}
      CalcData(const double value) : m_sum(value) {}

      double m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      lhs.m_toAssign = rhs.m_toAssign;
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      lx; rx;
      nodeCalcData.m_sum = modifyOp.m_toAssign;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  const vector<int> input = { 4, 1, 3, 5, 2 };
  const vector<int> expected = { 0, 1, 1, 0, 3 };

  MySegmentTree segTree;
  auto n = input.size();
  segTree.Build(n, 0);

  for (int i = 0; i < n; ++i)
  {
    if (input[i] == n)
    {
      ASSERT(mathdef::is_eq(Node::CalcData().m_sum, expected[i]));
    }
    else
    {
      ASSERT(mathdef::is_eq(segTree.Calculate(input[i], n).m_sum, expected[i]));
    }
    segTree.Modify(input[i] - 1, 1);
  }
}

TEST(SegmentTree, Test24)
{// Инверсии 2
// https://codeforces.com/edu/course/2/lesson/4/3/practice/contest/274545/problem/B
  throw runtime_error("Not implemented");
}

TEST(SegmentTree, Test25)
{// Вложенные отрезки
// https://codeforces.com/edu/course/2/lesson/4/3/practice/contest/274545/problem/C

  struct Node
  {
    typedef int T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value) {}

      T m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
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
      lx; rx;
      nodeCalcData.m_sum = modifyOp.m_toAssign;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  const vector<int> input = { 5, 1, 2, 2, 3, 1, 3, 4, 5, 4 };
  const vector<int> expected = { 1, 0, 0, 0, 3 };
  auto n = expected.size();

  MySegmentTree segTree;
  segTree.Build(2 * n, 0);

  vector<int> p(n, -1);
  vector<int> res(n);
  for (int i = 0; i < 2 * n; ++i)
  {
    int v = input[i];
    if (p[v - 1] >= 0)
    {
      res[v - 1] = segTree.Calculate(p[v - 1], i).m_sum;
      segTree.Modify(p[v - 1], 1);
    }
    else
    {
      p[v - 1] = i;
    }
  }

  for (int i = 0; i < n; ++i)
  {
    ASSERT_EQUAL(res[i], expected[i]);
  }
}

TEST(SegmentTree, Test26)
{// Пересекающиеся отрезки
// https://codeforces.com/edu/course/2/lesson/4/3/practice/contest/274545/problem/D

  struct Node
  {
    typedef int T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value) {}

      T m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(const T toAssign) : m_toAssign(toAssign) {}

      T m_toAssign;
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
      lx; rx;
      nodeCalcData.m_sum = modifyOp.m_toAssign;
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  const vector<int> input = { 5, 1, 2, 2, 3, 1, 3, 4, 5, 4 };
  const vector<int> expected = { 1, 0, 1, 1, 1 };
  auto n = expected.size();

  MySegmentTree segTree;
  segTree.Build(2 * n, 0);

  vector<int> p(n, -1);
  vector<int> res(n);
  for (int i = 0; i < 2 * n; ++i)
  {
    int v = input[i];
    if (p[v - 1] >= 0)
    {
      res[v - 1] = segTree.Calculate(p[v - 1], i).m_sum - 1;
      segTree.Modify(p[v - 1], -1);
    }
    else
    {
      p[v - 1] = i;
    }
    segTree.Modify(i, 1);
  }

  for (int i = 0; i < n; ++i)
  {
    ASSERT_EQUAL(res[i], expected[i]);
  }
}

TEST(SegmentTree, Test27)
{// Присваивание, прибавление и сумма
// https://codeforces.com/edu/course/2/lesson/5/4/practice/contest/280801/problem/A

  enum class OP
  {
    ADD,
    ASSIGN
  };

  struct Node
  {
    typedef ll T;

    struct CalcData
    {
      CalcData() = default;
      CalcData(const T value) : m_sum(value) {}

      T m_sum;
    };

    struct ModifyOp
    {
      ModifyOp() = default;
      ModifyOp(OP op, const T value) : m_toAssign(op == OP::ASSIGN ? std::make_optional(value) : std::nullopt), m_toAdd(op == OP::ADD ? std::make_optional(value) : std::nullopt) {}

      std::optional<T> m_toAssign;
      std::optional<T> m_toAdd;
    };
  };

  struct MySegmentTree : public SegmentTree::SegmentTree<Node>
  {
    void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) override
    {
      if (rhs.m_toAssign)
      {
        lhs.m_toAssign = rhs.m_toAssign;
        lhs.m_toAdd = rhs.m_toAdd;
      }
      else
      {
        if (lhs.m_toAdd)
        {
          *lhs.m_toAdd += *rhs.m_toAdd;
        }
        else
        {
          lhs.m_toAdd = rhs.m_toAdd;
        }
      }
    }

    void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t lx, const size_t rx) override
    {
      if (modifyOp.m_toAssign)
      {
        nodeCalcData.m_sum = *modifyOp.m_toAssign * (rx - lx);
      }
      if(modifyOp.m_toAdd)
      {
        nodeCalcData.m_sum += *modifyOp.m_toAdd * (rx - lx);
      }
    }

    CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) override
    {
      CalcData res;
      res.m_sum = leftChild.m_sum + rightChild.m_sum;
      return res;
    }
  };

  {
    MySegmentTree segTree;
    int n = 5;
    segTree.Build(n, 0);

    segTree.Modify(0, 3, { OP::ASSIGN, 3 });
    segTree.Modify(2, 4, { OP::ADD, 2 });
    ASSERT_EQUAL(segTree.Calculate(1, 3).m_sum, 8);
    segTree.Modify(1, 5, { OP::ADD, 1 });
    segTree.Modify(0, 2, { OP::ASSIGN, 2 });
    ASSERT_EQUAL(segTree.Calculate(0, 3).m_sum, 10);
    ASSERT_EQUAL(segTree.Calculate(3, 5).m_sum, 4);
  }

  {
    MySegmentTree segTree;
    int n = 10;
    segTree.Build(n, 0);

    segTree.Modify(0, 7, { OP::ASSIGN, 3 });
    ASSERT_EQUAL(segTree.Calculate(1, 8).m_sum, 18);
    segTree.Modify(2, 10, { OP::ADD, 4 });
    //for (int i = 0; i < n; ++i)
    //{
    //  segTree.Get(i);
    //}
    ASSERT_EQUAL(segTree.Calculate(5, 9).m_sum, 22);
    segTree.Modify(3, 7, { OP::ADD, 1 });
    ASSERT_EQUAL(segTree.Calculate(1, 6).m_sum, 34);
    segTree.Modify(0, 4, { OP::ASSIGN, 2 });
    segTree.Modify(1, 5, { OP::ADD, 2 });
    ASSERT_EQUAL(segTree.Calculate(0, 7).m_sum, 40);
    segTree.Modify(4, 9, { OP::ADD, 6 });
    ASSERT_EQUAL(segTree.Calculate(0, 10).m_sum, 82);

    segTree.Modify(3, 6, { OP::ASSIGN, 1 });
    segTree.Modify(5, 7, { OP::ADD, 2 });
    ASSERT_EQUAL(segTree.Calculate(4, 8).m_sum, 30);

    segTree.Modify(0, 10, { OP::ASSIGN, 1 });
    segTree.Modify(2, 8, { OP::ADD, 2 });
    segTree.Modify(4, 5, { OP::ASSIGN, 0 });
    ASSERT_EQUAL(segTree.Calculate(0, 10).m_sum, 19);
  }
}
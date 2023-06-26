#pragma once

#include <vector>
#include <stdexcept>
#include <tuple>
#include <functional>
#include <optional>

namespace SegmentTree
{
  template<typename TNode>
  class SegmentTree
  {
  public:
    typedef typename TNode::T T;
    typedef typename TNode::CalcData CalcData;
    typedef typename TNode::ModifyOp ModifyOp;

    SegmentTree() : m_size((size_t)-1), m_realSize((size_t)-1) {}

    void Build(size_t n, const T& initValue)
    {
      Init(n);
      Build(n, initValue, 0, 0, m_size);
    }

    void Build(const std::vector<T>& arr)
    {
      Init(arr.size());
      Build(arr, 0, 0, m_size);
    }

    void Modify(size_t i, const ModifyOp& modifyOp)
    {
      Modify(i, i + 1, modifyOp);
    }

    void Modify(size_t l, size_t r, const ModifyOp& modifyOp)
    {
      ValidateBounds(l, r);

      InternalNode updateNode;
      updateNode.m_modifyOp = std::optional(modifyOp);
      Modify(l, r, updateNode, 0, 0, m_size);
    }

    CalcData Get(size_t i)
    {
      return Calculate(i, i + 1);
    }

    CalcData Calculate(size_t l, size_t r)
    {
      ValidateBounds(l, r);

      return *Calculate(l, r, 0, 0, m_size);
    }

  private:
    struct InternalNode
    {
      std::optional<CalcData> m_calcOnSegmentData;
      std::optional<ModifyOp> m_modifyOp;
    };

    virtual void UpdateModifyOp(ModifyOp& lhs, const ModifyOp& rhs) = 0;

    virtual void UpdateCalcData(CalcData& nodeCalcData, const ModifyOp& modifyOp, const size_t, const size_t) = 0;

    virtual CalcData CombineCalcData(const CalcData& leftChild, const CalcData& rightChild) = 0;

    void Init(size_t n)
    {
      m_realSize = n;
      m_size = 1;
      while (m_size < n) m_size *= 2;

      m_tree.assign(2 * m_size - 1, InternalNode{} /*init element (list node)*/);
      //m_tree.resize(2 * m_size - 1);
    }

    //bool IsDummyNode(size_t nodeNum) const
    //{
    //  size_t treeMaxLvl = MostSignificantBitIdx(m_size);
    //  size_t nodeLvl = MostSignificantBitIdx(nodeNum + 1);
    //  size_t minForbiddenIdxOnLastLayer = (1 << treeMaxLvl) - 1 + m_realSize;

    //  return nodeNum >= minForbiddenIdxOnLastLayer / (size_t(1) << (treeMaxLvl - nodeLvl));
    //}

    void ValidateBounds(size_t l, size_t r)
    {
      if (!mathdef::is_within(l, 0, r, true, false) || !mathdef::is_within(r, l, m_realSize, false, true))
      {
        const std::string err_msg = "l=" + std::to_string(l) + " or r=" + std::to_string(r) + " is outside of [0;" + std::to_string(m_realSize) + "] or l>=r";
        throw std::domain_error(err_msg.c_str());
      }
    }

    static InternalNode BuildOneElement(const T& value) // construct list node
    {
      InternalNode res;
      res.m_calcOnSegmentData = std::make_optional<CalcData>(value);
      return res;
    }

    void Propagate(size_t x, size_t lx, size_t rx)
    {
      if (!m_tree[x].m_modifyOp.has_value() || rx - lx == 1) return;

      size_t m = (lx + rx) / 2;

      _UpdateModifyOp(m_tree[2 * x + 1].m_modifyOp, m_tree[x].m_modifyOp);
      _UpdateCalcData(m_tree[2 * x + 1].m_calcOnSegmentData, m_tree[x].m_modifyOp, lx, m);
      _UpdateModifyOp(m_tree[2 * x + 2].m_modifyOp, m_tree[x].m_modifyOp);
      _UpdateCalcData(m_tree[2 * x + 2].m_calcOnSegmentData, m_tree[x].m_modifyOp, m, rx);
      m_tree[x].m_modifyOp = std::nullopt;
    }

    void Build(size_t n, const T& initValue, size_t x, size_t lx, size_t rx)
    {
      if (rx - lx == 1)
      {
        if (lx < n)
          m_tree[x] = BuildOneElement(initValue);
        return;
      }

      size_t m = (lx + rx) / 2;
      Build(n, initValue, 2 * x + 1, lx, m);
      Build(n, initValue, 2 * x + 2, m, rx);
      m_tree[x].m_calcOnSegmentData =
        _CombineCalcData(m_tree[2 * x + 1].m_calcOnSegmentData, m_tree[2 * x + 2].m_calcOnSegmentData);
    }

    void Build(const std::vector<T>& arr, size_t x, size_t lx, size_t rx)
    {
      if (rx - lx == 1)
      {
        if (lx < arr.size())
          m_tree[x] = BuildOneElement(arr[lx]);
        return;
      }

      size_t m = (lx + rx) / 2;
      Build(arr, 2 * x + 1, lx, m);
      Build(arr, 2 * x + 2, m, rx);
      m_tree[x].m_calcOnSegmentData =
        _CombineCalcData(m_tree[2 * x + 1].m_calcOnSegmentData, m_tree[2 * x + 2].m_calcOnSegmentData);
    }

    void _UpdateCalcData(std::optional<CalcData>& calcOnSegmentData, const std::optional<ModifyOp>& modifyOp, size_t lx, size_t rx)
    {
      if (!modifyOp.has_value() || !calcOnSegmentData.has_value())
        return;

      UpdateCalcData(*calcOnSegmentData, *modifyOp, lx, rx);
    }

    void _UpdateModifyOp(std::optional<ModifyOp>& lhs, const std::optional<ModifyOp>& rhs)
    {
      if (!rhs.has_value())
        return;

      if (!lhs.has_value())
      {
        lhs = rhs;
      }
      else
      {
        UpdateModifyOp(*lhs, *rhs);
      }
    }

    void Modify(size_t l, size_t r, const InternalNode& updateNode, size_t x, size_t lx, size_t rx)
    {
      Propagate(x, lx, rx);

      if (rx <= l || r <= lx)
        return;

      if (l <= lx && rx <= r)
      {
        _UpdateModifyOp(m_tree[x].m_modifyOp, updateNode.m_modifyOp);
        _UpdateCalcData(m_tree[x].m_calcOnSegmentData, updateNode.m_modifyOp, lx, rx);
        return;
      }

      size_t m = (lx + rx) / 2;
      Modify(l, r, updateNode, 2 * x + 1, lx, m);
      Modify(l, r, updateNode, 2 * x + 2, m, rx);
      m_tree[x].m_calcOnSegmentData =
        _CombineCalcData(m_tree[2 * x + 1].m_calcOnSegmentData, m_tree[2 * x + 2].m_calcOnSegmentData);
    }

    void Set(int i, T value, int x, int lx, int rx)
    {
      Propagate(x, lx, rx);
      
      if (rx - lx == 1)
      {
        m_tree[x] = BuildOneElement(std::move(value));
        return;
      }

      int m = (lx + rx) / 2;
      if (i < m)
      {
        Set(i, std::move(value), 2 * x + 1, lx, m);
      }
      else
      {
        Set(i, std::move(value), 2 * x + 2, m, rx);
      }

      m_tree[x].m_calcOnSegmentData =
        _CombineCalcData(m_tree[2 * x + 1].m_calcOnSegmentData, m_tree[2 * x + 2].m_calcOnSegmentData);
    }

    std::optional<CalcData> Calculate(size_t l, size_t r, size_t x, size_t lx, size_t rx)
    {
      Propagate(x, lx, rx);

      if (rx <= l || r <= lx)
        return std::nullopt; // neutral element (Zero element) or Node default constructor

      if (l <= lx && rx <= r)
        return m_tree[x].m_calcOnSegmentData;

      const size_t m = (lx + rx) / 2;
      const auto lhsCalcData = Calculate(l, r, 2 * x + 1, lx, m);
      const auto rhsCalcData = Calculate(l, r, 2 * x + 2, m, rx);

      return _CombineCalcData(lhsCalcData, rhsCalcData);
    }

    std::optional<CalcData> _CombineCalcData(const std::optional<CalcData>& lhsCalcData, const std::optional<CalcData>& rhsCalcData)
    {
      return !lhsCalcData ? rhsCalcData : rhsCalcData ? std::optional(CombineCalcData(*lhsCalcData, *rhsCalcData)) : lhsCalcData;
    }

  private:
    std::vector<InternalNode> m_tree;
    size_t m_size;
    size_t m_realSize;
  };
}
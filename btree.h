#include <iostream>
#include <memory>
#include <vector>

class BTreeNode : public std::enable_shared_from_this<BTreeNode> {
  std::vector<int> keys;
  int t;

  std::vector<std::shared_ptr<BTreeNode>> Child;
  int n;
  bool leaf;

public:
  BTreeNode(int _t, bool _leaf);

  void insertNonFull(int k);

  void splitChild(int i, std::shared_ptr<BTreeNode> y);

  void traverse();

  std::shared_ptr<BTreeNode> search(int k);

  int findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k) {
      ++idx;
    }
    return idx;
  }

  int getPred(int idx) {
    std::shared_ptr<BTreeNode> cur = Child[idx];
    while (!cur->leaf) {
      cur = cur->Child[cur->n];
    }

    return cur->keys[cur->n - 1];
  }

  int getSucc(int idx) {
    std::shared_ptr<BTreeNode> cur = Child[idx];
    while (!cur->leaf) {
      cur = cur->Child[0];
    }

    return cur->keys[0];
  }

  void remove(int k);
  void removeFromLeaf(int idx);
  void removeFromNonLeaf(int idx);

  void fill(int idx);
  void borrowFromPrev(int idx);
  void borrowFromNext(int idx);
  void merge(int idx);

  friend class BTree;
};

class BTree {
  std::shared_ptr<BTreeNode> root;
  int t;

public:
  BTree(int _t) {
    root = nullptr;
    t = _t;
  }

  void traverse() {
    if (root != nullptr) {
      root->traverse();
    }
  }

  std::shared_ptr<BTreeNode> search(int k) {
    return (root == nullptr) ? nullptr : root->search(k);
  }

  void insert(int k);
  void remove(int k);
};

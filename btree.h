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
};

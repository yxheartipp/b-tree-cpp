#include "btree.h"
#include <memory>

BTreeNode::BTreeNode(int _t, bool _leaf) {
  t = _t;
  leaf = _leaf;

  keys.resize(2 * t - 1);
  Child.resize(2 * t);

  n = 0;
}

void BTreeNode::traverse() {
  int i;
  for (i = 0; i < n; ++i) {
    if (leaf == false) {
      Child[i]->traverse();
    }
    std::cout << " " << keys[i];
  }

  if (leaf == false) {
    Child[i]->traverse();
  }
}

std::shared_ptr<BTreeNode> BTreeNode::search(int k) {
  int i = 0;
  while (i < n && k > keys[i]) {
    i++;
  }

  if (keys[i] == k) {
    return shared_from_this();
  }

  if (leaf == true) {
    return nullptr;
  }

  return Child[i]->search(k);
}

void BTree::insert(int k) {

  if (root == nullptr) {
    root = std::make_shared<BTreeNode>(t, true);
    root->keys[0] = k;
    root->n = 1;
  } else {
    // 根节点满了，增加高度
    if (root->n == 2 * t - 1) {
      std::shared_ptr<BTreeNode> s = std::make_shared<BTreeNode>(t, false);

      s->Child[0] = root;

      s->splitChild(0, root);

      int i = 0;

      if (s->keys[0] < k) {
        i++;
      }
      s->Child[i]->insertNonFull(k);

      root = s;
    } else {
      root->insertNonFull(k);
    }
  }
}

void BTree::remove(int k) {
  if (!root) {
    std::cout << "Tree is empty" << std::endl;
    return;
  }

  root->remove(k);
  if (root->n == 0) {
    std::shared_ptr<BTreeNode> tmp = root;
    if (root->leaf) {
      root = nullptr;
    } else {
      root = root->Child[0];
    }
  }
  return;
}

void BTreeNode::insertNonFull(int k) {
  int i = n - 1;
  if (leaf == true) {
    while (i >= 0 && keys[i] > k) {
      keys[i + 1] = keys[i];
      i--;
    }

    keys[i + 1] = k;
    n += 1;
  } else {
    while (i >= 0 && keys[i] > k) {
      i--;
    }
    if (Child[i + 1]->n == 2 * t - 1) {
      splitChild(i + 1, Child[i + 1]);

      if (keys[i + 1] < k) {
        i++;
      }
    }
    Child[i + 1]->insertNonFull(k);
  }
}

void BTreeNode::splitChild(int i, std::shared_ptr<BTreeNode> y) {
  // 申请所需新的节点的内存空间
  std::shared_ptr<BTreeNode> z = std::make_shared<BTreeNode>(y->n, y->leaf);

  z->n = t - 1;

  for (int j = 0; j < t - 1; j++) {
    z->keys[j] = y->keys[j + t];
  }

  if (y->leaf == false) {
    for (int j = 0; j < t; j++) {
      z->Child[j] = y->Child[j + t];
    }
  }

  y->n = t - 1;

  for (int j = n; j >= i + 1; j--) {
    Child[j + 1] = Child[j];
  }

  Child[i + 1] = z;

  for (int j = n - 1; j >= i; j--) {
    keys[j + 1] = keys[j];
  }

  keys[i] = y->keys[t - 1];

  n += 1;
}

void BTreeNode::remove(int k) {
  int idx = findKey(k);

  if (idx < n && keys[idx] == k) {
    if (leaf) {
      removeFromLeaf(idx);
    } else {
      removeFromNonLeaf(idx);
    }
  } else {
    if (leaf) {
      std::cout << "The key " << k << " is no exist in the tree" << std::endl;
      return;
    }

    bool flag = ((idx == n) ? true : false);
    // 下一个节点不够删除，提前进行处理
    if (Child[idx]->n < t) {
      fill(idx);
    }

    if (flag && idx > 0) {
      Child[idx - 1]->remove(k);
    } else {
      Child[idx]->remove(k);
    }
  }
  return;
}

void BTreeNode::removeFromLeaf(int idx) {
  for (int i = idx + 1; i < n; ++i) {
    keys[i - 1] = keys[i];
  }
  n--;
  return;
}

void BTreeNode::removeFromNonLeaf(int idx) {
  int k = keys[idx];
  // 从子节点中借一个
  if (Child[idx]->n >= t) {
    int pred = getPred(idx);
    keys[idx] = pred;
    Child[idx]->remove(pred);
  } else if (Child[idx + 1]->n >= t) {
    int succ = getSucc(idx);
    keys[idx] = succ;
    Child[idx]->remove(succ);
  } else {
    merge(idx);
    Child[idx]->remove(k);
  }
  return;
}

void BTreeNode::fill(int idx) {
  // 兄弟节点够借用
  if (idx != 0 && Child[idx - 1]->n >= t) {
    borrowFromPrev(idx);
  } else if (idx != n && Child[idx]->n >= t) {
    borrowFromNext(idx);
  } else {
    if (idx != n) {
      merge(idx);
    } else {
      merge(idx - 1);
    }
    return;
  }
}

void BTreeNode::borrowFromPrev(int idx) {
  std::shared_ptr<BTreeNode> child = Child[idx];
  std::shared_ptr<BTreeNode> sibling = Child[idx - 1];

  for (int i = child->n - 1; i >= 0; --i) {
    child->keys[i + 1] = child->keys[i];
  }

  if (!child->leaf) {
    for (int i = child->n; i >= 0; --i) {
      child->Child[i + 1] = child->Child[i];
    }
  }
  child->keys[0] = keys[idx - 1];
  if (!child->leaf) {
    child->Child[0] = sibling->Child[sibling->n];
  }

  keys[idx - 1] = sibling->keys[sibling->n - 1];
  child->n += 1;
  sibling->n -= 1;

  return;
}

void BTreeNode::borrowFromNext(int idx) {
  std::shared_ptr<BTreeNode> child = Child[idx];
  std::shared_ptr<BTreeNode> sibling = Child[idx - 1];

  child->keys[child->n] = keys[idx];

  if (!child->leaf) {
    child->Child[child->n + 1] = sibling->Child[0];
  }

  keys[idx] = sibling->keys[0];

  for (int i = 1; i < sibling->n; i++) {
    sibling->keys[i - 1] = sibling->keys[i];
  }

  if (!sibling->leaf) {
    for (int i = 1; i <= sibling->n; ++i) {
      sibling->Child[i - 1] = sibling->Child[i];
    }
  }

  child->n += 1;
  sibling->n -= 1;

  return;
}

void BTreeNode::merge(int idx) {
  std::shared_ptr<BTreeNode> child = Child[idx];
  std::shared_ptr<BTreeNode> sibling = Child[idx + 1];

  child->keys[t - 1] = keys[idx];
  for (int i = 0; i < sibling->n; ++i) {
    child->keys[i + t] = sibling->keys[i];
  }

  if (!child->leaf) {
    for (int i = 0; i < sibling->n; ++i) {
      child->Child[i + t] = sibling->Child[i];
    }
  }

  for (int i = idx + 1; i < n; ++i) {
    keys[i - 1] = keys[i];
  }
  for (int i = idx + 2; i <= n; ++i) {
    Child[i - 1] = Child[i];
  }

  child->n += sibling->n + 1;
  n--;

  return;
}
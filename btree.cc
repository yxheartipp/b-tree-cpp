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
  std::shared_ptr<BTreeNode> z = std::make_shared<BTreeNode>(y->t, y->leaf);

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

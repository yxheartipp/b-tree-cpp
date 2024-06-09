#include "btree.h"

int main() {
  BTree t(3);

  t.insert(10);
  t.insert(20);
  t.insert(5);
  t.insert(6);
  t.insert(12);
  t.insert(30);
  t.insert(7);
  t.insert(17);

  std::cout << "Traversal of the constructed tree is " << std::endl;

  t.traverse();
  int k = 6;

  (t.search(k) != NULL) ? std::cout << "\nPresent"
                        : std::cout << "\nNot Present";

  k = 15;
  (t.search(k) != NULL) ? std::cout << "\nPresent"
                        : std::cout << "\nNot Present";

  return 0;
}
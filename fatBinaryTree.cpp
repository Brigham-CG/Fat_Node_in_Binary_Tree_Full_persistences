#include "fatBinaryTree.hpp"
#include "fatNode.hpp"
#include "types.hpp"
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>

FatBinaryTree::FatBinaryTree() {
  this->mainPtr = new FatNode(numeric_limits<int>::max());
  this->currentVer = 0;
  this->maxVer = 0;
  FatNode::version++;
}

FatNode *FatBinaryTree::root(const size_t version) {
  return mainPtr->getLeft(version);
}

FatNode *FatBinaryTree::root() { return mainPtr->getLeft(this->currentVer); }

FatNode *FatBinaryTree::find(int x) {

  FatNode *ptr = this->root();

  while (ptr && ptr->getValue(this->currentVer) != x) {

    if (x > ptr->getValue(this->currentVer))
      ptr = (ptr)->getLeft(this->currentVer);
    else
      ptr = (ptr)->getRight(this->currentVer);
  }

  return ptr;
}

FatNode *FatBinaryTree::findParent(int x) {

    FatNode *ptr = this->root(this->currentVer);
    FatNode *ret = this->mainPtr;
    while (ptr and ptr->getValue(this->currentVer) != x) {
      ret = ptr;
      if (x < ptr->getValue(this->currentVer))
        ptr = ptr->getLeft(this->currentVer);
      else
        ptr = ptr->getRight(this->currentVer);
    }
    return ret;

}

void FatBinaryTree::insert(int x) {
  if (this->currentVer < this->maxVer)
    this->invalidateNodes(this->currentVer + 1);

  FatNode *fnd = this->findParent(x);
  bool inserted = false;

  // Find the correct position
  if (x < fnd->getValue(this->currentVer)) {
    if (!fnd->getLeft(this->currentVer)) {
      fnd->inject(x, Side::LEFT);
      inserted = true;
  }
  
  } else {
    if (!fnd->getRight(this->currentVer)) {
      fnd->inject(x, Side::RIGHT);
      inserted = true;
    }
  }

  if (inserted) {
    this->currentVer++;
    this->maxVer++;
  }
}

void FatBinaryTree::remove(int x) {

  cout << "mv:" << this->maxVer << endl;
  if (this->currentVer < this->maxVer)
    this->invalidateNodes(this->currentVer + 1);

  FatNode *parent = this->findParent(x);
  FatNode *child;
  Side side;

  if (x < parent->getValue(this->currentVer)) {
    side = Side::LEFT;
    child = parent->getLeft(this->currentVer);
    if (!child)
      return;
  } else {
    side = Side::RIGHT;
    child = parent->getRight(this->currentVer);
    if (!child)
      return;
  }

  if (child->getLeft(this->currentVer) and child->getRight(this->currentVer)) {
    // TODO: choose a random subtree.
    FatNode *tmp = child;
    parent = child;
    child = child->getLeft(this->currentVer);
    if (child->getRight(this->currentVer))
      side = Side::RIGHT;
    while (child->getRight(this->currentVer)) {
      parent = child;
      child = child->getRight(this->currentVer);
    }
    tmp->change(child->getValue(this->currentVer));
    // All this is just one operation, so all is contained in one version.
    FatNode::version--;
  }
  // TODO: dejar de usar side  y usar la rama que no es nula!

  if (child->getLeft(this->currentVer)) {
    parent->inject(child->getLeft(this->currentVer), side);
    // child->inject(nullptr, Side::LEFT);
  } else {
    parent->inject(child->getRight(this->currentVer), side);
    // child->inject(nullptr, Side::RIGHT);
  }

  this->currentVer++;
  this->maxVer++;
}

void FatBinaryTree::printBFS(const size_t version) {
  queue<FatNode *> nodeq;
  nodeq.push(this->root(version));
  while (!nodeq.empty()) {
    if (nodeq.front()) {
      cout << nodeq.front()->getValue(version) << ' ';
      nodeq.push(nodeq.front()->getLeft(version));
      nodeq.push(nodeq.front()->getRight(version));
    }
    nodeq.pop();
  }
  cout << "\n";
}

void FatBinaryTree::pprint() {

  vector<FatNode *> nodesV;
  nodesV.push_back(this->root());
  size_t size;
  
  cout << "current ver root: " << this-> currentVer << endl;

  while (nodesV.size() != 0) {
    size = nodesV.size();
    for (size_t i = 0; i < size; i++) {
      if (nodesV[i]) {
      
        cout << "current ver: " << this-> currentVer << endl;
        cout << " Value: " << nodesV[i]->getValue(this->currentVer) << ' ';

        nodesV.push_back(nodesV[i]->getLeft(this->currentVer));

        nodesV.push_back(nodesV[i]->getRight(this->currentVer));

      } else {
        cout << "Node nullptr\n";
      }
    }
    cout << '\n';
    nodesV.erase(nodesV.begin(), nodesV.begin() + size);
  }
}

string FatBinaryTree::printGraphviz(const size_t version) {
  stringstream output;
  size_t nulls = 0;
  queue<FatNode *> nodeq;
  nodeq.push(this->root(version));

  output << "digraph g{\n";

  while (!nodeq.empty()) {
    if (nodeq.front()) {
      FatNode *parentPtr = nodeq.front();
      int parent = parentPtr->getValue(version);
      if (parentPtr->getLeft(version)) {
        int childLeft = parentPtr->getLeft(version)->getValue(version);
        output << "  " << parent << " -> " << childLeft << ";\n";
      } else {
        output << "  null" << nulls << " [shape=none, label=\"null\"];\n";
        output << "  " << parent << " -> null" << nulls << ";\n";
        nulls++;
      }

      if (parentPtr->getRight(version)) {
        int childRight = parentPtr->getRight(version)->getValue(version);
        output << "  " << parent << " -> " << childRight << ";\n";
      } else {
        output << "  null" << nulls << " [shape=none, label=\"null\"];\n";
        output << "  " << parent << " -> null" << nulls << ";\n";
        nulls++;
      }

      nodeq.push(nodeq.front()->getLeft(version));
      nodeq.push(nodeq.front()->getRight(version));
    }
    nodeq.pop();
  }
  output << "}\n";

  return output.str();
}

bool FatBinaryTree::redo() {
  if (this->currentVer < this->maxVer) {
    this->currentVer++;
    return true;
  }
  return false;
}

bool FatBinaryTree::undo() {
  if (this->currentVer > 0) {
    this->currentVer--;
    return true;
  }
  return false;
}

void FatBinaryTree::invalidateNodes(const size_t version) {
  // TODO: delete the invalidates nodes!
  const size_t prefVer = version - 1;
  this->mainPtr->invalidateVers(version);

  queue<FatNode *> nodeq;
  nodeq.push(this->root(prefVer));
  while (!nodeq.empty()) {
    if (nodeq.front()) {
      nodeq.front()->invalidateVers(version);
      nodeq.push(nodeq.front()->getLeft(prefVer));
      nodeq.push(nodeq.front()->getRight(prefVer));
    }
    nodeq.pop();
  }

  this->maxVer = this->currentVer;
}
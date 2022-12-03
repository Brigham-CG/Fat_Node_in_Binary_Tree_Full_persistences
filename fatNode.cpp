#include "fatNode.hpp"
#include <cassert>
#include "fatBinaryTree.hpp"

size_t FatNode::version = 0;

FatNode::FatNode() {

  this->left[this->version] = nullptr;
  this->right[this->version] = nullptr;

}

FatNode::FatNode(const int value) {

  this->value[this->version] = value;
  this->left[this->version] = nullptr;
  this->right[this->version] = nullptr;
  
}

FatNode *FatNode::inject(const int value, const Side side) {
  FatNode *tmp = new FatNode(value);
  if (side == Side::LEFT) {
    this->left[this->version] = tmp;
  } else {
    this->right[this->version] = tmp;
  }
  this->version++;
  return tmp;
}

FatNode *FatNode::inject(FatNode *fatnode, const Side side) {
  if (side == Side::LEFT) {
    this->left[this->version] = fatnode;
  } else {
    this->right[this->version] = fatnode;
  }
  this->version++;
  return fatnode;
}

void FatNode::eject(const Side side) {
  if (side == Side::LEFT) {
    this->left[this->version] = nullptr;
  } else {
    this->right[this->version] = nullptr;
  }
  this->version++;
}

void FatNode::change(const int value) {
  this->value[this->version] = value;
  this->version++;
}

FatNode *FatNode::getLeft(const size_t version) {
  auto relevant = this->left.lower_bound(version);
  assert(relevant != this->left.end());
  return (*relevant).second;
}

FatNode *FatNode::getRight(const size_t version) {
  auto relevant = this->right.lower_bound(version);
  assert(relevant != this->right.end());
  return (*relevant).second;
}

int FatNode::getValue(const size_t version) {
  auto relevant = this->value.lower_bound(version);
  assert(relevant != this->value.end());
  return (*relevant).second;
}

void FatNode::setValue(int x, const size_t version, FatNode *n, FatBinaryTree fbt) {

  if (version < fbt.maxVer)
    fbt.invalidateNodes(fbt.currentVer + 1);

  bool inserted = false;

  // Find the correct position

    if ( x < n->getValue(version) ) {
      if (! n->getLeft(version)) {
        n->inject(x, Side::LEFT);
      } 
    }
     else {
    if (!n->getRight(version)) {
      n->inject(x, Side::RIGHT);
    }
  }
  inserted = true;

  if (inserted) {
    fbt.currentVer++;
    fbt.maxVer++;
  }
}

template <typename T> void FatNode::invalidateVers(vmap<T> &versionsMap) {
  auto relevant = versionsMap.lower_bound(version);
  assert(relevant != versionsMap.end());

  auto i = relevant;
  while (i != versionsMap.begin()) {
    if ((*i).second)
      i = versionsMap.erase(i);
    i--;
  }
  if (i != relevant)
    versionsMap.erase(i);
}

void FatNode::invalidateVers(const size_t version) {
  // Remember that this->version stores the lattest version + 1
  // and versions greater and equal than input version are removed.
  this->version = version;
  this->invalidateVers(this->left);
  this->invalidateVers(this->right);
  this->invalidateVers(this->value);
}

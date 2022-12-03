#pragma once
#include <string>
#include <vector>
using namespace std;

class FatNode;
class FatBinaryTree {
  public:
    size_t currentVer;
    size_t maxVer;

    // This FatNode points to the root and mantain it versions.
    // The tree is allocated in the left pointer, use root() to get the real root
    FatNode *mainPtr;

    FatBinaryTree();
    // To find the current version on nodes:
    // check for versions <= to the current version

    // Redo and undo return true if the tree changes, return false if not.
    bool redo();
    bool undo();


    FatNode *find(int x);

    // Returns the previous element if it doesn't find the searched element
    FatNode *findParent(int x);

    // Returns the root ptr.
    FatNode *root(const size_t version);
    FatNode *root();

    void insert(int x);
    void remove(int x);

    void printBFS(const size_t version);
    void pprint();
    string printGraphviz(const size_t version);

    void invalidateNodes(const size_t version);
   
};
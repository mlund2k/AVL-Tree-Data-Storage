#include <memory>
#include <string>
using namespace std;

class AVL;

class BSTNode {
 public:
 	BSTNode(int key);
 	BSTNode(int key, std::weak_ptr<BSTNode> parent);
 	bool IsLeaf() const;
 	bool IsMissingChild() const;
 	bool HasLeftChild() const;
 	bool HasRightChild() const;
 	void DeleteChild(std::shared_ptr<BSTNode> v);
 	void ReplaceChild(std::shared_ptr<BSTNode> v, std::shared_ptr<BSTNode> u);

 private:
  int key_;
  std::weak_ptr<BSTNode> parent_;
  std::shared_ptr<BSTNode> left_;
  std::shared_ptr<BSTNode> right_;
  int height_; // height of node
  int bf_; // balance factor of node

  friend AVL;
}; // class BSTNode

class AVL {
 public:
 	AVL();

 	void Insert(int key);
	void RRrot(std::shared_ptr<BSTNode> currentNode);// right right rotation
	void LLrot(std::shared_ptr<BSTNode> currentNode);// left left rotation
	void RLrot(std::shared_ptr<BSTNode> currentNode);// right left rotation
	void LRrot(std::shared_ptr<BSTNode> currentNode);// left right rotation
	bool Delete(int key);
 	bool Find(int key) const;
 	std::string JSON() const;
 	size_t size() const;
 	bool empty() const;
 	int DeleteMin();
	int getHeight(std::shared_ptr<BSTNode> currentNode); // get current node height
	int max(int x, int y); // find max of 2 heights
	int BF(std::shared_ptr<BSTNode> currentNode); // determine balance factor for a node

 private:
	void DeleteLeaf(std::shared_ptr<BSTNode> currentNode);
	int DeleteMin(std::shared_ptr<BSTNode> currentNode);

 	std::shared_ptr<BSTNode> root_;
 	size_t size_;
}; // class AVL
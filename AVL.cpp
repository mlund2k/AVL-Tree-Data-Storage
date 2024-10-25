#include "AVL.h"

#include <cassert>
#include <iostream>
#include <string>
#include <queue>

#include "json.hpp"


BSTNode::BSTNode(int key) :
	key_(key),
	parent_(std::weak_ptr<BSTNode>()),
	left_(nullptr),
	right_(nullptr),
	height_(0),
	bf_(0) {}

BSTNode::BSTNode(int key, std::weak_ptr<BSTNode> parent) :
	key_(key),
	parent_(parent),
	left_(nullptr),
	right_(nullptr),
	height_(0),
	bf_(0) {}

bool BSTNode::IsLeaf() const {
	return left_ == nullptr && right_ == nullptr;
}

bool BSTNode::HasLeftChild() const {
	return left_ != nullptr;
}

bool BSTNode::HasRightChild() const {
	return right_ != nullptr;
}

void BSTNode::DeleteChild(std::shared_ptr<BSTNode> v) {
	if (left_ == v) {
		left_ = nullptr;
	} else if (right_ == v) {
		right_ = nullptr;
	} else {
		std::cerr << "BSTNode::DeleteChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

void BSTNode::ReplaceChild(std::shared_ptr<BSTNode> v, std::shared_ptr<BSTNode> u) {
	if (left_ == u || right_ == u) {
		std::cerr << "BSTNode::ReplaceChild Error: child passed as replacement\n";
	}
	if (left_ == v) {
		left_ = u;
		u->parent_ = v->parent_;
	} else if (right_ == v) {
		right_ = u;
		u->parent_ = v->parent_;
	} else {
		std::cerr << "BSTNode::ReplaceChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

AVL::AVL() : root_(nullptr), size_(0) {}

void AVL::Insert(int key) {
	if (root_ == nullptr) { // if there is no root
		root_ = std::make_shared<BSTNode>(key); // make given key the new root
		size_++; // increase the size
		return; // key has been inserted
	}

	std::shared_ptr<BSTNode> currentNode = root_, lastNode = nullptr; // introduce node variables to iterate over tree
	while (currentNode != nullptr) { // until we run into a nullptr node
		lastNode = currentNode; // keep track of node we were just at
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_; // based on key value either go right or left
	}
	if (key < lastNode->key_) { // if key value less than node we were just at
		lastNode->left_ = std::make_shared<BSTNode>(key, lastNode); // insert key to left of that node
	} else { // if key value greater than node we were just at
		lastNode->right_ = std::make_shared<BSTNode>(key, lastNode); // insert key to right of that node
	}
	size_++; // increase the size
	
	if(size_ > 2){
		std::shared_ptr<BSTNode> p = lastNode->parent_.lock(); // create pointer to climb through tree
		while(p != nullptr){ // until we reach root node
			if(p->right_ != nullptr){
				getHeight(p->right_);
				BF(p->right_);
			}
			if(p->left_ != nullptr){
				getHeight(p->left_);
				BF(p->left_);			}
			if(BF(p) > 1){ // if encountered a right problem
				if(p->right_->left_ == nullptr){ // if right left grandchild is null
					LLrot(p); // perform left-left rotation
				}
				else if(p->right_->right_ == nullptr){ // if right grandchild is null
					LRrot(p); // perform left-right rotation
				}
				else if(p->right_->right_->height_ > p->right_->left_->height_){
					LLrot(p); // perform left-left rotation
				}
				else if(p->right_->left_->height_ > p->right_->right_->height_){
					LRrot(p); // perform left-right rotation
				}
			}
			else if(BF(p) < -1){ // if encountered left problem
				if(p->left_->right_ == nullptr){ // if left right grandchild is null
					RRrot(p); // perform right-right rotation
				}
				else if(p->left_->left_ == nullptr){ // if left grandchild is null
					RLrot(p); // perform right-left rotation
				}
				else if(p->left_->left_->height_ > p->left_->right_->height_){
					RRrot(p); // perform right-right rotation
				}
				else if(p->left_->right_->height_ > p->left_->left_->height_){
					RLrot(p); // perform right-left rotation
				}
			}
			if(p == root_){
				break;
			}
			else{
				p = p->parent_.lock(); // set to next parent
			}
			std::shared_ptr<BSTNode> test = lastNode; // create pointer to update heights
			while(test != nullptr){
				getHeight(test);
				BF(test);
				test = test->parent_.lock();
			}
		}
	}
	std::shared_ptr<BSTNode> test = lastNode; // create pointer to update heights
	while(test != nullptr){
		getHeight(test);
		BF(test);
		test = test->parent_.lock();
	}
	return; // done rebalancing
}

void AVL::RRrot(std::shared_ptr<BSTNode> currentNode){
	std::shared_ptr<BSTNode> b = currentNode->left_; // define left child
	std::shared_ptr<BSTNode> a = b->left_; // define left grandchild
	std::shared_ptr<BSTNode> b_rtree = b->right_; // define right tree of b

	if(currentNode == root_){ // if this function is called with the root node
		b->right_ = currentNode; // c now right child of b
		b->parent_.reset(); // since b is root it shouldnt point to any parent
		root_ = b; // b is new root
		currentNode->parent_ = b; // b is new parent of current node
		if(b_rtree != nullptr){ // if there is a right tree for b
			currentNode->left_ = b_rtree; // right tree of b now left tree of c
			b_rtree->parent_ = currentNode; // c now parent of right b tree
		}
		else{
			currentNode->left_.reset(); // or just reset this node
		}
	}
	else{
		b->right_ = currentNode; // c now right child of b
		std::shared_ptr<BSTNode> parent = currentNode->parent_.lock(); // convert weak pointer to shared pointer
		parent->ReplaceChild(currentNode, b); // c parent now parent of b
		currentNode->parent_ = b; // b now parent of c
		if(b_rtree != nullptr){ // if there is a right tree for b
			currentNode->left_ = b_rtree; // right tree of b now left tree of c
			b_rtree->parent_ = currentNode; // c now parent of right b tree
		}
		else{
			currentNode->left_.reset(); // or just reset this node
		}
	}
	getHeight(currentNode);
	currentNode->bf_ = BF(currentNode); // find balance factor of c
	getHeight(a);
	a->bf_ = BF(a); // find balance factor of a
	getHeight(b);
	b->bf_ = BF(b); // find balance factor of b
	
	std::shared_ptr<BSTNode> test = b->parent_.lock(); // define dummy pointer to iterate through branches of tree
	while(test != nullptr){ // until test reaches the root node
		getHeight(test);
		test->bf_ = BF(test); // find balance factor of node
		test = test->parent_.lock(); // set to next parent
	}
}

void AVL::LLrot(std::shared_ptr<BSTNode> currentNode){
	std::shared_ptr<BSTNode> b = currentNode->right_; // define right child
	std::shared_ptr<BSTNode> a = b->right_; // define right grandchild
	std::shared_ptr<BSTNode> b_ltree = b->left_; // define left tree of b

	if(currentNode == root_){ // if this function is called with the root node
		b->left_ = currentNode; // c now left child of b
		b->parent_.reset(); // since b is root it shouldnt point to any parent
		root_ = b; // b is new root
		currentNode->parent_ = b; // b is new parent of current node
		if(b_ltree != nullptr){ // if there is a right tree for b
			currentNode->right_ = b_ltree; // left tree of b now right tree of c
			b_ltree->parent_ = currentNode; // c now parent of left b tree
		}
		else{
			currentNode->right_.reset(); // or just reset this node
		}
	}
	else{
		b->left_ = currentNode; // c now left child of b
		std::shared_ptr<BSTNode> parent = currentNode->parent_.lock(); // convert weak pointer to shared pointer
		parent->ReplaceChild(currentNode, b); // c parent now parent of b
		currentNode->parent_ = b; // b now parent of c
		if(b_ltree != nullptr){ // if there is a right tree for b
			currentNode->right_ = b_ltree; // left tree of b now right tree of c
			b_ltree->parent_ = currentNode; // c now parent of left b tree
		}
		else{
			currentNode->right_.reset(); // or just reset this node
		}
	}
	getHeight(currentNode);
	currentNode->bf_ = BF(currentNode); // find balance factor of c
	getHeight(a);
	a->bf_ = BF(a); // find balance factor of a
	getHeight(b);
	b->bf_ = BF(b); // find balance factor of b

	std::shared_ptr<BSTNode> test = b->parent_.lock(); // define dummy pointer to iterate through branches of tree
	while(test != nullptr){ // until test reaches the root node
		getHeight(test);
		test->bf_ = BF(test); // find balance factor of node
		test = test->parent_.lock(); // set to next parent
	}
}

void AVL::LRrot(std::shared_ptr<BSTNode> currentNode){
	std::shared_ptr<BSTNode> b = currentNode->right_; // define right child
	std::shared_ptr<BSTNode> a = b->left_; // define right-left child
	std::shared_ptr<BSTNode> a_ltree = a->left_; // define left tree of a
	std::shared_ptr<BSTNode> a_rtree = a->right_; // define right tree of a

	if(currentNode == root_){ // if this function is called with the root node
		a->left_ = currentNode; // c now left child of a
		a->right_ = b; // b now right child of a
		a->parent_.reset(); // since a is root it shouldnt point to any parent
		root_ = a; // a is new root
		currentNode->parent_ = a; // a is new parent of current node
		b->parent_ = a; // a is new parent of b
		if(a_ltree != nullptr){ // if there is a left tree for a
			currentNode->right_ = a_ltree; // left tree of a now right tree of c
			a_ltree->parent_ = currentNode; // c now parent of left a tree
		}
		else{
			currentNode->right_.reset(); // or just reset this node
		}
		if(a_rtree != nullptr){ // if there is a right tree for a
			b->left_ = a_rtree; // right tree of a now left tree of b
			a_rtree->parent_ = b; // b is now parent of right a tree
		}
		else{
			b->left_.reset(); // or just reset this node
		}
	}
	else{
		a->left_ = currentNode; // c now left child of a
		a->right_ = b; // b now right child of a
		std::shared_ptr<BSTNode> parent = currentNode->parent_.lock(); // convert weak pointer to shared pointer
		parent->ReplaceChild(currentNode, a); // c parent now parent of a
		b->parent_ = a; // a now parent of b
		currentNode->parent_ = a; // a now parent of c
		if(a_ltree != nullptr){ // if there is a left tree for a
			currentNode->right_ = a_ltree; // left tree of a now right tree of c
			a_ltree->parent_ = currentNode; // c is now parent of left a tree
		}
		else{
			currentNode->right_.reset(); // or just reset this node
		}
		if(a_rtree != nullptr){ // if there is a right tree for a
			b->left_ = a_rtree; // right tree of a now left tree of b
			a_rtree->parent_ = b; // b is now parent of right a tree
		}
		else{
			b->left_.reset(); // or just reset this node
		}
	}
	getHeight(currentNode);
	currentNode->bf_ = BF(currentNode); // find balance factor of c
	getHeight(a);
	a->bf_ = BF(a); // find balance factor of a
	getHeight(b);
	b->bf_ = BF(b); // find balance factor of b

	std::shared_ptr<BSTNode> test = a->parent_.lock(); // define dummy pointer to iterate through branches of tree
	while(test != nullptr){ // until test reaches the root node
		getHeight(test);
		test->bf_ = BF(test); // find new balance factor of node
		test = test->parent_.lock(); // set to next parent
	}
}

void AVL::RLrot(std::shared_ptr<BSTNode> currentNode){
	std::shared_ptr<BSTNode> b = currentNode->left_; // define left child
	std::shared_ptr<BSTNode> a = b->right_; // define left-right child
	std::shared_ptr<BSTNode> a_ltree = a->left_; // define left tree of a
	std::shared_ptr<BSTNode> a_rtree = a->right_; // define right tree of a

	if(currentNode == root_){ // if this function is called with the root node
		a->right_ = currentNode; // c now right child of a
		a->left_ = b; // b now left child of a
		a->parent_.reset(); // since a is root it shouldnt point to any parent
		root_ = a; // a is new root
		currentNode->parent_ = a; // a is new parent of current node
		b->parent_ = a; // a is new parent of b
		if(a_ltree != nullptr){ // if there is a left tree for a
			b->right_ = a_ltree; // left tree of a now right tree of b
			a_ltree->parent_ = b; // b now parent of left a tree
		}
		else{
			b->right_.reset(); // or just reset this node
		}
		if(a_rtree != nullptr){ // if there is a right tree for a
			currentNode->left_ = a_rtree; // right tree of a now left tree of c
			a_rtree->parent_ = currentNode; // c is now parent of right a tree
		}
		else{
			currentNode->left_.reset(); // or just reset this node
		}
	}
	else{
		a->right_ = currentNode; // c now right child of a
		a->left_ = b; // b now left child of a
		std::shared_ptr<BSTNode> parent = currentNode->parent_.lock(); // convert weak pointer to shared pointer
		parent->ReplaceChild(currentNode, a); // c parent now parent of a
		b->parent_ = a; // a now parent of b
		currentNode->parent_ = a; // a now parent of c
		if(a_ltree != nullptr){ // if there is a left tree for a
			b->right_ = a_ltree; // left tree of a now right tree of b
			a_ltree->parent_ = b; // b is now parent of left a tree
		}
		else{
			b->right_.reset(); // or just reset this node
		}
		if(a_rtree != nullptr){ // if there is a right tree for a
			currentNode->left_ = a_rtree; // right tree of a now left tree of c
			a_rtree->parent_ = currentNode; // c is now parent of right a tree
		}
		else{
			currentNode->left_.reset(); // or just reset this node
		}
	}
	getHeight(currentNode);
	currentNode->bf_ = BF(currentNode); // find balance factor of c
	getHeight(a);
	a->bf_ = BF(a); // find balance factor of a
	getHeight(b);
	b->bf_ = BF(b); // find balance factor of b

	std::shared_ptr<BSTNode> test = a->parent_.lock(); // define dummy pointer to iterate through branches of tree
	while(test != nullptr){ // until test reaches the root node
		getHeight(test);
		test->bf_ = BF(test); // find new balance factor of node
		test = test->parent_.lock(); // set to next parent
	}
}

bool AVL::Delete(int key) {
	std::shared_ptr<BSTNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			if (currentNode->IsLeaf()) {
				DeleteLeaf(currentNode);
			} else if (currentNode->left_ == nullptr) {
				assert(currentNode->right_ != nullptr);
				std::shared_ptr<BSTNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->right_);
				size_--; assert(size_ >= 0);
			} else if (currentNode->right_ == nullptr) {
				assert(currentNode->left_ != nullptr);
				std::shared_ptr<BSTNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->left_);
				size_--; assert(size_ >= 0);
			} else {
				currentNode->key_ = DeleteMin(currentNode);
			}
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

int AVL::DeleteMin() {
	return DeleteMin(root_);
}


void AVL::DeleteLeaf(std::shared_ptr<BSTNode> currentNode) {
	std::shared_ptr<BSTNode> parent = currentNode->parent_.lock();
	if (parent == nullptr) {
		// Delete root
		root_ = nullptr;
		size_--; assert(size_ == 0);
	} else {
		if (parent->right_ == currentNode) {
			parent->right_ = nullptr;
		} else if (parent->left_ == currentNode) {
			parent->left_ = nullptr;
		} else {
			std::cerr << "BST::DeleteLeaf Error: inconsistent state\n";
		}
		size_--; assert(size_ >= 0);
	}
}

int AVL::DeleteMin(std::shared_ptr<BSTNode> currentNode) {
	std::shared_ptr<BSTNode> lastNode = nullptr;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = currentNode->left_;
	}
	int result = lastNode->key_;
	std::shared_ptr<BSTNode> parent = lastNode->parent_.lock();
	if (parent == nullptr) {
		// lastNode is root
		if (lastNode->right_ != nullptr) {
			root_ = lastNode->right_;
			lastNode->right_->parent_.reset();
		} else {
			root_ = nullptr;
		}
	} else {
		// lastNode under the root
		if (lastNode->right_ != nullptr) {
			parent->left_ = lastNode->right_;
			lastNode->right_->parent_ = parent;
		} else {
			parent->left_ = nullptr;
		}
  }
	size_--; assert(size_ >= 0);
	return result;
}

size_t AVL::size() const {
	return size_;
}

bool AVL::empty() const {
	return size_ == 0;
}

bool AVL::Find(int key) const {
	std::shared_ptr<BSTNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			return true;
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

std::string AVL::JSON() const {
	nlohmann::json result;
	std::queue< std::shared_ptr<BSTNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
		result["height"] = root_->height_; // write height to json output
		nodes.push(root_);
		while (!nodes.empty()) {
			auto v = nodes.front();
			nodes.pop();
			std::string key = std::to_string(v->key_);
			result[key]["balance factor"] = v->bf_; // write node bf to json output
			result[key]["height"] = v->height_; // write node height to json output
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
		}
	}
	result["size"] = size_;
	return result.dump(2) + "\n";
}

int AVL::getHeight(std::shared_ptr<BSTNode> currentNode){
	if(currentNode == nullptr){
		currentNode->height_ = 0;
	}
	else{
		if(currentNode->IsLeaf()){
			currentNode->height_ = 0;
		}
		else if(currentNode->HasRightChild() && currentNode->HasLeftChild()){
			int lh = currentNode->left_->height_;
			int rh = currentNode->right_->height_;
			currentNode->height_ = 1 + max(lh,rh);
		}
		else if(currentNode->HasRightChild()){
			currentNode->height_ = 1 + currentNode->right_->height_;
		}
		else if(currentNode->HasLeftChild()){
			currentNode->height_ = 1 + currentNode->left_->height_;
		}
	}
	return currentNode->height_;
}

int AVL::max(int x, int y){
	return (x>y)? x : y;
}

int AVL::BF(std::shared_ptr<BSTNode> currentNode){
	if(currentNode->HasLeftChild() && currentNode->HasRightChild()){
		currentNode->bf_ = (currentNode->right_->height_ - currentNode->left_->height_);
		return (currentNode->right_->height_ - currentNode->left_->height_); // else return right height minus left height
	}
	else if(currentNode->HasLeftChild()){ // if there is no right child
		currentNode->bf_ = (-1 - currentNode->left_->height_);
		return (-1 - currentNode->left_->height_); // height of right as -1
	}
	else if(currentNode->HasRightChild()){ // if there is no left child
		currentNode->bf_ = (currentNode->right_->height_ + 1);
		return (currentNode->right_->height_ + 1); // height of left as -1
	}
	else{
		currentNode->bf_ = 0;
		return 0;
	}
}

#include <iostream>
#define MAXUINT 4294967294

using namespace std;

#pragma once

class Node {
public:

	Node() {
		childrenAmount = 0;
		firstNode = nullptr;
		secondNode = nullptr;
		thirdNode = nullptr;
		parent = nullptr;
		leftChild = nullptr;
		rightChild = nullptr;
		tabOfLeft = nullptr;
		tabOfRight = nullptr;
		firstValue = MAXUINT;                   // its MAXUSHORT, so if value is unset, we will know that
		secondValue = MAXUINT;                  // this knowledge, that this variable is unset, will be useful, when we got only one child in node, so unset value will
		thirdValue = MAXUINT;                   // be always greater, and because of this, leftChild will be always set up, instead of rightChild
	}

	~Node() {
		if (tabOfLeft != nullptr)
			delete[] tabOfLeft;
		if (tabOfRight != nullptr)
			delete[] tabOfRight;
	}

	// add nodes from input to temp pointers, in this stage, we don't know who is who, we will determine it later, in building tree
	void addNode(Node *connectedNode, unsigned int value) {
		if (firstNode == nullptr) {
			firstNode = connectedNode;
			firstValue = value;
		}
		else if (secondNode == nullptr) {
			secondNode = connectedNode;
			secondValue = value;
		}
		else if (thirdNode == nullptr) {
			thirdNode = connectedNode;
			thirdValue = value;
		}
	}

	// set parent, nothing intresting, takes one of the tempValues, which were in pairs with tempPointers 
	void setParent(Node *connectedNode) {
		if (connectedNode == firstNode)
			parentValue = firstValue;
		else if (connectedNode == secondNode)
			parentValue = secondValue;
		else
			parentValue = thirdValue;

		parent = connectedNode;
	}

	void setChildren() {
		if (parent != nullptr) {                                // this line is no needed, just for safety
			if (firstNode != parent) {                          // firstNode, secondNode and thirdNode are some sort of temp pointers, when we read input from user, then we just set them, and we don't know which pointer 
																// is parent and child, but when this function is being called, then parent is already known, so we just check, if these temp pointers, are different that parent, if they are, then they must be children
				if (secondNode != parent) {
					if (firstValue < secondValue) {				// it's for setting rightChild always as the child, with greater value, so we will use this knowledge, to optymalise code searching for the longest path
						leftChild = firstNode;
						leftValue = firstValue;
						rightChild = secondNode;
						rightValue = secondValue;
					}
					else {
						leftChild = secondNode;
						leftValue = secondValue;
						rightChild = firstNode;
						rightValue = firstValue;
					}
					return;
				}
				else if (thirdNode != parent) {
					if (firstValue < thirdValue) {
						leftChild = firstNode;
						leftValue = firstValue;
						rightChild = thirdNode;
						rightValue = thirdValue;
					}
					else {
						leftChild = thirdNode;
						leftValue = thirdValue;
						rightChild = firstNode;
						rightValue = firstValue;
					}
					return;
				}
			}

			else {
				if (secondValue < thirdValue) {
					leftChild = secondNode;
					leftValue = secondValue;
					rightChild = thirdNode;
					rightValue = thirdValue;
				}
				else {
					leftChild = thirdNode;
					leftValue = thirdValue;
					rightChild = secondNode;
					rightValue = secondValue;
				}
				return;
			}
		}
	}

	unsigned int balanceTree() {
		setChildren();                                                  // when we call this function, we always got the parent set already for this node, so these 2 pointers, that left unused, must be children of this node
		if (leftChild != nullptr) {
			childrenAmount++;                                           // if leftChild != nullptr, then we got one child, so we add it it whole counter
			leftChild->setParent(this);
			childrenAmount += leftChild->balanceTree();				    //returns amounf of all children of leftChild and adds it to this->childrenAmount
		}
		if (rightChild != nullptr) {
			childrenAmount++;
			rightChild->setParent(this);
			childrenAmount += rightChild->balanceTree();
		}
		return childrenAmount;                                          // returns childrenAmount, so parent will know, how many he got in summary
	}

	// this function calculatates optimal table sizef for tabOfLeft and tabOfRight, nothing intresting to comment
	void calculateTables(unsigned int tabSize) {
		if (leftChild != nullptr) {
			tabOfLeft = new unsigned int[tabSize];
			for (unsigned int i = 0; i < tabSize; i++)
				tabOfLeft[i] = 0;
			if (tabSize - 1 > 0)
				leftChild->calculateTables(tabSize - 1);
			else
				return;
		}
		else {
			tabOfLeft = new unsigned int[1];
			tabOfLeft[0] = 0;
		}
		if (rightChild != nullptr) {
			tabOfRight = new unsigned int[tabSize];
			for (unsigned int i = 0; i < tabSize; i++)
				tabOfRight[i] = 0;
			if (tabSize - 1 > 0)
				rightChild->calculateTables(tabSize - 1);
			else
				return;
		}
		else {
			tabOfRight = new unsigned int[1];
			tabOfRight[0] = 0;
		}
	}

	int returnLongestPath(unsigned int movementsAmount) {
		movementsAmount--;                                                          // so if we are there, then we are deeper in the tree structure, that means, we got less movements available

		if (movementsAmount > childrenAmount)									    // if movements amount is greater than amount of children, then it's going to be unefficient path, so there is no point in checking it
			return 0;

		unsigned int max = 0;
		if (rightChild != nullptr) {                                                // if there is only one child, it's always left child, so if rightChild != nullptr, then there must be 2 children
			if (movementsAmount == 1)
				return rightValue + parentValue;								    // if i can only do one move, then rightValue is the best option, because when i build tree, rightValue is always bigger than leftValue

			for (int i = 0, j = movementsAmount; i <= movementsAmount; i++, j--) {
				if (i != 0) {
					if (tabOfLeft[i] == 0) {                                        // if tabOfLeft[i] == 0, it means, that calculating the longest path, haven't been done yet, because 0 is default value, so we have to calculate it now, otherwise if it's not 0, we already got the best score in this table cell 
						if (leftChild->leftChild == nullptr)						// as i said before, if node has one child, it's always left child, so if this->leftChild has no leftChild (kinda complicated, i know :D), then this->leftChild has no children at all, it means, 
							tabOfLeft[i] = leftValue;							    // that there is no sense, to check this node, so we only return the value to this child, and it's the best score
						else
							tabOfLeft[i] = leftChild->returnLongestPath(i);         // calling function, to calculate the longest way for "i" movements, and saves it to the table, so we won't calculate again, and just take it from table
					}
				}
				if (j != 0) {
					if (tabOfRight[j] == 0) {                                       // it's pretty much the same, but we do this for rightChild
						if (rightChild->leftChild == nullptr)
							tabOfRight[j] = rightValue;
						else
							tabOfRight[j] = rightChild->returnLongestPath(j);
					}
				}

				if (max < (tabOfLeft[i] + tabOfRight[j]))									 // checks if it's greater than max, if it is, then we got new maximum
					max = tabOfLeft[i] + tabOfRight[j];
			}

			return max + parentValue;                                                       // returns max + parentValue, where parentValue is the path length from parent to this child
		}

		else {
			if (leftChild->leftChild == nullptr)											 // it's similar to the situation in 174rd line, we checks if leftChild has any children, if not, then just return his path length from this node to this->leftChild
				return leftValue + parentValue;
			else
				return leftChild->returnLongestPath(movementsAmount) + parentValue;          // if leftChild has children, we just call this function, and calculate the longest path, dont' worry about function's argument, movementsAmount is being lowered in first line of this function
		}
	}

	Node *parent, *leftChild, *rightChild;
	Node *firstNode, *secondNode, *thirdNode;
	unsigned int firstValue, secondValue, thirdValue;
	unsigned int parentValue, leftValue, rightValue;
	unsigned int *tabOfLeft, *tabOfRight;                                           // these tables holds the longest path values for X movements, for example leftChild->returnLongestPath(X) == tabOfLeft[X]
	unsigned int childrenAmount;
};
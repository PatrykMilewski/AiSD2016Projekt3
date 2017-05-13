#include <stdio.h>

#include "Node.h"

int main() {
	int nodesAmount, movementsAvaliable;
	scanf_s("%d %d", &nodesAmount, &movementsAvaliable);
	nodesAmount++;
	Node **nodesTable = new Node*[nodesAmount + 1];
	Node *pointerToFirstNode;
	for (int i = 1; i <= nodesAmount; i++) {
		nodesTable[i] = new Node();
	}

	int firstNode, secondNode, value;
	for (int i = 1; i < nodesAmount; i++) {
		scanf_s("%d %d %d", &firstNode, &secondNode, &value);
		nodesTable[firstNode]->addNode(nodesTable[secondNode], value);
		nodesTable[secondNode]->addNode(nodesTable[firstNode], value);
	}

	nodesTable[1]->leftChild = nodesTable[1]->firstNode;    // i set these manualy, not by function, beacause the root has no parent, and it would cause problems
	nodesTable[1]->leftValue = nodesTable[1]->firstValue;
	nodesTable[1]->leftChild->setParent(nodesTable[1]);

	pointerToFirstNode = nodesTable[1]->leftChild;

	pointerToFirstNode->balanceTree();

	pointerToFirstNode->calculateTables(movementsAvaliable + 1);

	int longestPath = pointerToFirstNode->returnLongestPath(movementsAvaliable);

	printf("%d", longestPath);

	for (int i = 1; i < nodesAmount; i++) {
		delete nodesTable[i];
	}
	delete nodesTable;

	return 0;
}
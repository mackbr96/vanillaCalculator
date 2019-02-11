#ifndef TREE_H
#define TREE_H

#define NUM 257

typedef struct tree tree;

struct tree{
	int middle;
	int attribute;
	struct tree *leftNode;
	struct tree *rightNode;
};
tree *mktree(int type, tree *leftChild, tree *rightChild);
int treeEval(tree * t);


#endif

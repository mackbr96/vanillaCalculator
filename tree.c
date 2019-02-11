#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"


tree *mktree(int opp, tree *old, tree *new) {
	tree *t;
	t = malloc(sizeof(struct tree));
	switch(opp) {
		case('+'):	
			t -> middle = '+';
			t -> leftNode = old;
			t -> rightNode = new;
			return t;
		case('*'):
			t -> middle = '*';
			t -> leftNode = old;
			t -> rightNode = new;
			return t;
		case('-'):
			t -> middle = '-';
			t -> leftNode = old;
			t -> rightNode = new;
			return t;
		case('/'):
			t -> middle = '/';
			t -> leftNode = old;
			t -> rightNode = new;
			return t;
		case (NUM):
			t->leftNode = old;
			t->rightNode = new;
			return t;
		default:
			fprintf(stderr, "UNKOWN OPPERATION IN THE MKTREE QUITING %d\n", opp);
			exit(1);
	}
}

double addTree(tree *t) {
	double value;
	switch(t->middle) {
		case('+'):
			value = addTree(t->leftNode) + addTree(t->rightNode);
			break;
		case('-'):
			value = addTree(t->leftNode) - addTree(t->rightNode);
			break;
		case('*'):
			value = addTree(t->leftNode) * addTree(t->rightNode);
			break;
		case('/'):
			value = addTree(t->leftNode) / addTree(t->rightNode);
			break;
		default:
			value = t->attribute;
	}
	return value;
}	

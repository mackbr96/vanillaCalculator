#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define EOS 256
#define NUM 257

typedef struct tree tree;

int current_token;
int current_attribute;

int get_token();

double addTree(tree *t);

tree *term();
tree *factor();
tree *expr();
tree *mktree(int opp, tree *old, tree *new);

void match(int);
void printTree(tree *t);



struct tree {
	tree *leftNode;
	tree *rightNode;
	char middle;
	int attribute;
};

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

void printTree(tree *t) {
	if(t->leftNode != NULL) {
		fprintf(stderr, "%d\n", t->leftNode->attribute);
		printTree(t->leftNode);
	}
	if(t->rightNode != NULL) {
		fprintf(stderr, "%d\n", t->rightNode->attribute);
		printTree(t->rightNode);
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

//Tokenizer/scanner
int get_token() {
	int c;

	while(1) {
		switch (c = getchar()) {
			case '\n' : 
				fprintf(stderr, "[EOS]%c", c);
				return EOS;
			case ' ': case '\t': //Ignore white space
				continue; 
			case '+': //Notice addition and subtraction symbols
				fprintf(stderr, "[ADDOP:%c]", c);
				return c;
			case '-':
				fprintf(stderr, "[SUBOP:%c]", c);
				return c;
			case '*': //Notice Multiplicatal symbols
				fprintf(stderr, "[MULOP:%c]", c);
				return c;
			case '/':
				fprintf(stderr, "[DIVOP:%c]", c);
				return c;
			case '(': case ')':
				fprintf(stderr, "[%c]", c);
				return c;
			default:
				if(isdigit(c)) { //Found a digit
					int value = 0;
					do {
						value = 10 * value + c - '0';
					} while(isdigit(c = getchar()));

					current_attribute = value;

					ungetc(c, stdin);

					fprintf(stderr, "[NUM%d]", value);
					return NUM;
				} else {
					fprintf(stderr, "{%c}", c);
					return c;
					//assert(0);
				}	
		}
	}
}

//Syntax analzer (parcer)
/*
 * Context Free Gramar for the calculator language
 * Version 1:
 *		E -> E '+' E | E '*' E| '('E')' | NUM
 * Problems with version 1:
 *   -Ambiguous: there are 2 distict derivations for the input "2=3*4"
 *
 *
 * Version 2 Induce precedence and associative laws:
 * 	E - > E '+' T  | T
 * 	T - > T '*' F  | F
 * 	F -> '(' E ')' | NUM
 * 	
 * 	E  -> T E'
 * 	E' -> + T E' | Empty
 * 	T  -> * F T  | Empty
 * 	F  -> ( E )  | Num
 *
 * */
//TIME FOR THE REAL PARCER

void match(int token) {
	if (current_token == token) {
		current_token = get_token();
	} else {
		fprintf(stderr, "Bad unexpected token: %d\n", current_token);
		exit(1);
	}
}

tree *expr() {
	tree *value = term();
	while( current_token == '+' || current_token == '-') { //We found a + or -
		if(current_token == '+') {
			match('+');
			value = mktree('+', value, term());
		}
		else {
			match('-');
			value = mktree('-', value, term());
		}
	}
	return value;
}

tree *term() {
	tree *value = factor();
	while( current_token == '*' || current_token == '/') { //We found a * or /
		if(current_token == '*') {
			match('*');
			value = mktree('*', value, factor());
		}
		else {
			match('/');
			value = mktree('/', value, factor());
		}
	}
	return value;
}

tree *factor() {
	tree *value;
	if(current_token == '(' ) {
		match('(');
		value = expr();
		match(')');
	}
	else if (current_token == NUM) {
		value = mktree(NUM, NULL, NULL);
		value -> attribute = current_attribute;
		match(NUM);
		
	}
	else {
		fprintf(stderr,"Error in factor()\n");
		exit(1);
	}
	return value;
}


int main() {
	current_token = get_token();
	tree *value = expr();
	
	if(current_token != EOS) {
		fprintf(stderr, "Unknown Error before end of statement");
	}
	//printTree(value);
	fprintf(stderr, "value = %f\n", addTree(value));

	return 1;
}

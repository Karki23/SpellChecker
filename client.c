#include<stdio.h>
#include<stdlib.h>
#include "header.h"
int main(int argc, char const *argv[])
{

	struct TrieNode *root = getNode();
	frw("dict.txt",0,root); // inserts dictionary words into trie tree
	frw("input.txt",1,root); //the main spell checeker part which outputs the line by line wrong words into a new output.txt
	//char string[] = "";
	//int level=0;

	//display(root,string,level);
	return 0;
}
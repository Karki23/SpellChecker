#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"

int CHAR_TO_INDEX(char c){
	return ((int)c - (int)'a');
}

int search(struct TrieNode *root, const char *key) 
{ 
    int level; 
    int length = strlen(key); 
    int index; 
    struct TrieNode *pCrawl = root; 
  
    for (level = 0; level < length; level++) 
    { 
        index = CHAR_TO_INDEX(key[level]); 
  
        if (!pCrawl->children[index]) 
            return 0; 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 

int isLeafNode(struct TrieNode* root) 
{ 
    if(root->isEndOfWord){
    	return 1;
    } 
    return 0;
}

void display(struct TrieNode* root, char str[], int level) 
{ 
    // If node is leaf node, it indiicates end 
    // of string, so a null charcter is added 
    // and string is displayed 
    if (isLeafNode(root))  
    { 
        str[level] = '\0'; 
        printf("%s\n",str); 
    } 
  
    int i; 
    for (i = 0; i < 26; i++)  
    { 
        // if NON NULL child is found 
        // add parent key to str and 
        // call the display function recursively 
        // for child node 
        if (root->children[i])  
        { 
            str[level] = i + 'a'; 
            display(root->children[i], str, level + 1); 
        } 
    } 
} 
// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(void) 
{ 
    struct TrieNode *pNode = NULL; 
  
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 
  
    if (pNode) 
    { 
        int i; 
  
        pNode->isEndOfWord = 0; 
  
        for (i = 0; i < ALPHABET_SIZE; i++) 
            pNode->children[i] = NULL; 
    } 
  
    return pNode; 
} 

void frw(char* filename, int flag, struct TrieNode *root)
{

	if(flag == 0)
	{
		
		FILE * fp = fopen(filename, "r");
		if (fp == NULL) return ;
		char c;
		int end=0;
		int count = 0;
		int index;
		while((c = fgetc(fp)) != EOF)
		{
			struct TrieNode *pCrawl = root;
			while(c!= ' ' && c!= '\n'){
				if(isupper(c))
					c = tolower(c);
				index = CHAR_TO_INDEX(c);
				if(index<0){
					break;
				}
				if(!pCrawl->children[index])
					pCrawl->children[index] = getNode();

				pCrawl = pCrawl->children[index];
				c = fgetc(fp);
				if(c==EOF){
					end=1;
					break;
				}
			}
			pCrawl->isEndOfWord = 1;
			if(end)
				break;
		}
		fclose(fp);
	}

	if(flag == 1)
	{
		FILE *fp = fopen(filename,"r");
		FILE *fpw = fopen("output.txt","w");
		if(fp == NULL)
		{
			printf("Input file Could Not Open");
			return;
		}
		char c;
		char string_to_search[25];
		strcpy(string_to_search,"");
		int count = 0;
		int count_of_nlc = 1;
		int present;
		fputs("ERRORS ON LINE 1 : ",fpw);
		while((c = fgetc(fp)) != EOF)
		{
			if(isupper(c))
					c = tolower(c);
			if(c=='\n'){
					fputc('\n',fpw);
					count_of_nlc = count_of_nlc + 1;
					fprintf(fpw, "ERRORS ON LINE %d : ",count_of_nlc);
				}
			if(CHAR_TO_INDEX(c)<0 || CHAR_TO_INDEX(c)>25)
			{
				string_to_search[count] = '\0';
				present = search(root, string_to_search);
				if(!present){
					fputs(string_to_search, fpw);
					fputc(' ',fpw);
				}
				count = 0;
				strcpy(string_to_search,"");
			}
			else
			{
				string_to_search[count]=c;
				count+=1;
			}
		}
		string_to_search[count] = '\0';
		present = search(root, string_to_search);
		if(!present){
			fputs(string_to_search, fpw);
			fputs("\n",fpw);
		}
		fclose(fpw);
		fclose(fp);
		return;	
	}
}
  




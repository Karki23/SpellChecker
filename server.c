#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"
// This function calculates the index of the trie tree where a particular node must be inserted
int CHAR_TO_INDEX(char c){
	return ((int)c - (int)'a');
}

//Seaches for a string key in the trie tree with root as *root
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

//Checks if a particular node is the leaf node by checking for the isEndOfWord bit
int isLeafNode(struct TrieNode* root)
{
    if(root->isEndOfWord){
    	return 1;
    }
    return 0;
}

//A recursive function to display the contents of the trie
void display(struct TrieNode* root, char str[], int level)
{
    if (isLeafNode(root))
    {
        str[level] = '\0';
        printf("%s\n",str);
    }

    int i;
    for (i = 0; i < 26; i++)
    {
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


//Checks wether a given permutation of the given string is there in the trie or not, if there return teh permutation else return ""
char* check_permuatations(char* string,struct TrieNode *root){
	char* temp_string;
	char temp;
	int str_len = strlen(string);
	for(int i=0;i<str_len;i++){
		temp_string = string;
		if(i==0){
			temp = temp_string[i];
			temp_string[i] = temp_string[i+1];
			temp_string[i+1] = temp;
			if(search(root,temp_string)){
				return temp_string;
			}
		}
		else if(i==(str_len-1)){
			temp = temp_string[i];
			temp_string[i] = temp_string[i-1];
			temp_string[i-1] = temp;
			if(search(root,temp_string)){
				return temp_string;
			}
		}
		else{
			temp = temp_string[i];
			temp_string[i] = temp_string[i-1];
			temp_string[i-1] = temp;
			if(search(root,temp_string)){
				return temp_string;
			}

			temp_string = string;

			temp = temp_string[i];
			temp_string[i] = temp_string[i+1];
			temp_string[i+1] = temp;
			if(search(root,temp_string)){
				return temp_string;
			}
		}
	}
	return "";
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
		char* perm;
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
					//call for permutation function here
					fputc('(',fpw);
					perm = check_permuatations(string_to_search,root);
					fputs(perm,fpw);
					fputc(')',fpw);
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
			fputc('(',fpw);
			perm = check_permuatations(string_to_search,root);
			fputs(perm,fpw);
			fputc(')',fpw);
			fputs("\n",fpw);
		}
		fclose(fpw);
		fclose(fp);
		return;
	}
}

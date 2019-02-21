/*
 * prob2.c
 *
 *  Created on:
 *      Author:
 */

/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* the trie node data structure */
struct s_trie_node
{
	char * translation; /* NULL if node not a word */

	/* pointer array to child nodes */
	struct s_trie_node * children[UCHAR_MAX+1];
};

/* pointer to the root node of the trie structure */
static struct s_trie_node * proot = NULL;

/* helper functions for trie structure */

/* allocate new node on the heap
   output: pointer to new node (must be freed) */
struct s_trie_node * new_node(void);

/* delete node and all its children
   input: pointer to node to delete
   postcondition: node and children are freed */
void delete_node(struct s_trie_node * pnode);

/* add word to trie, with translation
   input: word and translation
   output: non-zero if new node added, zero otherwise
   postcondition: word exists in trie */
int add_word(const char * word, char * translation);

/* read dictionary file into trie structure */
unsigned int load_dictionary(const char * filename);

/* search trie structure for word and return translations
   input: word to search
   output: translation, or NULL if not found */
char * lookup_word(const char * word);

/* maximum number of characters for word to search */
#define WORD_MAX 256

/* maximum number of characters in line */
#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

/* main function */
int main(int argc, char * argv[]) {
	char word[WORD_MAX], * translation;
	int len;

	if (argc <= 1)
		return 0; /* no dictionary specified */

	/* load dictionary */
	proot = new_node();
	load_dictionary(argv[1]);

	do {
		printf("Enter word to translate: ");
		fflush(stdout);
		if (!fgets(word, WORD_MAX, stdin))
			abort();

		/* strip trailing newline */
		len = strlen(word);
		if (len > 0 && word[len-1] == '\n') {
			word[len-1] = '\0';
			--len;
		}

		/* exit on empty string */
		if (len == 0)
			break;

		/* lookup word */
		translation = lookup_word(word);

		if (translation)
			printf("%s -> %s\n", word, translation);
		else
			printf("\"%s\" not found\n", word);
	} while (1);

	/* clean up trie structure */
	delete_node(proot);

	return 0;
}

/* allocate new node on the heap
   output: pointer to new node (must be freed) */
struct s_trie_node * new_node(void) {
	struct s_trie_node * pn = (struct s_trie_node *)
	        malloc(sizeof(struct s_trie_node));
	pn->translation = NULL;
	int i;
	for (i = 0; i < UCHAR_MAX + 1; i++)
	    pn->children[i] = NULL;
//	pn->children = (struct s_trie_node **) malloc(sizeof(int) * (UCHAR_MAX + 1));
	return pn;
}

/* delete node and all its children
   input: pointer to node to delete
   postcondition: node and children are freed */
void delete_node(struct s_trie_node * pnode) {
	if (pnode == NULL) return;
	int i;
	for (i = 0; i < UCHAR_MAX + 1; i++) {
	    delete_node(pnode->children[i]);
	}
	free(pnode);
}

/* add word to trie, with translation
   input: word and translation
   output: non-zero if new node added, zero otherwise
   postcondition: word exists in trie */
int add_word(const char * word, char * translation) {
	if (strlen(word) == 0) return 0;
	int i;
	struct s_trie_node * pn = proot;
	int added_new = 0;
	size_t len = strlen(word);
	for (i = 0; i < len; i++) {
        if (!pn->children[word[i]]) {
            pn->children[word[i]] = new_node();
            added_new = 1;
        }
        pn = pn->children[word[i]];
    }
    // NOTE missed this one because did not read problem description carefully
    if (pn->translation != NULL) {
        char * oldtrans = pn->translation;
        char * newtrans = (char *) malloc(sizeof(char) * (strlen(oldtrans) + strlen(translation) + 2)); // +2 for the null byte
        size_t oldlen = strlen(oldtrans);
        strcpy(newtrans, oldtrans);
        strcpy(newtrans + oldlen, ",");
        strcpy(newtrans + oldlen + 1, translation);
        pn->translation = newtrans;
        free(oldtrans);
    } else {
        pn->translation = (char *) malloc(sizeof(char) * strlen(translation) + 1);
        strcpy(pn->translation, translation);
    }
    return added_new;
}

/* delimiter for dictionary */
#define DELIMS "\t"

/* read dictionary file into trie structure */
unsigned int load_dictionary(const char * filename) {
	FILE * pfile;
	char line[LINE_MAX], * word, * translation;
	unsigned int icount = 0;

	/* ensure file can be opened */
	if ( !(pfile = fopen(filename,"r")) )
		return icount;

	/* read lines */
	while ( (fgets(line, LINE_MAX, pfile)) ) {
		/* strip trailing newline */
		int len = strlen(line);
		if (len > 0 && line[len-1] == '\n') {
			line[len-1] = '\0';
			--len;
		}

		if (len == 0 || line[0] == '#')
			continue; /* ignore comment/empty lines */

		/* separate word and translation */
		word = line + strspn(line, DELIMS);
		if ( !word[0] )
			continue; /* no word in line */
		translation = word + strcspn(word, DELIMS);
		*translation++ = '\0';
		translation += strspn(translation, DELIMS);

		/* add word to trie */
		if (add_word(word, translation))
			icount++;
	}

	/* finish */
	fclose(pfile);
	return icount;
}

/* search trie structure for word and return translations
   input: word to search
   output: translation, or NULL if not found */
char * lookup_word(const char * word) {
	int i;
	struct s_trie_node* pn;
	for (i = 0, pn = proot; i < strlen(word); i++) {
	    // missed this because order was swapped and pn was NULL for pn->translation
        pn = pn->children[word[i]];
        if (pn == NULL) return NULL;
	}
	return pn->translation;
}


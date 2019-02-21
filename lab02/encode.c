#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 256
#define MAX_LEN 30

struct tnode
{
    struct tnode *left;   /*used when in tree*/
    struct tnode *right;  /*used when in tree*/
    struct tnode *parent; /*used when in tree*/
    struct tnode *next;   /*used when in list*/
    float freq;
    int isleaf;
    char symbol;
};

/*global variables*/
char code[MAX_SYMBOLS][MAX_LEN];
struct tnode *root = NULL;  /*tree of symbols*/
struct tnode *qhead = NULL; /*list of current symbols*/
struct cnode *chead = NULL; /*list of code*/

/*
    @function   talloc
    @desc       allocates new node 
*/
struct tnode *talloc(int symbol, float freq)
{
    struct tnode *p = (struct tnode *)malloc(sizeof(struct tnode));
    if (p != NULL)
    {
        p->left = p->right = p->parent = p->next = NULL;
        p->symbol = symbol;
        p->freq = freq;
        p->isleaf = 1;
    }
    return p;
}

/*
    @function display_tnode_list
    @desc     displays the list of tnodes during code construction
*/
void pq_display(struct tnode *head)
{
    struct tnode *p = NULL;
    printf("list:");
    for (p = head; p != NULL; p = p->next)
    {
        printf("(%c,%f) ", p->symbol, p->freq);
    }
    printf("\n");
}

/*
    @function pq_insert
    @desc     inserts an element into the priority queue
    NOTE:     makes use of global variable qhead
*/
void pq_insert(struct tnode *p)
{
    struct tnode *curr = NULL;
    struct tnode *prev = NULL;
    printf("inserting:%c,%f\n", p->symbol, p->freq);
    for (curr = qhead; curr != NULL && p->freq > curr->freq;
         prev = curr, curr = curr->next)
    {
    }

    if (curr == qhead) // also works for qhead == NULL
    {
        p->next = qhead;
        qhead = p;
    }
    else /*insert between prev and next*/
    {
        p->next = curr;
        prev->next = p;
    }
}

/*
    @function pq_pop
    @desc     removes the first element
    NOTE:     makes use of global variable qhead
*/

struct tnode *pq_pop()
{
    if (qhead == NULL)
        return NULL;
    struct tnode *p = qhead;
    qhead = qhead->next;
    printf("popped:%c,%f\n", p->symbol, p->freq);
    return p;
}

/*
	@function build_code
	@desc     generates the string codes given the tree
	NOTE: makes use of the global variable root
*/
void generate_code(struct tnode *root, int depth)
{
    if (root == NULL) return;
    int symbol;
    int len; /*length of code*/
    if (root->isleaf)
    {
        symbol = root->symbol;
        printf("symbol: (%d, %c)\n", symbol, symbol);
        len = depth;
        /*start backwards*/
        code[symbol][len] = 0;
        for (len = len - 1; len >= 0; root = root->parent, len--)
        {
            if (root->parent->left == root)
                code[symbol][len] = '0';
            else
                code[symbol][len] = '1';
        }
        printf("built code:%c,%s\n", symbol, code[symbol]);
    }
    else
    {
        generate_code(root->left, depth + 1);
        generate_code(root->right, depth + 1);
    }
}

/*
	@func	dump_code
	@desc	output code file
*/
void dump_code(FILE *fp)
{
    int i = 0;
    for (i = 0; i < MAX_SYMBOLS; i++)
    {
        if (code[i][0]) /*non empty*/
        {
            fprintf(fp, "%d %s\n", i, code[i]);
        }
    }
}

/*
	@func	encode
	@desc	outputs compressed stream
*/
void encode(char *str, FILE *fout)
{
    while (*str)
    {
        fprintf(fout, "%s", code[*str]);
        str++;
    }
}

/*
    @func   encode
    @decc outputs compressed stream (from fp)
*/
void encode_fp(FILE *fp, FILE *fout)
{
    char c;
    while ((c = getc(fp)) != EOF)
    {
        if (c >= 0 && c < MAX_SYMBOLS)
            fprintf(fout, "%s", code[c]);
    }
}

void freetree(struct tnode *root)
{
    if (root == NULL)
        return;
    freetree(root->right);
    freetree(root->left);
    free(root);
}

/*
    @function main
*/
int main()
{
    /*test pq*/
    struct tnode *p = NULL;
    struct tnode *lc, *rc;
    static float freq[MAX_SYMBOLS];
    int i = 0;
    // const char *IN_FILE = "book.txt";
    // const char *CODE_FILE = "book_code.txt";
    // const char *OUT_FILE = "book_encoded.txt";
    const char *IN_FILE = "book.txt";
    const char *CODE_FILE = "book_code.txt";
    const char *OUT_FILE = "book_encoded.txt";
    FILE *fout = NULL;
    /*zero out code*/
    memset(code, 0, sizeof(code));

    qhead = NULL;
    /*Read infile with freq*/
    FILE *fin = fopen(IN_FILE, "r");
    int c;
    while ((c = getc(fin)) != EOF)
    {
        if (c >= 0 && c < MAX_SYMBOLS)
        {
            freq[c]++;
        }
    }
    fclose(fin);
    for (i = 0; i < MAX_SYMBOLS; i++)
        if (freq[i] > 0)
            pq_insert(talloc(i, freq[i]));
    printf("QUEUE:\n");
    pq_display(qhead);
    /*build tree*/
    while (qhead->next != NULL)
    {
        lc = pq_pop();
        rc = pq_pop();
        /*create parent*/
        p = talloc(0, lc->freq + rc->freq);
        /*set parent link*/
        lc->parent = rc->parent = p;
        /*set child link*/
        p->right = rc;
        p->left = lc;
        /*make it non-leaf*/
        p->isleaf = 0;
        /*add the new node to the queue*/
        pq_insert(p);
    }
    printf("Tree built\n");
    /*get root*/
    root = pq_pop();
    /*build code*/
    generate_code(root, 0);
    /*output code*/
    // puts("code:");
    fout = fopen(CODE_FILE, "w");
    // dump_code(stdout);
    dump_code(fout);
    fclose(fout);

    /*encode a sample string*/
    // puts("orginal:abba cafe bad");
    fout = fopen(OUT_FILE, "w");
    fin = fopen(IN_FILE, "r"); // open infile for reading again
    // encode_fp(fin, stdout);
    encode_fp(fin, fout);
    fclose(fin);
    fclose(fout);
    freetree(root);
    getchar();
    return 0;
}

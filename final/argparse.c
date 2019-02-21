#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "argparse.h"

char* ctoa(char ch)
{
    char* str = (char*) malloc(sizeof(char) * 2);
    str[0] = ch;
    str[1] = '\0';
    return str;
}

//$args_out output string array for arguments
//$flags_out output char array for flags
//$argoutc output for number of arguments received
//$flagoutc output for number of flags received
//$argc number of incoming argument tokens (as supplied by c)
//$argv array of argument tokens (as supplied by c)
//$validfset a bit vector with all valid flags set
int parse_args(aqueue* outq, int* outfset, int argc, char* argv[], int validfset)
{
    short i;
    int len;
    char flag;
    if (argc > MAXARGS) argc = MAXARGS;
    *outfset = 0;
    // skip first arg
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            len = strlen(argv[i]);
            if (len == 1) continue;
            flag = argv[i][1];
            if (flag > 'z' || flag < 'a' || !isflagset(validfset, flag))
            {
                fprintf(stderr, "Unrecognized flag -%c. Abort.\n", flag);
                return -1;
            }
            *outfset |= (1 << (flag - 'a'));
            enqueue(outq, ctoa(flag), 1);
            if (len > 2)
            {
                enqueue(outq, argv[i] + 2, 0);
            }
        }
        else
        {
            enqueue(outq, argv[i], 0);
        }
    }
    return 0;
}

// $flags: array of flag characters ('a'-'z')
// $flagc: number of flags in the array
// $returns: a bit vector with all the flags in flags[] set
int getfset(char flags[], int flagc)
{
    int i;
    int fset = 0;
    for (i = 0; i < flagc; i++) {
        fset |= (1 << (flags[i] - 'a'));
    }
    return fset;
}

// $fset: bit vector of flags that are set
// $flag: flag ('a'-'z')
// $return: whether the flag bit is set in fset
int isflagset(int fset, char flag)
{
    return fset & (1 << (flag - 'a'));
}

aqueue* aqueue_create()
{
    aqueue* q = (aqueue*) malloc(sizeof(aqueue));
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    return q;
}

void aqueue_delete(aqueue* q)
{
    qnode* front = q->front;
    qnode* t;
    while (front != NULL)
    {
        t = front;
        front = front->next;
        free(t);
    }
    free(q);
}

void enqueue_node(aqueue* q, qnode* toadd)
{
    if (q->back == NULL)
    {
        q->front = toadd;
        q->back = toadd;
    }
    else
    {
        q->back->next = toadd;
        q->back = toadd;
    }
    q->size++;
}

void enqueue(aqueue* q, char* val, int isflag)
{
    qnode* n = (qnode*) malloc(sizeof(qnode));
    n->val = val;
    n->isflag = isflag;
    n->next = NULL;
    enqueue_node(q, n);
}

void dequeue(aqueue* q)
{
    qnode* n = q->front;
    if (n != NULL)
    {
        free(n);
        q->front = n->next;
        if (q->front == NULL)
            q->back = NULL;
        q->size--;
    }
} 

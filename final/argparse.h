#ifndef ARGPARSE_H
#define ARGPARSE_H

static const unsigned int MAXARGS = 32;
struct qnode;

typedef struct qnode
{
    struct qnode* next;
    char* val;
    int isflag;
} qnode;

typedef struct aqueue
{
    qnode* front;
    qnode* back;
    int size;
} aqueue;

int parse_args(aqueue* outq, int* outfset,
        int argc, char* argv[], int validfset);
int getfset(char flags[], int flagc);
int isflagset(int flags, char f);

aqueue* aqueue_create();
void aqueue_delete(aqueue* q);
void enqueue_node(aqueue* q, qnode* toadd);
void enqueue(aqueue* q, char* val, int isflag);
void dequeue(aqueue* front);

#endif


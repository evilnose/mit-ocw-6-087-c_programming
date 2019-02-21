#include <stddef.h>
#include <stdio.h>
#include <malloc.h>

struct node
{
    int data;
    struct node* next;
};

// a)
void display(struct node* head)
{
    struct node* pn;
    for (pn = head; pn != NULL; pn = pn->next)
        printf("%d ", pn->data);
}

struct node* nalloc(int data)
{
    struct node* p = (struct node*)malloc(sizeof(struct node));
    if (p != NULL)
    {
        p->data = data;
        p->next = NULL;
    }
    return p;
}

struct node* addback(struct node* head, int data)
{
    struct node* last = nalloc(data);
    if (head == NULL)
        return last;
    while (head->next != NULL)
        head = head->next;
    head->next = last;
    return head;
}

struct node* find(struct node* head, int data)
{
    for (;head != NULL; head = head->next)
    {
        if (head->data == data)
            return head;
    }
    return NULL;
}

struct node* delnode(struct node* head, struct node* pelement)
{
    struct node* pn = find(head, pelement->data);
    if (pn != NULL) {
        if (pn->next == head)
        {
            head = pn->next;
        }
        pn = pn->next;
    }
    return head;
}

void freelist(struct node* head)
{
    struct node * pn;
    while (head)
    {
        pn = head;
        head = head->next;
        free(pn);
    }
}


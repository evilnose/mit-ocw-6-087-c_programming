#include <stdlib.h>
#include <stdio.h>

struct tnode {
    int data;
    struct tnode *left;
    struct tnode *right;
};

struct tnode *talloc(int data) {
    struct tnode *p = (struct tnode *) (malloc(sizeof(struct tnode)));
    if (p != NULL) {
        p->data = data;
        p->left = p->right = NULL;
    }
    return p;
}

struct tnode *addnode(struct tnode *root, int data) {
    struct tnode *p = NULL;
    if (root == NULL) {
        p = talloc(data);
        return p;
    } else if (data < root->data) {
        root->left = addnode(root->left, data);
    } else {
        root->right = addnode(root->right, data);
    }
    return root;
}

void preorder(struct tnode *root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

int deltree(struct tnode* root) {
    if (root == NULL) return 0;
    int res = deltree(root->left) + deltree(root->right);
    free(root);
    return res + 1;
}

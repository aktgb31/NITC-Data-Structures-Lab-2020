#include <stdio.h>
#include <stdlib.h>

// Structure to store information in each node of bst
struct node
{
    int key;
    struct node *left, *right, *p;
};

//Structure containing the root node
struct BINARY_SEARCH_TREE
{
    struct node *root;
};

//Creating new node
struct node *CREATE_NODE(int item)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = temp->p = NULL;
    return temp;
}

//Inorder traveral
void INORDER(struct node *root)
{
    if (root != NULL)
    {
        INORDER(root->left);
        printf("%d ", root->key);
        INORDER(root->right);
    }
}

//Preorder traveral
void PREORDER(struct node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->key);
        PREORDER(root->left);
        PREORDER(root->right);
    }
}

//Postorder traveral
void POSTORDER(struct node *root)
{
    if (root != NULL)
    {
        POSTORDER(root->left);
        POSTORDER(root->right);
        printf("%d ", root->key);
    }
}

//Insert node in a Binary Search tree
void INSERT(struct BINARY_SEARCH_TREE *T, int key)
{
    struct node *prev, *curr, *temp;
    prev = NULL;
    curr = T->root;
    while (curr != NULL)
    {
        prev = curr;
        if (key < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    temp = CREATE_NODE(key);
    if (prev == NULL)
        T->root = temp;
    else
    {
        if (key < prev->key)
        {
            prev->left = temp;
            temp->p = prev;
        }
        else
        {
            prev->right = temp;
            temp->p = prev;
        }
    }
}

//Searching a key in bst
struct node *SEARCH(struct node *T, int key)
{
    if (T == NULL)
        return NULL;
    if (key == T->key)
        return T;
    if (key < T->key)
        return SEARCH(T->left, key);
    else
        return SEARCH(T->right, key);
}

//Find the level of node with key
int LEVEL(struct node *T, int key)
{
    if (T == NULL)
        return -1;
    if (key == T->key)
        return 1;
    if (key < T->key)
    {
        int left = LEVEL(T->left, key);
        if (left == -1)
            return -1;
        return 1 + left;
    }
    else
    {
        int right = LEVEL(T->right, key);
        if (right == -1)
            return -1;
        return 1 + right;
    }
}

//Minimum Value of node in bst
struct node *MIN_VALUE(struct node *T)
{
    struct node *temp = T;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

//Maximum Value of node in bst
struct node *MAX_VALUE(struct node *T)
{
    struct node *temp = T;
    while (temp->right != NULL)
    {
        temp = temp->right;
    }
    return temp;
}

//Successor of node with given key
struct node *SUCCESSOR(struct node *T, int key)
{
    struct node *temp = SEARCH(T, key);
    if (temp == NULL || T == NULL)
        return NULL;

    if (temp->right != NULL)
        return MIN_VALUE(temp->right);

    struct node *temp2 = temp->p;
    while (temp2 != NULL && temp == temp2->right)
    {
        temp = temp2;
        temp2 = temp2->p;
    }
    return temp2;
}

//predecessor of node with given key
struct node *PREDECESSOR(struct node *T, int key)
{
    struct node *temp = SEARCH(T, key);
    if (temp == NULL || T == NULL)
        return NULL;

    if (temp->left != NULL)
        return MAX_VALUE(temp->left);

    struct node *temp2 = temp->p;
    while (temp2 != NULL && temp == temp2->left)
    {
        temp = temp2;
        temp2 = temp2->p;
    }
    return temp2;
}

void transplant(struct BINARY_SEARCH_TREE *T, struct node *x, struct node *y)
{
    if (x->p == NULL)
        T->root = y;
    else
    {
        if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
    }
    if (y != NULL)
        y->p = x->p;
}

//Deleting a node in BST
int DELETE(struct BINARY_SEARCH_TREE *T, struct node *x)
{
    if (x == NULL)
        return -1;
    if (x->left == NULL)
    {
        transplant(T, x, x->right);
    }
    else if (x->right == NULL)
    {
        transplant(T, x, x->left);
    }
    else
    {
        struct node *y = MIN_VALUE(x->right);
        if (y->p != x)
        {
            transplant(T, y, y->right);
            y->right = x->right;
            x->right->p = y;
        }
        transplant(T, x, y);
        y->left = x->left;
        x->left->p = y;
    }
    return x->key;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    //freopen("output2.txt", "w", stdout);
    struct BINARY_SEARCH_TREE T;
    T.root = NULL;
    char ch;
    while (scanf("%c", &ch) && ch != 'e')
    {
        switch (ch)
        {
        case 'a':
        {
            int k;
            scanf("%d", &k);
            INSERT(&T, k);
        }
        break;
        case 'd':
        {
            int k;
            scanf("%d", &k);
            printf("%d\n", DELETE(&T, SEARCH(T.root, k)));
        }
        break;
        case 'l':
        {
            int k;
            scanf("%d", &k);
            printf("%d\n", LEVEL(T.root, k));
        }
        break;
        case 'm':
        {
            printf("%d\n", MIN_VALUE(T.root)->key);
        }
        break;
        case 'x':
        {
            printf("%d\n", MAX_VALUE(T.root)->key);
        }
        break;
        case 'r':
        {
            int k;
            scanf("%d", &k);
            struct node *temp = PREDECESSOR(T.root, k);
            if (temp == NULL)
                printf("-1\n");
            else
                printf("%d\n", temp->key);
        }
        break;
        case 'u':
        {
            int k;
            scanf("%d", &k);
            struct node *temp = SUCCESSOR(T.root, k);
            if (temp == NULL)
                printf("-1\n");
            else
                printf("%d\n", temp->key);
        }
        break;
        case 'i':
        {
            INORDER(T.root);
            printf("\n");
        }
        break;
        case 'p':
        {
            PREORDER(T.root);
            printf("\n");
        }
        break;
        case 't':
        {
            POSTORDER(T.root);
            printf("\n");
        }
        break;
        case 's':
        {
            int k;
            scanf("%d", &k);
            if (SEARCH(T.root, k) != NULL)
                printf("1\n");
            else
                printf("-1\n");
        }
        break;
        }
    }
    return 0;
}
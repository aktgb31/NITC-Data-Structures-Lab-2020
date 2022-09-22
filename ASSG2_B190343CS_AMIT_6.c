#include <stdio.h>
#include <stdlib.h>

struct node
{
    int key;
    struct node* left, * right;
};
struct Binary_Search_Tree
{
    struct node* root;
};
struct node* createnode(int item)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}
void insert(struct Binary_Search_Tree* T, int key)
{
    struct node* prev, * curr, * temp;
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
    temp = createnode(key);
    if (prev == NULL)
        T->root = temp;
    else
    {
        if (key < prev->key)
        {
            prev->left = temp;
        }
        else
        {
            prev->right = temp;
        }
    }
}

struct node* lowest_common_ancestor(struct node* root, int a, int b)
{
    if (root == NULL)
        return NULL;
    if (a < root->key && b < root->key)
        return lowest_common_ancestor(root->left, a, b);
    if (a > root->key && b > root->key)
        return lowest_common_ancestor(root->left, a, b);

    return root;
}
int print_max_a_to_b(struct node* root, int a, int b)
{
    struct node* lca = lowest_common_ancestor(root, a, b);
    int ans = -1e7;
    struct node* temp = lca;

    while (temp->key != b)
    {
        if (ans < temp->key && temp->key != a)
            ans = temp->key;
        if (b < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }
    temp = lca;
    while (temp->key != a)
    {
        if (ans < temp->key && temp->key != b)
            ans = temp->key;
        if (a < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }
    return ans;
}

int main()
{
    struct Binary_Search_Tree T;
    T.root = NULL;
    int num;
    char c = ' ';
    while (c != '\n')
    {
        scanf("%d%c", &num, &c);
        insert(&T, num);
    }
    int a, b;
    scanf("%d%d", &a, &b);
    if (a > b)
    {
        int temp;
        temp = a;a = b;b = temp;
    }

    printf("%d\n", print_max_a_to_b(T.root, a, b));
    return 0;
}
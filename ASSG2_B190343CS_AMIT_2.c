#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    char* word;
    struct node* next;
};
struct list
{
    struct node* head;
};
//Create node for the linkedlist
struct node* create_node(char* key)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->word = (char*)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(temp->word, key);
    temp->next = NULL;
    return temp;
}

//Search the list with key
struct node* list_search(struct list* L, char* key)
{
    struct node* temp = L->head;
    while (temp != NULL)
    {
        if (strcmp(temp->word, key) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}
//Insert a node in the end of a list
void list_insert_tail(struct list* L, struct node* temp)
{

    if (L->head == NULL)
    {
        L->head = temp;
    }
    else
    {
        struct node* t = L->head;
        while (t->next != NULL)
        {
            t = t->next;
        }
        t->next = temp;
    }
}

struct Hash_Table
{
    struct list* table;
    int size;
};

struct Hash_Table HASHTABLE(int m)
{
    struct Hash_Table T;
    T.size = m;
    T.table = (struct list*)malloc(m * sizeof(struct list));
    for (int i = 0; i < m; i++)
    {
        T.table[i].head = NULL;
    }
    return T;
}

void INSERT(struct Hash_Table* T, char* string)
{
    int i = 0;
    int index = (strlen(string) * strlen(string)) % T->size;

    struct node* temp = create_node(string);
    if (list_search(&T->table[index], string) == NULL)
        list_insert_tail(&(T->table[index]), temp);
}

void PRINT(struct Hash_Table* T)
{
    for (int i = 0; i < T->size; i++)
        if (T->table[i].head != NULL)
        {
            printf("%d:", i);
            struct node* temp = T->table[i].head;
            printf("%s", temp->word);
            temp = temp->next;
            while (temp != NULL)
            {
                printf("-%s", temp->word);
                temp = temp->next;
            }
            printf("\n");
        }
        else
            printf("%d:null\n", i);
}


int main()
{
    //freopen("input.txt", "r", stdin);
    int table_size;
    scanf("%d ", &table_size);
    struct Hash_Table T = HASHTABLE(table_size);

    char string[1001];
    scanf("%[^\n]*c", string);
    int n = strlen(string);

    for (int i = 0;i < n;i++)
    {
        char temporary[501];
        int j = 0;
        for (;string[i] != ' ';++i, ++j)
        {
            temporary[j] = string[i];
        }
        temporary[j] = '\0';
        INSERT(&T, temporary);
    }
    PRINT(&T);
    return 0;
}
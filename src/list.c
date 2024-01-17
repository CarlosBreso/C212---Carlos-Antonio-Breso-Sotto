#include "list.h"
#include <stdio.h>
#include <stdlib.h>

// Init list structure
int init_list(int_ll_t *list)
{
    list->head = NULL;
    list->size = 0;
    
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);

    if (list->size != 0)
    {
        node *prev = list->head;
        node *current = prev->next;

        for (size_t i = 0; i < list->size; i++)
        {
            free(prev);
            prev = current;
            current = current->next; // prev y current son el mismo
        }
    }
    
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    free(list);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    // no need to use lock :)
    return list->size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);

    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    if (index >= list->size)
        index = list->size - 1;

    else if (index <= 0)
        index = 0;

    node *current = list->head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    out_value = &current->value;

    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    if (index >= list->size)
        index = list->size - 1;

    else if (index <= 0)
        index = 0;

    node *current = list->head;

    for (int i = 0; i < index - 1; i++)
    {
        current = current->next;
    }
    
    if (current->next != NULL)
    {
        node *temp = current->next;

        current->next = malloc(sizeof(node));
        current->next->value = value;
        
        current->next->next = temp;
    }
    else
    {
        current->next = malloc(sizeof(node));
        current->next->value = value;
    }
    
    list->size++;

    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);

    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1; // lista vacía
    }

    if (index >= list->size)
        index = list->size - 1;

    else if (index <= 0)
        index = 0;

    if (index == 0)
    {
        node *temp = list->head;
        if (list->size != 1)
        {
            list->head = list->head->next;
        }
        else
        {
            list->head = NULL;
        }
        
        *out_value = temp->value;
        free(temp);
    }
    else
    {
        node *prev = list->head;

        for (int i = 0; i < index - 1; i++)
        {
            prev = prev->next;
        }

        node * to_remove = prev->next;
        out_value = &to_remove->value;
        
        if (prev->next->next == NULL)
        {
            prev->next = NULL;
        }
        else
        {
            prev->next = prev->next->next;
        }
        free(to_remove);
    }
    list->size--;

    pthread_mutex_unlock(&list->lock);
    return 0;
}
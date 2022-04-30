//=================================================================
// Implementation for DLL module.
//
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

//===========================
/* Creating nodes and lists */
//===========================

LLNode* create_llnode(void* data) {
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

DLinkedList* create_dlinkedlist(CompareFunction compare) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    newList->compare = compare;
    return newList;
}

//============================
/* Accessing nodes and lists */
//============================

int getSize(DLinkedList* dLinkedList){
    return dLinkedList->size;
}

LLNode* getHead(DLinkedList* dLinkedList){
    return dLinkedList->head;
}

LLNode* getTail(DLinkedList* dLinkedList){
    return dLinkedList->tail;
}

LLNode* getNext(LLNode* node){
    return node->next;
}

LLNode* getPrev(LLNode* node){
    return node->prev;
}

void* getData(LLNode* node){
    return node->data;
}

//============================
/* Inserting nodes into lists */
//============================

void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data){
    if (prev_node == NULL) {
        printf("Cannot add a node after a null element\n");
        return;
    }

    LLNode* current = dLinkedList->head;
    while (current != prev_node) {
        current = current->next;
    }
    LLNode* newNode = create_llnode(data);

    if (current->next == NULL) {
        current->next = newNode;
        newNode->prev = current;
        dLinkedList->tail = newNode;
    }
    else {
        LLNode* next = current->next;
        current->next = newNode;
        newNode->prev = current;
        newNode->next = next;
        next->prev = newNode;
    }
    dLinkedList->size++;
}


void insertBefore(DLinkedList* dLinkedList, LLNode* next_node, void* data){
    if (next_node == NULL) {
        printf("Cannot add a node before a null element\n");
        return;
    }

    LLNode* current = dLinkedList->head;
    while (current != next_node) {
        current = current->next;
    }
    LLNode* newNode = create_llnode(data);

    if (current->prev == NULL) {
        current->prev = newNode;
        newNode->next = current;
        dLinkedList->head = newNode;
    }
    else {
        LLNode* prev = current->prev;
        current->prev = newNode;
        newNode->next = current;
        newNode->prev = prev;
        prev->next = newNode;
    }
    dLinkedList->size++;
}


void insertHead(DLinkedList* dLinkedList, void* data){
    if (dLinkedList->size == 0) {
        LLNode* node = create_llnode(data);
        dLinkedList->head = node;
        dLinkedList->tail = node;
        dLinkedList->size++;
    }
    else {
        insertBefore(dLinkedList, dLinkedList->head, data);
    }
}

void insertTail(DLinkedList* dLinkedList, void* data){
    if (dLinkedList->size == 0) {
        LLNode* node = create_llnode(data);
        dLinkedList->head = node;
        dLinkedList->tail = node;
        dLinkedList->size++;
    }
    else {
        insertAfter(dLinkedList, dLinkedList->tail, data);
    }
}

//============================
/* Looking up nodes in lists */
//============================

LLNode* findNode(DLinkedList* dLinkedList, void* data){
    LLNode* current = dLinkedList->head;
    while (current != NULL && !dLinkedList->compare(current->data, data)) {
        current = current->next;
    }
    return current;
}

//===========================
/* Deleting nodes and lists */
//===========================

void deleteNode(DLinkedList* dLinkedList, LLNode* Node) {
    if (dLinkedList->size == 1) {
        dLinkedList->head = NULL;
        dLinkedList->tail = NULL;
    }
    else if (Node == dLinkedList->head) {
        dLinkedList->head = dLinkedList->head->next;
        dLinkedList->head->prev = NULL;
    }
    else if (Node == dLinkedList->tail) {
        dLinkedList->tail = dLinkedList->tail->prev;
        dLinkedList->tail->next = NULL;
    }
    else {
        LLNode* next = Node->next;
        next->prev = Node->prev;
        Node->prev->next = next;
    }
    free(Node->data);
    free(Node);
    dLinkedList->size--;
}


void destroyList(DLinkedList* dLinkedList){
    LLNode* current = dLinkedList->head;
    
    while (current != NULL) {
        LLNode* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    dLinkedList->size = 0;
    free(dLinkedList);
}

//==================
/* Reversing lists */
//==================

void reverse(DLinkedList* dLinkedList){
    LLNode* tail = dLinkedList->head;
    LLNode* head = dLinkedList->tail;

    dLinkedList->head = head;
    dLinkedList->tail = tail;

    LLNode* current = tail;

    while (current != NULL) {
        LLNode* next = current->next;
        current->next = current->prev;
        current->prev = next;
        current = next;
    }
}
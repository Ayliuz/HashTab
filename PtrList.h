// PTRLIST . H

#pragma once

#include <cstdio>
#include <cstdlib>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <string>
#include "stack.h"

const int LERROK = 0;
const int LERREMOVE = 1;
const int LERDATA = 2;
const int LERINFO = 3;
const int LERPOS = 4;
const int LERSTRUCT = 5;
const int LERLEN = 6;

#ifndef $
#define $(x) std::cout<<"~"<<#x " = "<<x<<"\n";
#endif

typedef struct MyPtrList MyPtrList;
typedef unsigned int out_ptr;

typedef char* key_type;
typedef char* val_type;

typedef struct Pair
{
    key_type key;
    val_type value;
} list_type;


struct PtrListElem
{
    int list_elem_guard_begin = GUARDPOIS;

    PtrListElem* prev = NULL;

    list_type info = {};

    PtrListElem* next = NULL;

    int list_elem_guard_end = GUARDPOIS;
};

struct MyPtrList
{
    int plist_guard_begin = GUARDPOIS;

    unsigned int pList_len = 0;

    PtrListElem* head = NULL;
    PtrListElem* tail = NULL;

    int pListHash_struct = HASHDEFAULT;

    int plist_guard_end = GUARDPOIS;
};

void ptr_list_test(const char* dot);

void plist_Ctor(MyPtrList* list);
void plist_Dtor(MyPtrList* list);

void plist_elem_Ctor(PtrListElem* elem);
void plist_elem_Dtor(PtrListElem* elem);
PtrListElem* plist_elem_create(list_type val);

void plist_make_hash(MyPtrList* list);

PtrListElem* plist_push_back(MyPtrList* list, list_type val);
PtrListElem* plist_push_front(MyPtrList* list, list_type val);

PtrListElem* plist_insert(MyPtrList* list, PtrListElem* elem, list_type val);                 // behind input element
int plist_pop_back(MyPtrList* list);
int plist_pop_front(MyPtrList*list);

int plist_erase(MyPtrList* list, PtrListElem* elem);
int plist_clear(MyPtrList* list);

int plist_is_OK(MyPtrList* list);
int plist_elem_is_OK(PtrListElem* elem);

int plist_dump(const char dot[], const char DUMPNAME[], MyPtrList* list);
char* plist_elem_dump(PtrListElem* elem);
int plist_draw(const char dot[], const char DUMPNAME[], MyPtrList* list);


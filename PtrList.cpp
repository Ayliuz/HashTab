// PTRLIST . CPP

#include "PtrList.h"

/*
void ptr_list_test(const char *dot) {
    const char LISTDUMP[10] = "list";

    MyPtrList list;
    plist_Ctor(&list);

    plist_push_back(&list, 12);

    PtrListElem* k = plist_push_front(&list, {2 , 144});
    plist_insert(&list, k, {3 , 156});

    PtrListElem* del = plist_push_back(&list, {1 , 2222});
    plist_push_back(&list, 3333);
    plist_push_back(&list, 45454);
    plist_push_front(&list, 890);
    plist_erase(&list, del);
    for (int i = 0; i < 20; i++) plist_insert(&list, k, i*i + 2);

    plist_dump(dot, LISTDUMP, &list);

    plist_Dtor(&list);
}
*/

//************************************
/// Constructs MyPtrList object and initialize it.
///
/// \param [in] MyPtrList* list - pointer to MyPtrList object
///
///
/// \return void
///
//************************************

void plist_Ctor(MyPtrList* list)
{
    assert(list);

    list->plist_guard_begin = GUARD;
    list->plist_guard_end = GUARD;

    list->head = NULL;
    list->tail = NULL;

    list->pList_len = 0;

    plist_make_hash(list);
}

//************************************
/// Deletes MyPtrList structure and its data.
///
/// \param [in] MyPtrList* list - pointer to MyPtrList object
///
///
/// \return void
///
//************************************

void plist_Dtor(MyPtrList* list)
{
    assert(list);

    plist_clear(list);

    list->pListHash_struct = HASHPOIS;

    list->plist_guard_begin = GUARDPOIS;
    list->plist_guard_end = GUARDPOIS;
}

//************************************
/// Constructs PtrListElem object and initialize it.
///
/// \param [in] PtrListElem* list - pointer to PtrListElem object
///
///
/// \return void
///
//************************************

void plist_elem_Ctor(PtrListElem* elem)
{
    assert(elem);

    elem->list_elem_guard_begin = GUARD;
    elem->list_elem_guard_end = GUARD;

    elem->prev = NULL;
    elem->next = NULL;

    elem->info = {nullptr, nullptr};
}

//************************************
/// Deletes PtrListElem structure.
///
/// \param [in] PtrListElem* list - pointer to PtrListElem object
///
///
/// \return void
///
//************************************

void plist_elem_Dtor(PtrListElem* elem)
{
    assert(elem);

    elem->list_elem_guard_begin = GUARDPOIS;
    elem->list_elem_guard_end = GUARDPOIS;

    elem->prev = NULL;
    elem->next = NULL;

    elem->info = {nullptr, nullptr};
}

//************************************
/// Hashes the list.
///
/// \param [in] MyPtrList* list - pointer to MyPtrList object
///
/// \return changes ListHash_struct in list.
///
//************************************

void plist_make_hash(MyPtrList* list)
{
    assert(list);

    list->pListHash_struct = HASHDEFAULT;
    list->pListHash_struct = hash (list, sizeof (*list));
}

PtrListElem* plist_elem_create(list_type val)
{
    PtrListElem* new_item = (PtrListElem*) calloc(1, sizeof(*new_item));
    plist_elem_Ctor(new_item);
    new_item->info = val;

    return new_item;
}

#define INSERT(head_or_tail,pos,not_pos) \
            \
            PtrListElem* new_item = plist_elem_create(val);\
            \
            if(!(list->head_or_tail))\
            {\
                list->head = new_item;\
                list->tail = new_item;\
            }\
            else\
            {\
                list->head_or_tail->pos = new_item;\
                new_item->not_pos = list->head_or_tail;\
                list->head_or_tail = new_item;\
            }\
            \
            (list->pList_len)++;\
            plist_make_hash(list);

PtrListElem* plist_push_back(MyPtrList* list, list_type val)
{
    assert(list);

    INSERT(tail, next, prev);

    return new_item;
}

PtrListElem* plist_push_front(MyPtrList* list, list_type val)
{
    assert(list);

    INSERT(head, prev, next);

    return new_item;
}

//************************************
/// Inserts element in list after the certain element
///
/// \param [in] MyPtrList* list - pointer to MyPtrList object
/// \param [in] PtrListElem* elem - pointer to the element to insert after
/// \param [in] list_type val - value to insert
///
/// \return pointer to PtrListElem object which include val
///
//************************************

PtrListElem* plist_insert(MyPtrList* list, PtrListElem* elem, list_type val)
{
    assert(list && elem && (elem->next || elem == list->tail));

    PtrListElem* new_item = NULL;

    if(elem == list->tail)
    {
        return plist_push_back(list, val);
    }
    else
    {
        new_item = plist_elem_create(val);

        elem->next->prev = new_item;
        new_item->prev = elem;
        new_item->next = elem->next;
        elem->next = new_item;
    }

    (list->pList_len)++;
    plist_make_hash(list);

    return new_item;
}

#undef INSERT

#define REMOVE(head_or_tail,pos,not_pos)\
                    if(!(list->head_or_tail))\
                    {\
                        return LERREMOVE;\
                    }\
                    \
                    PtrListElem* del = list->head_or_tail;\
                    list->head_or_tail = list->head_or_tail->pos;\
                    \
                    if(list->head_or_tail) \
                    {\
                        list->head_or_tail->not_pos = NULL;\
                    }\
                    else\
                    {\
                        list->head = NULL;\
                        list->tail = NULL;\
                    }\
                    \
                    plist_elem_Dtor(del);\
                    free(del);\
                    \
                    (list->pList_len)--;\
                    plist_make_hash(list)

int plist_pop_back(MyPtrList* list)
{
    assert(list);

    REMOVE(tail, prev, next);

    return 0;
}

int plist_pop_front(MyPtrList* list)
{
    assert(list);

    REMOVE(head, next, prev);

    return 0;
}

int plist_erase(MyPtrList* list, PtrListElem* elem)
{
    assert(list && elem && (elem->next || elem == list->tail) && (elem->prev || elem == list->head));

    if(elem == list->head)
    {
        return plist_pop_front(list);
    }
    if(elem == list->tail)
    {
        return plist_pop_back(list);
    }

    PtrListElem* del = elem;
    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;

    plist_elem_Dtor(del);
    free(del);

    (list->pList_len)--;
    plist_make_hash(list);

    return 0;
}

#undef REMOVE

#define CUR_HEAD PtrListElem* cur = list->head

int plist_clear(MyPtrList* list)
{
    assert(list);

    CUR_HEAD;
    while(cur)
    {
        PtrListElem* del = cur;
        cur = cur->next;
        plist_elem_Dtor(del);
        free(del);
    }

    list->pList_len = 0;

    list->head = NULL;
    list->tail = NULL;

    plist_make_hash(list);

    return 0;
}

#define PREV(cur) cur->prev

#define NEXT(cur) cur->next

#define DO_REAL_HASH  \
    int StructHash_buf = list->pListHash_struct;\
    list->pListHash_struct = HASHDEFAULT;\
    int RealHash_buf = hash (list, sizeof (*list));

#define RE_STORE_HASH list->pListHash_struct = StructHash_buf;\

int plist_is_OK(MyPtrList* list)
{
    assert(list);
    int error = LERROK;

    DO_REAL_HASH;
    if ((list->plist_guard_begin != GUARD) || (list->plist_guard_end != GUARD) ||  RealHash_buf != StructHash_buf)
    {
        return LERSTRUCT;
    }
    RE_STORE_HASH;

    unsigned int count = 0;

    CUR_HEAD;
    while(cur)
    {
        if (error = plist_elem_is_OK(cur))
        {
            return error;
        }

        if (PREV(cur) && (NEXT(PREV(cur)) != cur) || NEXT(cur) && (PREV(NEXT(cur))!= cur))
        {
            return LERPOS;
        }

        if(count > list->pList_len)
        {
            return LERLEN;
        }

        cur = cur->next;
        count ++;
    }

    if (list->pList_len > count)
    {
        return LERLEN;
    }

    return error;
}




int plist_elem_is_OK(PtrListElem* elem)
{
    assert(elem);

    if ((elem->list_elem_guard_begin != GUARD) || (elem->list_elem_guard_end != GUARD))
    {
        return LERDATA;
    }

    if (!elem->info.key || !elem->info.value)
    {
        return LERINFO;
    }

    return LERROK;
}

#define CASE_LIST_OK(ERROR,MESSAGE)\
            case ERROR:\
            printf("   " MESSAGE "\n");\
            break;

#define TO_GRAPHVIZ(gv_name, DUMPNAME) strdup(DUMPNAME); strcat(gv_name, ".gv")

#define TO_BMP(bmp_name, DUMPNAME) strdup(DUMPNAME); strcat(bmp_name, ".bmp")

#define CONCAT_DOT(arg)    strcat(dotty, arg)

int plist_dump(const char dot[], const char DUMPNAME[], MyPtrList* list)
{
    assert(list);

    printf("~In File: %s\n~In Line: %d\n", __FILE__, __LINE__);
    printf("~List [0x%X]\n~{\n   Length = %u\n   Head = [0x%X]\n   Tail = [0x%X]\n", (out_ptr) list, list->pList_len, (out_ptr) list->head, (out_ptr) list->tail);
    printf("   Struct_guard_begin  = %s\n", ((list->plist_guard_begin) == GUARD)    ? "GUARD": "ERROR");
    printf("   Struct_guard_end  = %s\n",   ((list->plist_guard_end) == GUARD)      ? "GUARD": "ERROR");

    DO_REAL_HASH;
    printf("   In memory Struct_Hash = %i\n", StructHash_buf);
    printf("        Real Struct_Hash = %i\n", RealHash_buf);
    RE_STORE_HASH;

    printf("   INFO: \n");\
    switch(plist_is_OK(list))
    {
        CASE_LIST_OK(LERROK,    "List is OK");
        CASE_LIST_OK(LERDATA,   "Data memory in the list was damaged");
        CASE_LIST_OK(LERINFO,   "List values are not suitable");
        CASE_LIST_OK(LERLEN,    "List length is not suitable");
        CASE_LIST_OK(LERPOS,    "Links between list elements are damaged");
        CASE_LIST_OK(LERSTRUCT, "Structure of the list is damaged");
    default:
        printf("   List verification failed\n");
    }

    printf("~}\n\n");

    if (plist_draw(dot, DUMPNAME, list))
        return 1;

    return 0;
}

char* plist_elem_dump(PtrListElem* elem)
{
    assert(elem);

    char* dump = (char*) calloc(1000, sizeof(*dump));
    char dump_str[800] = "%i [shape = none, label = <<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\" CELLPADDING = \"4\">\
                                        <TR>\
                                        <TD> %s </TD>\
                                        <TD> %p </TD>\
                                        </TR>\
                                        <TR>\
                                        <TD  COLSPAN = \"2\"> %p </TD>\
                                        </TR>\
                                        <TR>\
                                        <TD>%p</TD>\
                                        <TD>%p</TD>\
                                        </TR>\
                                        </TABLE>>];\n";
    sprintf(dump, dump_str, elem, elem->info.key , (out_ptr)elem->info.value, (out_ptr) elem, (out_ptr) elem->prev, (out_ptr) elem->next);
    if(elem->prev)
    {
        sprintf(dump,"%i -> %i;\n", elem->prev, elem);
    }

    return dump;
}

int plist_draw(const char dot[], const char DUMPNAME[], MyPtrList* list)
{
    char* gv_name = TO_GRAPHVIZ(gv_name, DUMPNAME);  // make grathviz file name
    char* bmp_name = TO_BMP(bmp_name, DUMPNAME);     // make bmp file name

    FILE* dumptxt = fopen(gv_name, "w");
    if (!dumptxt) return 1;

    fprintf(dumptxt, "digraph ge\n{ rankdir = LR;\n");

    CUR_HEAD;
    for(int i = 0; (i < list->pList_len) && cur; i++ , cur = cur->next)
    {
        char* dump_elem_str = plist_elem_dump(cur);
        fprintf(dumptxt, dump_elem_str);
        free(dump_elem_str);
    }

    fprintf(dumptxt, "}");
    fclose(dumptxt);

    char* dotty = (char*) calloc(200, sizeof(*dotty));
    dotty = strcpy(dotty, dot);                         // make
    CONCAT_DOT(gv_name);                                // dot
    //CONCAT_DOT(" -o ");                                 // compile
    //CONCAT_DOT(bmp_name);                               // command

    system(dotty);                                  //compile graphviz
    //system(bmp_name);                               //open bmp

    free(gv_name);
    free(bmp_name);
    free(dotty);

    return 0;
}

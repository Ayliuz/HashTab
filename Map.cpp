//
// Created by Elias on 01.04.2019.
//

#include "Map.h"

using namespace std;

Map::Map(size_t size, hash_func_type hash_func) :
size_(size), container_(nullptr), hash_(hash_func)
{
    container_ = new MyPtrList [size_];

    for(size_t ind = 0; ind < size_; ind++)
    {
        plist_Ctor(&container_[ind]);
    }
}

Map::~Map()
{
    for(size_t ind = 0; ind < size_; ind++)
    {
        plist_Dtor(&container_[ind]);
    }
    delete [] container_;
    container_ = nullptr;
    size_ = SIZEPOIS;
}

int Map::verify()
{
    int error = 0;
    for(size_t ind = 0; ind < size_; ind++)
    {
        if ((error = plist_is_OK(&container_[ind])) != LERROK)
        {
            throw ind;
        }
    }
    return error;
}

void Map::dump(const char dot[], const char DUMPNAME[])
{
    printf("~In File: %s\n~In Line: %i\n", __FILE__,  __LINE__);
    printf("~Map [0x%p]\n~{\n   Size = %u\n   Container = [0x%p]\n", this, size_, container_);

    try
    {
        this->verify();
    }
    catch (int ind)
    {
        printf("List Error, index = %i",ind);
        plist_dump(dot, "Erlist", &container_[ind]);
    }

    string gv_Name = static_cast <string> (DUMPNAME) + ".gv";

    ofstream graph(gv_Name);
    try
    {
        if (graph.fail())
            throw gv_Name;
    }
    catch(const char name[])
    {
        printf("File %s can't be opened", name);
    }

    graph << "digraph ge { rankdir = LR;\n";

    for(size_t ind = 0; ind < size_; ind++)
    {
        graph << "subgraph ge" << ind << "\n{ rankdir = LR;\n";
        PtrListElem* cur = container_[ind].head;
        for(int i = 0; cur && (i < container_[ind].pList_len); i++ , cur = cur->next)
        {
            char* dump_elem_str = plist_elem_dump(cur);
            graph << dump_elem_str;
            free(dump_elem_str);
        }
        graph << "}\n\n";
    }
    graph << "}\n\n";
    graph.close();
}

val_type Map::find(key_type key)
{
    assert(key);
    int index = hash_(key, size_);
    try
    {
        if (container_[index].pList_len < 1)
        {
            throw EMPTY_LIST;
        }

        PtrListElem* cur = container_[index].head;
        while (cur && (strcmp(key, cur->info.key) != 0))
            cur = cur->next;
        return cur->info.value;
    }
    catch (int err)
    {
        switch (err)
        {
            case EMPTY_LIST:
                printf("Нет подходящего значения\n");
                break;

            default:
                printf("Неизвестная ошибка\n");
        }
    }
    return nullptr;
}

PtrListElem* Map::insert(const key_type key, const val_type value)
{
    assert(key && value);

    int index = (*hash_)(key, size_);
    list_type pair = {key, value};
    PtrListElem* cur = container_[index].head;

    while(cur)
    {
        if(cur->info.key == key)
        {
            return nullptr;
        }
        cur = cur->next;
    }

    return plist_push_back(&container_[index], pair);
}

void Map::erase (key_type key)
{
    assert(key);
    int index = (*hash_)(key, size_);
    plist_clear(&container_[index]);
}

void Map::clear ()
{
    for(size_t ind = 0; ind < size_; ind++)
    {
        plist_clear(&container_[ind]);
    }
}


int Map::fill_to_dictionary(char* data)
{
    assert(data);

    int count = 0;
    for (char* str = data; *str; count++)
    {
        list_type map_value = {nullptr, nullptr};

        map_value.key = str;
        map_value.value = strchr(str, '\0') + 1;
        str = strchr(map_value.value, '\0') + 1;                // str to the next pointer

        int index = hash_(map_value.key, size_);
        plist_push_back(&container_[index], map_value);
    }

    return count;
}
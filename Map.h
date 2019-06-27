//
// Created by Elias on 01.04.2019.
//

#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "PtrList.h"

using namespace std;

const char dot[] = "C:\\graphviz-2.38\\release\\bin\\gvedit.exe " ;
const char DUMPNAME[] = "HashTab";

const int EMPTY_LIST = 1;

typedef int (*hash_func_type)(key_type , size_t) ;

class Map {
    private:
        size_t  size_;
        hash_func_type hash_;
        MyPtrList* container_;
    public:
        explicit Map(size_t size, hash_func_type hash_func);
        ~Map();

        inline size_t size()
        {
            return size_;
        }

        inline MyPtrList* container()
        {
            return container_;
        }

        int fill_to_dictionary(char* data);         // ':' --------> '\0'

        int verify();
        void dump(const char dot[], const char DUMPNAME[]);
        val_type find(key_type key);
        PtrListElem* insert(key_type key, val_type value);
        void erase(key_type key);
        void clear ();
};



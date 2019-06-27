#include <cstdio>
#include <cmath>
#include <iostream>
#include <windows.h>
#include <cassert>
#include "Map.h"

const char DICTIONARY_NAME[] = "dictfull.txt";
const char CSV_NAME[] = "hash.csv";
const int HASH_FUNC_NUM = 5;

int get_file_size(const char* file_name);
int get_data_base(const char* file_name, char* data_container, int container_length);   // returns number of strings
int get_csv_spreading(char* dictionary, int str_num, const char* CSV_NAME,  hash_func_type hash_array [], const char* hash_names []);

int hash_init_char(key_type key, size_t size);
int hash_length (key_type key, size_t size);
int hash_char_sum (key_type key, size_t size);
int hash_rol (key_type key, size_t size);
int hash_faq6(key_type key, size_t size);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int dict_length = get_file_size(DICTIONARY_NAME);
    char* dictionary = new char [dict_length + 10];      // + 10 for a little gap

    int str_num = get_data_base(DICTIONARY_NAME, dictionary, dict_length);  // number o strings (pairs)

    hash_func_type hash_array [HASH_FUNC_NUM] = {hash_init_char,  hash_length, hash_char_sum, hash_rol, hash_faq6 };
    const char*    hash_names [HASH_FUNC_NUM] = {"Initial character", "Length", "Character sum", "Rol and xor", "FAQ6 Bob Jenkins"};

    Map map(str_num, hash_faq6);
    map.fill_to_dictionary(dictionary);

    printf("%s\n", map.find("grave"));
    delete [] dictionary;
    return 0;
}

int get_csv_spreading(char* dictionary, int str_num, const char* CSV_NAME,  hash_func_type hash_array [], const char* hash_names [])
{
    FILE* csv_file = fopen(CSV_NAME, "w");
    try
    {
        if (!csv_file)
            throw CSV_NAME;
    }
    catch(const char name[])
    {
        printf("File %s can't be opened", name);
    }

    for (int f_ind = 0; f_ind < HASH_FUNC_NUM; ++f_ind)
    {
        Map map(str_num, hash_array[f_ind]);
        map.fill_to_dictionary(dictionary);

        fprintf(csv_file, "%s; ", hash_names[f_ind]);
        for (int index = 0; index < map.size(); ++index)
        {
            fprintf(csv_file, "%i; ", map.container()[index].pList_len);
        }
        fprintf(csv_file, "\n");
    }

    fclose(csv_file);
    return 0;
}



int get_data_base(const char* file_name, char* data_container, int container_length)
{
    assert(file_name && data_container && container_length);

    FILE* dictionary_file = fopen(file_name, "r");
    try
    {
        if (!dictionary_file)
            throw file_name;
    }
    catch(const char name[])
    {
        printf("File %s can't be opened", name);
    }

    fread(data_container, sizeof(*data_container), container_length, dictionary_file);
    data_container[container_length + 1] = '\0';                  // null end of reading
    data_container[container_length + 2] = '\0';

    int str_count = 0;

    char* ch = strchr(data_container, ':');
    *(ch++) = '\0';

    for (; *ch != '\0'; ch++) {
        if(*ch == '\n')                                             // strings to null-terminated
        {
            *ch = '\0';
            ch = strchr(ch + 1, ':');
            *ch = '\0';
            str_count++;
        }
    }

    fclose(dictionary_file);

    return str_count + 1;                                           // the last ending null character
}

int get_file_size(const char* file_name)
{
    assert(file_name);

    FILE* file = fopen(file_name, "r");
    try
    {
        if (!file)
            throw DICTIONARY_NAME;
    }
    catch(const char name[])
    {
        printf("File %s can't be opened", name);
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);

    return length;
}

int hash_init_char(key_type key, size_t size)
{
    assert(key);
    return key[0];
}

int hash_length (key_type key, size_t size)
{
    assert(key);
    return strlen(key);
}

int hash_char_sum (key_type key, size_t size)
{
    assert(key);

    int sum = 0;
    for(char* str = key; *str; str++)
    {
        sum += *str;
    }
    return sum % size;
}

int hash_rol (key_type key, size_t size)
{
    assert(key);

    unsigned int hash = 0;

    for(char* str = key; *str; str++)
    {
        hash = ((hash >> 1u) | (hash << (8u - 1u))) ^ (unsigned char) (*str);
    }
    return hash % size;
}

int hash_faq6(key_type key, size_t size)                          // By Bob Jenkins
{
    assert(key);

    unsigned int hash = 0;

    for (char* str = key; *str; str++) {
        hash += (unsigned char) (*str);
        hash += (hash << 10u);
        hash ^= (hash >> 6u);
    }
    hash += (hash << 3u);
    hash ^= (hash >> 11u);
    hash += (hash << 15u);

    return hash % size;
}
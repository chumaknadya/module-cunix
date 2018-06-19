#ifndef BUILTINS_H_
#define BUILTINS_H_

#include "hash.h"

int export(hashtable_t* env, const char* args);

int env(hashtable_t* env, const char* args);

int echo(hashtable_t* env, const char* args);

int ls(hashtable_t* env, const char* args);

#endif

//
//  first.h
//  
//
//  Created by Andrew Boss on 2/21/17.
//
//
#ifndef first_h
#define first_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

struct Node {
    char *word;
    int letter;
    int prefixCount;
    int wordCount;
    struct Node *children[26];
};

struct Node *makeNode();

void freeNodes(struct Node *ptr);

void readDict(FILE *dict_file);

void addWordToTrie(char *word);

void readData(FILE *data_file);

void matchStr(char* str);

void printResult();

void recursivePrint(FILE *out, struct Node *ptr);

#endif  /*first_h */

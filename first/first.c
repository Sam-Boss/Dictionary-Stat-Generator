//
//  first.c
//  
//
//  Created by Andrew Boss on 2/21/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "first.h"

struct Node *root;
int outFileNumber = 1;

int main(int argc, char **argv){
    FILE *mapping_file;
    char buffer[1000];
    
    if(argc < 2){
        printf("invalid input\n");
        return 0;
    }
    mapping_file = fopen(argv[1],"r");
    //loop reads mapping file, creates dict_file, data_file
    while(fgets(buffer,sizeof(buffer),mapping_file)){
        FILE *dict_file;
        FILE *data_file;
        root = NULL;
        
        char *holdMyDict = strtok(buffer," ");
        char *holdMyData = strtok(NULL," \n");
        
        //sends dict_file to readDict,data_file to readData
        dict_file = fopen(holdMyDict,"r");
        readDict(dict_file);
        fclose(dict_file);
        data_file = fopen(holdMyData,"r");
        readData(data_file);
        fclose(data_file);
        
        //print, free up data, increment # of line mappings by 1
        printResult();
        freeNodes(root);
        outFileNumber++;
    }
    fclose(mapping_file);
    return 0;
}

//returns a new node structure to be used
struct Node *makeNode() {
    struct Node *baby = (struct Node*) malloc(sizeof(struct Node));
    int i;
    
    for(i = 0; i < 26; i++) {
        baby->children[i] = 0;
    }
    baby->word = NULL;
    baby->letter = -1;
    baby->prefixCount = 0;
    baby->wordCount = 0;
    
    return baby;
}

//recursively frees all nodes used to create trie
void freeNodes(struct Node *ptr){
    int i;
    if(ptr == NULL){
        return;
    }
    for(i = 0; i < 26; i++){
        freeNodes(ptr->children[i]);
    }
    if(ptr->word != NULL){
        free(ptr->word);
    }
    free(ptr);
   }

//reads dictionary file, searches for words, and calls addWordToTrie if word is found
void readDict(FILE *dict_file){
    char buffer[1000];
    int index = 0;
    char i;
    
    while(true){
        i = fgetc(dict_file);
        if(isalpha(i) == false){
        //check if word already stored in buffer, if yes -> add to trie
            if(index > 0){
                buffer[index] = '\0';
                addWordToTrie(buffer);
                index = 0;
            }
        //ends loop when file is over
            if(i == EOF){
                break;
            }
        }else{
            //part of a word -> make lower case, add to buffer, increment
            buffer[index] = tolower(i);
            index++;
        }
    }
}

//takes word and adds it into the trie
void addWordToTrie(char *word){
    struct Node *ptr;
    int i;
    
    if(root == NULL){
        root = makeNode();
    }
    ptr = root;
    
    for(i = 0; i < strlen(word); i++){
        int alph = tolower(word[i]) - 'a';
        if(ptr->children[alph] == NULL){
            ptr->children[alph] = makeNode();
            ptr->children[alph]->letter = alph;
        }
        ptr = ptr->children[alph];
    }
    ptr->word = (char*)malloc(sizeof(char)*(strlen(word)+1));
    
    for(i = 0; i < strlen(word) + 1; i++){
        if(i == strlen(word)){
            ptr->word[strlen(word)] = '\0';
        }else{
            ptr->word[i] = word[i];
        }
    }
}
    
//reads data_file, searches for words, and calls matchStr if word is found
void readData(FILE *data_file){
    char buffer[1000];
    int index = 0;
    char i;
    
    while(true){
        i = fgetc(data_file);
        if(isalpha(i) == false){
            //check if word already stored in buffer, if yes -> compare to trie
            if(index > 0){
                buffer[index] = '\0';
                matchStr(buffer);
                index = 0;
            }
            //ends loop when file is over
            if(i == EOF){
                break;
            }
        }else{
            //part of a word -> make lower case, add to buffer, increment
            buffer[index] = tolower(i);
            index++;
        }
    }
}

//compares word from data_file against words in trie
void matchStr(char* str){
    struct Node *ptr;
    int i;
    
    if(root == NULL){
        return;
    }
    ptr = root;
    
    for(i = 0; i < strlen(str); i++){
        int letter = tolower(str[i]) - 'a';
        ptr = ptr->children[letter];
        if(ptr == NULL){
            return;
        }
        if(i < strlen(str) - 1){
            ptr->prefixCount++;
        }
    }
    //unique word not prefix
    ptr->wordCount++;
}

//creates outi.txt files, where i = line # in mapping program
void printResult(){
    FILE *out;
    char result_file_name[32];
    
    sprintf(result_file_name,"out%d.txt",outFileNumber);
    out = fopen(result_file_name, "w");
    
    if(out == NULL){
        printf("error\n");
        return;
    }
    recursivePrint(out, root);
    fclose(out);
}
           
//called by printResult -> prints out words in trie by iterating through recursively
void recursivePrint(FILE *out, struct Node *ptr){
    int i;
    
    if(ptr == NULL){
        return;
    }
    if(ptr->word != NULL){
        fprintf(out, "%s %d %d\n",ptr->word,ptr->wordCount,ptr->prefixCount);
    }
    for(i = 0; i < 26;i++){
        recursivePrint(out,ptr->children[i]);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "mylib.h"

/**
 * Allocate memory of size s, checking for errors in allocation
 * @param s the size of memory to allocate
 * @return a pointer to the memory that has been allocated
 */
void *emalloc(size_t s){
    void *result = malloc(s);
    if(NULL==result){
        fprintf(stderr, "Memory Alloation Failed!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Reallocate memory of size s, checking for errors in reallocation
 * @param arr the memory that will be copied into the reallocated
 * memory location
 * @param s the size of memory to reallocate
 * @return a pointer to the memory that has been reallocated
 */
void *erealloc(void *arr, size_t s){
    void *result = realloc(arr, s);
    if(NULL == result){
        fprintf(stderr, "Memory Alloation Failed!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Get a word from a file stream up to a limit, and write it into s
 * @param s the string to write the word into
 * @param limit the maximum number of characters to read
 * @param stream the file stream to read from
 * @return EOF if end of file is reached before word is read, 0 if read word
 * is the null byte, other if word is read successfully. Essentially,
 * using while(getword(word, limit, stdin) != EOF) will loop until
 * stdin presents no more words.
 */
int getword(char *s, int limit, FILE *stream){
    int c;
    char *w = s;
    /*Ensure parameters make sense, exits if they are invalid*/
    assert(limit>0 && s!= NULL && stream != NULL);

    /*Skip to start of word*/
    while(!isalnum(c= getc(stream)) && EOF!=c){
        ;
    }

    if(EOF==c){
        return EOF;
    } else if(--limit>0){
        /*Reduce the limit by 1 to account for null byte*/
        *w++ = tolower(c);
    }
    
    while(--limit>0){
        if(isalnum(c=getc(stream))){
            *w++ = tolower(c);
        } else if('\'' == c){
            /*Skip ' characters*/
            limit++;
        } else {
            /*Anything else is end of word*/
            break;
        }
    }

    *w = '\0';
    return w-s;
                           
}

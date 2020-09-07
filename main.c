#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "mylib.h"
#include "tree.h"

#define MAX_LENGTH 100

static void print_info(int freq, char *word){
    printf("%-4d %s\n", freq, word);
}

int main(int argc, char **argv){
    const char *optstring = "c:df:orh";
    int c_flag=0, d_flag=0, o_flag=0, unknown_word_count=0;
    char option,
        *filename = emalloc(sizeof(filename[0]) * MAX_LENGTH+1),
        *word = emalloc(sizeof(word[0]) * MAX_LENGTH+1);
    FILE *outputFile, *inputFile;
    clock_t start, end;
    double fillTime, searchTime;
    tree t;
    tree_t tt = BST;

    /*Read in the options, to see what program must do*/
    while((option = getopt(argc, argv, optstring)) != EOF){
        switch (option){
            case 'c':
                /*Check spelling of words in argument file*/
                c_flag=1; /*To ignore d, o flags*/
                filename = emalloc(sizeof(filename[0]) * strlen(optarg));
                strcpy(filename, optarg);
                break;
            case 'd':
                /*Print the depth of tree and nothing else, ensure c is not set*/
                if(!c_flag){
                    d_flag=1;
                }
                break;
            case 'f':
                /*Write dot output to argument. o must be specified first*/
                if(c_flag){
                    /*Ignore this entirely if c is given*/
                    break;
                }
                if(!o_flag){
                    fprintf(stderr,
                            "Please specifiy -o before giving -f argument!");
                    exit(EXIT_FAILURE);
                }
                filename = emalloc(sizeof(filename[0]) * strlen(optarg));
                strcpy(filename, optarg);
                break;
            case 'o':
                /*Output representation of the tree in dot form to file*/
                /*Set the default file name to output to*/
                filename = emalloc(sizeof(filename[0])
                                   * strlen("tree-view.dot\0"));
                strcpy(filename, "tree-view.dot\0");
                o_flag = 1;
                break;
            case 'r':
                /*Make this a rbt instead of bst*/
                tt = RBT;
                break;
            case 'h':
                /*Print a help message!*/
                printf("USAGE: ./asgn [OPTION] <STDIN>\n");
                printf("Perform various operations using a binary tree.  "
                       "By default, words\n"
                       "are read from stdin and added to the tree,"
                       "before being printed out\n"
                       "alongside their frequencies to stdout.\n"
                       "\n");
                printf("-c FILENAME  Check spelling of words in "
                       "FILENAME using words\n"
                       "\tread from stdin as the dictionary.  Print timing\n"
                       "\tinfo & unknown words to stderr (ignore -d & -o)\n"
                       "-d           Only print the tree depth (ignore -o)\n"
                       "-f FILENAME  Write DOT output to"
                       "FILENAME (if -o given)\n"
                       "-o           Output the tree in DOT form to"
                       "file 'tree-view.dot'\n"
                       "-r           Make the tree an RBT "
                       "(the default is a BST)\n"
                       "\n"
                       "-h           Print this message\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Not a valid option!");
                break;
        }
    }

    /*define the tree, including the tree type to use*/
    t = tree_new(tt);

    start = clock();
    /*Get all of out input from stdin*/
    while(getword(word, MAX_LENGTH, stdin) != EOF){
        t=tree_insert(t ,word);
    }
    end=clock();
    fillTime = (end-start)/(double)CLOCKS_PER_SEC;
    
    /*Now, what we do depends on flags*/
    if(c_flag){
        /*If -c arg was give, we ignore other options*/
        /*Open the file for reading*/
        inputFile = fopen(filename, "r");
        /*Ensure file as actually opened*/
        if(inputFile == NULL){
            fprintf(stderr, "Can't open file specified! %s\n", filename);
            exit(EXIT_FAILURE);
        }
        
        start = clock();
        /*Get all of out input from file and check if in tree*/
        while(getword(word, MAX_LENGTH, inputFile) != EOF){
            if(!tree_search(t, word)){
                printf("%s\n", word);
                unknown_word_count++;
            }            
        }
        end=clock();
        searchTime = (end-start)/(double)CLOCKS_PER_SEC;
        fprintf(stderr,
                "Fill time\t: %f\nSearch time\t: %f\nUnknown words = %d\n",
                fillTime, searchTime, unknown_word_count);
        /*Once finished, close the file again*/
        fclose(inputFile);
    } else if(d_flag){
        /*just print the depth of the tree*/
        printf("%d\n", tree_depth(t));
    } else if(o_flag){
        /*Output dot representation to filename*/
        /*Open our file and give it to the dot printing functions*/
        printf("Creating dot file '%s'\n", filename);
        outputFile = fopen(filename, "w");
        /*Ensure file as actually opened*/
        if(outputFile == NULL){
            fprintf(stderr, "Can't open file specified! %s\n", filename);
            exit(EXIT_FAILURE);
        }
        tree_output_dot(t, outputFile);
        /*Now file has been written to, close it again*/
        fclose(outputFile);
    } else {
        /*Nothing else specified, just do a preorder walk and print information*/
        tree_preorder(t, print_info);
    }
    free(filename);
    free(word);
    tree_free(t);
    return EXIT_SUCCESS;    
}

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
                                   * strlen("output-dot.dot\0"));
                strcpy(filename, "output-dot.dot\0");
                o_flag = 1;
                break;
            case 'r':
                /*Make this a rbt instead of bst*/
                tt = RBT;
                break;
            case 'h':
                /*Print a help message!*/
                printf("tree-test usage:\n"
                       "Reads words from stdin and inserts them into "
                       "a tree structure.\n");
                printf("-c filename Check the spelling of words in filename "
                       "against words read from stdin. Prints all unknown "
                       "words to stdout. Print timing information and "
                       "unknown word count to stderr. "
                       "Ignores -d and -o options.\n");
                printf("-d Print the depth of the tree to "
                       "stdout and do nothing else\n");
                printf("-f filename Write the dot output to filename rather "
                       "than the default. Requires the -o option to be "
                       "specified first.\n");
                printf("-o Output a representation of the tree in dot form to "
                       "file. Default file is output-dot.dot\n");
                printf("-r Make the tree a red-black tree opposed to the "
                       "default binary search tree\n");
                printf("-h Display this help message\n");
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
        printf("Tree Depth: %d\n", tree_depth(t));
    } else if(o_flag){
        /*Output dot representation to filename*/
        /*Open our file and give it to the dot printing functions*/
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
        /*What else could be specified?! Maybe ask about here, nothing given*/
    }
    free(filename);
    free(word);
    tree_free(t);
    return EXIT_SUCCESS;    
}

# TreeDataStructureProject

An investigation into Binary Search Trees and Red Black Trees. This project gives the data structures themselves, a way to create them and insert items into them, then investigate the results of this. Made for a university group project for University of Otago paper COSC242.

mylib.c contains several useful functions that help in the development of the project, including error-checking memory allocation.

tree.c contains the implementation of the binary search tree data structure and the extensions required to make this into a red black tree. Also contains all of the functions such as insert, search, depth, and so forth.

main.c is the main executable of the project, where the tree structure is initialised and tested. This specific implementation has been set up to act as a spellcheck function by reading words from a dictionary, then checking to see if the next words to be read are in the dictionary, and outputing those that are not. main.c must be compiled, then takes command line arguments in order to create the desired outcome.


Specifically, the following flags have the respective following effects:

-c filename : Check the spelling of words in filename using words read from stdin as the dictionary. Print all unknown words to stdout. Print timing information and unknown word count to stderr. When this option is given then the -d and -o options should be ignored.

-d : print the depth of the tree to stdout and do nothing else

-o : output a representation of the tree in "dot" form to a file (default tree-view.dot)

-f filename : write the dot output to filename not the default file. -o must be given first

-r : make the tree a red black tree rather than the default binary search tree

-h : print a help message describing usage

The dot notation may be compiled to a pdf file using
dot -Tpdf < tree-view.dot > tree-view.pdf
on a UNIX machine at least.

#include <stdio.h>
#include<string.h>
#include "ptree.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <libgen.h>

// Defining the constants described in ptree.h
const unsigned int MAX_PATH_LENGTH = 1024;

// If TEST is defined (see the Makefile), will look in the tests 
// directory for PIDs, instead of /proc.
#ifdef TEST
    const char *PROC_ROOT = "tests";
#else
    const char *PROC_ROOT = "/proc";
#endif
 

/*
 * Creates a PTree rooted at the process pid. The root of the tree is
 * placed in root. The function returns 0 if the tree was created 
 * successfully and 1 if the tree could not be created or if at least
 * one PID was encountered that could not be found or was not an 
 * executing process.
 */
int generate_ptree(struct TreeNode **root, pid_t pid) {

    char procfile[MAX_PATH_LENGTH + 1];
    char namestr[MAX_PATH_LENGTH + 1];
    char exestr[MAX_PATH_LENGTH + 4];
    char childrenstr[MAX_PATH_LENGTH * 2];
//    char *realname;


    //PID CHECK
    if (sprintf(procfile, "%s/%d", PROC_ROOT, pid) < 0) {
        fprintf(stderr, "sprintf failed to produce a filename\n");
        return 1;
    }

    struct stat sb; 

    int result = lstat(procfile, &sb);
    
    if (result != 0) {
        fprintf(stderr, "PID dir does not exist\n");
        return 1;
    }

    //EXE CHECK
    if (sprintf(exestr, "%s/exe", procfile) < 0) {
        fprintf(stderr, "sprintf failed to write to exestr properly \n");
        return 1;
    }

    result = lstat(exestr, &sb);

    if (result != 0) {
        fprintf(stderr, "PID/exe does not exist\n");
        return 1;
    }

    //CMDLINE CHECK
    if (sprintf(namestr, "%s/cmdline", procfile) < 0) {
        fprintf(stderr, "sprintf failed to write to namestr properly\n");
        return 1;
    }

    result = lstat(namestr, &sb);

    if (result != 0) {
        fprintf(stderr, "PID/cmdline does not exist\n");
        return 1;
    }

    //CHILDREN CHECK

    if (sprintf(childrenstr, "%s/task/%i/children", procfile, pid) < 0) {
        fprintf(stderr, "sprintf failed to write to children properly\n");
        return 1;
    }

    result = lstat(childrenstr, &sb);

    if (result != 0) {
        fprintf(stderr, "children folder is not present\n");
        return 1;
    }


    struct TreeNode * current = malloc(sizeof(struct TreeNode));


    if (current == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }

    current->pid = pid;
    current->name = NULL;
    current->child_procs = NULL;
    current->next_sibling = NULL;

    FILE *cmdline = fopen(namestr, "r");

    if (cmdline == NULL) {
        fprintf(stderr, "something went wrong opening cmdline\n");
        return 1;
    }

    //assigning the name to the struct member
    if (fgets(namestr, MAX_PATH_LENGTH + 1, cmdline) != NULL) {
        

        if (strcmp(namestr, "") == 0 || strcmp(namestr, "\0") == 0) {
            current->name = NULL;
        }

        else {

            current->name = malloc(sizeof(char) * (MAX_PATH_LENGTH + 1));

            if (current->name == NULL) {
                fprintf(stderr, "code shit the bed\n");
                return 1;
            }

            strcpy(current->name, basename(namestr));

        
        }
        
    }

    if (fclose(cmdline) != 0) {
        fprintf(stderr, "cmdline did not close properly");
        return 1;
    }
    

    FILE *children = fopen(childrenstr, "r");

    char* cnode;
    char *ptr = NULL;

    int error = 0;
    int total_errors = 0;
 
    while (fgets(childrenstr, MAX_PATH_LENGTH * 2, children) != NULL) {
        char *saveptr;

        cnode = strtok_r(childrenstr, " ", &saveptr);

        struct TreeNode * x;
        struct TreeNode * first = NULL;
        struct TreeNode * previous = NULL;
        
        while (cnode != NULL) {

            x = malloc(sizeof(struct TreeNode));

            if (x == NULL) {
              fprintf(stderr, "malloc failed\n");
              return 1;
            }


            error = generate_ptree(&x, strtol(cnode, &ptr, 10));
            total_errors += error;
            if (error == 0) {

                if (previous != NULL) {
                    previous->next_sibling = x;
                }
                 previous = x;      
            }


            cnode = strtok_r(NULL, " ", &saveptr);


            if (first == NULL && error == 0) {
                    first = x;
                }

        }
        current->child_procs = first;

    }

    *root = current;

    if (fclose(children) != 0) {
        fprintf(stderr, "children did not close properly");
        return 1;
    }

    if (total_errors > 0) {
        return 1;
    }
    return 0;

}

/*
 * Prints the TreeNodes encountered on a preorder traversal of an PTree
 * to a specified maximum depth. If the maximum depth is 0, then the 
 * entire tree is printed.
 */
void print_ptree(struct TreeNode *root, int max_depth) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;

    if ((max_depth > 0 && depth < max_depth) || max_depth == 0) {

        if (root != NULL) {
            printf("%*s", depth * 2, "");
            //current node is printed

            if (root->name != NULL) {
                printf("%i: %s\n", root->pid, root->name);
            }
            
            else {
                printf("%i\n", root->pid);
            }


            //we create a temp node as the first child
            struct TreeNode * child = root->child_procs;
            depth++;

            //while our root isnt a leaf or our temp isnt the last child of that root
            while (child != NULL && root->child_procs != NULL) {

                print_ptree(child, max_depth);
                child = child->next_sibling;
            }

            depth--;

        }
    }
}

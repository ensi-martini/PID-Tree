#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ptree.h"

/*void free_tree(struct TreeNode * root) {

	//if this is fully a leaf node, we are safe to free
	if (root->next_sibling == NULL && root->child_procs == NULL) {
		free(root->name);
		free(root);
	}

	else if (root->next_sibling == NULL) {
		free_tree(root->child_procs);
	}

	else if (root->child_procs == NULL ) {
		free_tree(root->next_sibling);
	}

}*/



int main(int argc, char **argv) {
    // TODO: Update error checking and add support for the optional -d flag
    
	if (argc != 2 && argc != 4) {
		fprintf(stderr, "Usage:\n\tptree [-d N] PID\n");
		return 1;
	}
    	
	struct TreeNode *root = NULL;
	int num;
	int depth = 0;
	char *ptr = NULL;
	int pid_ind = 1;
	int pid = 0;

	//ALL POSSIBLE CASES:
	//./print_ptree 1234 -d 1
	//./print_ptree -d 1 1234
	//./print_ptree 1234 -dx 1
	//./print_ptree 1234 -d1 1
	//./print_ptree 1234 1 -d
	//./print_ptree 1234 -d -d
	//./print_ptree 1234 -d1 -d
	//./print_ptree 1234 -d -d1

	if (argc == 4) {

		while ((num = getopt(argc, argv, "d:")) != -1) {

			switch (num) {

				case 'd':			

					if (strcmp(optarg, argv[optind - 1]) == 0) {
						depth = strtol(optarg, &ptr, 10);

						if (strcmp(ptr, "") != 0) { //our ptr will be empty string when all input is part of a number
							fprintf(stderr, "Depth given is not a int\n");
							return 1;
						}

						else if (depth < 0) {
							fprintf(stderr, "Depth provided is not in valid range\n");
							return 1;
						}

						else {
							//printf("-d flag triggered, depth value of %s\n", optarg); 
							pid_ind = 3;
						}					
					}

					else {
						fprintf(stderr, "-d flag used improperly (unwanted suffix)\n");
						return 1;
					}

					break;

				case '?':
					return 1;

				}
		}
	}

	pid = strtol(argv[pid_ind], &ptr, 10);

	if ((pid == 0 && argv[pid_ind][0] != '0') || ptr[0] != '\0' || pid < 0) {
		fprintf(stderr, "Bad PID value given\n");
		return 1;
	}
	
	int error = generate_ptree(&root, pid);

	if (error != 0) {
		return 2;
	}


	if (root != NULL) {
		print_ptree(root, depth);
	}

	//free_tree(root);

	
	return 0;

	


	
}


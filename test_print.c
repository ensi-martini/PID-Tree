#include <stdio.h>
#include <stdlib.h>

#include "ptree.h"


//int main(int argc, char *argv[]) {
  //   // Creates a ptree to test printing
  //   struct TreeNode root, child_one, child_two, grandchild;
  //   root.name = "root process";
  //   root.child_procs = &child_one;
  //   root.next_sibling = NULL;
  //   root.pid=5;

  //   child_one.name = "first child";
  //   child_one.child_procs = NULL;
  //   child_one.next_sibling = &child_two;

  // //  child_two.name = "second child";
  //   child_two.child_procs = &grandchild;
  //   child_two.next_sibling = NULL;

  //   grandchild.name = "grandchild";
  //   grandchild.child_procs = NULL;
  //   grandchild.next_sibling = NULL;

  //   print_ptree(&root, 0);

  //   return 0;

    int main(int argc, char *argv[]) {
    // Creates a ptree to test printing
    struct TreeNode root, child_one, child_two, grandchild, brother, brother1;
    root.name = "LeBron James";
    root.child_procs = &child_one;
    root.next_sibling = NULL;
    root.pid = 23;

    child_one.name = "Lonzo Ball";
    child_one.child_procs = &child_two;
    child_one.next_sibling = &brother;
    child_one.pid = 2;

    child_two.name = "Jae Crowder";
    child_two.child_procs = NULL;
    child_two.next_sibling = &grandchild;
    child_two.pid = 9;

    grandchild.name = "IT";
    grandchild.child_procs = NULL;
    grandchild.next_sibling = NULL;
    grandchild.pid = 3;

    brother.name = "Melo";
    brother.child_procs = NULL;
    brother.next_sibling = &brother1;
    brother.pid = 1;

    brother1.name = "Hello";
    brother1.child_procs = NULL;
    brother1.next_sibling = NULL;
    brother1.pid = 4;

    print_ptree(&root, 2);

    return 0;

}


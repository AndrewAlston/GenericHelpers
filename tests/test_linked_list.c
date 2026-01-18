#include <stdlib.h>
#include <stdio.h>
#include <asm-generic/types.h>
#include "linked_list.h"

int main(void)
{
    struct route4tree *tree = init_tree4();

    // Insert a few nodes to create branches
    int *data1 = malloc(sizeof(int)); *data1 = 1;
    insert_tree4(tree, 0x0A000000, 8, data1); // 10.0.0.0/8, for those of us that dont read in hex :)

    int *data2 = malloc(sizeof(int)); *data2 = 2;
    insert_tree4(tree, 0x0A010000, 16, data2); // 10.1.0.0/16

    int *data3 = malloc(sizeof(int)); *data3 = 3;
    insert_tree4(tree, 0xC0A80000, 16, data3); // 192.168.0.0/16

    // Free entire tree
    free_tree4(tree);

    //edge case, if its null should return without crushing
    struct route4tree *empty = NULL;
    free_tree4(empty); 

    printf("free_tree4 executed without crash.\n");

    return 0;
}

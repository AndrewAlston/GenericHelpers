#include <stdlib.h>
#include <stdio.h>
#include <asm-generic/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "linked_list.h"

int main(void)
{
    char prefix_str[INET_ADDRSTRLEN] = {0};
    struct data {
        __u32 prefix;
        __u8 cidr;
    };
    struct route4tree *tree = init_tree4();
    if (!tree) {
        printf("Failed allocating memory for base binary tree\n");
        return EXIT_FAILURE;
    }
    struct data *test_data = calloc(4, sizeof(struct data));
    if (!test_data) {
        printf("Failed allocating memory for test data\n");
        free(tree);
        return EXIT_FAILURE;
    }
    test_data[0].prefix = htonl(0xA0000000); test_data[0].cidr = 8;
    test_data[1].prefix = htonl(0xA0100000); test_data[1].cidr = 16;
    test_data[2].prefix = htonl(0xC0A80000); test_data[2].cidr = 16;
    test_data[3].prefix = htonl(0xC8F7FF00); test_data[3].cidr = 24;

    // Insert the test prefix's
    for (int i = 0; i < 4; i++) {
        inet_ntop(AF_INET, &test_data[i].prefix, prefix_str, INET_ADDRSTRLEN);
        printf("Inserting prefix %s/%d\n", prefix_str, test_data[i].cidr);
        insert_tree4(tree, test_data[i].prefix, test_data[i].cidr, &test_data[i]);
    }
    fflush(stdout);
    for (int i = 0; i < 4; i++) {
        struct route4tree *node = lookup_exact(tree, test_data[i].prefix, test_data[i].cidr);
        if (node && node->data) {
            struct data *lookup_test = node->data;
            if (lookup_test) {
                char PREFIX_STR[INET_ADDRSTRLEN] = {0};
                inet_ntop(AF_INET, &lookup_test->prefix, PREFIX_STR, INET6_ADDRSTRLEN);
                printf("Retrieved exact match for prefix %s/%d\n", PREFIX_STR, lookup_test->cidr);
            } else {
                printf("Lookup failed!\n");
            }
        }
    }
    free(test_data);
    printf("Attempting to free up tree\n");
    free_tree4(tree);
    printf("Completed\n");
    //edge case, if its null should return without crashing
    struct route4tree *empty = NULL;
    free_tree4(empty); 

    printf("free_tree4 executed without crash.\n");

    return 0;
}

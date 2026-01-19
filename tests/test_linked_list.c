#include <stdlib.h>
#include <stdio.h>
#include <asm-generic/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "linked_list.h"

/** @brief Basic structure used to add test data to the tree
 */
struct data {
    __u32 prefix; /**< Prefix we are adding to the tree */
    __u8 cidr; /**< CIDR of the added prefix */
};

/** @brief This code exists purely to test the functionality of the linked list code
 */
int main(void)
{
    char prefix_str[INET_ADDRSTRLEN] = {0};
    struct route4tree *tree = init_tree4();
    struct data *test_data1 = NULL, *test_data2 = NULL, *test_data3 = NULL, *test_data4 = NULL;
    struct data *lpm_lookup1 = NULL, *lpm_lookup2 = NULL;
    if (!tree) {
        printf("Failed allocating memory for base binary tree\n");
        return EXIT_FAILURE;
    }
    if ((test_data1 = calloc(1, sizeof(struct data))) == NULL)
        goto cleanup;
    if ((test_data2 = calloc(1, sizeof(struct data))) == NULL)
        goto cleanup;
    if ((test_data3 = calloc(1, sizeof(struct data))) == NULL)
        goto cleanup;
    if ((test_data4 = calloc(1, sizeof(struct data))) == NULL)
        goto cleanup;
    if ((lpm_lookup1 = calloc(1, sizeof(struct data))) == NULL)
        goto cleanup;
    if ((lpm_lookup2 = calloc(1, sizeof(struct data))) == NULL)
        goto cleanup;
    test_data1->prefix = htonl(0x0A000000); test_data1->cidr = 8;
    test_data2->prefix = htonl(0x0A100000); test_data2->cidr = 16;
    test_data3->prefix = htonl(0xC0A80000); test_data3->cidr = 16;
    test_data4->prefix = htonl(0xC8F7FF00); test_data4->cidr = 24;
    lpm_lookup1->prefix = htonl(0x0A0B0000); lpm_lookup1->cidr = 24;
    lpm_lookup2->prefix = htonl(0xC0A8B900); lpm_lookup2->cidr = 24;

    insert_tree4(tree, test_data1->prefix, test_data1->cidr, test_data1, NULL);
    insert_tree4(tree, test_data2->prefix, test_data2->cidr, test_data2, NULL);
    insert_tree4(tree, test_data3->prefix, test_data3->cidr, test_data3, NULL);
    insert_tree4(tree, test_data4->prefix, test_data4->cidr, test_data4, NULL);
    struct route4tree *test_node = NULL;
    __u32 pfx = 0; __u8 cidr = 0;
    for (int i = 0; i < 6; i++) {
        switch (i) {
            case 0:
                pfx = test_data1->prefix; cidr = test_data1->cidr;
                break;
            case 1:
                pfx = test_data2->prefix; cidr = test_data2->cidr;
                break;
            case 2:
                pfx = test_data3->prefix; cidr = test_data3->cidr;
                break;
            case 3:
                pfx = test_data4->prefix; cidr = test_data4->cidr;
                break;
            case 4:
                pfx = lpm_lookup1->prefix; cidr = lpm_lookup1->cidr;
                break;
            case 5:
                pfx = lpm_lookup2->prefix; cidr = lpm_lookup2->cidr;
            default:
                break;
        }
        inet_ntop(AF_INET, &pfx, prefix_str, INET_ADDRSTRLEN);
        printf("Looking up %s/%d\n", prefix_str, cidr);
        test_node = lookup_lpm(tree, pfx, cidr, NULL, NULL);
        if (test_node && test_node->data) {
            struct data *lookup_test = test_node->data;
            if (lookup_test) {
                if (lookup_test->prefix == pfx) {
                    inet_ntop(AF_INET, &lookup_test->prefix, prefix_str, INET_ADDRSTRLEN);
                    printf("[%d] Retrieved exact match for prefix %s/%d\n", i, prefix_str, cidr);
                } else {
                    inet_ntop(AF_INET, &pfx, prefix_str, INET_ADDRSTRLEN);
                    printf("Expected %s/%d ", prefix_str, cidr);
                    inet_ntop(AF_INET, &lookup_test->prefix, prefix_str, INET_ADDRSTRLEN);
                    printf("Got %s/%d\n", prefix_str, lookup_test->cidr);
                }
            }
        } else {
            printf("Got no data back - lookup failed!\n");
        }
    }
    printf("Attempting to free up tree\n");
    free_tree4(&tree);
    printf("Completed, tree is now %p\n", tree);
    printf("Attempting to free NULL tree\n");
    free_tree4(&tree);
    printf("free_tree4 executed without crash.\n");
    // These won't be free'd up by freeing the tree, because they are not directly in the tree
    // Exact matches will have their pointers free'd on tree cleanup
    free(lpm_lookup1);
    free(lpm_lookup2);
    return EXIT_SUCCESS;
cleanup:
    if (test_data1)
        free(test_data1);
    if (test_data2)
        free(test_data2);
    if (test_data3)
        free(test_data3);
    if (test_data4)
        free(test_data4);
    if (lpm_lookup1)
        free(lpm_lookup1);
    if (lpm_lookup2)
        free(lpm_lookup2);
    return EXIT_SUCCESS;
}

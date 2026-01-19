//
// Created by andrew on 1/17/26.
//

#include <stdlib.h>
#include <asm-generic/types.h>
#include "linked_list.h"

struct route4tree *init_tree4(void)
{
    struct route4tree *tree = calloc(1, sizeof(struct route4tree));
    if (!tree)
        return NULL;
    return tree;
}

struct route4tree *insert_tree4(struct route4tree *tree, __u32 addr, __u8 cidr, void *data)
{
    if (!data)
        return NULL;
    if (!tree)
        return NULL;
    struct route4tree *current = tree;
    for (int i = cidr-1; i >= 0; i--) {
        __u32 bit = (addr >>i)&1;
        if (bit) {
            if (current->set) {
                current = current->set;
                continue;
            }
            current->set = calloc(1, sizeof(struct route4tree));
            if (!current->set) {
                return NULL;
            }
            current->set->parent = current;
            current = current->set;
        } else {
            if (current->unset) {
                current = current->unset;
                continue;
            }
            current->unset = calloc(1,sizeof(struct route4tree));
            if (!current->unset) {
                return NULL;
            }
            current->unset->parent = current;
            current = current->unset;
        }
    }
    if (!current->data) {
        current->data = data;
    } else {
        free(current->data);
        current->data = data;
    }
    return current;
}

struct route4tree *lookup_lpm(struct route4tree *tree, __u32 addr, __u8 cidr)
{
    struct route4tree *current = tree;
    for (int i = 0; i < cidr; i++) {
        __u32 bit = (addr >> (31-i)&1);
        if (bit) {
            if (current->set) {
                current = current->set;
                continue;
            }
            if (current->data)
                return current;
            return NULL;
        }
        if (current->unset) {
            current = current->unset;
            continue;
        }
        if (current->data)
            return current;
        return NULL;
    }
    return NULL;
}

struct route4tree *lookup_exact(struct route4tree *tree, __u32 addr, __u8 cidr)
{
    struct route4tree *current = tree;
    for (int i = cidr-1; i >= 0; i--) {
        __u32 bit = (addr >>i)&1;
        if (bit) {
            if (current->set) {
                current = current->set;
                continue;
            }
            return NULL;
        }
        if (current->unset) {
            current = current->unset;
            continue;
        }
        return NULL;
    }
    if (current && current->data)
        return current;
    return NULL;
}

void remove_node(struct route4tree *tree, __u32 address, __u8 cidr)
{
    // Swap the address to little endian order
    struct route4tree *current = tree;
    // Iterate to the last known entry;
    int current_cidr = 0;
    for (int i = cidr-1; i >= 0; i--) {
        __u32 bit = (address >> (31-i))&1;
        if (bit) {
            if (current->set) {
                current_cidr++;
                current = current->set;
            } else {
                break;
            }
        } else {
            if (current->unset) {
                current_cidr++;
                current = current->unset;
            } else {
                break;
            }
        }
    }
    do {
        // If we have an exact match - we need to handle removing a single path
        if (current_cidr == cidr) {
            if (current->data) {
                free(current->data);
                current->data = NULL;
            }
            // If there are child entries bail
            if (current->set || current->unset)
                return;
            // Set the current cidr to 0 to avoid repeating this condition
            current_cidr = 0;
            // Move to the parent
            current = current->parent;
        } else {
            // If there are no child entries transverse up the tree and free things up as we go
            if (!current->set && !current->unset && !current->data) {
                // Store the current entry
                struct route4tree *tmp = current;
                // Transverse up the tree
                current = current->parent;
                //  If the previously saved parent entry matches the set entry, free it up
                if (tmp == current->set) {
                    free(current->set);
                    // If the previously saved parent entry matches unset entry, free it up
                } else if (tmp == current->unset)
                    free(current->unset);
                // Break if there are still children
            } else {
                break;
            }
        }
        // Halt when we get to the top of the tree
    } while (current != tree);
}

void free_tree4(struct route4tree *tree)
{
    struct route4tree *current = tree;

    while(current) {
        //descend as far as possible
        if(current->set) {
            current = current->set;
            continue;
        }
        if(current->unset) {
            current = current->unset;
            continue;
        }

        if(current->data) {
            free(current->data);
            current->data = NULL;
        }

        if(current->parent) {
            //used same logic as remove_node above
            // Store the current entry
            struct route4tree *tmp = current;
            // Traverse up the tree
            current = current->parent;
            //  If the previously saved parent entry matches the set entry, free it up
            if (tmp == current->set) {
                free(current->set);
                current->set = NULL;
                // If the previously saved parent entry matches unset entry, free it up
            } else if (tmp == current->unset) {
                free(current->unset);
                current->unset = NULL;
            }
            //if it doesnt have parent means its the root
        }else {
            free(current);
            break;
        }
    }
}

//
// Created by andrew on 1/17/26.
//

#ifndef GENERICHELPER_LINKED_LIST_H
#define GENERICHELPER_LINKED_LIST_H


/** @brief The route4tree structure is used in the creation of a 32 bit binary tree
 */
struct route4tree {
    struct route4tree *set; /**< Pointer used for when a binary bit is set */
    struct route4tree *unset; /**< Pointer used for when a binary bit is not set */
    struct route4tree *parent; /**< Pointer to the parent entry of the tree */
    void *data; /**< Void pointer to data to be stored in the tree */
};

/** @def callback
 * @brief This is used for callbacks on insert_tree4 in case of special processing
 */
typedef int(callback)(struct route4tree *, void *);
typedef int(callback_remove)(struct route4tree *, void *, void *);

/** @brief Initialize a binary tree head entry
 * @returns A pointer to the head of a binary tree
 */
struct route4tree *init_tree4(void);

/** @brief This inserts an entry into a binary tree
 * @details Data is a void pointer making this generic and able to store any data in the tree.
 * It should be noted that the tree must be initialized before first insertion using
 * init_tree4();
 * @param[in] tree An initialized binary tree
 * @param[in] addr A 32 bit address to be inserted into the tree
 * @param[in] cidr An integer representing how many bits of the address to insert into the tree
 * @param[in] data A void pointer to the data to insert into the tree
 * @param[in] cb A Function pointer used for inserting data - if NULL the tree data pointer will simply point to the data parameter
 * @returns A route4tree structure at the inserted position in the tree
 */
struct route4tree *insert_tree4(struct route4tree *tree, __u32 addr, __u8 cidr, void *data, callback cb);

/** @brief This does a longest prefix match against the binary tree
 * @details This function will return at the deepest point in the tree that is matched
 * and has data.  It is useful in the data pointer to store the actual prefix and cidr
 * so that when this returns you can know exactly what has been matched.
 * @param[in] tree The head of an initialized binary tree
 * @param[in] addr A 32 bit address to be searched for in the tree
 * @param[in] cidr The maximum number of bits to transverse to in the tree
 * @returns A tree entry at the deepest point possible in the transversal
 */
struct route4tree *lookup_lpm(struct route4tree *tree, __u32 addr, __u8 cidr);

/** @brief This does an exact match lookup against the tree
 * @details This function will return the tree structure at the exact match point
 * if it can transverse the tree to the depths represented by cidr and if data
 * is present at the end of the transversal
 * @param[in] tree The head of an initialized binary tree
 * @param[in] addr An address to lookup in the tree
 * @param[in] cidr The depth to transverse to in the tree
 * @returns A pointer to the tree entry if matched, or NULL
 */
struct route4tree *lookup_exact(struct route4tree *tree, __u32 addr, __u8 cidr);

/** @brief This function removes a node from the tree
 * @details This function will remove a node from the tree, and all empty
 * parent nodes.  If a parent node still has children or data the function will cease
 * the reverse transversal
 * @param[in] tree The head of an initialized binary tree
 * @param[in] address The address to lookup in the tree - This should be in big endian order
 * @param[in] cidr The depth to transverse before we start back tracing and freeing
 * @param[in] data If this is set AND callback is set, the callback is used with the node data and
 * the supplied data to determine removal
 * @param[in] cb If set call the callback removal function with the node data pointer and the supplied data pointer
 */
void remove_node(struct route4tree *tree, __u32 address, __u8 cidr, void *data, callback_remove cb);

/**
 * @brief Frees an entire 32-bit binary tree using post-order traversal.
 *
 * @details This function traverses the entire tree starting from the given node
 * and frees all dynamically allocated memory for:
 *   - child nodes (`set` and `unset` pointers)
 *   - the `data` pointer stored at each node
 *   - the node itself
 *
 * The traversal is performed in a post-order manner (children first, then parent)
 * to ensure no node is freed before its children, avoiding use-after-free errors.
 * Parent pointers are used to explicitly walk back up the tree without recursion.
 *
 * After calling this function on the root of a tree, all memory associated with
 * that tree is released. The caller must not use any pointer to this tree or its
 * nodes after this function returns.
 *
 * @param[in] tree A double pointer to the root of a 32-bit binary tree (`struct route4tree`)
 *                 to be freed. If the inner pointer is NULL, the function does nothing.
 */
void free_tree4(struct route4tree **tree);


#endif //GENERICHELPER_LINKED_LIST_H
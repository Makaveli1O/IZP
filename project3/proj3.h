/**
 * @file proj3.h
 * @author Samuel Líška
 * @date December 6 2018
 * @brief Program demonstrates usage of cluster analysis
 * @detail Program loads file filled with object informations, such as ID, X-coord, Y-coord and number of objects
 * which are transformed into clusters using structures, functions that does numeric calculations with clusters, which
 * can be suited for your need depending on your input program arguments.
 */


/**
 * @mainpage
 * This program does cluster analysis method over imported file based on single linkage method.
 * User starts the program with 1 or 2 arguments. If only 1 argument has been used, program will act as
 * number 1 was inputed. 
 *
 * This program is using work with pointers, dynamic memory and especially understand the concept
 * of dynamic allocation in C language.
 *
 * @detail Starting formula: ./proj3 "file_name" "number_of_clusters"
 *
 * This software is open-source, free to use and modify. 
 */

/**
 * @struct obj_t
 * 
 * @brief Object structure with attributes of object, such as coords and ID
 *
 * @var obj_t::id 
 * Identification number of an object
 * @var obj_t::x
 * X coord of object
 * @var obj_t::y
 * Y coord of object
 */

struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * @struct cluster_t
 * @brief structure containing cluster
 * @var cluster_t::size
 * Actual size of cluster(number of items(object) it contains)
 * @var cluster_t::capacity
 * Eventual size of cluster (maximum number of items(objects) cluster can hold)
 * @var cluster_t::*obj
 * Pointer to array of objects
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/**
 * @defgroup ClusterOperations Operations over clusters
 * @brief Functions that does operations over clusters.
 *
 * @defgroup ClusterCalculations Calculations over clusters
 * @brief Functions that does calculations with clusters
 */



/**
 * @brief Initialization of cluster "c" with capacity of "cap"
 *
 * @param c pointer to actual cluster
 *
 * @param cap desired capacity of cluster(size of cluster)
 *
 * @pre
 * - "c" is not NULL
 * - "cap" is not negative
 *
 * @post
 * - "c" is initialized to "cap" objects
 *
 * @ingroup ClusterOperations
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Clearing cluster "c" of all objects and initialization to an empty cluster
 *
 * @param c is pointer to actual cluster
 * 
 * @pre
 * - "c" is not NULL
 *
 * @post
 * - each object from cluster "c" is removed. Initialized to en empty cluster.
 * @ingroup ClusterOperations
 */

void clear_cluster(struct cluster_t *c);

extern const int CLUSTER_CHUNK;

/**
 * @brief Resizing cluster "c" into desired size "new_cap"
 *
 * @param c is pointer to actual cluster
 *
 * @param new_cap is new size of cluster
 *
 * @pre
 * - "c" is not NULL
 * - "new_cap" is not negative
 *
 * @return
 * - Resized cluster to "new_cap" 
 * @ingroup ClusterOperations
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Appending cluster with object"obj_t"
 *
 * @param "c" is pointer to actual cluster
 *
 * @param "obj" is object that we want cluster to be appended with
 *
 * @pre
 * - "c" is not NULL
 * - "obj" is not NULL
 *
 * @post
 * Appends cluster "c" with object "obj", increases size with "resize_cluster" if necessary 
 * @ingroup ClusterOperations
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Merging two clusters togetether
 *
 * @param "c1" is pointer to first cluster
 *
 * @param "c2" is pointer to second cluster
 *
 * @pre
 * - "c1" is not NULL
 * - "c2" is not NULL
 *
 * @post
 * Merges two cluster "c1" and "c2" together, creating 1 cluster with objects of each
 * 
 * @ingroup ClusterOperations
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Removing cluster "c" from clusters array "carr"
 *
 * @param "carr" is pointer to actual clusters array
 *
 * @param "narr" is number of clusters in "carr"
 *
 * @param "idx" is index of desired cluster which you want to be removed
 *
 * @pre
 * - "idx" is not greater than "narr"
 * - "narr" is greater than 0
 * @return
 * Function returns number of clusters in array (after removing)
 * 
 * @ingroup ClusterOperations
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief Calculating distance between two clusters
 *
 * @param "o1" is pointer to first cluster
 *
 * @param "o2" is pointer to second cluster
 *
 * @pre
 * - "c1" is not NULL
 * - "c2" is not NULL
 * - "c1" size is greater than 0
 * - "c2" size is greater than 0
 *
 * @return
 * Function returns distance between cluster "c1" and cluster "c2"
 *
 * @ingroup ClusterCalculations
 */

float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Calculating distance between two clusters
 *
 * @param "c1" is pointer to first cluster structure
 *
 * @param "c2" is pointer to second cluster structure
 *
 * @pre
 * - "c1" is not NULL
 * - "c2" is not NULL
 * 
 * @return
 * Function returns floating number distance between two clusters "c1" and "c2"
 *
 * @ingroup ClusterCalculations
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Finding two closest clusters
 *
 * @param "carr" is pointer to an array of clusters
 *
 * @param "narr" is size of the "carr" array
 *
 * @param "c1" is pointer to first cluster structure
 *
 * @param "c2" is pointer to second cluster structure
 *
 * @pre
 * - "narr" is greater than 0
 * 
 * @post
 * Function is calculating distance between each cluster and finds the closest neighbour
 * 
 * @ingroup ClusterCalculations
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**@brief Sorting clusters by ID
 *
 * @param "c" is pointer to actual cluster
 *
 * @pre
 * - "c" is not NULL
 * 
 * @post
 * Sorts  objects in cluster "c" in ascending order, by their identification number
 *
 * @ingroup ClusterOperations
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Printing clusters
 *
 * @param "c" is pointer to actual cluster
 *
 * @pre
 * - "c" is not NULL
 * 
 * @post
 * Prints cluster "c" to STDOUT.
 *
 * @ingroup ClusterOperations
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Loading objects and creating clusters
 *
 * @param "filename" is name of the actual file, which we want object informations from
 *
 * @param "arr" is pointer to the beginning of the array of clusters
 *
 * @pre
 * - "arr" is not NULL
 * - file "filename" exists
 * - file is written exactly in specific format
 * - coords in file must meet requirements 0 < N < 1000
 *    			       
 * @post
 * Function loads objects from the file "filename", and creates cluster for each loaded object. Each object is saved into cluster array "arr"
 *
 * @return
 * Function returns number of loaded objects.
 *
 *@ingroup ClusterOperations
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief Prints multiple clusters to STDOUT.
 * 
 * @param "carr" is array of cluster.
 *
 * @param "narr" is desired number of clusters in "carr" array, to be printed.
 *
 * @pre
 * - "carr" is not NULL
 * - "narr" is greater than 0
 * 
 * @post
 * Specific information of "Narr" number of cluster from "carr" are printed to stdout.
 *@ingroup ClusterOperations
 */
void print_clusters(struct cluster_t *carr, int narr);

#ifndef PRIM_T
#define PRIM_T
#include "structures.h"

/**
 * @brief Applies Prim's algorithm to find the minimum spanning tree of a graph.
 *
 * This function uses Prim's algorithm to construct a minimum spanning tree
 * from a given graph. It starts from a specified starting vertex and iteratively
 * adds the minimum weight edges connecting visited vertices to unvisited vertices
 * until all vertices are visited.
 *
 * @param graphe The original graph represented by a Graphe structure.
 * @param arbreCouvrant The resulting graph representing the minimum spanning tree.
 * @param sommetDepart The name of the starting vertex from which the algorithm begins.
 *
 *
 * @warning If the starting vertex does not exist in the graph, the function prints an error
 *          message and returns without modifying the spanning tree.
 */
void Prim(Graphe* graphe, Graphe* arbreCouvrant, char* sommetDepart);

/**
 * @brief Calculates the minimum weight of the path between two nodes in a graph using recursion.
 *
 * This function recursively explores all possible paths from the source node to the destination node
 * and calculates the minimum weight of the path. It avoids revisiting nodes that have already been visited
 * to prevent infinite loops.
 *
 * @param graph Pointer to the graph structure.
 * @param src The name of the source node.
 * @param dest The name of the destination node.
 * @param visited A 2D array to keep track of visited nodes.
 * @param nbVisites The number of nodes visited so far.
 * @return The minimum weight of the path if a path exists, otherwise -1.
 */
int PoidDuCheminRec(Graphe* graph, char* src, char* dest, char visited[][50], int nbVisites);

/**
 * @brief Calculate the weight of the path between two nodes in a graph.
 *
 * This function initializes the visited nodes list and calls the recursive function
 * to calculate the weight of the path from the source node to the destination node.
 *
 * @param graph Pointer to the graph structure.
 * @param src The source node.
 * @param dest The destination node.
 * @return The weight of the path from src to dest.
 */
int PoidDuChemin(Graphe* graph, char* src, char* dest);

/**
 * @brief Checks if a given vertex exists in the graph.
 *
 * This function traverses the graph to determine if a vertex with the specified name exists.
 * It compares the given vertex name with the names of the vertices in the graph.
 *
 * @param graphe Pointer to the graph structure.
 * @param sommet The name of the vertex to check for existence.
 * @return int Returns 1 if the vertex exists, 0 otherwise.
 */
int sommetExiste(Graphe* graphe, const char* sommet);

/**
 * @brief Adds a new event to the graph.
 *
 * This function allocates memory for a new point, initializes its
 * values with the provided neighbor, and adds it to the head of the
 * graph's points list.
 *
 * @param graphe Pointer to the graph structure.
 * @param voisin Neighbor structure to add to the graph.
 *
 * @note In case of memory allocation failure, an error message is displayed
 *       and the function returns without modifying the graph.
 */
void ajouterEvenement(Graphe* graphe, Voisin voisin);
#endif
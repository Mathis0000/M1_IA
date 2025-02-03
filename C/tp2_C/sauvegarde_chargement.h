#ifndef SAUVEGARDE_T
#define SAUVEGARDE_T
#include "structures.h"

/**
 * @brief Loads a graph from a file.
 *
 * This function reads a file containing information about the edges of a graph
 * and adds them to the specified graph. The file should contain lines in the format:
 * "vertex_name -> neighbor_name : weight", where "vertex_name" and "neighbor_name" are strings
 * representing the vertices, and "weight" is an integer representing the weight of the edge.
 *
 * @param graphe A pointer to the Graphe structure where the edges will be added.
 * @param filename The path to the file to read.
 *
 * @note If the file cannot be opened, an error message is displayed.
 * @note If a line in the file does not follow the expected format, an error message is displayed
 *       and the line is ignored.
 * @note If an edge with a negative weight is detected, an error message is displayed
 *       and the edge is ignored.
 */
void chargerGraphe(Graphe* graphe, const char* filename);

/**
 * @brief Saves a graph to a file.
 *
 * This function takes a graph and a filename as input, and writes
 * the graph's information to the specified file. Each line of the file
 * contains information about a link between two points in the graph, in the format
 * "point_name -> link_name : weight".
 *
 * @param graphe A pointer to the graph to save.
 * @param filename The name of the file to save the graph in.
 */
void sauvegarderGraphe(Graphe* graphe, const char* filename);

/**
 * @brief Frees the memory allocated for a graph.
 *
 * This function frees the memory allocated for all the points in a graph
 * and sets the graph's head pointer to NULL.
 *
 * @param graphe A pointer to the graph to free.
 */
void libererGraphe(Graphe* graphe);

/**
 * @brief Creates a graph by manually adding edges.
 *
 * This function allows the user to create a graph by adding edges one by one.
 * The user must specify the number of edges to add, then enter each edge in the
 * format "A -> B : weight". Incorrect entries are handled, and the user is prompted
 * to retry until a valid entry is provided.
 *
 * @param graphe Pointer to the Graphe structure to modify.
 */
void creerGrapheManuellement(Graphe* graphe);

#endif
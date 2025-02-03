#ifndef AFFICHER_T
#define AFFICHER_T
#include "structures.h"

/**
 * @brief Displays the main menu of the application.
 *
 * This function displays the various options available to the user,
 * such as loading a graph, creating edges, saving a graph, generating
 * a spanning tree from the loaded graph, loading a tree, saving a tree,
 * calculating the distance of a point from the starting point of the tree,
 * displaying the graph, displaying the spanning tree, and quitting the application.
 * The user is prompted to choose an option by entering the corresponding number.
 */
void afficherMenu();

/**
 * @brief Affiche les liens et les poids des voisins dans un graphe.
 *
 * Cette fonction parcourt tous les points du graphe et affiche pour chacun
 * d'eux le nom du voisin, le nom du lien et le poids du lien.
 *
 * @param graphe Un pointeur vers la structure Graphe contenant les points à afficher.
 */
void afficherArbre(Graphe* graphe);
#endif
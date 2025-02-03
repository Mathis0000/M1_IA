#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * @struct Voisin
 * @brief Represents a neighbor with a name, link name, and weight.
 * 
 * This structure is used to store information about a neighbor in a graph or network.
 * 
 * @var Voisin::self
 * A character array representing the name of the current point.
 * 
 * @var Voisin::nom_lien
 * A character array representing the name of the neighbor.
 * 
 * @var Voisin::poids
 * An integer representing the weight of the link to the neighbor.
 */
typedef struct {
    char self[50];
    char nom_lien[50];
    int poids;
} Voisin;

typedef struct Points {
    Voisin voisin;
    struct Points* next;
} Points;

typedef struct {
    Points* head;
} Graphe;

#define MAX_SIZE 100
#endif // STRUCTURES_H

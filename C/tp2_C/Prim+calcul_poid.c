#include "Prim+calcul_poid.h"
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
void Prim(Graphe* graphe, Graphe* arbreCouvrant, char* sommetDepart) {
    char visites[MAX_SIZE][50]; // Liste des sommets visités
    int nbVisites = 0;
    int trouve = 1;

    // Vérifier si le sommet de départ existe dans le graphe
    Points* current = graphe->head;
    int sommetTrouve = 0;
    while (current != NULL) {
        if (strcmp(current->voisin.self, sommetDepart) == 0) {
            sommetTrouve = 1;
            break;
        }
        current = current->next;
    }

    if (!sommetTrouve) {
        printf("Le sommet de départ n'existe pas dans le graphe.\n");
        return;
    }

    strcpy(visites[nbVisites++], sommetDepart);

    while (trouve) {
        current = graphe->head;
        Voisin minVoisin;
        minVoisin.poids = INT_MAX; // Valeur maximale possible d'un int
        trouve = 0;

        // Trouver l'arête minimale connectant un sommet visité à un sommet non visité
        while (current != NULL) {
            int sommetVisite = 0;
            int sommetNonVisite = 1;

            // Vérifier si le sommet source est visité
            for (int i = 0; i < nbVisites; i++) {
                if (strcmp(current->voisin.self, visites[i]) == 0) {
                    sommetVisite = 1;
                    break;
                }
            }

            // Vérifier si le sommet destination est non visité
            for (int i = 0; i < nbVisites; i++) {
                if (strcmp(current->voisin.nom_lien, visites[i]) == 0) {
                    sommetNonVisite = 0;
                    break;
                }
            }

            // Si la condition est satisfaite, évaluer l'arête
            if (sommetVisite && sommetNonVisite && current->voisin.poids < minVoisin.poids) {
                minVoisin = current->voisin;
                trouve = 1;
            }

            current = current->next;
        }

        // Ajouter l'arête minimale à l'arbre couvrant
        if (trouve) {
            ajouterEvenement(arbreCouvrant, minVoisin);
            Voisin inverseVoisin;
            strcpy(inverseVoisin.self, minVoisin.nom_lien);
            strcpy(inverseVoisin.nom_lien, minVoisin.self);
            inverseVoisin.poids = minVoisin.poids;
            ajouterEvenement(arbreCouvrant, inverseVoisin); //permet de rester en non oriente
            strcpy(visites[nbVisites++], minVoisin.nom_lien);
        }
    }
}

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
int PoidDuCheminRec(Graphe* graph, char* src, char* dest, char visited[][50], int nbVisites) {
    if (strcmp(src, dest) == 0) {
        return 0;
    }

    Points* current = graph->head;
    int minWeight = INT_MAX;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->voisin.self, src) == 0) {
            int alreadyVisited = 0;
            for (int i = 0; i < nbVisites; i++) {
                if (strcmp(visited[i], current->voisin.nom_lien) == 0) {
                    alreadyVisited = 1;
                    break;
                }
            }

            if (!alreadyVisited) {
                strcpy(visited[nbVisites], current->voisin.nom_lien);
                int weight = PoidDuCheminRec(graph, current->voisin.nom_lien, dest, visited, nbVisites + 1);
                if (weight != -1) {
                    minWeight = current->voisin.poids + weight;
                    found = 1;
                }
            }
        }
        current = current->next;
    }

    return found ? minWeight : -1;
}

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
int PoidDuChemin(Graphe* graph, char* src, char* dest) {
    char visited[MAX_SIZE][50];
    int nbVisites = 0;
    strcpy(visited[nbVisites++], src);
    return PoidDuCheminRec(graph, src, dest, visited, nbVisites);
}


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
int sommetExiste(Graphe* graphe, const char* sommet) {
    Points* current = graphe->head;
    while (current != NULL) {
        if (strcmp(current->voisin.self, sommet) == 0 || strcmp(current->voisin.nom_lien, sommet) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

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
void ajouterEvenement(Graphe* graphe, Voisin voisin) {
    Points* newPoint = (Points*)malloc(sizeof(Points));
    if (newPoint == NULL) {
        perror("Erreur d'allocation de mémoire");
        return;
    }
    newPoint->voisin = voisin;
    newPoint->next = graphe->head;
    graphe->head = newPoint;
}
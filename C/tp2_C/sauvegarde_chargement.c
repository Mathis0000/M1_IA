#include "sauvegarde_chargement.h"
#include "./Prim+calcul_poid.h"

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
void chargerGraphe(Graphe* graphe, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Voisin voisin;
    char line[150];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%49s -> %49s : %f", voisin.self, voisin.nom_lien, &voisin.poids) != 3) {
            fprintf(stderr, "Erreur: format incorrect dans la ligne: %s", line);
            continue;
        }
        if (voisin.poids < 0) {
            fprintf(stderr, "Erreur: poids négatif détecté pour l'arête %s -> %s\n", voisin.self, voisin.nom_lien);
            continue;
        }
        ajouterEvenement(graphe, voisin);
    }

    fclose(file);
}

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
void sauvegarderGraphe(Graphe* graphe, const char* filename) {
    FILE* file = fopen(filename, "w+");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Points* current = graphe->head;
    while (current != NULL) {
        fprintf(file, "%s -> %s : %f\n", current->voisin.self, current->voisin.nom_lien, current->voisin.poids);
        current = current->next;
    }

    fclose(file);
}

/**
 * @brief Frees the memory allocated for a graph.
 *
 * This function frees the memory allocated for all the points in a graph
 * and sets the graph's head pointer to NULL.
 *
 * @param graphe A pointer to the graph to free.
 */
void libererGraphe(Graphe* graphe) {
    Points* current = graphe->head;
    Points* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    graphe->head = NULL;
}

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
void creerGrapheManuellement(Graphe* graphe) {
    int nbAretes;
    printf("Entrez le nombre d'arêtes à ajouter : (Une seul à ajouter, automatiquement en non orienté) ");
    while (scanf("%d", &nbAretes) != 1 || nbAretes < 0 || getchar() != '\n') {
        printf("Erreur: veuillez entrer un nombre entier positif.\n");
        printf("Entrez le nombre d'arêtes à ajouter: ");
        while (getchar() != '\n'); // Consommer les entrées incorrectes
    }

    for (int i = 0; i < nbAretes; i++) {
        Voisin voisin;
        int valid = 0;
        while (!valid) {
            printf("Entrez l'arête %d (format: A -> B : poids): ", i + 1);
            char input[150];
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%49s -> %49s : %f", voisin.self, voisin.nom_lien, &voisin.poids) == 3 && voisin.poids >= 0) {
                valid = 1;
            } else {
                printf("Erreur: format incorrect ou poids négatif. Veuillez réessayer.\n");
            }
        }
        ajouterEvenement(graphe, voisin);
        Voisin inverseVoisin;
        strcpy(inverseVoisin.self, voisin.nom_lien);
        strcpy(inverseVoisin.nom_lien, voisin.self);
        inverseVoisin.poids = voisin.poids;
        ajouterEvenement(graphe, inverseVoisin); //permet de rester en non oriente
    }
}
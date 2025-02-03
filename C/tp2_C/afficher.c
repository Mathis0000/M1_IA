#include "afficher.h"
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
void afficherMenu() {
    printf("Menu:\n");
    printf("1. Charger un graphe\n");
    printf("2. Créer des arêtes\n");
    printf("3. Sauvegarder un graphe\n");
    printf("4. Générer un arbre couvrant à partir du graphe chargé\n");
    printf("5. Charger un arbre\n");
    printf("6. Sauvegarder un arbre\n");
    printf("7. Distance d’un point par rapport au point de départ de l’arbre\n");
    printf("8. Afficher le graphe\n");
    printf("9. Afficher l'arbre couvrant\n");
    printf("10. Quitter\n");
    printf("Choisissez une option: ");
}

/**
 * @brief Affiche les liens et les poids des voisins dans un graphe.
 *
 * Cette fonction parcourt tous les points du graphe et affiche pour chacun
 * d'eux le nom du voisin, le nom du lien et le poids du lien.
 *
 * @param graphe Un pointeur vers la structure Graphe contenant les points à afficher.
 */
void afficherArbre(Graphe* graphe) {
    Points* current = graphe->head;
    while (current != NULL) {
        printf("%s -> %s : %d\n", current->voisin.self, current->voisin.nom_lien, current->voisin.poids);
        current = current->next;
    }
}
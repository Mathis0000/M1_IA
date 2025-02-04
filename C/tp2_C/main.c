/**
 * @brief Main program for managing and displaying graphs and spanning trees.
 *
 * This program allows loading, saving, manually creating graphs, 
 * generating spanning trees from graphs, and calculating distances 
 * between vertices in a spanning tree. It provides an interactive menu 
 * to perform these operations.
 *
 * @details
 * The main features include:
 * - Load a graph from a file.
 * - Manually create a graph.
 * - Save a graph to a file.
 * - Generate a minimum spanning tree from a graph using Prim's algorithm.
 * - Load a spanning tree from a file.
 * - Save a spanning tree to a file.
 * - Calculate the distance between two vertices in a spanning tree.
 * - Display the graph or the spanning tree.
 */
#include "./structures.h"
#include "./sauvegarde_chargement.h"
#include "./Prim+calcul_poid.h"
#include "./afficher.h"

int main() {
    Graphe graphe = {NULL};
    Graphe arbreCouvrant = {NULL};

    int choix;
    char filename[50];
    char sommetDepart[50];
    char sommetArriver[50];

    do {
        afficherMenu();
        scanf("%d", &choix);
        getchar(); // Consommer le '\n' restant

        switch (choix) {
            case 1: 
                libererGraphe(&graphe);
                printf("Entrez le nom du fichier à charger: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                chargerGraphe(&graphe, filename);
                afficherArbre(&graphe);
                break;

            case 2:
                creerGrapheManuellement(&graphe);
                break;

            case 3:
                if (graphe.head == NULL) {
                    printf("Le graphe est vide et ne peut pas être sauvegardé.\n");
                } else {
                    printf("Entrez le nom du fichier pour sauvegarder le graphe: ");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = '\0';
                    sauvegarderGraphe(&graphe, filename);
                }
                break;

            case 4:
                libererGraphe(&arbreCouvrant);
                printf("Entrez le sommet de départ: ");
                fgets(sommetDepart, sizeof(sommetDepart), stdin);
                sommetDepart[strcspn(sommetDepart, "\n")] = '\0';
                Prim(&graphe, &arbreCouvrant, sommetDepart);
                afficherArbre(&arbreCouvrant);
                break;

            case 5:
                libererGraphe(&arbreCouvrant);
                printf("Entrez le nom du fichier à charger pour l'arbre: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                chargerGraphe(&arbreCouvrant, filename);
                afficherArbre(&arbreCouvrant);
                break;

            case 6:
                 if (arbreCouvrant.head == NULL) {
                    printf("L'arbre couvrant n'a pas encore été généré. Veuillez générer l'arbre couvrant d'abord.\n");
                } else {
                    printf("Entrez le nom du fichier pour sauvegarder l'arbre: ");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = '\0';
                    sauvegarderGraphe(&arbreCouvrant, filename);
                }
                break;

            case 7:
                if (arbreCouvrant.head == NULL) {
                    printf("L'arbre couvrant n'a pas encore été généré. Veuillez générer l'arbre couvrant d'abord.\n");
                } else {
                    do {
                        printf("Entrez le sommet de départ pour calculer les distances: ");
                        fgets(sommetDepart, sizeof(sommetDepart), stdin);
                        sommetDepart[strcspn(sommetDepart, "\n")] = '\0';
                        if (!sommetExiste(&arbreCouvrant, sommetDepart)) {
                            printf("Le sommet de départ '%s' n'existe pas dans l'arbre couvrant. Veuillez réessayer.\n", sommetDepart);
                        }
                    } while (!sommetExiste(&arbreCouvrant, sommetDepart));
                    
                    do {
                        printf("Entrez le sommet d'arrivée pour calculer les distances: ");
                        fgets(sommetArriver, sizeof(sommetArriver), stdin);
                        sommetArriver[strcspn(sommetArriver, "\n")] = '\0';
                        if (!sommetExiste(&arbreCouvrant, sommetArriver)) {
                            printf("Le sommet d'arrivée '%s' n'existe pas dans l'arbre couvrant. Veuillez réessayer.\n", sommetArriver);
                        }
                    } while (!sommetExiste(&arbreCouvrant, sommetArriver));

                    float distanceTotale = PoidDuChemin(&arbreCouvrant, sommetDepart, sommetArriver);

                    if (distanceTotale != -1) {
                        printf("Le chemin entre %s et %s est : %f\n", sommetDepart, sommetArriver, distanceTotale);
                    }
                }
                break;

            case 8:
                 if (graphe.head == NULL) {
                    printf("Le graphe n'a pas encore été généré.\n");
                } else{
                    afficherArbre(&graphe);
                }                
                break;

            case 9:
                 if (arbreCouvrant.head == NULL) {
                    printf("L'arbre couvrant n'a pas encore été généré. Veuillez générer l'arbre couvrant d'abord.\n");
                } else{
                    afficherArbre(&arbreCouvrant);
                }
                break;

            case 10:
                printf("Quitter.\n");
                break;

            default:
                printf("Option invalide. Veuillez réessayer.\n");
        }
    } while (choix != 10);

    libererGraphe(&graphe);
    libererGraphe(&arbreCouvrant);

    return 0;
}
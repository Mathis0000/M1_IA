#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

void chargerGraphe(Graphe* graphe, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Voisin voisin;
    while (fscanf(file, "%49s -> %49s : %d", voisin.self, voisin.nom_lien, &voisin.poids) == 3) {
        if (voisin.poids < 0) {
            fprintf(stderr, "Erreur: poids négatif détecté pour l'arête %s -> %s\n", voisin.self, voisin.nom_lien);
            continue;
        }
        ajouterEvenement(graphe, voisin);
    }

    fclose(file);
}

void sauvegarderGraphe(Graphe* graphe, const char* filename) {
    FILE* file = fopen(filename, "w+");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Points* current = graphe->head;
    while (current != NULL) {
        fprintf(file, "%s -> %s : %d\n", current->voisin.self, current->voisin.nom_lien, current->voisin.poids);
        current = current->next;
    }

    fclose(file);
}

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

void afficherMenu() {
    printf("Menu:\n");
    printf("1. Charger un graphe\n");
    printf("2. Créer des arêtes\n");
    printf("3. Sauvegarder un graphe\n");
    printf("4. Générer un arbre couvrant à partir du graphe chargé\n");
    printf("5. Charger un arbre\n");
    printf("6. Sauvegarder un arbre\n");
    printf("7. Distance d’un point par rapport au point de départ de l’arbre\n");
    printf("8. Quitter\n");
    printf("Choisissez une option: ");
}

void afficherArbre(Graphe* graphe) {
    Points* current = graphe->head;
    while (current != NULL) {
        printf("%s -> %s : %d\n", current->voisin.self, current->voisin.nom_lien, current->voisin.poids);
        current = current->next;
    }
}

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
            strcpy(visites[nbVisites++], minVoisin.nom_lien);
        }
    }
}


int ComptePoints(Graphe* graphe) {
    int nbPoints = 0;
    Points* current = graphe->head;
    while (current != NULL) {
        nbPoints++;
        current = current->next;
    }
    return nbPoints;
}

int findPathWeight(Graphe* graph, char* src, char* dest, int* visited, int* path, int* pathIndex) {
    Points* current = graph->head;
    while (current != NULL) {
        if (strcmp(current->voisin.self, src) == 0 && !visited[*pathIndex]) {
            visited[*pathIndex] = 1;
            path[*pathIndex] = current->voisin.poids;
            (*pathIndex)++;

            if (strcmp(current->voisin.nom_lien, dest) == 0) {
                return current->voisin.poids;
            }

            int weight = current->voisin.poids + findPathWeight(graph, current->voisin.nom_lien, dest, visited, path, pathIndex);
            if (weight > 0) {
                return weight;
            }
        }
        current = current->next;
    }

    (*pathIndex)--;
    return -1;
}

void creerGrapheManuellement(Graphe* graphe) {
    int nbAretes;
    printf("Entrez le nombre d'arêtes à ajouter: ");
    scanf("%d", &nbAretes);
    getchar(); // Consommer le '\n' restant

    for (int i = 0; i < nbAretes; i++) {
        Voisin voisin;
        do {
            printf("Entrez l'arête %d (format: A -> B : poids): ", i + 1);
            scanf("%49s -> %49s : %d", voisin.self, voisin.nom_lien, &voisin.poids);
            getchar(); // Consommer le '\n' restant
            if (voisin.poids < 0) {
                printf("Erreur: le poids ne peut pas être négatif. Veuillez réessayer.\n");
            }
        } while (voisin.poids < 0);
        ajouterEvenement(graphe, voisin);
    }
}

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
                printf("Entrez le nom du fichier pour sauvegarder le graphe: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                sauvegarderGraphe(&graphe, filename);
                break;

            case 4:
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
                printf("Entrez le nom du fichier pour sauvegarder l'arbre: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                sauvegarderGraphe(&arbreCouvrant, filename);
                break;

            case 7:
                if (arbreCouvrant.head == NULL) {
                    printf("L'arbre couvrant n'a pas encore été généré. Veuillez générer l'arbre couvrant d'abord.\n");
                } else {
                    printf("Entrez le sommet de départ pour calculer les distances: ");
                    fgets(sommetDepart, sizeof(sommetDepart), stdin);
                    sommetDepart[strcspn(sommetDepart, "\n")] = '\0';

                    printf("Entrez le sommet d'arrivée pour calculer les distances: ");
                    fgets(sommetArriver, sizeof(sommetArriver), stdin);
                    sommetArriver[strcspn(sommetArriver, "\n")] = '\0';

                    int V = ComptePoints(&arbreCouvrant);
                    int visited[V];
                    int path[V];
                    int pathIndex = 0;

                    for (int i = 0; i < V; i++) {
                        visited[i] = 0;
                    }


                    ///////pourquoi marche uniquement avec charge???
                    sauvegarderGraphe(&arbreCouvrant, "filename.txt");
                    chargerGraphe(&arbreCouvrant, "filename.txt");
                    int distanceTotale = findPathWeight(&arbreCouvrant, sommetDepart, sommetArriver, visited, path, &pathIndex);

                    if (distanceTotale != -1) {
                        printf("Le chemin entre %s et %s est : %d\n", sommetDepart, sommetArriver, distanceTotale);
                    }
                }
                break;

            case 8:
                printf("Quitter.\n");
                break;

            default:
                printf("Option invalide. Veuillez réessayer.\n");
        }
    } while (choix != 8);

    libererGraphe(&graphe);
    libererGraphe(&arbreCouvrant);

    return 0;
}

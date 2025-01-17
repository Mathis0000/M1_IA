#include "afficher.h"

/**
 * @brief Displays the menu to the user.
 *
 * This function is responsible for presenting the menu options to the user.
 * It does not take any parameters and does not return any value.
 */
void afficherMenu() {
    printf("\033[1;34m+----------------------------+\033[0m\n");
    printf("\033[1;34m|             \033[1;34mMenu            |\033[0m\n");
    printf("\033[1;34m+----------------------------+\033[0m\n");
    printf("\033[1;32m| 1. Ajouter un evenement     |\033[0m\n");
    printf("\033[1;32m| 2. Supprimer un evenement   |\033[0m\n");
    printf("\033[1;32m| 3. Sauvegarder l'agenda     |\033[0m\n");
    printf("\033[1;32m| 4. Charger un agenda        |\033[0m\n");
    printf("\033[1;32m| 5. Afficher l'agenda        |\033[0m\n");
    printf("\033[1;31m| 6. Quitter                  |\033[0m\n");
    printf("\033[1;34m+----------------------------+\033[0m\n");
}


/**
 * @brief Displays the agenda with index.
 *
 * This function takes a pointer to an Agenda structure and prints out
 * each entry in the agenda along with a corresponding number.
 *
 * @param agenda A pointer to the Agenda structure to be displayed.
 */
void afficherAgendaAvecNumeros(Agenda* agenda) {
    Node* current = agenda->head;
    int count = 1;
    while (current != NULL) {
        printf("%d.%s %02d-%02d-%04d %02d:%02d %s\n",
            count,
            current->evenement.titre,
            current->evenement.jour,
            current->evenement.mois,
            current->evenement.annee,
            current->evenement.heure,
            current->evenement.minute,
            current->evenement.commentaire);
        current = current->next;
        count++;
    }
}


/**
 * @brief Displays the contents of the given agenda.
 * 
 * This function takes a pointer to an Agenda structure and prints out
 * its contents.
 * 
 * @param agenda A pointer to the Agenda structure to be displayed.
 */
void afficherAgenda(Agenda* agenda) {
    Node* current = agenda->head;
    while (current != NULL) {
        printf("%s %02d-%02d-%04d %02d:%02d %s\n",
                current->evenement.titre,
                current->evenement.jour,
                current->evenement.mois,
                current->evenement.annee,
                current->evenement.heure,
                current->evenement.minute,
                current->evenement.commentaire);
                current = current->next;
    }
}
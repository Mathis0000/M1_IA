#include "afficher.h"

/**
 * @brief Displays the menu to the user.
 *
 * This function is responsible for presenting the menu options to the user.
 * It does not take any parameters and does not return any value.
 */
void afficherMenu() {
    printf("Menu:\n");
    printf("1. Ajouter un evenement\n");
    printf("2. Supprimer un evenement\n");
    printf("3. Sauvegarder l'agenda\n");
    printf("4. Charger un agenda\n");
    printf("5. Afficher l'agenda\n");
    printf("6. Quitter\n");
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
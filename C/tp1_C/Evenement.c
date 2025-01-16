#include "Evenement.h"

/**
 * Adds an event to the agenda.
 * 
 * This function inserts a new event into the specified agenda.
 * 
 * @param agenda A pointer to the Agenda where the event will be added.
 * @param evenement The event to be added to the agenda.
 */
void ajouterEvenement(Agenda* agenda, Evenement evenement) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erreur d'allocation de mémoire");
        return;
    }
    //creation nouveau noeud
    newNode->evenement = evenement;
    newNode->next = NULL;
    if (agenda->head == NULL) {
        agenda->head = newNode;
    } else {
        Node* current = agenda->head;
        Node* prev = NULL;

        while (current != NULL && comparerEvenements(&current->evenement, &evenement) < 0) {
            prev = current;
            current = current->next;
        }

        if (prev == NULL) {
        newNode->next = agenda->head;
        agenda->head = newNode;
        } else {
            prev->next = newNode;
            newNode->next = current;
        }
    }
}


 /**
* Compares two events based on their date and time.
*
* This function compares two events by their year, month, day, hour, and minute.
* It returns a negative value if the first event is earlier than the second event,
* a positive value if the first event is later than the second event, and zero if
* both events are at the same date and time.
*
* @param e1 Pointer to the first event.
* @param e2 Pointer to the second event.
* @return An integer indicating the comparison result:
*         - Negative value if e1 is earlier than e2.
*         - Positive value if e1 is later than e2.
*         - Zero if e1 and e2 are at the same date and time.
*/
int comparerEvenements(Evenement* e1, Evenement* e2) {
    if (e1->annee != e2->annee) return e1->annee - e2->annee;
    if (e1->mois != e2->mois) return e1->mois - e2->mois;
    if (e1->jour != e2->jour) return e1->jour - e2->jour;
    if (e1->heure != e2->heure) return e1->heure - e2->heure;
    return e1->minute - e2->minute;
}


/**
 * Loads events from a file into an agenda.
 *
 * This function opens a file containing events, reads each event, and adds it to the provided agenda.
 * The file should have events in the following format:
 * 
 *     title day/month/year hour/minute comment
 *
 * @param agenda A pointer to the Agenda structure where events will be loaded.
 * @param filename The name of the file containing the events.
 */
void chargerAgenda(Agenda* agenda, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Evenement evenement;
    while (fscanf(file, "%s %d/%d/%d %d/%d %[^\n]\n",
                evenement.titre,
                &evenement.jour,
                &evenement.mois,
                &evenement.annee,
                &evenement.heure,
                &evenement.minute,
                evenement.commentaire) != EOF) {
        ajouterEvenement(agenda, evenement);
    }

    fclose(file);
}


/**
 * Returns the number of days in a given month of a specific year.
 *
 * This function calculates the number of days in a specified month and year.
 * It takes into account leap years for the month of February.
 *
 * @param mois The month for which the number of days is to be determined (1 = January, 2 = February, ..., 12 = December).
 * @param annee The year for which the number of days is to be determined.
 * @return The number of days in the specified month and year.
 */
int joursDansMois(int mois, int annee) {
    if (mois < 1 || mois > 12) return 0;
    if (mois == 2) {
        // Février
        return (annee % 4 == 0 && (annee % 100 != 0 || annee % 400 == 0)) ? 29 : 28;
    } else if (mois == 4 || mois == 6 || mois == 9 || mois == 11) {
        // Avril, Juin, Septembre, Novembre
        return 30;
    } else {
        // Janvier, Mars, Mai, Juillet, Août, Octobre, Décembre
        return 31;
    }
}


/**
 * Frees all the memory allocated for the agenda.
 *
 * This function iterates through all the nodes in the agenda and frees the memory
 * allocated for each node. After all nodes are freed, the head of the agenda is set to NULL.
 *
 * @param agenda A pointer to the Agenda structure to be freed.
 */
void libererAgenda(Agenda* agenda) {
    Node* current = agenda->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    agenda->head = NULL;
}


/**
 * Saves the agenda to a specified file.
 *
 * This function takes an agenda and a filename as input and writes the agenda's
 * events to the specified file in a formatted manner. Each event is written
 * in the format: "title dd/mm/yyyy hh/mm comment".
 *
 * @param agenda Pointer to the Agenda structure containing the events to be saved.
 * @param filename The name of the file where the agenda will be saved.
 */
void sauvegarderAgenda(Agenda* agenda, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Node* current = agenda->head;
    while (current != NULL) {
        fprintf(file, "%s %02d/%02d/%04d %02d/%02d %s\n",
                current->evenement.titre,
                current->evenement.jour,
                current->evenement.mois,
                current->evenement.annee,
                current->evenement.heure,
                current->evenement.minute,
                current->evenement.commentaire);
        current = current->next;
    }

    fclose(file);
}


/**
 * Removes an event from the agenda based on its number.
 *
 * This function traverses the linked list of events in the agenda and removes
 * the event that corresponds to the given number. If the event number is invalid,
 * it prints an error message.
 *
 * @param agenda Pointer to the Agenda structure.
 * @param numero The number of the event to be removed.
 */
void supprimerEvenement(Agenda* agenda, int numero) {
    Node* current = agenda->head;
    Node* prev = NULL;
    int count = 1;

    while (current != NULL && count != numero) {
        prev = current;
        current = current->next;
        count++;
    }

    if (current == NULL) {
        printf("Numero d'evenement invalide.\n");
        return;
    }

    if (prev == NULL) {
        agenda->head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
}

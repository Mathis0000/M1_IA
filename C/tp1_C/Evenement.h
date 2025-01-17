#ifndef AJOUTEREVENEMENT_T
#define AJOUTEREVENEMENT_T

#include "structures.h"

/**
 * Adds an event to the agenda.
 * 
 * This function inserts a new event into the specified agenda.
 * 
 * @param agenda A pointer to the Agenda where the event will be added.
 * @param evenement The event to be added to the agenda.
 */
void ajouterEvenement(Agenda* agenda, Evenement evenement);

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
int comparerEvenements(Evenement* e1, Evenement* e2);


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
void chargerAgenda(Agenda* agenda, const char* filename);


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
int joursDansMois(int mois, int annee);


/**
 * Frees all the memory allocated for the agenda.
 *
 * This function iterates through all the nodes in the agenda and frees the memory
 * allocated for each node. After all nodes are freed, the head of the agenda is set to NULL.
 *
 * @param agenda A pointer to the Agenda structure to be freed.
 */
void libererAgenda(Agenda* agenda);


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
 void sauvegarderAgenda(Agenda* agenda, const char* filename);


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
 void supprimerEvenement(Agenda* agenda, int numero);


/**
 * Gets the current date and stores it in the provided variables.
 *
 * This function retrieves the current date from the system and stores it in the
 * variables passed as parameters. The values for day, month, and year are
 * updated to reflect the current date.
 *
 * @param jour Pointer to an integer where the current day will be stored.
 * @param mois Pointer to an integer where the current month will be stored.
 * @param annee Pointer to an integer where the current year will be stored.
 */
 void obtenirDateActuelle(int *jour, int *mois, int *annee);


#endif
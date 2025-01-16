#ifndef AFFICHERMENU_T
#define AFFICHERMENU_T
#include "structures.h"

/**
 * @brief Displays the menu to the user.
 *
 * This function is responsible for presenting the menu options to the user.
 * It does not take any parameters and does not return any value.
 */
void afficherMenu();


/**
 * @brief Displays the agenda with index.
 *
 * This function takes a pointer to an Agenda structure and prints out
 * each entry in the agenda along with a corresponding number.
 *
 * @param agenda A pointer to the Agenda structure to be displayed.
 */
void afficherAgendaAvecNumeros(Agenda* agenda);


/**
 * @brief Displays the contents of the given agenda.
 * 
 * This function takes a pointer to an Agenda structure and prints out
 * its contents.
 * 
 * @param agenda A pointer to the Agenda structure to be displayed.
 */
void afficherAgenda(Agenda* agenda);

#endif
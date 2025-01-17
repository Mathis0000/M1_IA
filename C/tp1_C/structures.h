/**
 * @struct Evenement
 *Structure representing an event.
 * 
 * This structure is used to store information about an event, including its title,
 * date, time, and a comment.
 * 
 * @var Evenement::titre
 * Title of the event. It is a string with a maximum length of 30 characters.
 * 
 * @var Evenement::jour
 * Day of the event. It is an integer representing the day of the month.
 * 
 * @var Evenement::mois
 * Month of the event. It is an integer representing the month of the year.
 * 
 * @var Evenement::annee
 * Year of the event. It is an integer representing the year.
 * 
 * @var Evenement::heure
 * Hour of the event. It is an integer representing the hour of the day (24-hour format).
 * 
 * @var Evenement::minute
 * Minute of the event. It is an integer representing the minute of the hour.
 * 
 * @var Evenement::commentaire
 * Comment about the event. It is a string with a maximum length of 256 characters.
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char titre[30];
    int jour;
    int mois;
    int annee;
    int heure;
    int minute;
    char commentaire[256];
} Evenement;

typedef struct Node {
    Evenement evenement;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} Agenda;

#endif // STRUCTURES_H

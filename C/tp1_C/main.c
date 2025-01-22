/**
 * Main program for managing a schedule.
 *
 * This file contains the entry point of the program and the main loop
 * for managing a schedule. Users can add, delete, display, save, and load events.
 */

#include "./Evenement.h"
#include "./afficher.h"
#include "./structures.h"

int main() {
    Agenda agenda;
    agenda.head = NULL;

    int choix;
    Evenement evenement;
    char nom_fichier[256];
    int numero;

    while (1) {
        do {
            afficherMenu();
            printf("Choisissez une option: ");
            if (scanf("%d", &choix) != 1) {
            printf("Entrée invalide. Veuillez entrer un numéro valide.\n");
            while (getchar() != '\n'); // Clear the input buffer
            } else {
            break;
            }
        } while (1);

        switch (choix) {
            case 1: {
                int result_mois;
                int jourActuel, moisActuel, anneeActuelle;
                obtenirDateActuelle(&jourActuel, &moisActuel, &anneeActuelle);

                do {
                    printf("Entrez la date (JJ/MM/AAAA): ");
                    result_mois = scanf("%d/%d/%d", &evenement.jour, &evenement.mois, &evenement.annee);

                    if (result_mois != 3) {
                        printf("Format de date invalide. Veuillez réessayer.\n");
                        while (getchar() != '\n'); // Clear the input buffer
                        continue;
                    }

                    int joursMax = joursDansMois(evenement.mois, evenement.annee);
                    if (evenement.jour < 1 || evenement.jour > joursMax || evenement.mois < 1 || evenement.mois > 12 || evenement.annee < 2025 || evenement.annee > 2100) {
                        printf("Date invalide. Veuillez réessayer.\n");
                        continue;
                    }

                    // Vérifier si la date est déjà révolue
                    if (evenement.annee < anneeActuelle ||
                        (evenement.annee == anneeActuelle && evenement.mois < moisActuel) ||
                        (evenement.annee == anneeActuelle && evenement.mois == moisActuel && evenement.jour <= jourActuel)) {
                        printf("La date est déjà révolue. Veuillez entrer une date future.\n");
                        continue;
                    }

                    break;
                } while (1);

                int result_heure;
                do {
                    printf("Entrez l'heure (HH/MM): ");
                    result_heure = scanf("%d/%d", &evenement.heure, &evenement.minute);

                    if (result_heure != 2 || evenement.heure < 0 || evenement.heure > 23 || evenement.minute < 0 || evenement.minute > 59) {
                        printf("Heure ou minute invalide. Veuillez réessayer.\n");
                        while (getchar() != '\n'); // Clear the input buffer
                    }
                } while (result_heure != 2 || evenement.heure < 0 || evenement.heure > 23 || evenement.minute < 0 || evenement.minute > 59);

                // verifie que taille inferieur a 30
                while (getchar() != '\n');

                do {
                    printf("Entrez le titre (max 30 caractères): ");
                    fgets(evenement.titre, sizeof(evenement.titre), stdin);

                    // Vérifier si la chaîne a été tronquée (pas de '\n' dans le tampon)
                    if (evenement.titre[strlen(evenement.titre) - 1] != '\n') {
                        printf("Le titre est trop long. Veuillez entrer un titre de 30 caractères maximum.\n");

                        // Vider le tampon pour éviter des conflits
                        while (getchar() != '\n');
                        continue;
                    }

                    // Supprimer le caractère de nouvelle ligne si présent
                    evenement.titre[strcspn(evenement.titre, "\n")] = 0;

                    if (strlen(evenement.titre) > 30) {
                        printf("Le titre est trop long. Veuillez entrer un titre de 30 caractères maximum.\n");
                        continue;
                    }

                    break;
                } while (1);

                
                // verifie que taille inferieur a 256
                do {
                    printf("Entrez le commentaire: ");
                    fgets(evenement.commentaire, sizeof(evenement.commentaire), stdin);

                    // Vérifier si la chaîne a été tronquée (pas de '\n' dans le tampon)
                    if (evenement.commentaire[strlen(evenement.commentaire) - 1] != '\n') {
                        printf("Le commentaire est trop long. Veuillez entrer un commentaire de 256 caractères maximum.\n");

                        // Vider le tampon pour éviter des conflits
                        while (getchar() != '\n');
                        continue;
                    }

                    // Supprimer le caractère de nouvelle ligne si présent
                    evenement.commentaire[strcspn(evenement.commentaire, "\n")] = 0;

                    if (strlen(evenement.titre) > 30) {
                        printf("Le commentaire est trop long. Veuillez entrer un commentaire de 256 caractères maximum.\n");
                        continue;
                    }

                    break;
                } while (1);

                ajouterEvenement(&agenda, evenement);
                break;
            }
            case 2:
                afficherAgendaAvecNumeros(&agenda);
                printf("Entrez le numero de l'evenement a supprimer: ");
                if (scanf("%d", &numero) != 1) {
                    printf("Entrée invalide. Veuillez entrer un numéro valide.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    break;
                }
                supprimerEvenement(&agenda, numero);
                break;
            case 3:
                printf("Entrez le nom du fichier: ");
                scanf("%s", nom_fichier);
                sauvegarderAgenda(&agenda, nom_fichier);
                break;
            case 4:
                libererAgenda(&agenda);
                printf("Entrez le nom du fichier: ");
                scanf("%s", nom_fichier);
                chargerAgenda(&agenda, nom_fichier);
                break;
            case 5:
                afficherAgenda(&agenda);
                break;
            case 6:
                libererAgenda(&agenda);
                return 0;
            default:
                printf("Option invalide.\n");
        }
    }

    return 0;
}
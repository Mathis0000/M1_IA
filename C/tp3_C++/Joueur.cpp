#include "Joueur.h"
#include "Potion.h"
#include "Arme.h"
#include "Armure.h"
#include "AttaqueObjet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <limits>

/**
 * @brief Construct a new Joueur object
 * 
 * @param n The name of the player
 * @param pv The player's health points
 * @param vap The player's attack power
 * @param vam The player's magic power
 * @param dp The player's defense points
 * @param dm The player's magic defense points
 */
Joueur::Joueur(const std::string& n, int pv, int vap, int vam, int dp, int dm)
    : Personnage(n, pv, vap, vam, dp, dm) {}

/**
 * @brief Engage in a battle with an enemy character.
 *
 * This function simulates a turn-based battle between the player and an enemy character.
 * The player and the enemy take turns to attack each other until one of them is defeated.
 *
 * @param ennemi Reference to the enemy character.
 * @param attaques A vector of available attacks for the enemy.
 * @param nomAleatoire The name of the enemy character.
 */
void Joueur::rencontrerEnnemi(Personnage& ennemi, const std::vector<Attaque>& attaques, std::string nomAleatoire) {
    bool tourJoueur = rand() % 2 == 0;
    while (getPointsDeVie() > 0 && ennemi.getPointsDeVie() > 0) {
        if (tourJoueur) {
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "C'est votre tour!" << std::endl;
            std::vector<Attaque> attaquesDisponibles = getAttaquesDisponibles();
            if (attaquesDisponibles.empty()) {
                std::cout << "Vous n'avez aucune attaque disponible!" << std::endl;
                break;
            }
            std::cout << "Choisissez une attaque :" << std::endl;
            for (size_t i = 0; i < attaquesDisponibles.size(); ++i) {
                std::cout << i + 1 << ". " << attaquesDisponibles[i].nom
                          << ", Attaque Magique : " << attaquesDisponibles[i].degatMagique
                          << ", Attaque Physique : " << attaquesDisponibles[i].degatPhysique << std::endl;
            }
            int choix;
            while (true) {
                std::cin >> choix;
        
                // Vérifie si l'entrée est un entier valide
                if (std::cin.fail()) {
                    // Effacer l'état d'erreur de cin
                    std::cin.clear();
                    // Ignorer l'entrée incorrecte
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Entrée invalide. Veuillez entrer un int." << std::endl;
                }
                else {
                    break;
                }
                
            } 
            if (choix > 0 && static_cast<size_t>(choix) <= attaquesDisponibles.size()) {
                attaquer(ennemi, attaquesDisponibles[choix - 1]);
            } else {
                std::cout << "Choix invalide! Tour perdu." << std::endl;
            }
        } else {
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "C'est le tour du " << nomAleatoire << " !" << std::endl;
            if (!attaques.empty()) {
                int attaqueAleatoire = rand() % attaques.size();
                ennemi.attaquer(*this, attaques[attaqueAleatoire]);
            }
        }
        tourJoueur = !tourJoueur;

        // Vérifier si le joueur a été vaincu
        if (getPointsDeVie() <= 0) {
            std::cout << "Vous avez été vaincu!" << std::endl;
            auto& inventaire = getInventaire();
            auto it = std::find_if(inventaire.begin(), inventaire.end(), [](const std::shared_ptr<Objet>& obj) {
                return std::dynamic_pointer_cast<Potion>(obj) != nullptr;
            });
            if (it != inventaire.end()) {
                (*it)->utiliser(*this);
            }
        }

        // Vérifier si l'ennemi a été vaincu
        if (ennemi.getPointsDeVie() <= 0) {
            std::cout << "Vous avez vaincu le " << nomAleatoire << " !" << std::endl;
            break;
        }
    }
}

/**
 * @brief Handles the encounter with an object.
 * 
 * This function is called when the player encounters an object. It adds the encountered object
 * to the player's collection of objects.
 * 
 * @param objet A shared pointer to the object that the player encounters.
 */
void Joueur::rencontrerObjet(std::shared_ptr<Objet> objet) {
    ajouterObjet(objet);
}
/**
 * @brief Clears the inventory of the player.
 * 
 * This function removes all items from the player's inventory,
 * effectively resetting it to an empty state.
 */
void Joueur::clearInventaire() {
    inventaire.clear();
}

/**
 * @brief Retrieves the list of available attacks from the player's inventory.
 *
 * This function iterates through the player's inventory and collects all the 
 * attacks that are available. It checks if each item in the inventory is of 
 * type `AttaqueObjet` and, if so, adds its associated attack to the list of 
 * available attacks.
 *
 * @return A vector containing all the available attacks (`std::vector<Attaque>`).
 */
std::vector<Attaque> Joueur::getAttaquesDisponibles() const {
    std::vector<Attaque> attaquesDisponibles;
    for (const auto& objet : inventaire) {
        if (auto attaqueObjet = std::dynamic_pointer_cast<AttaqueObjet>(objet)) {
            attaquesDisponibles.push_back(attaqueObjet->attaque);
        }
    }
    return attaquesDisponibles;
}

/**
 * @brief Loads a list of attacks from a file.
 *
 * This function reads a text file containing information about attacks,
 * and returns a list of Attaque objects. Each line of the file should be in the format:
 * "name/magicDamage/physicalDamage".
 *
 * @param fichier The path to the file containing the attacks.
 * @return A vector of Attaque objects loaded from the file.
 */
std::vector<Attaque> chargerAttaques(const std::string& fichier) {
    std::vector<Attaque> attaques;
    std::ifstream in(fichier);
    if (!in) {
        std::cerr << "Erreur : Impossible de charger les attaques." << std::endl;
        return attaques;
    }
    std::string ligne;
    while (std::getline(in, ligne)) {
        std::istringstream iss(ligne);
        std::string nom;
        int degatMagique, degatPhysique;
        std::getline(iss, nom, '/');
        iss >> degatMagique;
        iss.ignore();
        iss >> degatPhysique;
        if (!nom.empty() && iss) {
            attaques.emplace_back(nom, degatMagique, degatPhysique);
        } else {
            std::cerr << "Erreur : Format incorrect dans le fichier des attaques." << std::endl;
        }
    }
    return attaques;
}

/**
 * @brief Loads a list of weapons from a file.
 *
 * This function reads a file containing information about weapons and
 * creates a list of Arme objects using the data read. Each line of the file
 * should be in the following format:
 * name/physicalAttackBonus/magicAttackBonus
 *
 * @param fichier The path to the file to read.
 * @return A vector of shared pointers to Arme objects.
 */
std::vector<std::shared_ptr<Arme>> chargerArmes(const std::string& fichier) {
    std::vector<std::shared_ptr<Arme>> armes;
    std::ifstream in(fichier);
    if (!in) {
        std::cerr << "Erreur : Impossible de charger les armes." << std::endl;
        return armes;
    }
    std::string ligne;
    while (std::getline(in, ligne)) {
        std::istringstream iss(ligne);
        std::string nom;
        int bonusAttaquePhysique, bonusAttaqueMagique;
        std::getline(iss, nom, '/');
        iss >> bonusAttaquePhysique;
        iss.ignore();
        iss >> bonusAttaqueMagique;
        if (!nom.empty() && iss) {
            armes.push_back(std::make_shared<Arme>(nom, bonusAttaquePhysique, bonusAttaqueMagique));
        } else {
            std::cerr << "Erreur : Format incorrect dans le fichier des armes." << std::endl;
        }
    }
    return armes;
}

/**
 * @brief Loads a list of armors from a file.
 *
 * This function reads a text file containing information about armors
 * and creates a list of Armure objects. Each line of the file should be in the format:
 * name/physicalDefenseBonus/magicDefenseBonus
 *
 * @param fichier The path to the file containing the armor information.
 * @return A vector of shared pointers to Armure objects loaded from the file.
 */
std::vector<std::shared_ptr<Armure>> chargerArmures(const std::string& fichier) {
    std::vector<std::shared_ptr<Armure>> armures;
    std::ifstream in(fichier);
    if (!in) {
        std::cerr << "Erreur : Impossible de charger les armures." << std::endl;
        return armures;
    }
    std::string ligne;
    while (std::getline(in, ligne)) {
        std::istringstream iss(ligne);
        std::string nom;
        int bonusDefensePhysique, bonusDefenseMagique;
        std::getline(iss, nom, '/');
        iss >> bonusDefensePhysique;
        iss.ignore();
        iss >> bonusDefenseMagique;
        if (!nom.empty() && iss) {
            armures.push_back(std::make_shared<Armure>(nom, bonusDefensePhysique, bonusDefenseMagique));
        } else {
            std::cerr << "Erreur : Format incorrect dans le fichier des armures." << std::endl;
        }
    }
    return armures;
}

/**
 * @brief Generates a random name from a list of names stored in a file.
 *
 * This function reads enemy names from the file "ennemis.txt" and selects one at random.
 * If the file cannot be opened, an error message is displayed and an empty string is returned.
 *
 * @return std::string A random name selected from the file "ennemis.txt", or an empty string in case of an error.
 */
std::string nomAleatoire() {
    std::ifstream in("ennemis.txt");
    if (!in) {
        std::cerr << "Erreur : Impossible de charger les noms des ennemis." << std::endl;
        return "";
    }

    std::vector<std::string> noms;
    std::string nom;
    while (std::getline(in, nom)) {
        noms.push_back(nom);
    }

    int chance = rand() % noms.size();
    return noms[chance];
}

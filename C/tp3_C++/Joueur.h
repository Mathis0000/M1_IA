#ifndef JOUEUR_H
#define JOUEUR_H

#include "Personnage.h"
#include "Attaque.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @class Joueur
 * @brief Represents a player character in the game, inheriting from Personnage.
 * 
 * The Joueur class provides functionalities for interacting with enemies, objects,
 * saving and loading game state, and managing the player's inventory.
 */

/**
 * @brief Constructs a new Joueur object.
 * 
 * @param n The name of the player.
 * @param pv The player's health points.
 * @param vap The player's attack power.
 * @param vam The player's magic power.
 * @param dp The player's defense points.
 * @param dm The player's magic defense points.
 */

class Joueur : public Personnage {
public:
    Joueur(const std::string& n, int pv, int vap, int vam, int dp, int dm);

    /**
     * @brief Engage in an encounter with an enemy character.
     * 
     * This function simulates a meeting with an enemy character, allowing for 
     * interactions such as attacks.
     * 
     * @param ennemi A reference to the enemy character (of type Personnage) that is encountered.
     * @param attaques A vector of possible attacks (of type Attaque) that can be used during the encounter.
     * @param nomAleatoire A randomly generated name for the encounter.
     */
    void rencontrerEnnemi(Personnage& ennemi, const std::vector<Attaque>& attaques, std::string nomAleatoire);

    /**
     * @brief Handles the interaction between the player and an object.
     * 
     * This function is called when the player encounters an object in the game.
     * It processes the interaction based on the type and properties of the object.
     * 
     * @param objet A shared pointer to the object that the player encounters.
     */
    void rencontrerObjet(std::shared_ptr<Objet> objet);

    /**
     * @brief Saves the current state of the object to a file.
     * 
     * This function serializes the object's data and writes it to the specified file.
     * 
     * @param fichier The path to the file where the object's state will be saved.
     */
    void sauvegarder(const std::string& fichier);

    /**
     * @brief Loads data from a specified file.
     * 
     * This function reads data from the file specified by the given filename
     * and loads it into the appropriate data structures.
     * 
     * @param fichier The name of the file to load data from.
     */
    void charger(const std::string& fichier);

    /**
     * @brief Clears the inventory of the player.
     * 
     * This function removes all items from the player's inventory,
     * effectively resetting it to an empty state.
     */
    void clearInventaire();

private:
    /**
     * @brief Retrieves the list of available attacks.
     * 
     * @return std::vector<Attaque> A vector containing the available attacks.
     */
    std::vector<Attaque> getAttaquesDisponibles() const;
};

/**
 * @brief Loads a list of attacks from a file.
 * 
 * This function reads a file specified by the given path and loads the
 * attacks defined in that file into a vector of Attaque objects.
 * 
 * @param fichier The path of the file containing the attacks to load.
 * @return std::vector<Attaque> A vector containing the loaded attacks.
 */
std::vector<Attaque> chargerAttaques(const std::string& fichier);

/**
 * @brief Loads a list of weapons from a file.
 * 
 * This function reads a file specified by the `fichier` parameter and creates a 
 * list of shared `Arme` objects (std::shared_ptr<Arme>). Each weapon is added 
 * to a vector which is then returned.
 * 
 * @param fichier The path to the file containing the weapon information.
 * @return std::vector<std::shared_ptr<Arme>> A vector of shared pointers to the loaded `Arme` objects.
 */
std::vector<std::shared_ptr<Arme>> chargerArmes(const std::string& fichier);

/**
 * @brief Loads a list of armors from a file.
 * 
 * This function reads a file specified by the `fichier` parameter and creates a
 * list of `Armure` objects using smart pointers (`shared_ptr`).
 * 
 * @param fichier The path to the file containing the armor data.
 * @return std::vector<std::shared_ptr<Armure>> A vector of smart pointers
 *         to the `Armure` objects loaded from the file.
 */
std::vector<std::shared_ptr<Armure>> chargerArmures(const std::string& fichier);

/**
 * @brief Generates a random name.
 * 
 * This function creates and returns a random name as a string.
 * 
 * @return std::string A randomly generated name.
 */
std::string nomAleatoire();

#endif // JOUEUR_H

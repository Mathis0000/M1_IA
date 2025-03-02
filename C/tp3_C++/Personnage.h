#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include <string>
#include <vector>
#include <memory>
#include "Objet.h"
#include "Arme.h"
#include "Armure.h"
#include "Attaque.h"

class Personnage : public std::enable_shared_from_this<Personnage> {
protected:
    std::string nom;
    int pointsDeVie;
    int valeurAttaquePhysique;
    int valeurAttaqueMagique;
    int defensePhysique;
    int defenseMagique;
    std::vector<std::shared_ptr<Objet>> inventaire;
    std::shared_ptr<Arme> armeEquipee;
    std::shared_ptr<Armure> armureEquipee;

public:
    Personnage(const std::string& n, int pv, int vap, int vam, int dp, int dm);
    virtual ~Personnage() = default;

    int getPointsDeVie() const;
    void setPointsDeVie(int pv);

    int getValeurAttaquePhysique() const;
    void setValeurAttaquePhysique(int vap);

    int getValeurAttaqueMagique() const;
    void setValeurAttaqueMagique(int vam);

    int getDefensePhysique() const;
    void setDefensePhysique(int dp);

    int getDefenseMagique() const;
    void setDefenseMagique(int dm);

    const std::vector<std::shared_ptr<Objet>>& getInventaire() const;
    std::vector<std::shared_ptr<Objet>>& getInventaire();
    const std::string& getNom() const;

    std::shared_ptr<Arme> getArmeEquipee() const;
    std::shared_ptr<Armure> getArmureEquipee() const;

    /**
     * @brief Attacks the target character with a specified attack.
     * 
     * @param cible The target character to be attacked.
     * @param attaque The attack to be used against the target character.
     */
    void attaquer(Personnage& cible, const Attaque& attaque);

    /**
     * @brief Inflicts damage to the character.
     * 
     * This function reduces the character's health points by the specified amount of damage.
     * 
     * @param degats The amount of damage to inflict on the character.
     */
    void subirDegats(int degats);

    /**
     * @brief Adds an object to the character's inventory.
     * 
     * @param objet A shared pointer to the object to be added.
     */
    void ajouterObjet(std::shared_ptr<Objet> objet);

    /**
     * @brief Equips the character with a specified weapon.
     * 
     * This function assigns a weapon to the character, allowing them to use it in combat.
     * 
     * @param arme A shared pointer to the weapon (Arme) to be equipped.
     */
    void equiperArme(std::shared_ptr<Arme> arme);

    /**
     * @brief Unequips the weapon from the character.
     * 
     * This function removes the currently equipped weapon from the character,
     * leaving the character without any weapon.
     */
    void desequiperArme();

    /**
     * @brief Equips the character with the specified armor.
     * 
     * This function assigns the provided armor to the character, enhancing their defense capabilities.
     * 
     * @param armure A shared pointer to the Armure object to be equipped.
     */
    void equiperArmure(std::shared_ptr<Armure> armure);

    /**
     * @brief Unequips the armor from the character.
     * 
     * This function removes the currently equipped armor from the character,
     * making them vulnerable to attacks. It does not destroy the armor, 
     * just unequips it.
     */
    void desequiperArmure();
};

#endif // PERSONNAGE_H

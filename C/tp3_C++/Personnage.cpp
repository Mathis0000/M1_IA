#include "Personnage.h"
#include <iostream>

/**
 * @brief Construct a new Personnage object
 * 
 * @param n The name of the character
 * @param pv The health points of the character
 * @param vap The physical attack value of the character
 * @param vam The magical attack value of the character
 * @param dp The physical defense value of the character
 * @param dm The magical defense value of the character
 */
Personnage::Personnage(const std::string& n, int pv, int vap, int vam, int dp, int dm)
    : nom(n), pointsDeVie(pv), valeurAttaquePhysique(vap), valeurAttaqueMagique(vam),
      defensePhysique(dp), defenseMagique(dm),
      armeEquipee(nullptr), armureEquipee(nullptr) {}

int Personnage::getPointsDeVie() const { return pointsDeVie; }
void Personnage::setPointsDeVie(int pv) { pointsDeVie = pv; }

int Personnage::getValeurAttaquePhysique() const { return valeurAttaquePhysique; }
void Personnage::setValeurAttaquePhysique(int vap) { valeurAttaquePhysique = vap; }

int Personnage::getValeurAttaqueMagique() const { return valeurAttaqueMagique; }
void Personnage::setValeurAttaqueMagique(int vam) { valeurAttaqueMagique = vam; }

int Personnage::getDefensePhysique() const { return defensePhysique; }
void Personnage::setDefensePhysique(int dp) { defensePhysique = dp; }

int Personnage::getDefenseMagique() const { return defenseMagique; }
void Personnage::setDefenseMagique(int dm) { defenseMagique = dm; }

const std::vector<std::shared_ptr<Objet>>& Personnage::getInventaire() const { return inventaire; }
std::vector<std::shared_ptr<Objet>>& Personnage::getInventaire() { return inventaire; }
const std::string& Personnage::getNom() const { return nom; }

std::shared_ptr<Arme> Personnage::getArmeEquipee() const { return armeEquipee; }
std::shared_ptr<Armure> Personnage::getArmureEquipee() const { return armureEquipee; }

/**
 * @brief Attacks another character with a specific attack.
 * 
 * This method allows a character to attack another character using a specific attack.
 * Physical and magical damage are calculated based on the attacker's attack values
 * and the target character's defenses. The most effective damage (physical or magical)
 * is inflicted on the target.
 * 
 * @param cible The character being attacked.
 * @param attaque The attack used to attack the target character.
 */
void Personnage::attaquer(Personnage& cible, const Attaque& attaque) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << nom << " attaque avec l'attaque: " << attaque.nom << std::endl;

    int degatsPhysiques = valeurAttaquePhysique + attaque.degatPhysique - cible.defensePhysique;
    int degatsMagiques = valeurAttaqueMagique + attaque.degatMagique - cible.defenseMagique;

    std::cout << "Dégâts Physiques: " << degatsPhysiques << " (Attaque Physique: " << valeurAttaquePhysique << ", Dégât du sort Physique: " << attaque.degatPhysique << ", Défense Physique: " << cible.defensePhysique << ")" << std::endl;
    std::cout << "Dégâts Magiques: " << degatsMagiques << " (Attaque Magique: " << valeurAttaqueMagique << ", Dégât du sort Magique: " << attaque.degatMagique << ", Défense Magique: " << cible.defenseMagique << ")" << std::endl;

    int degatsTotaux = std::max(std::max(degatsPhysiques, 0), std::max(degatsMagiques, 0));
    std::cout << "Dégâts les plus efficaces : " << degatsTotaux << std::endl;

    std::cout << "Cible avant attaque: " << std::endl;
    std::cout << cible.nom << " - Points de vie: " << cible.getPointsDeVie() << std::endl;

    cible.subirDegats(degatsTotaux);

    std::cout << "Cible après attaque: " << std::endl;
    std::cout << cible.nom << " - Points de vie: " << cible.getPointsDeVie() << std::endl;
}

/**
 * @brief Inflicts damage to the character.
 *
 * This function reduces the character's health points based on the damage
 * received. If the health points drop to zero or below, the character is
 * considered defeated.
 *
 * @param degats The amount of damage to inflict on the character.
 */
void Personnage::subirDegats(int degats) {
    pointsDeVie -= degats;
    if (pointsDeVie <= 0) {
        pointsDeVie = 0;
        std::cout << nom << " a été vaincu!" << std::endl;
    } else {
        std::cout << nom << " a subi " << degats << " dégâts!" << std::endl;
    }
}

/**
 * @brief Adds an object to the character's inventory.
 * 
 * This function adds an object to the character's inventory if the inventory
 * is not full. If the inventory contains less than 10 objects, the object is
 * added and a confirmation message is displayed. Otherwise, a message
 * indicating that the inventory is full is displayed.
 * 
 * @param objet A shared pointer to the object to add to the inventory.
 */
void Personnage::ajouterObjet(std::shared_ptr<Objet> objet) {
    if (inventaire.size() < 10) {
        inventaire.push_back(objet);
        std::cout << "Vous avez ajouté " << objet->nom << " à votre inventaire." << std::endl;
    } else {
        std::cout << "Votre inventaire est plein!" << std::endl;
    }
}

/**
 * @brief Equips a weapon to the character, updating the character's attack values.
 * 
 * This function equips a new weapon to the character. If a weapon is already equipped,
 * it first removes the attack bonuses provided by the currently equipped weapon.
 * Then, it equips the new weapon and adds its attack bonuses to the character's
 * physical and magical attack values.
 * 
 * @param arme A shared pointer to the weapon to be equipped. If nullptr, no weapon is equipped.
 */
void Personnage::equiperArme(std::shared_ptr<Arme> arme) {
    if (armeEquipee) {
        valeurAttaquePhysique -= armeEquipee->bonusAttaquePhysique;
        valeurAttaqueMagique -= armeEquipee->bonusAttaqueMagique;
    }
    armeEquipee = arme;
    if (arme) {
        valeurAttaquePhysique += arme->bonusAttaquePhysique;
        valeurAttaqueMagique += arme->bonusAttaqueMagique;
    }
}

/**
 * @brief Unequips the currently equipped weapon from the character.
 *
 * This method checks if a weapon is equipped. If so, it subtracts the physical
 * and magical attack bonuses of the weapon from the character's attack values,
 * then unequips the weapon by setting it to nullptr.
 */
void Personnage::desequiperArme() {
    if (armeEquipee) {
        valeurAttaquePhysique -= armeEquipee->bonusAttaquePhysique;
        valeurAttaqueMagique -= armeEquipee->bonusAttaqueMagique;
        armeEquipee = nullptr;
    }
}

/**
 * @brief Equips the character with a new armor.
 *
 * This function updates the character's physical and magical defense
 * based on the bonuses provided by the new armor. If the character
 * already has an armor equipped, the bonuses from the old armor are
 * subtracted before adding the bonuses from the new armor.
 *
 * @param armure A shared pointer to the new armor to be equipped. If nullptr,
 *               the character will have no armor equipped.
 */
void Personnage::equiperArmure(std::shared_ptr<Armure> armure) {
    if (armureEquipee) {
        defensePhysique -= armureEquipee->bonusDefensePhysique;
        defenseMagique -= armureEquipee->bonusDefenseMagique;
    }
    armureEquipee = armure;
    if (armure) {
        defensePhysique += armure->bonusDefensePhysique;
        defenseMagique += armure->bonusDefenseMagique;
    }
}

/**
 * @brief Unequips the currently equipped armor from the character.
 *
 * This function checks if an armor is equipped. If so, it subtracts the physical
 * and magical defense bonuses of the armor from the character's defense values,
 * then unequips the armor by setting it to nullptr.
 */
void Personnage::desequiperArmure() {
    if (armureEquipee) {
        defensePhysique -= armureEquipee->bonusDefensePhysique;
        defenseMagique -= armureEquipee->bonusDefenseMagique;
        armureEquipee = nullptr;
    }
}


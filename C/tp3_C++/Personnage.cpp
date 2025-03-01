#include "Personnage.h"
#include <iostream>

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

void Personnage::subirDegats(int degats) {
    pointsDeVie -= degats;
    if (pointsDeVie <= 0) {
        pointsDeVie = 0;
        std::cout << nom << " a été vaincu!" << std::endl;
    } else {
        std::cout << nom << " a subi " << degats << " dégâts!" << std::endl;
    }
}

void Personnage::ajouterObjet(std::shared_ptr<Objet> objet) {
    if (inventaire.size() < 10) {
        inventaire.push_back(objet);
        std::cout << "Vous avez ajouté " << objet->nom << " à votre inventaire." << std::endl;
    } else {
        std::cout << "Votre inventaire est plein!" << std::endl;
    }
}

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

void Personnage::desequiperArme() {
    if (armeEquipee) {
        valeurAttaquePhysique -= armeEquipee->bonusAttaquePhysique;
        valeurAttaqueMagique -= armeEquipee->bonusAttaqueMagique;
        armeEquipee = nullptr;
    }
}

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

void Personnage::desequiperArmure() {
    if (armureEquipee) {
        defensePhysique -= armureEquipee->bonusDefensePhysique;
        defenseMagique -= armureEquipee->bonusDefenseMagique;
        armureEquipee = nullptr;
    }
}

std::ostream& operator<<(std::ostream& os, const Personnage& personnage) {
    os << "Nom: " << personnage.nom << std::endl;
    os << "Points de vie: " << personnage.pointsDeVie << std::endl;
    os << "Attaque Physique: " << personnage.valeurAttaquePhysique << std::endl;
    os << "Attaque Magique: " << personnage.valeurAttaqueMagique << std::endl;
    os << "Défense Physique: " << personnage.defensePhysique << std::endl;
    os << "Défense Magique: " << personnage.defenseMagique << std::endl;
    return os;
}

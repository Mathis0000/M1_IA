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

Joueur::Joueur(const std::string& n, int pv, int vap, int vam, int dp, int dm)
    : Personnage(n, pv, vap, vam, dp, dm) {}

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
            std::cin >> choix;
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

void Joueur::rencontrerObjet(std::shared_ptr<Objet> objet) {
    ajouterObjet(objet);
}
void Joueur::clearInventaire() {
    inventaire.clear();
}

std::vector<Attaque> Joueur::getAttaquesDisponibles() const {
    std::vector<Attaque> attaquesDisponibles;
    for (const auto& objet : inventaire) {
        if (auto attaqueObjet = std::dynamic_pointer_cast<AttaqueObjet>(objet)) {
            attaquesDisponibles.push_back(attaqueObjet->attaque);
        }
    }
    return attaquesDisponibles;
}

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

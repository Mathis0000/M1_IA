#ifndef JOUEUR_H
#define JOUEUR_H

#include "Personnage.h"
#include "Attaque.h"
#include <vector>
#include <memory>
#include <string>

class Joueur : public Personnage {
public:
    Joueur(const std::string& n, int pv, int vap, int vam, int dp, int dm);

    void rencontrerEnnemi(Personnage& ennemi, const std::vector<Attaque>& attaques, std::string nomAleatoire);
    void rencontrerObjet(std::shared_ptr<Objet> objet);
    void sauvegarder(const std::string& fichier);
    void charger(const std::string& fichier);
    void clearInventaire();

private:
    std::vector<Attaque> getAttaquesDisponibles() const;
};

std::vector<Attaque> chargerAttaques(const std::string& fichier);
std::vector<std::shared_ptr<Arme>> chargerArmes(const std::string& fichier);
std::vector<std::shared_ptr<Armure>> chargerArmures(const std::string& fichier);
std::string nomAleatoire();

#endif // JOUEUR_H

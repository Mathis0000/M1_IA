#include "AttaqueObjet.h"
#include "Personnage.h"
#include <iostream>

AttaqueObjet::AttaqueObjet(const Attaque& a) : Objet(a.nom), attaque(a) {}

void AttaqueObjet::utiliser(Personnage& personnage) {
    std::cout << "Vous utilisez l'attaque: " << attaque.nom << std::endl;
}

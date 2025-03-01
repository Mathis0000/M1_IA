#ifndef ATTAQUEOBJET_H
#define ATTAQUEOBJET_H

#include "Objet.h"
#include "Attaque.h"

class AttaqueObjet : public Objet {
public:
    Attaque attaque;
    AttaqueObjet(const Attaque& a);
    void utiliser(Personnage& personnage) override;
};

#endif // ATTAQUEOBJET_H

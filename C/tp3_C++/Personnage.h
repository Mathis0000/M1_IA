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

    void attaquer(Personnage& cible, const Attaque& attaque);
    void subirDegats(int degats);
    void ajouterObjet(std::shared_ptr<Objet> objet);
    void equiperArme(std::shared_ptr<Arme> arme);
    void desequiperArme();
    void equiperArmure(std::shared_ptr<Armure> armure);
    void desequiperArmure();

    friend std::ostream& operator<<(std::ostream& os, const Personnage& personnage);
};

#endif // PERSONNAGE_H

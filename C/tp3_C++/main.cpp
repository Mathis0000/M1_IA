#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Personnage.h"
#include "Potion.h"
#include "Arme.h"
#include "Armure.h"
#include "Attaque.h"
#include "AttaqueObjet.h"
#include "Joueur.h"



using namespace std;

//partie interface graphique
void mettreAJourInventaire(Joueur& joueur, vector<sf::RectangleShape>& inventorySlots, vector<sf::Text>& inventorySlotTexts, vector<sf::RectangleShape>& attackButtons, vector<sf::Text>& attackButtonTexts, sf::Font& font) {
    attackButtons.clear();
    attackButtonTexts.clear();
    for (auto& text : inventorySlotTexts) {
        text.setString("");
    }

    for (size_t i = 0; i < joueur.getInventaire().size(); ++i) {
        const auto& objet = joueur.getInventaire()[i];
        if (auto arme = dynamic_pointer_cast<Arme>(objet)) {
            inventorySlotTexts[i].setString(objet->nom + " (Arme)\nAttaque Physique: " + to_string(arme->bonusAttaquePhysique) +
                            ", Attaque Magique: " + to_string(arme->bonusAttaqueMagique));
        } else if (auto armure = dynamic_pointer_cast<Armure>(objet)) {
            inventorySlotTexts[i].setString(objet->nom + " (Armure)\nDefense Physique: " + to_string(armure->bonusDefensePhysique) +
                            ", Defense Magique: " + to_string(armure->bonusDefenseMagique));
        } else if (auto attaqueObjet = dynamic_pointer_cast<AttaqueObjet>(objet)) {
            sf::RectangleShape attackButton(sf::Vector2f(500, 80));
            attackButton.setFillColor(sf::Color::Red);
            attackButton.setPosition(520, 120 + i * 90);
            attackButtons.push_back(attackButton);

            sf::Text attackButtonText;
            attackButtonText.setFont(font);
            attackButtonText.setString(objet->nom + "\nPhysique: " + to_string(attaqueObjet->attaque.degatPhysique) +
                    ", Magique: " + to_string(attaqueObjet->attaque.degatMagique));
            attackButtonText.setCharacterSize(20);
            attackButtonText.setFillColor(sf::Color::White);
            attackButtonText.setPosition(attackButton.getPosition().x + 10, attackButton.getPosition().y + 10);
            attackButtonTexts.push_back(attackButtonText);
        } else if (dynamic_pointer_cast<Potion>(objet)) {
            inventorySlotTexts[i].setString(objet->nom + " (Potion)");
        } else {
            inventorySlotTexts[i].setString(objet->nom);
        }
    }
}

std::vector<sf::Texture> chargerTexturesMechant(const std::string& cheminDossier, int nombreTextures, int taille) {
    std::vector<sf::Texture> textures;
    for (int i = 0; i < nombreTextures; ++i) {
        std::string filename = cheminDossier + "/frame_" + std::to_string(i + 1).insert(0, 4 - std::to_string(i + 1).length(), '0') + ".png";
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Erreur : impossible de charger " << filename << std::endl;
        }
        if (taille == 1) {
            sf::Image image = texture.copyToImage();
            sf::Image resizedImage;
            resizedImage.create(image.getSize().x * 3, image.getSize().y * 3);
            for (unsigned int x = 0; x < image.getSize().x; ++x) {
                for (unsigned int y = 0; y < image.getSize().y; ++y) {
                    sf::Color color = image.getPixel(x, y);
                    for (int dx = 0; dx < 3; ++dx) {
                        for (int dy = 0; dy < 3; ++dy) {
                            resizedImage.setPixel(x * 3 + dx, y * 3 + dy, color);
                        }
                    }
                }
            }
            texture.loadFromImage(resizedImage);
        }
        textures.push_back(texture);
    }
    return textures;
}

// Fonction pour afficher et animer un méchant
int afficherMechant(sf::RenderWindow& window, std::vector<std::vector<sf::Texture>>& mechantsTextures, sf::Vector2f position, float frameTime, int& frameIndex, int& mechantActuel, sf::Clock& clock) {
    // Vérifier que mechantActuel est dans les limites
    if (mechantActuel < 0 || mechantActuel >= static_cast<int>(mechantsTextures.size())) {
        cerr << "Erreur : mechantActuel hors limites." << endl;
        return -1;
    }

    // Animer le méchant actuel
    if (clock.getElapsedTime().asSeconds() > frameTime / 2) {
        frameIndex = (frameIndex + 1) % mechantsTextures[mechantActuel].size();
        clock.restart();
    }

    // Vérifier que frameIndex est dans les limites après l'incrémentation
    if (frameIndex < 0 || frameIndex >= static_cast<int>(mechantsTextures[mechantActuel].size())) {
        frameIndex = 0; // Réinitialiser à 0 si hors limites
    }

    // Ajuster la position pour mechants2
    sf::Vector2f adjustedPosition = position;
    adjustedPosition.x -= 300; // Ajuster la position en x
    adjustedPosition.y -= 300; // Ajuster la position en y

    if (mechantActuel == 2) {
        adjustedPosition.y += 100; // Ajuster la position en y
        adjustedPosition.x += 50; // Ajuster la position en x
    }

    // Dessiner le méchant actuel
    sf::Sprite mechantSprite;
    mechantSprite.setTexture(mechantsTextures[mechantActuel][frameIndex]);
    mechantSprite.setPosition(adjustedPosition);
    window.draw(mechantSprite);
    return 0;
}


int interface() {

    sf::Music music_battle;

    // Charger un fichier audio au format OGG
    if (!music_battle.openFromFile("audio/battle.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier audio !" << std::endl;
        return -1;
    }
    
    sf::Music music_boss_battle;

    // Charger un fichier audio au format OGG
    if (!music_boss_battle.openFromFile("audio/boss_battle.ogg")) {
        std::cerr << "Erreur : Impossible de charger le fichier audio !" << std::endl;
        return -1;
    }

    sf::Music music_defaite;

    // Charger un fichier audio au format OGG
    if (!music_defaite.openFromFile("audio/defaite.ogg")) {
        std::cerr << "Erreur : Impossible de charger le fichier audio !" << std::endl;
        return -1;
    }

    sf::Music music_menu;

    // Charger un fichier audio au format OGG
    if (!music_menu.openFromFile("audio/menu.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier audio !" << std::endl;
        return -1;
    }

    sf::Music music_pouvoir;

    // Charger un fichier audio au format OGG
    if (!music_pouvoir.openFromFile("audio/pouvoir.ogg")) {
        std::cerr << "Erreur : Impossible de charger le fichier audio !" << std::endl;
        return -1;
    }

    sf::Music music_sacrifice;

    // Charger un fichier audio au format OGG
    if (!music_sacrifice.openFromFile("audio/sacrifice.ogg")) {
        std::cerr << "Erreur : Impossible de charger le fichier audio !" << std::endl;
        return -1;
    }


    sf::VideoMode videoMode(1920, 1080);
    sf::RenderWindow window(videoMode, "Introduction");
    window.setPosition(sf::Vector2i(0, 0));

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("image/village.jpg")) {
        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
        return 1;
    }

    window.setVerticalSyncEnabled(true);

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Texture backgroundTexturegrotte;
    if (!backgroundTexturegrotte.loadFromFile("image/grotte.jpg")) {
        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
        return 1;
    }

    sf::Sprite backgroundSpritegrotte;
    backgroundSpritegrotte.setTexture(backgroundTexturegrotte);
    backgroundSpritegrotte.setScale(
        static_cast<float>(videoMode.width) / backgroundTexturegrotte.getSize().x,
        static_cast<float>(videoMode.height) / backgroundTexturegrotte.getSize().y
    );

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police Arial.ttf !" << std::endl;
        return 1;
    }

    // Initialisation des textes et boutons
    unsigned int characterSize = videoMode.width / 80;
    std::string texteUtf8 = u8"Dans un monde ravagé par la guerre et la corruption, "
                            "les anciennes légendes parlent d’un artefact perdu, "
                            "la Pierre de Lumen, capable de restaurer l’équilibre entre "
                            "la lumière et les ténèbres."
                            "Cet artefact est caché dans le Donjon du Néant, une forteresse maudite "
                            "où résident des créatures cauchemardesques et leur maître impitoyable : "
                            "Malakar, l’Ombre Éternelle."
                            "Tuer 5 de ces généraux devrait être suffisant pour éveiller sa colère..."
                            "Vous incarnez un aventurier solitaire, chargé d’infiltrer ce donjon maudit "
                            "pour récupérer la pierre avant que les ténèbres ne consument définitivement le royaume.";

    float maxTextWidth = videoMode.width / 3.0f;
    sf::Text text;
    text.setLineSpacing(1.5f);

    std::string wrappedText;
    std::istringstream words(texteUtf8);
    std::string word;
    float currentLineWidth = 0.0f;
    while (words >> word) {
        sf::Text tempText(word, font, characterSize);
        float wordWidth = tempText.getLocalBounds().width;
        if (currentLineWidth + wordWidth > maxTextWidth) {
            wrappedText += "\n";
            currentLineWidth = 0.0f;
        }
        wrappedText += word + " ";
        currentLineWidth += wordWidth + tempText.getLetterSpacing();
    }

    texteUtf8 = wrappedText;
    sf::String texteSfml = sf::String::fromUtf8(texteUtf8.begin(), texteUtf8.end());
    text.setFont(font);
    text.setString(texteSfml);
    text.setCharacterSize(characterSize);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(videoMode.width / 2.0f, videoMode.height / 2.0f));

    // Initialisation des boutons
    sf::RectangleShape barbareButton(sf::Vector2f(200, 50));
    barbareButton.setFillColor(sf::Color::Red);
    barbareButton.setPosition(videoMode.width / 2.0f - 100, text.getPosition().y + textRect.height / 2.0f + 20);

    sf::Text barbareButtonText;
    barbareButtonText.setFont(font);
    barbareButtonText.setString("Barbare");
    barbareButtonText.setCharacterSize(30);
    barbareButtonText.setFillColor(sf::Color::White);
    sf::FloatRect barbareButtonTextRect = barbareButtonText.getLocalBounds();
    barbareButtonText.setOrigin(barbareButtonTextRect.left + barbareButtonTextRect.width / 2.0f, barbareButtonTextRect.top + barbareButtonTextRect.height / 2.0f);
    barbareButtonText.setPosition(barbareButton.getPosition().x + barbareButton.getSize().x / 2.0f, barbareButton.getPosition().y + barbareButton.getSize().y / 2.0f);

    sf::RectangleShape magicienButton(sf::Vector2f(200, 50));
    magicienButton.setFillColor(sf::Color::Red);
    magicienButton.setPosition(videoMode.width / 2.0f - 100, barbareButton.getPosition().y + barbareButton.getSize().y + 20);

    sf::Text magicienButtonText;
    magicienButtonText.setFont(font);
    magicienButtonText.setString("Magicien");
    magicienButtonText.setCharacterSize(30);
    magicienButtonText.setFillColor(sf::Color::White);
    sf::FloatRect magicienButtonTextRect = magicienButtonText.getLocalBounds();
    magicienButtonText.setOrigin(magicienButtonTextRect.left + magicienButtonTextRect.width / 2.0f, magicienButtonTextRect.top + magicienButtonTextRect.height / 2.0f);
    magicienButtonText.setPosition(magicienButton.getPosition().x + magicienButton.getSize().x / 2.0f, magicienButton.getPosition().y + magicienButton.getSize().y / 2.0f);

    sf::RectangleShape quitButton(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(videoMode.width / 2.0f - 100, magicienButton.getPosition().y + magicienButton.getSize().y + 20);

    sf::Text quitButtonText;
    quitButtonText.setFont(font);
    quitButtonText.setString("Quitter");
    quitButtonText.setCharacterSize(30);
    quitButtonText.setFillColor(sf::Color::White);
    sf::FloatRect quitButtonTextRect = quitButtonText.getLocalBounds();
    quitButtonText.setOrigin(quitButtonTextRect.left + quitButtonTextRect.width / 2.0f, quitButtonTextRect.top + quitButtonTextRect.height / 2.0f);
    quitButtonText.setPosition(quitButton.getPosition().x + quitButton.getSize().x / 2.0f, quitButton.getPosition().y + quitButton.getSize().y / 2.0f);

    sf::RectangleShape quitButtonn(sf::Vector2f(200, 50));
    quitButtonn.setFillColor(sf::Color::Red);
    quitButtonn.setPosition(videoMode.width - 220, videoMode.height - 70);

    sf::Text quitButtonnText;
    quitButtonnText.setFont(font);
    quitButtonnText.setString("Quitter");
    quitButtonnText.setCharacterSize(30);
    quitButtonnText.setFillColor(sf::Color::White);
    sf::FloatRect quitButtonnTextRect = quitButtonnText.getLocalBounds();
    quitButtonnText.setOrigin(quitButtonnTextRect.left + quitButtonnTextRect.width / 2.0f, quitButtonnTextRect.top + quitButtonnTextRect.height / 2.0f);
    quitButtonnText.setPosition(quitButtonn.getPosition().x + quitButtonn.getSize().x / 2.0f, quitButtonn.getPosition().y + quitButtonn.getSize().y / 2.0f);

    sf::Sprite gameOverBackground;
    sf::Texture gameOverTexture;

    sf::Text gameOverText;
    bool gameOver = false;

    // Charger les textures des frames du héros
    std::vector<sf::Texture> heroTextures(11);
    for (int i = 0; i < 11; i++) {
        std::string filename = "image/heros/frame_" + std::to_string(i + 1).insert(0, 4 - std::to_string(i + 1).length(), '0') + ".png";
        if (!heroTextures[i].loadFromFile(filename)) {
            std::cerr << "Erreur : impossible de charger " << filename << std::endl;
            return -1;
        }
        sf::Image image = heroTextures[i].copyToImage();
        sf::Image resizedImage;
        resizedImage.create(image.getSize().x * 2, image.getSize().y * 2);
        for (unsigned int x = 0; x < image.getSize().x; ++x) {
            for (unsigned int y = 0; y < image.getSize().y; ++y) {
                sf::Color color = image.getPixel(x, y);
                for (int dx = 0; dx < 2; ++dx) {
                    for (int dy = 0; dy < 2; ++dy) {
                        resizedImage.setPixel(x * 2 + dx, y * 2 + dy, color);
                    }
                }
            }
        }
        heroTextures[i].loadFromImage(resizedImage);
    }

    sf::Sprite heroSprite;
    heroSprite.setPosition(0, 200);
    int frameIndex = 0;
    sf::Clock clock;
    sf::Clock clockheros;
    float frameTime = 0.1f;
    float frametimemechant = 0.1f;
    int frameIndexmechant = 0;
    bool showHero = false; 


    // Charger les textures des frames du boss
    std::vector<sf::Texture> bossTextures(48);
    for (int i = 0; i < 48; i++) {
        std::string filename = "image/boss/frame_" + std::to_string(i + 1).insert(0, 4 - std::to_string(i + 1).length(), '0') + ".png";
        if (!bossTextures[i].loadFromFile(filename)) {
            std::cerr << "Erreur : impossible de charger " << filename << std::endl;
            return -1;
        }
        sf::Image imageboss = bossTextures[i].copyToImage();
        sf::Image resizedImageboss;
        resizedImageboss.create(imageboss.getSize().x * 5, imageboss.getSize().y * 5);
        for (unsigned int x = 0; x < imageboss.getSize().x; ++x) {
            for (unsigned int y = 0; y < imageboss.getSize().y; ++y) {
                sf::Color color = imageboss.getPixel(x, y);
                for (int dx = 0; dx < 5; ++dx) {
                    for (int dy = 0; dy < 5; ++dy) {
                        resizedImageboss.setPixel(x * 5 + dx, y * 5 + dy, color);
                    }
                }
            }
        }
        bossTextures[i].loadFromImage(resizedImageboss);
    }

    sf::Sprite bossSprite;
    bossSprite.setPosition(1000, 0);
    int frameIndexboss = 0;
    sf::Clock clockboss;
    float frameTimeboss = 0.1f;
    bool showboss = false; 

    // Charger les textures des méchants
    std::vector<std::vector<sf::Texture>> mechantsTextures;
    mechantsTextures.push_back(chargerTexturesMechant("image/mechants1", 48,0));
    mechantsTextures.push_back(chargerTexturesMechant("image/mechants2", 55,1));
    mechantsTextures.push_back(chargerTexturesMechant("image/mechants3", 47,1));

    
    // Initialisation des objets de combat
    srand(time(nullptr));
    vector<Attaque> attaques = chargerAttaques("attaques.txt");
    vector<shared_ptr<Arme>> armes = chargerArmes("armes.txt");
    vector<shared_ptr<Armure>> armures = chargerArmures("armures.txt");
    Joueur joueur("Joueur", 100, 10, 10, 5, 5);
    Personnage ennemi(nomAleatoire(), 50, 8, 8, 3, 3);

    if (!attaques.empty()) {
        int numero_attaque = rand() % attaques.size();
        auto attaque_heros = make_shared<AttaqueObjet>(attaques[numero_attaque]);
        joueur.ajouterObjet(attaque_heros);
    }
    shared_ptr<Arme> arme;
    shared_ptr<Armure> armure;

    // Configurer l'inventaire et les boutons d'attaque
    sf::RectangleShape inventoryBackground(sf::Vector2f(800, 950));
    inventoryBackground.setFillColor(sf::Color(100, 100, 100, 200));
    inventoryBackground.setPosition(450, 100);

     // vie joueur
     sf::RectangleShape healthBarBackground(sf::Vector2f(200, 20));
     healthBarBackground.setFillColor(sf::Color::Red);
     healthBarBackground.setPosition(20, 20);

     sf::RectangleShape healthBar(sf::Vector2f(200, 20));
     healthBar.setFillColor(sf::Color::Green);
     healthBar.setPosition(20, 20);

 //vie mechant
 sf::RectangleShape healthBarBackgroundEnnemi(sf::Vector2f(100, 20));
     healthBarBackgroundEnnemi.setFillColor(sf::Color::Red);
     healthBarBackgroundEnnemi.setPosition(20, 20);

     sf::RectangleShape healthBarEnnemi(sf::Vector2f(100, 20));
     healthBarEnnemi.setFillColor(sf::Color::Green);
     healthBarEnnemi.setPosition(20, 20);

    vector<sf::RectangleShape> inventorySlots(10);
    vector<sf::Text> inventorySlotTexts(10);
    vector<sf::RectangleShape> attackButtons;
    vector<sf::Text> attackButtonTexts;
    auto potion = make_shared<Potion>();
    for (int i = 0; i < 10; ++i) {
        inventorySlots[i].setSize(sf::Vector2f(500, 80));
        inventorySlots[i].setFillColor(sf::Color::Transparent);
        inventorySlots[i].setOutlineThickness(2);
        inventorySlots[i].setOutlineColor(sf::Color::White);
        inventorySlots[i].setPosition(520, 120 + i * 90);

        inventorySlotTexts[i].setFont(font);
        inventorySlotTexts[i].setCharacterSize(20);
        inventorySlotTexts[i].setFillColor(sf::Color::White);
        inventorySlotTexts[i].setPosition(530, 130 + i * 90);
    }

    sf::Texture backgroundTextureBoss;
    if (!backgroundTextureBoss.loadFromFile("image/salle_du_boss_vide.jpg")) {
        std::cerr << "Erreur : Impossible de charger l'image salle_du_boss_vide.jpg !" << std::endl;
        return 1;
    }

    sf::Sprite backgroundSpriteBoss;
    backgroundSpriteBoss.setTexture(backgroundTextureBoss);
    backgroundSpriteBoss.setScale(
        static_cast<float>(videoMode.width) / backgroundTextureBoss.getSize().x,
        static_cast<float>(videoMode.height) / backgroundTextureBoss.getSize().y
    );

    

    bool showBarbareMagicien = true;
    bool salleboss = false;
    int nombre_de_tours = 0;
    int quelle_monstre= rand() % 4;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }        
            if(showBarbareMagicien){
                if (!music_menu.getStatus()) {
                    music_menu.play();
                }    
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (showBarbareMagicien) {
                        

                        if (barbareButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);
                            arme = make_shared<Arme>("Epee", 5, 0);
                            armure = make_shared<Armure>("Armure de l'apprenti soldat", 5, 0);
                            showBarbareMagicien = false;
                            showHero = true;
                            heroSprite.setTexture(heroTextures[0]);

                        joueur.equiperArmure(armure);
                        joueur.ajouterObjet(armure);
                        joueur.equiperArme(arme);
                        joueur.ajouterObjet(arme);
                        joueur.ajouterObjet(potion);
                        mettreAJourInventaire(joueur, inventorySlots, inventorySlotTexts, attackButtons, attackButtonTexts, font);
                        }
                        if (magicienButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            joueur.setValeurAttaqueMagique(joueur.getValeurAttaqueMagique() + 15);
                            arme = make_shared<Arme>("Baton magique", 0, 5);
                            armure = make_shared<Armure>("Armure de l'apprenti magicien", 0, 5);
                            showBarbareMagicien = false;
                            showHero = true;
                            heroSprite.setTexture(heroTextures[0]);

                        joueur.equiperArmure(armure);
                        joueur.ajouterObjet(armure);
                        joueur.equiperArme(arme);
                        joueur.ajouterObjet(arme);
                        joueur.ajouterObjet(potion);
                        mettreAJourInventaire(joueur, inventorySlots, inventorySlotTexts, attackButtons, attackButtonTexts, font);
                        }

                        
                    }
                    if (!showBarbareMagicien && showHero) {
                        music_menu.stop();
                        if (!music_battle.getStatus()&&nombre_de_tours<4) {
                            music_battle.play();
                        }                     
                        for (size_t i = 0; i < attackButtons.size(); ++i) {
                            if (attackButtons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                                if (!attaques.empty()) {
                                    joueur.attaquer(ennemi, attaques[i]);
                                    if (ennemi.getPointsDeVie() > 0) {
                                        ennemi.attaquer(joueur, attaques[rand() % attaques.size()]);
                                    }
                                    if (joueur.getPointsDeVie() <= 0) {
                                        
                                            cout << "Vous avez été vaincu!" << endl;
                                            auto& inventaire = joueur.getInventaire();
                                            auto it = find_if(inventaire.begin(), inventaire.end(), [](const shared_ptr<Objet>& obj) {
                                                return dynamic_pointer_cast<Potion>(obj) != nullptr;
                                            });
                                            if (it != inventaire.end()) {
                                                (*it)->utiliser(joueur);
                                            }
                                            mettreAJourInventaire(joueur, inventorySlots, inventorySlotTexts, attackButtons, attackButtonTexts, font);
                    

                                            
                                    }



                                    if (ennemi.getPointsDeVie() <= 0) {
                                        std::cout << "Vous avez vaincu l'ennemi!" << std::endl;
                                        if (nombre_de_tours < 4) {
                                            ennemi = Personnage(nomAleatoire(), 50, 8, 8, 3, 3);
                                            quelle_monstre = rand() % mechantsTextures.size();
                    
                                            int aléatoire = rand() % 4;
                                            if(aléatoire == 0){
                                                auto armureAleatoire = armures[rand() % armures.size()];
                                                if (joueur.getArmureEquipee()) {
                                                    auto& inventaire = joueur.getInventaire();
                                                    inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                                            [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmureEquipee(); }),
                                                                    inventaire.end());
                                                    joueur.desequiperArmure();
                                                }
                                                joueur.ajouterObjet(armureAleatoire);
                                                joueur.equiperArmure(armureAleatoire);
                                            }
                                            if(aléatoire == 1){
                                                auto armeAleatoire = armes[rand() % armes.size()];
                                                if (joueur.getArmeEquipee()) {
                                                    auto& inventaire = joueur.getInventaire();
                                                    inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                                            [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmeEquipee(); }),
                                                                    inventaire.end());
                                                    joueur.desequiperArme();
                                                }
                                                joueur.ajouterObjet(armeAleatoire);
                                                joueur.equiperArme(armeAleatoire);
                                            }
                                            if(aléatoire == 2){
                                                auto attaqueAleatoire = attaques[rand() % attaques.size()];
                                                joueur.ajouterObjet(make_shared<AttaqueObjet>(attaqueAleatoire));
                                            }
                                            if(aléatoire == 3){
                                                joueur.ajouterObjet(potion);
                                            }
                                        }
                                        if (nombre_de_tours == 4) {
                                            music_battle.stop();
                                            if (!music_boss_battle.getStatus()) {
                                                music_boss_battle.play();
                                            }
                                            salleboss = true;
                                            showboss = true;
                                            ennemi = Personnage("Malakar, l'Ombre Éternelle", 170, 20, 20, 10, 10);
                                        }
                                        std::string text;
                                            if (joueur.getPointsDeVie() <= 0) {
                                                music_boss_battle.stop();
                                                if (!music_defaite.getStatus()) {
                                                    music_defaite.play();
                                                }    
                                                
                                                showHero = false;
                                                gameOver = true;
                                                if (!backgroundTexture.loadFromFile("image/pays_devaste.jpg")) {
                                                    std::cerr << "Erreur : Impossible de charger l'image village.jpg !" << std::endl;
                                                    return 1;
                                                }
                                                

                                                text = u8"Le sol tremble sous vos pieds. Malakar, l'Ombre Eternelle, bien que blessé, se redresse lentement, un sourire sinistre aux lèvres. "
                                                    "Son regard brûlant de malice transperce votre âme. Vous tentez une dernière attaque, mais votre corps ne répond plus. "
                                                    "L'air est devenu trop lourd, l'obscurité trop oppressante. "
                                                    "Un rire glacial résonne dans la salle du trône. \"Tu as été un adversaire valeureux, aventurier... mais ici, c'est MOI qui dicte la fin de cette histoire.\" "
                                                    "Des ombres s'élèvent du sol, s'enroulant autour de vous comme des chaînes vivantes. Vous luttez, mais la force vous abandonne. "
                                                    "Peu à peu, la lumière quitte vos yeux, votre essence absorbée par l'abîme sans fin de Malakar. "
                                                    "Le monde extérieur ne sait rien de ce qui vient de se produire. Les villageois attendent, espérant voir leur héros revenir. Mais il ne reviendra jamais. "
                                                    "La nuit s'étend, plus sombre et plus froide que jamais. Dans les jours qui suivent, les ténèbres s'abattent sur le royaume. "
                                                    "Au sommet de son trône d'ombres, Malakar règne désormais sans opposition. "
                                                    "L'ombre a triomphé.";
                                            }
                                            auto inventaire2 = joueur.getInventaire();
                                            auto it2 = find_if(inventaire2.begin(), inventaire2.end(), [](const shared_ptr<Objet>& obj) {
                                                return dynamic_pointer_cast<Potion>(obj) != nullptr;
                                            });
                                            if (joueur.getPointsDeVie() > 0 && it2 != inventaire2.end() && ennemi.getPointsDeVie() <= 0) {
                                                music_boss_battle.stop();
                                                if (!music_pouvoir.getStatus()) {
                                                    music_pouvoir.play();
                                                }    
                                                showHero = false;
                                                gameOver = true;
                                                if (!backgroundTexture.loadFromFile("image/yeux.jpg")) {
                                                    std::cerr << "Erreur : Impossible de charger l'image village.jpg !" << std::endl;
                                                    return 1;
                                                }
                                                
                                                text = u8"Le silence s’installe dans la salle du trône dévastée. Le corps de Malakar, l’Ombre Éternelle, s’effondre dans un tourbillon de ténèbres, ne laissant derrière lui qu’un écho lointain de son rire maudit."
                                                "Vous tenez enfin entre vos mains la Pierre de Lumen, son éclat vacillant dans l’obscurité pesante du donjon."
                                                "Mais alors que vous contemplez la Pierre de Lumen, une voix résonne dans votre esprit… une promesse de pouvoir, de domination."
                                                "Pourquoi restaurer un monde faible, alors que vous pourriez le modeler à votre image ?"
                                                "Dans un murmure, vous absorbez l’essence de la pierre. Une onde d’énergie obscure vous traverse, et vos yeux brillent d’un éclat surnaturel."
                                                "Vous quittez le donjon, mais ce n’est pas la lumière que vous ramenez… c’est une nouvelle ère d’ombres qui commence."
                                                "Désormais, c’est vous qui régnez sur la nuit...";
                                            }
                                            if (joueur.getPointsDeVie() > 0 && it2 == inventaire2.end() && ennemi.getPointsDeVie() <= 0) {
                                                music_boss_battle.stop();
                                                if (!music_sacrifice.getStatus()) {
                                                    music_sacrifice.play();
                                                }      
                                                showHero = false;
                                                gameOver = true;
                                                if (!backgroundTexture.loadFromFile("image/bad_end.jpg")) {
                                                    std::cerr << "Erreur : Impossible de charger l'image village.jpg !" << std::endl;
                                                    return 1;
                                                } 
                                                
                                                text = u8"Le silence s’installe dans la salle du trône dévastée. Le corps de Malakar, l’Ombre Éternelle, s’effondre dans un tourbillon de ténèbres, ne laissant derrière lui qu’un écho lointain de son rire maudit."
                                                "Vous tenez enfin entre vos mains la Pierre de Lumen, son éclat vacillant dans l’obscurité pesante du donjon."
                                                "Alors que vous vous apprêtez à quitter le donjon, vous sentez la noirceur de Malakar toujours présente…"
                                                "Vous comprenez alors la vérité : la pierre ne peut être contenue, elle doit être détruite."
                                                "Dans un dernier acte de courage, vous brisez la Pierre de Lumen entre vos mains. Une explosion de lumière consume tout, purifiant le mal… mais aussi vous-même."
                                                "Lorsque les villageois s’approchent du donjon, ils ne trouvent que des ruines baignées d’un éclat lunaire. Votre nom ne sera peut-être jamais chanté, mais le monde, lui, est sauvé.";
                                            }
                                            if( gameOver){
                                                gameOverBackground.setTexture(backgroundTexture);
                                                gameOverBackground.setScale(
                                                    static_cast<float>(videoMode.width) / backgroundTexture.getSize().x,
                                                    static_cast<float>(videoMode.height) / backgroundTexture.getSize().y
                                                );
                                                float maxTextWidth = videoMode.width / 3.0f;
                                                gameOverText.setLineSpacing(1.5f);
                                            
                                                std::string wrappedText;
                                                std::istringstream words(text);
                                                std::string word;
                                                float currentLineWidth = 0.0f;
                                                while (words >> word) {
                                                    sf::Text tempText(word, font, characterSize);
                                                    float wordWidth = tempText.getLocalBounds().width;
                                                    if (currentLineWidth + wordWidth > maxTextWidth) {
                                                        wrappedText += "\n";
                                                        currentLineWidth = 0.0f;
                                                    }
                                                    wrappedText += word + " ";
                                                    currentLineWidth += wordWidth + tempText.getLetterSpacing();
                                                }
                                            
                                                sf::String texteSfml = sf::String::fromUtf8(wrappedText.begin(), wrappedText.end());
                                                gameOverText.setFont(font);
                                                gameOverText.setString(texteSfml);
                                                gameOverText.setCharacterSize(characterSize);
                                                gameOverText.setFillColor(sf::Color::White);
                                            
                                                sf::FloatRect textRect = gameOverText.getLocalBounds();
                                                gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                                                gameOverText.setPosition(sf::Vector2f(videoMode.width / 2.0f, videoMode.height / 2.0f +20));
                                            }
                                        nombre_de_tours++;
                                    }
                                        
                                    mettreAJourInventaire(joueur, inventorySlots, inventorySlotTexts, attackButtons, attackButtonTexts, font);

                                }
                            }
                        }
                    }
                    if (quitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ||
                        quitButtonn.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        window.close();
                    }
                }
            }
        }
        // Animation du héros
        if (showHero && clockheros.getElapsedTime().asSeconds() > frameTime) {
            window.draw(heroSprite);
            frameIndex = (frameIndex + 1) % heroTextures.size();
            heroSprite.setTexture(heroTextures[frameIndex]);
            clockheros.restart();
        }

        // Animation du boss
        if (showboss && clockboss.getElapsedTime().asSeconds() > frameTimeboss) {
            window.draw(bossSprite);
            frameIndexboss = (frameIndexboss + 1) % bossTextures.size();
            bossSprite.setTexture(bossTextures[frameIndexboss]);
            clockboss.restart();
        }

        

        // Mettre à jour la taille de la barre de vie du joueur
        healthBar.setSize(sf::Vector2f(2 * joueur.getPointsDeVie(), 20));

        // Mettre à jour la taille de la barre de vie de l'ennemi
        healthBarEnnemi.setSize(sf::Vector2f(2 * ennemi.getPointsDeVie(), 20));

        window.clear();
        window.draw(backgroundSprite);
        if (showBarbareMagicien) {
            window.draw(text);
            window.draw(barbareButton);
            window.draw(barbareButtonText);
            window.draw(magicienButton);
            window.draw(magicienButtonText);
            window.draw(quitButton);
            window.draw(quitButtonText);
        } else if (!showBarbareMagicien) {


            if (!salleboss) {
                window.draw(backgroundSpritegrotte);
            }
            if (salleboss) {
                window.draw(backgroundSpriteBoss);
                window.draw(bossSprite);
            }
            

            window.draw(inventoryBackground);
            for (const auto& attackButton : attackButtons) {
                window.draw(attackButton);
            }
            for (const auto& attackButtonText : attackButtonTexts) {
                window.draw(attackButtonText);
            }
            for (int i = 0; i < 10; ++i) {
                window.draw(inventorySlots[i]);
                window.draw(inventorySlotTexts[i]);
            }
            if (showHero) {
                window.draw(heroSprite);
                if (!salleboss) {
                    while (afficherMechant(window, mechantsTextures, sf::Vector2f(1500, 500), frametimemechant, frameIndexmechant, quelle_monstre, clock) == -1) { // probleme de chargement d'une des textures recharge tant que probleme
                        quelle_monstre = rand() % mechantsTextures.size();
                    }
                }
                // Draw health bar above the hero sprite
                healthBar.setSize(sf::Vector2f(2 * joueur.getPointsDeVie(), 20)); // Update health bar size
                healthBar.setPosition(heroSprite.getPosition().x + 100, heroSprite.getPosition().y - 30); // Position to the right of the hero sprite
                healthBarBackground.setPosition(heroSprite.getPosition().x + 100, heroSprite.getPosition().y - 30); // Position to the right of the hero sprite
                window.draw(healthBarBackground);
                window.draw(healthBar);
                // Mettre à jour la taille de la barre de vie de l'ennemi
                healthBarEnnemi.setSize(sf::Vector2f(2 * ennemi.getPointsDeVie(), 20)); // Update health bar size
                healthBarEnnemi.setPosition(heroSprite.getPosition().x + heroSprite.getGlobalBounds().width + 1000, heroSprite.getPosition().y - 30); // Position plus à droite du joueur
                healthBarBackgroundEnnemi.setPosition(heroSprite.getPosition().x + heroSprite.getGlobalBounds().width + 1000, heroSprite.getPosition().y - 30); // Position plus à droite du joueur
                window.draw(healthBarBackgroundEnnemi);
                window.draw(healthBarEnnemi);
                
            }
            if (gameOver) {
                window.draw(gameOverBackground);
                window.draw(gameOverText);
            }
            window.draw(quitButtonn);
            window.draw(quitButtonnText);
        }

        window.display();
    }

    return 0;
}


void sauvegarderEtatJeu(int numeroTour, const Joueur& joueur, const string& fichier) {
    ofstream out(fichier, ios::out | ios::trunc);
    if (!out) {
        cerr << "Erreur : Impossible de sauvegarder l'état du jeu." << endl;
        return;
    }
    out << "Tour: " << numeroTour << endl;
    out << "Points de vie: " << joueur.getPointsDeVie() << endl;
    out << "Attaque Physique: " << joueur.getValeurAttaquePhysique() << endl;
    out << "Attaque Magique: " << joueur.getValeurAttaqueMagique() << endl;
    out << "Défense Physique: " << joueur.getDefensePhysique() << endl;
    out << "Défense Magique: " << joueur.getDefenseMagique() << endl;
    out << "Inventaire:" << endl;
    for (const auto& objet : joueur.getInventaire()) {
        out << "- " << objet->nom;
        if (auto arme = dynamic_pointer_cast<Arme>(objet)) {
            out << " (Arme) " << arme->bonusAttaquePhysique << " " << arme->bonusAttaqueMagique;
        } else if (auto armure = dynamic_pointer_cast<Armure>(objet)) {
            out << " (Armure) " << armure->bonusDefensePhysique << " " << armure->bonusDefenseMagique;
        } else if (auto attaqueObjet = dynamic_pointer_cast<AttaqueObjet>(objet)) {
            out << " (Attaque) " << attaqueObjet->attaque.degatPhysique << " " << attaqueObjet->attaque.degatMagique;
        } else if (dynamic_pointer_cast<Potion>(objet)) {
            out << " (Potion)";
        }
        out << endl;
    }
}

int chargerEtatJeu(Joueur& joueur, vector<Attaque>& attaques) {
    ifstream in("sauvegarde.txt", ios::in);
    if (!in) {
        cerr << "Erreur : Impossible de charger l'état du jeu." << endl;
        return 1;
    }
    string ligne;
    int numeroTour = 0;
    while (getline(in, ligne)) {
        istringstream iss(ligne);

        if (ligne.find("Tour:") == 0) {
            iss.ignore(5); // Ignore "Tour: "
            iss >> numeroTour;
        } else if (ligne.find("Points de vie:") == 0) {
            iss.ignore(14);
            int pointsDeVie;
            if (iss >> pointsDeVie) {
                joueur.setPointsDeVie(pointsDeVie);
            }
        } else if (ligne.find("Attaque Physique:") == 0) {
            iss.ignore(18);
            int attaquePhysique;
            if (iss >> attaquePhysique) {
                joueur.setValeurAttaquePhysique(attaquePhysique);
            }
        } else if (ligne.find("Attaque Magique:") == 0) {
            iss.ignore(17);
            int attaqueMagique;
            if (iss >> attaqueMagique) {
                joueur.setValeurAttaqueMagique(attaqueMagique);
            }
        } else if (ligne.find("Défense Physique:") == 0) {
            iss.ignore(18);
            int defensePhysique;
            if (iss >> defensePhysique) {
                joueur.setDefensePhysique(defensePhysique);
            }
        } else if (ligne.find("Défense Magique:") == 0) {
            iss.ignore(17);
            int defenseMagique;
            if (iss >> defenseMagique) {
                joueur.setDefenseMagique(defenseMagique);
            }
        } else if (ligne.find("Inventaire:") == 0) {
            joueur.clearInventaire();
        }else if (ligne.find("- ") == 0) {  // Objet d'inventaire
            string objet = ligne.substr(2); // Retirer "- "
            if (objet.find("Arme") != string::npos) {
                // Arme
                string nomArme = objet.substr(0, objet.find(" (Arme)"));
                size_t pos = objet.find(" (Arme) ") + 8; // Position after " (Arme) "
                int bonusAttaquePhysique = stoi(objet.substr(pos, objet.find(' ', pos) - pos));
                pos = objet.find(' ', pos) + 1;
                int bonusAttaqueMagique = stoi(objet.substr(pos));
                cout<<objet << " Arme: " << nomArme << " " << bonusAttaquePhysique << " " << bonusAttaqueMagique << endl;
                auto arme = make_shared<Arme>(nomArme, bonusAttaquePhysique, bonusAttaqueMagique);
                joueur.ajouterObjet(arme);
                joueur.equiperArme(arme);
            } else if (objet.find("Armure") != string::npos) {
                // Armure
                string nomArmure = objet.substr(0, objet.find(" (Armure)"));
                size_t pos = objet.find(" (Armure) ") + 10; // Position after " (Armure) "
                int bonusDefensePhysique = stoi(objet.substr(pos, objet.find(' ', pos) - pos));
                pos = objet.find(' ', pos) + 1;
                int bonusDefenseMagique = stoi(objet.substr(pos));
                cout<<objet << " Armure: " << nomArmure << " " << bonusDefensePhysique << " " << bonusDefenseMagique << endl;
                auto armure = make_shared<Armure>(nomArmure, bonusDefensePhysique, bonusDefenseMagique);
                joueur.ajouterObjet(armure);
                joueur.equiperArmure(armure);

            } else if (objet.find("Attaque") != string::npos) {

                string nomAttaque = objet.substr(0, objet.find(" (Attaque)"));
                size_t pos = objet.find(" (Attaque) ") + 11; // Position after " (Attaque) "
                int degatPhysique = stoi(objet.substr(pos, objet.find(' ', pos) - pos));
                pos = objet.find(' ', pos) + 1;
                int degatMagique = stoi(objet.substr(pos));
                cout<<objet << " Attaque: " << nomAttaque << " " << degatPhysique << " " << degatMagique << endl;
                auto attaque = Attaque(nomAttaque, degatPhysique, degatMagique);
                attaques.push_back(attaque);
                auto attaqueObjet = make_shared<AttaqueObjet>(attaque);
                joueur.ajouterObjet(attaqueObjet);
            } else if (objet.find("Potion") != string::npos) {
                // Potion
                auto potion = make_shared<Potion>();
                joueur.ajouterObjet(potion);
            }
        }
    }
    return numeroTour;
}


int main() {
    int choixInterface;
    cout << "Choisissez votre interface :" << endl;
    cout << "1. Interface Graphique" << endl;
    cout << "2. Interface Console" << endl;
    cin >> choixInterface;
    if (choixInterface == 1) {
        interface();
    } else {
        //partie terminale
        cout<<endl;
        cout << "Dans un monde ravagé par la guerre et la corruption, les anciennes légendes parlent d’un artefact perdu, la Pierre de Lumen, capable de restaurer l’équilibre entre la lumière et les ténèbres. "<<endl;
        cout<< "Cet artefact est caché dans le Donjon du Néant, une forteresse maudite où résident des créatures cauchemardesques et leur maître impitoyable : Malakar, l’Ombre Éternelle."<<endl;
        cout<< "Tuer 10 de ces généraux devrait être suffisant pour éveiller sa colère..."<<endl;
        cout<< "Vous incarnez un aventurier solitaire, chargé d’infiltrer ce donjon maudit pour récupérer la pierre avant que les ténèbres ne consument définitivement le royaume." << endl;
        cout<<endl;
        srand(time(nullptr));
        vector<Attaque> attaques = chargerAttaques("attaques.txt");
        vector<shared_ptr<Arme>> armes = chargerArmes("armes.txt");
        vector<shared_ptr<Armure>> armures = chargerArmures("armures.txt");

        int choixClasse;
        cout << "Choisissez votre classe :" << endl;
        cout << "1. Barbare (dégâts physiques accrus)" << endl;
        cout << "2. Magicien (dégâts magiques accrus)" << endl;
        cin >> choixClasse;

        Joueur joueur("Joueur", 100, 10, 10, 5, 5);

        // Ajouter une attaque aléatoire à l'inventaire du joueur
        if (!attaques.empty()) {
            int numero_attaque = rand() % attaques.size();
            auto attaque_heros = make_shared<AttaqueObjet>(attaques[numero_attaque]);
            joueur.ajouterObjet(attaque_heros);
        }

        shared_ptr<Arme> arme;
        shared_ptr<Armure> armure;
        if (choixClasse == 1) {
            // Barbare : bonus de dégâts physiques
            joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);
            arme = make_shared<Arme>("Épée", 5, 0);
            armure = make_shared<Armure>("Armure de l'apprenti soldat", 5, 0);
        } else if (choixClasse == 2) {
            // Magicien : bonus de dégâts magiques
            joueur.setValeurAttaqueMagique(joueur.getValeurAttaqueMagique() + 15);
            arme = make_shared<Arme>("Bâton magique", 0, 5);
            armure = make_shared<Armure>("Armure de l'apprenti magicien", 0, 5);
        } else {
            cout << "Choix invalide! Barbare choisi par défaut." << endl;
            joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);
            arme = make_shared<Arme>("Épée", 5, 0);
            armure = make_shared<Armure>("Armure de l'apprenti soldat", 5, 0);
        }

        // Équiper l'arme et l'armure
        joueur.equiperArmure(armure);
        joueur.ajouterObjet(armure);
        joueur.equiperArme(arme);
        joueur.ajouterObjet(arme);

        auto potion = make_shared<Potion>();
        joueur.ajouterObjet(potion);

        int nombre_de_tours = 0;

        while (nombre_de_tours < 10) {
            // Clear the terminal
            string nomAleatoiree = nomAleatoire();
            cout << "Vous avez rencontré un " << nomAleatoiree << " sur votre chemin!" << endl;
            // Display the current turn
            cout << "Nombre de monstre rencontré: " << nombre_de_tours + 1 << endl;
            cout << "Vie restante du joueur :"<< joueur.getPointsDeVie() << endl;
            cout << "Inventaire du joueur:" << endl;
            for (const auto& objet : joueur.getInventaire()) {
                cout << "- " << objet->nom;
                if (dynamic_pointer_cast<Arme>(objet)) {
                cout << " (Arme)";
                } else if (dynamic_pointer_cast<Armure>(objet)) {
                cout << " (Armure)";
                } else if (dynamic_pointer_cast<AttaqueObjet>(objet)) {
                cout << " (Attaque)";
                } else if (dynamic_pointer_cast<Potion>(objet)) {
                cout << " (Potion)";
                }
                cout << endl;
            }

            cout<<"Voulez vous sauvegarder la partie ? (0: Oui), Charger la dernière partie sauvegardée ? (1: Oui), Continuer à jouer ? (2: Oui)"<<endl;
            int choix_sauvegarde;
            cin>>choix_sauvegarde;
            if(choix_sauvegarde == 0){
                sauvegarderEtatJeu(nombre_de_tours, joueur, "sauvegarde.txt");
                cout<<"Partie sauvegardée"<<endl;
            }
            if(choix_sauvegarde == 1){
                nombre_de_tours = chargerEtatJeu(joueur, attaques);

                cout<<"Partie chargée"<<endl;
            }
            Personnage ennemi("Ennemi", 50, 8, 8, 3, 3);

            joueur.rencontrerEnnemi(ennemi, attaques, nomAleatoiree);
            if (joueur.getPointsDeVie() == 0) {
                break;
            }
            cout<<endl;
            int chance = rand() % 4;
            if (chance == 0) {
                // Ajouter une nouvelle attaque au hasard suite au combat contre le monstre
                if (!attaques.empty()) {
                    int numero_attaque = rand() % attaques.size();
                    auto nouvelle_attaque = make_shared<AttaqueObjet>(attaques[numero_attaque]);
                    cout << "Vous avez de la chance aventurier, le monstre avait sur lui une nouvelle attaque!" << endl;
                    cout << "Vous avez obtenu : " << nouvelle_attaque->nom << endl;
                    joueur.ajouterObjet(nouvelle_attaque);
                }
            } //vous looter l'armure que cachait le monstre
            else if (chance == 1) {
                if (!armures.empty()) {
                    auto armureAleatoire = armures[rand() % armures.size()];
                    cout << "Vous avez trouvé une superbe armure qui appartenait à cette horible créature : " << armureAleatoire->nom << ", Bonus Défense Physique : " << armureAleatoire->bonusDefensePhysique<< ", Bonus Défense Magique : " << armureAleatoire->bonusDefenseMagique<< endl;
                    cout << "Stats de l'armure : " << endl;
                    if (joueur.getArmureEquipee()) {
                        cout << "Votre armure actuelle : " << joueur.getArmureEquipee()->nom << ", Bonus défense Physique : " << joueur.getArmureEquipee()->bonusDefensePhysique << ", Bonus défense Magique : " << joueur.getArmureEquipee()->bonusDefenseMagique<< endl;
                    } else {
                        cout << "Vous n'avez pas d'armure équipée actuellement." << endl;
                    }
                    cout << "Voulez-vous équiper la nouvelle armure ? (1: Oui, 0: Non)" << endl;
                    int choix;
                    cin >> choix;
                    if (choix == 1) {
                        if (joueur.getArmureEquipee()) {
                            auto& inventaire = joueur.getInventaire();
                            inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                    [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmureEquipee(); }),
                                            inventaire.end());
                            joueur.desequiperArmure();
                        }
                        joueur.ajouterObjet(armureAleatoire);
                        joueur.equiperArmure(armureAleatoire);
                        cout << "Vous avez équipé l'armure: " << armureAleatoire->nom << endl;
                    } else {
                        cout << "Vous avez décidé de ne pas équiper la nouvelle armure." << endl;
                    }
                }
            } //vous looter l'arme que cachait le monstre
            else if (chance == 2) {
                if (!armes.empty()) {
                    auto armeAleatoire = armes[rand() % armes.size()];
                    cout << "Cette horible créature cachait une incroyable arme : " << armeAleatoire->nom << ", Bonus Attaque Physique : " << armeAleatoire->bonusAttaquePhysique << ", Bonus Attaque Magique : " << armeAleatoire->bonusAttaqueMagique << endl;
                    if (joueur.getArmeEquipee()) {
                        cout << "Votre arme actuelle : " << joueur.getArmeEquipee()->nom << ", Bonus Attaque Physique : " << joueur.getArmeEquipee()->bonusAttaquePhysique << ", Bonus Attaque Magique : " << joueur.getArmeEquipee()->bonusAttaqueMagique << endl;
                    } else {
                        cout << "Vous n'avez pas d'arme équipée actuellement." << endl;
                    }
                    cout << "Voulez-vous équiper la nouvelle arme ? (1: Oui, 0: Non)" << endl;
                    int choix;
                    cin >> choix;
                    if (choix == 1) {
                        if (joueur.getArmeEquipee()) {
                            auto& inventaire = joueur.getInventaire();
                            inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                    [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmeEquipee(); }),
                                            inventaire.end());
                            joueur.desequiperArme();
                        }
                        joueur.ajouterObjet(armeAleatoire);
                        joueur.equiperArme(armeAleatoire);
                        cout << "Vous avez équipé l'arme: " << armeAleatoire->nom << endl;
                    } else {
                        cout << "Vous avez décidé de ne pas équiper la nouvelle arme." << endl;
                    }
                }
            }//trouve qqch sur le chemin
            else if (chance == 3) {
                int chance_chemin = rand() % 3;
                if (chance_chemin == 0) {
                    if (!armures.empty()) {
                        auto armureAleatoire = armures[rand() % armures.size()];
                        cout << "Vous avez trouvé une vielle armure dans une grotte : " << armureAleatoire->nom << ", Bonus Défense Physique : " << armureAleatoire->bonusDefensePhysique<< ", Bonus Défense Magique : " << armureAleatoire->bonusDefenseMagique<< endl;
                        cout << "Stats de l'armure : " << endl;
                        if (joueur.getArmureEquipee()) {
                            cout << "Votre armure actuelle : " << joueur.getArmureEquipee()->nom << ", Bonus défense Physique : " << joueur.getArmureEquipee()->bonusDefensePhysique << ", Bonus défense Magique : " << joueur.getArmureEquipee()->bonusDefenseMagique<< endl;
                        } else {
                            cout << "Vous n'avez pas d'armure équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle armure ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmureEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmureEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArmure();
                            }
                            joueur.ajouterObjet(armureAleatoire);
                            joueur.equiperArmure(armureAleatoire);
                            cout << "Vous avez équipé l'armure: " << armureAleatoire->nom << endl;
                        } else {
                            cout << "Vous avez décidé de ne pas équiper la nouvelle armure." << endl;
                        }
                    }
                }
                else if (chance_chemin == 1) {
                    if (!armes.empty()) {
                        auto armeAleatoire = armes[rand() % armes.size()];
                        cout << "Vous trébuché sur quelque chose durant votre voyage... : " << armeAleatoire->nom << ", Bonus Attaque Physique : " << armeAleatoire->bonusAttaquePhysique << ", Bonus Attaque Magique : " << armeAleatoire->bonusAttaqueMagique << endl;
                        if (joueur.getArmeEquipee()) {
                            cout << "Votre arme actuelle : " << joueur.getArmeEquipee()->nom << ", Bonus Attaque Physique : " << joueur.getArmeEquipee()->bonusAttaquePhysique << ", Bonus Attaque Magique : " << joueur.getArmeEquipee()->bonusAttaqueMagique << endl;
                        } else {
                            cout << "Vous n'avez pas d'arme équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle arme ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmeEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmeEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArme();
                            }
                            joueur.ajouterObjet(armeAleatoire);
                            joueur.equiperArme(armeAleatoire);
                            cout << "Vous avez équipé l'arme: " << armeAleatoire->nom << endl;
                        } else {
                            cout << "Vous avez décidé de ne pas équiper la nouvelle arme." << endl;
                        }
                    }  
                }
                else {
                    cout << "Sur le chemin, vous vous rappelez les conseils de votre vieux maître et préparez une potion avec des herbes médicinales. " << endl;
                    joueur.ajouterObjet(potion);            
                }
            }
            else {
                // Le joueur rencontre un ami qui lui donne un objet au choix
                cout << "Vous rencontrez un ami sur le chemin. Il vous propose de choisir un objet :" << endl;
                cout << "1. Une arme" << endl;
                cout << "2. Une armure" << endl;
                cout << "3. Une potion" << endl;
                int choix;
                cin >> choix;

                shared_ptr<Objet> nouvelObjet;
                if (choix == 1) {
                    if (!armes.empty()) {
                        auto armeAleatoire = armes[rand() % armes.size()];
                        cout << "Votre meilleur amis à cette arme : " << armeAleatoire->nom << ", Bonus Attaque Physique : " << armeAleatoire->bonusAttaquePhysique << ", Bonus Attaque Magique : " << armeAleatoire->bonusAttaqueMagique << endl;
                        if (joueur.getArmeEquipee()) {
                            cout << "Votre arme actuelle : " << joueur.getArmeEquipee()->nom << ", Bonus Attaque Physique : " << joueur.getArmeEquipee()->bonusAttaquePhysique << ", Bonus Attaque Magique : " << joueur.getArmeEquipee()->bonusAttaqueMagique << endl;
                        } else {
                            cout << "Vous n'avez pas d'arme équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle arme ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmeEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmeEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArme();
                            }
                            joueur.ajouterObjet(armeAleatoire);
                            joueur.equiperArme(armeAleatoire);
                            cout << "Vous avez équipé l'arme: " << armeAleatoire->nom << endl;
                        } else {
                            cout << "Vous avez décidé de ne pas équiper la nouvelle arme." << endl;
                        }
                    }
                } else if (choix == 2) {
                    // Créer une nouvelle armure au hasard depuis armures.txt
                    if (!armures.empty()) {
                        auto armureAleatoire = armures[rand() % armures.size()];
                        cout << "Votre fabuleux amis à cette armure : " << armureAleatoire->nom << ", Bonus Défense Physique : " << armureAleatoire->bonusDefensePhysique<< ", Bonus Défense Magique : " << armureAleatoire->bonusDefenseMagique<< endl;
                        if (joueur.getArmureEquipee()) {
                            cout << "Votre armure actuelle : " << joueur.getArmureEquipee()->nom << ", Bonus défense Physique : " << joueur.getArmureEquipee()->bonusDefensePhysique << ", Bonus défense Magique : " << joueur.getArmureEquipee()->bonusDefenseMagique<< endl;
                        } else {
                            cout << "Vous n'avez pas d'armure équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle armure ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmureEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmureEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArmure();
                            }
                            joueur.ajouterObjet(armureAleatoire);
                            joueur.equiperArmure(armureAleatoire);
                            cout << "Vous avez équipé l'armure: " << armureAleatoire->nom << endl;
                        }
                } else if (choix == 3) {
                    cout << "Votre ami vous donne une potion." << endl;
                    joueur.ajouterObjet(potion);
                } else {
                    cout << "Choix invalide! Vous ne recevez aucun objet." << endl;
                }

                if (nouvelObjet) {
                    joueur.ajouterObjet(nouvelObjet);
                    cout << "Votre ami vous a donné: " << nouvelObjet->nom << endl;
                }
            }
        }
            this_thread::sleep_for(chrono::seconds(5));
            std::cout << std::string(100, '\n'); // Affiche 100 sauts de ligne
            nombre_de_tours++;
        }

        cout<<"------------------------------------------------------------"<<endl;
        cout<<"Aventurier, votre combat touche à sa fin. Soudain, l'air devient pesant et, derrière vous, apparaît le boss du donjon : Malakar, l'Ombre Éternelle."<<endl;
        cout<<"------------------------------------------------------------"<<endl;
        cout << "Vie restante du joueur :"<< joueur.getPointsDeVie() << endl;
            cout << "Inventaire du joueur:" << endl;
            for (const auto& objet : joueur.getInventaire()) {
                cout << "- " << objet->nom;
                if (dynamic_pointer_cast<Arme>(objet)) {
                cout << " (Arme)";
                } else if (dynamic_pointer_cast<Armure>(objet)) {
                cout << " (Armure)";
                } else if (dynamic_pointer_cast<AttaqueObjet>(objet)) {
                cout << " (Attaque)";
                } else if (dynamic_pointer_cast<Potion>(objet)) {
                cout << " (Potion)";
                }
                cout << endl;
            }
        Personnage ennemi("Ennemi", 100, 15, 15, 5, 5);
        joueur.rencontrerEnnemi(ennemi, attaques,"Malakar, l'Ombre Éternelle");
        auto inventaire = joueur.getInventaire();
        auto it = find_if(inventaire.begin(), inventaire.end(), [](const shared_ptr<Objet>& obj) {
            return dynamic_pointer_cast<Potion>(obj) != nullptr;
        });

        // Affiche le résultat final
        if (joueur.getPointsDeVie() <= 0) {
            cout << "Le sol tremble sous vos pieds. Malakar, l’Ombre Éternelle, bien que blessé, se redresse lentement, un sourire sinistre aux lèvres."<<endl;
            cout<<"Son regard brûlant de malice transperce votre âme. Vous tentez une dernière attaque, mais votre corps ne répond plus."<<endl;
            cout <<"L’air est devenu trop lourd, l’obscurité trop oppressante."<<endl;

            cout << 'Un rire glacial résonne dans la salle du trône. "Tu as été un adversaire valeureux, aventurier… mais ici, c’est MOI qui dicte la fin de cette histoire."'<<endl;


            cout << "Des ombres s’élèvent du sol, s’enroulant autour de vous comme des chaînes vivantes. Vous luttez, mais la force vous abandonne."<<endl;
            cout <<"Peu à peu, la lumière quitte vos yeux, votre essence absorbée par l’abîme sans fin de Malakar."<<endl;

            cout <<"Le monde extérieur ne sait rien de ce qui vient de se produire. Les villageois attendent, espérant voir leur héros revenir. Mais il ne reviendra jamais."<<endl;

            cout <<"La nuit s’étend, plus sombre et plus froide que jamais. Dans les jours qui suivent, les ténèbres s’abattent sur le royaume. "<<endl;

            cout<<"Au sommet de son trône d’ombres, Malakar règne désormais sans opposition."<<endl;

            cout<<"L’ombre a triomphé." << endl;
        } else{
                cout <<"Le silence s’installe dans la salle du trône dévastée. Le corps de Malakar, l’Ombre Éternelle, s’effondre dans un tourbillon de ténèbres, ne laissant derrière lui qu’un écho lointain de son rire maudit."<<endl;
                cout<<"Vous tenez enfin entre vos mains la Pierre de Lumen, son éclat vacillant dans l’obscurité pesante du donjon."<<endl;
                if (joueur.getPointsDeVie() > 0 && it != inventaire.end()) { //fin malveillante
                    cout << "Mais alors que vous contemplez la Pierre de Lumen, une voix résonne dans votre esprit… une promesse de pouvoir, de domination."<<endl;
                    cout<<"Pourquoi restaurer un monde faible, alors que vous pourriez le modeler à votre image ?"<<endl;

                    cout <<"Dans un murmure, vous absorbez l’essence de la pierre. Une onde d’énergie obscure vous traverse, et vos yeux brillent d’un éclat surnaturel."<<endl;
                    cout<<"Vous quittez le donjon, mais ce n’est pas la lumière que vous ramenez… c’est une nouvelle ère d’ombres qui commence."<<endl;
                    cout<<"Désormais, c’est vous qui régnez sur la nuit..." << endl;
                }
                else{
                    cout<<"Alors que vous vous apprêtez à quitter le donjon, vous sentez la noirceur de Malakar toujours présente…"<<endl;
                    cout<<"Vous comprenez alors la vérité : la pierre ne peut être contenue, elle doit être détruite."<<endl;
                    cout<<"Dans un dernier acte de courage, vous brisez la Pierre de Lumen entre vos mains. Une explosion de lumière consume tout, purifiant le mal… mais aussi vous-même."<<endl;
                    cout<<"Lorsque les villageois s’approchent du donjon, ils ne trouvent que des ruines baignées d’un éclat lunaire. Votre nom ne sera peut-être jamais chanté, mais le monde, lui, est sauvé."<<endl;
                }
        }


    }
    return 0;
}

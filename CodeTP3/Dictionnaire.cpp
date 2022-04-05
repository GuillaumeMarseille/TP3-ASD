/**
 * \file Dictionnaire.cpp
 * \brief Ce fichier contient une implantation des méthodes de la classe Dictionnaire
 * \author IFT-2008, Étudiant(e)
 * \version 0.1
 * \date avril 2022
 *
 */

#include "Dictionnaire.h"

// Limite du nombre de suggestions
#define LIMITE_SUGGESTIONS 5

namespace TP3 {
    Dictionnaire::Dictionnaire() : racine(0), cpt(0) {}

    Dictionnaire::Dictionnaire(std::ifstream &fichier) : racine(nullptr), cpt(0) {
        if (fichier) {
            for (std::string ligneDico; getline(fichier, ligneDico);) {
                if (ligneDico[0] != '#') //Élimine les lignes d'en-tête
                {
                    // Le mot anglais est avant la tabulation (\t).
                    std::string motAnglais = ligneDico.substr(0, ligneDico.find_first_of('\t'));

                    // Le reste (définition) est après la tabulation (\t).
                    std::string motTraduit = ligneDico.substr(motAnglais.length() + 1, ligneDico.length() - 1);

                    //On élimine tout ce qui est entre crochets [] (possibilité de 2 ou plus)
                    std::size_t pos = motTraduit.find_first_of('[');
                    while (pos != std::string::npos) {
                        std::size_t longueur_crochet = motTraduit.find_first_of(']') - pos + 1;
                        motTraduit.replace(pos, longueur_crochet, "");
                        pos = motTraduit.find_first_of('[');
                    }

                    //On élimine tout ce qui est entre deux parenthèses () (possibilité de 2 ou plus)
                    pos = motTraduit.find_first_of('(');
                    while (pos != std::string::npos) {
                        std::size_t longueur_crochet = motTraduit.find_first_of(')') - pos + 1;
                        motTraduit.replace(pos, longueur_crochet, "");
                        pos = motTraduit.find_first_of('(');
                    }

                    //Position d'un tilde, s'il y a lieu
                    std::size_t posT = motTraduit.find_first_of('~');

                    //Position d'un tilde, s'il y a lieu
                    std::size_t posD = motTraduit.find_first_of(':');

                    if (posD < posT) {
                        //Quand le ':' est avant le '~', le mot français précède le ':'
                        motTraduit = motTraduit.substr(0, posD);
                    } else {
                        //Quand le ':' est après le '~', le mot français suit le ':'
                        if (posT < posD) {
                            motTraduit = motTraduit.substr(posD, motTraduit.find_first_of("([,;\n", posD));
                        } else {
                            //Quand il n'y a ni ':' ni '~', on extrait simplement ce qu'il y a avant un caractère de limite
                            motTraduit = motTraduit.substr(0, motTraduit.find_first_of("([,;\n"));
                        }
                    }

                    //On ajoute le mot au dictionnaire
                    ajouteMot(motAnglais, motTraduit);
                    //std::cout<<motAnglais << " - " << motTraduit<<std::endl;
                }
            }
        }
    }

    Dictionnaire::~Dictionnaire() {
        _detruire(racine);
    }

    void Dictionnaire::ajouteMot(const std::string &motOriginal, const std::string &motTraduit) {
        if (!appartient(motOriginal));
    }

    void Dictionnaire::supprimeMot(const std::string &motOriginal) {

    }

    double Dictionnaire::similitude(const std::string &mot1, const std::string &mot2) {
        return 1.0;
    }

    std::vector<std::string> Dictionnaire::suggereCorrections(const std::string &motMalEcrit) {
        std::vector<std::string> suggestions;
        return suggestions;
    }

    std::vector<std::string> Dictionnaire::traduit(const std::string &mot) {
        std::vector<std::string> traductions;
        return traductions;
    }

    bool Dictionnaire::appartient(const std::string &mot) {
        return (_appartient(racine, mot) != 0);
    }

    bool Dictionnaire::estVide() const {
        return cpt == 0;
    }

    // Complétez ici l'implémentation avec vos méthodes privées.
    void Dictionnaire::_detruire(NoeudDictionnaire *&p_root) {
        if (p_root != 0) { // si l'arbre existe.
            _detruire(p_root->gauche); //recursive call gauche
            _detruire(p_root->droite); //recursive call droit
            delete p_root;
            p_root = 0;

        }
    }

    Dictionnaire::NoeudDictionnaire *Dictionnaire::_appartient
            (NoeudDictionnaire *const &p_root, const std::string &mot) const {
        //Cas arbre vide
        if (p_root == 0) {
            return 0;
        }
        //Cas mot dans racine
        if (p_root->mot == mot) {
            return p_root;
        }
        //Cas mot plus petit que mot racine. Debut recursivite.
        if (p_root->mot > mot) {
            return _appartient(p_root->gauche, mot);
        }
            //Cas mot plus grand que mot racine. Debut recursivite.
        else {
            return _appartient(p_root->droite, mot);
        }
        //Si on retourne 0, c'est qu'on est arrive a un noeud vide, donc le mot n'est pas dans le dico.
    }

    void Dictionnaire::_majHauteur(NoeudDictionnaire *&p_root) {
        //Si l'arbre n'est pas vide, hauteur est 1 + hauteur de la plus longue branch du noeud.
        if (p_root != 0) {
            p_root->hauteur = 1 + std::max(_hauteur(p_root->gauche), _hauteur(p_root->droite));
        }
    }

    int Dictionnaire::_hauteur(NoeudDictionnaire * &p_root) const
    {
        //Cas ou l'arbre est vide
        if (p_root == 0) {
            return -1;
        }
        //Sinon simplement retourner l'attribut heuteur
        return p_root->hauteur;
    }

    void Dictionnaire::_zigZigGauche(NoeudDictionnaire * &p_noeudCritique)
    {
        //On sauvegarde le noeud SC dans une variable
        NoeudDictionnaire * noeudSousCritique = p_noeudCritique->gauche;

        //Debut de la rotation SC devient son frere de droit, qui lui devient NC
        p_noeudCritique->gauche = noeudSousCritique->droite;
        noeudSousCritique->droite = p_noeudCritique;

        //On met a jours la hauteur de NC et SC
        _majHauteur(p_noeudCritique);
        _majHauteur(noeudSousCritique);

        //Fin de la rotation, NC devient SC
        p_noeudCritique = noeudSousCritique;
    }
    void Dictionnaire::_zigZagGauche(NoeudDictionnaire * &p_noeudCritique)
    {
        //On fait une rotation a droite
        _zigZigDroite(p_noeudCritique->gauche);
        //Ensuite rotation a gauche
        _zigZigGauche(p_noeudCritique);
    }
    void Dictionnaire::_zigZigDroite(NoeudDictionnaire * &p_noeudCritique)
    {
        //Comme pour zigZigGauche, mais rotation a droite.
        NoeudDictionnaire * noeudSousCritique = p_noeudCritique->droite;
        p_noeudCritique->droite = noeudSousCritique->gauche;
        noeudSousCritique->gauche = p_noeudCritique;

        _majHauteur(p_noeudCritique);
        _majHauteur(noeudSousCritique);

        p_noeudCritique = noeudSousCritique;
    }
    void Dictionnaire::_zigZagDroite(NoeudDictionnaire * &p_noeudCritique)
    {
        //Rotation a gauche suivi de rotation a droite
        _zigZigGauche(p_noeudCritique->droite);
        _zigZigDroite(p_noeudCritique);
    }

}//Fin du namespace

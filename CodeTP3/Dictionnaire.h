/**
 * \file Dictionnaire.h
 * \brief Ce fichier contient l'interface d'un dictionnaire.
 * \author IFT-2008, Étudiant(e)
 * \version 0.1
 * \date avril 2022
 *
 */


#ifndef DICO_H_
#define DICO_H_

#include <iostream>
#include <fstream> // pour les fichiers
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

namespace TP3 {

//classe représentant un dictionnaire des synonymes
    class Dictionnaire {
    public:

        //Constructeur
        Dictionnaire();

        //Constructeur de dictionnaire à partir d'un fichier
        //Le fichier doit être ouvert au préalable
        Dictionnaire(std::ifstream &fichier);

        //Destructeur.
        ~Dictionnaire();

        //Ajouter un mot au dictionnaire et l'une de ses traductions en équilibrant l'arbre AVL
        void ajouteMot(const std::string &motOriginal, const std::string &motTraduit);

        //Supprimer un mot et équilibrer l'arbre AVL
        //Si le mot appartient au dictionnaire, on l'enlève et on équilibre. Sinon, on ne fait rien.
        //Exception	logic_error si l'arbre est vide
        //Exception	logic_error si le mot n'appartient pas au dictionnaire
        void supprimeMot(const std::string &motOriginal);

        //Quantifier la similitude entre 2 mots (dans le dictionnaire ou pas)
        //Ici, 1 représente le fait que les 2 mots sont identiques, 0 représente le fait que les 2 mots sont complètements différents
        //On retourne une valeur entre 0 et 1 quantifiant la similarité entre les 2 mots donnés
        //Vous pouvez utiliser par exemple la distance de Levenshtein, mais ce n'est pas obligatoire !
        double similitude(const std::string &mot1, const std::string &mot2S) const;


        //Suggère des corrections pour le mot motMalEcrit sous forme d'une liste de mots, dans un vector, à partir du dictionnaire
        //S'il y a suffisament de mots, on redonne 5 corrections possibles au mot donné. Sinon, on en donne le plus possible
        //Exception	logic_error si le dictionnaire est vide
        std::vector<std::string> suggereCorrections(const std::string &motMalEcrit);

        //Trouver les traductions possibles d'un mot
        //Si le mot appartient au dictionnaire, on retourne le vecteur des traductions du mot donné.
        //Sinon, on retourne un vecteur vide
        std::vector<std::string> traduit(const std::string &mot);

        //Vérifier si le mot donné appartient au dictionnaire
        //On retourne true si le mot est dans le dictionnaire. Sinon, on retourne false.
        bool appartient(const std::string &data);

        //Vérifier si le dictionnaire est vide
        bool estVide() const;

        //Affiche à l'écran l'arbre niveau par niveau de façon à voir si l'arbre est bien balancé.
        //Ne touchez pas s.v.p. à cette méthode !
        friend std::ostream &operator<<(std::ostream &out, const Dictionnaire &d) {
            if (d.racine != 0) {
                std::queue<NoeudDictionnaire *> file;
                std::queue<std::string> fileNiveau;

                NoeudDictionnaire *noeudDicotemp;
                std::string niveauTemp;

                int hg = 0, hd = 0;

                file.push(d.racine);
                fileNiveau.push("1");
                while (!file.empty()) {
                    noeudDicotemp = file.front();
                    niveauTemp = fileNiveau.front();
                    out << noeudDicotemp->mot;
                    if (noeudDicotemp->gauche == 0) hg = -1; else hg = noeudDicotemp->gauche->hauteur;
                    if (noeudDicotemp->droite == 0) hd = -1; else hd = noeudDicotemp->droite->hauteur;
                    out << ", " << hg - hd;
                    out << ", " << niveauTemp;
                    out << std::endl;
                    file.pop();
                    fileNiveau.pop();
                    if (noeudDicotemp->gauche != 0) {
                        file.push(noeudDicotemp->gauche);
                        fileNiveau.push(niveauTemp + ".1");
                    }
                    if (noeudDicotemp->droite != 0) {
                        file.push(noeudDicotemp->droite);
                        fileNiveau.push(niveauTemp + ".2");
                    }
                }
            }
            return out;
        }

        //Methode publique ajoutee:
        bool estBalancee() const;

    private:

        // Classe interne représentant un noeud dans l'arbre AVL constituant le dictionnaire de traduction.
        class NoeudDictionnaire {
        public:

            std::string mot;                        // Un mot (en anglais)

            std::vector<std::string> traductions;    // Les différentes traductions possibles en français du mot en anglais
            // Par exemple, la liste française { "contempler", "envisager" et "prévoir" }
            // pourrait servir de traduction du mot anglais "contemplate".

            NoeudDictionnaire *gauche, *droite;        // Les enfants du noeud

            int hauteur;                            // La hauteur de ce noeud (afin de maintenir l'équilibre de l'arbre AVL)

            // Vous pouvez ajouter ici un contructeur de NoeudDictionnaire
            NoeudDictionnaire(const std::string &motOriginal, const std::string &motTraduit):
            mot(motOriginal), gauche(0), droite(0), traductions(0)
            {
                traductions.push_back(motTraduit);
            }
        };

        NoeudDictionnaire *racine;        // La racine de l'arbre des mots

        int cpt;                        // Le nombre de mots dans le dictionnaire

        //Vous pouvez ajouter autant de méthodes privées que vous voulez

        //Fonction pour detruire un dico
        void _detruire(NoeudDictionnaire *&p_root);

        //Verifie si un mot est deja dans le dico
        NoeudDictionnaire *_appartient(NoeudDictionnaire * const &p_root, const std::string &mot) const;

        //Met la hauteur de l'arbre a jour.
        void _majHauteur(NoeudDictionnaire *&p_root);

        //Retourne la hauteur d'un noeud
        int _hauteur(NoeudDictionnaire * &p_root) const;

        //Tous les zigzig et zigzag pour faire les rotations
        void _zigZigGauche(NoeudDictionnaire * &p_noeudCritique);
        void _zigZagGauche(NoeudDictionnaire * &p_noeudCritique);
        void _zigZigDroite(NoeudDictionnaire * &p_noeudCritique);
        void _zigZagDroite(NoeudDictionnaire * &p_noeudCritique);

        //Met a jours la hauteur d'un noeud
        void _miseAJourHauteurNoeud(NoeudDictionnaire * &p_root);

        //Fonction qui balance l'arbre AVL en utilisant les zigzig/zigzag
        void _balancerUnNoeud(NoeudDictionnaire * &p_root);

        //Fonctions pour verifier debalancement de NC et NSC
        bool _debalancementAGauche(NoeudDictionnaire * &p_root) const;
        bool _debalancementADroite(NoeudDictionnaire * &p_root) const;
        bool _sousArbrePencheAGauche(NoeudDictionnaire * &p_root) const;
        bool _sousArbrePencheADroite(NoeudDictionnaire * &p_root) const;
        //Fonction recursive pour ajouter mot
        void _insererAVL(NoeudDictionnaire * &p_root, const std::string &motOriginal, const std::string &motTraduit);
        //Fonction recursive pour ajouter une traduction
        NoeudDictionnaire *& _trouverMot(NoeudDictionnaire *&p_root, const std::string &mot) const;
        //Fonction recursive pour supprimer un mot
        void _supprimerAVL(NoeudDictionnaire * &p_root, const std::string &motASupprimer);
        //Fonction pour verifier si noeud a 2 fils
        bool _aDeuxfils(NoeudDictionnaire * &p_root) const;
        //Fonction qui retourne le plus petit noeud d'un arbre
        NoeudDictionnaire * _min(NoeudDictionnaire *p_root) const;
        //Fonction pour trouver distance entre 2 mots  en utilisant l'algorithm de Levenshtein (edit distance)
        unsigned int _getEditDistance(const std::string &first, const std::string &second) const;
        //Fonction pour parcourir le dictionnaire en PreOrdre
        void _parcousSuggestion(NoeudDictionnaire *p_root, std::vector<std::pair <double,
                std::string>> &p_vectPair, std::string p_motMalEcrit) const;
    };
}
#endif /* DICO_H_ */

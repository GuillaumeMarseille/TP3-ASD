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
        //Si le mot existe deja, on ne fait qu'ajouter la traduction au vectuer de ce mot
        if (appartient(motOriginal)){
            NoeudDictionnaire *& mot = _ajouterTraduction(racine, motOriginal, motTraduit);
            mot->traductions.push_back(motTraduit);
        } else{ //Sinon on ajoute le mot et sa traduction a l'arbre
            _insererAVL(racine, motOriginal, motTraduit);
        }
    }

    void Dictionnaire::supprimeMot(const std::string &motOriginal) {
        //Exception	logic_error si l'arbre est vide
        if (estVide()){
            throw std::logic_error("supprimerMot: l'arbre est vide.");
        }
        //Exception	logic_error si le mot n'appartient pas au dictionnaire
        if (!(appartient(motOriginal))){
            throw std::logic_error("supprimerMot: le mot n'appartient pas au dictionnaire.");

        }
        _supprimerAVL(racine, motOriginal);
    }
        //ATTENTION CE NEST PAS MA FONCTION https://www.techiedelight.com/find-similarity-between-two-strings-in-cpp/
    double Dictionnaire::similitude(const std::string &mot1, const std::string &mot2) {
        double max_length = std::max(mot1.length(), mot2.length());
        if (max_length > 0) {
            return (max_length - _getEditDistance(mot1, mot2)) / max_length;
        }
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
        //Comme pour zigZigGauche, mais rotation a gauche.
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

    void Dictionnaire::_miseAJourHauteurNoeud(NoeudDictionnaire * &p_root)
    {
        if (p_root != 0) {//Si la racine n'est pas un arbre vide
            p_root->hauteur = 1//Initialiser la hauteur a 1
                               + std::max(_hauteur(p_root->gauche),
                                          _hauteur(p_root->droite));//on va chercher la hauteur du noeud le plus haut
                                                                        //et on l'ajoute au 1.
        }
    }

    void Dictionnaire::_balancerUnNoeud(NoeudDictionnaire * &p_root)
    {
        //Cas ou arbre est vide
        if (p_root == 0) {
            return;
        }
        //Cas ou le debalancement penche a gauche du NC
        if (_debalancementAGauche(p_root)) {
            //Cas ou le noeud NC penche a gauche ET le noeud SC penche a droite (zigzag)
            if (_sousArbrePencheADroite(p_root->gauche)) {
                _zigZagGauche(p_root);
            //Cas ou le noeud NC penche a gauche ET le noeud SC penche a gauche OU est balance (zigzig)
            } else {
                _zigZigGauche(p_root);
            }
            //Cas ou le debalancement penche a droite du NC
        } else if (_debalancementADroite(p_root)) {
            //Cas ou le noeud NC penche a droite ET le noeud SC penche a gauche (zigzag)
            if (_sousArbrePencheAGauche(p_root->droite)) {
                _zigZagDroite(p_root);
                //Cas ou le noeud NC penche a droite ET le noeud SC penche a droite OU est balance (zigzig)
            } else {
                _zigZigDroite(p_root);
            }
        }
    }

    bool Dictionnaire::_debalancementAGauche(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {//Si arbre vide
            return false;
        }
        //Compare hauteur gauche et verifie si la difference est >= 2
        return _hauteur(p_root->gauche) - _hauteur(p_root->droite) >= 2;
    }
    bool Dictionnaire::_debalancementADroite(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {
            return false;
        }
        return _hauteur(p_root->droite) - _hauteur(p_root->gauche) >= 2;
    }
    bool Dictionnaire::_sousArbrePencheAGauche(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {//Si arbre vide
            return false;
        }
        //Compare hauteur arbre gauche et droite, retourne vrai si gauche > droite
        return (_hauteur(p_root->gauche) > _hauteur(p_root->droite));
    }
    bool Dictionnaire::_sousArbrePencheADroite(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {
            return false;
        }
        return (_hauteur(p_root->droite) > _hauteur(p_root->gauche));
    }

    void Dictionnaire::_insererAVL(NoeudDictionnaire * &p_root,const std::string &motOriginal,const std::string &motTraduit)
    {
        //Si on a atteint une feuille
        if (p_root == 0) {
            p_root = new NoeudDictionnaire(motOriginal, motTraduit);
            cpt++;
            return;
        }//Si le mot est plus grant que le mot du noeud acutel, on reappel la fonction avec le noeud de droite
        else if (p_root->mot < motOriginal) {
            _insererAVL(p_root->droite, motOriginal, motTraduit);
        }//Sinon avec le noeud de gauche
        else {
            _insererAVL(p_root->gauche, motOriginal, motTraduit);
        }

        _miseAJourHauteurNoeud(p_root);
        _balancerUnNoeud(p_root);
    }
    Dictionnaire::NoeudDictionnaire *& Dictionnaire::_ajouterTraduction(NoeudDictionnaire *&p_root, const std::string &mot, const std::string &motTraduit)
    {
        //Condition d'arret
        if (p_root->mot == mot) {
            return p_root;
        }
        //Cas mot plus petit que mot racine. Debut recursivite.
        if (p_root->mot > mot) {
            return _ajouterTraduction(p_root->gauche, mot, motTraduit);
        }
            //Cas mot plus grand que mot racine. Debut recursivite.
        else {
            return _ajouterTraduction(p_root->droite, mot,motTraduit);
        }
    }
    void Dictionnaire::_supprimerAVL(NoeudDictionnaire *&p_root, const std::string &motASupprimer){
        // Appels récursifs à gauche et à droite.
        if (p_root->mot < motASupprimer) {
            _supprimerAVL(p_root->droite, motASupprimer);
        }
        else if (p_root->mot > motASupprimer) {
            _supprimerAVL(p_root->gauche, motASupprimer);
        }
        else {
            // On a trouvé.
            // Si on est une feuille, on supprime simplement.
            if (_hauteur(p_root) == 0) {
                delete p_root;
                p_root = 0;
                cpt--;
            }
            else if (!_aDeuxfils(p_root)) {
                // Si on a juste un fils, on interchange et on supprime le fils (récursivement).
                if (p_root->gauche != 0) {
                    std::swap(p_root->mot, p_root->gauche->mot);
                    std::swap(p_root->traductions, p_root->gauche->traductions);
                    _supprimerAVL(p_root->gauche, motASupprimer);
                }
                else {
                    std::swap(p_root->mot, p_root->droite->mot);
                    std::swap(p_root->traductions, p_root->droite->traductions);
                    _supprimerAVL(p_root->droite, motASupprimer);
                }
            }
            else {
                // On a deux fils: cas compliqué.
                // On interchange la valeur du noeud avec celle de son successeur (minimum dans le sous-arbre de droite).
                NoeudDictionnaire * minDroite = _min(p_root->droite);
                std::swap(p_root->mot, minDroite->mot);
                std::swap(p_root->traductions, minDroite->traductions);

                // Puis, on appelle récursivement sur le sous-arbre de droite.
                _supprimerAVL(p_root->droite, motASupprimer);
            }
        }

        _miseAJourHauteurNoeud(p_root);
        _balancerUnNoeud(p_root);
    }
    bool Dictionnaire::_aDeuxfils(NoeudDictionnaire * &p_root) const{
        //retourne vrai si aucun fils n'est vide
        return (p_root->gauche != 0 && p_root->droite != 0);
    }
    Dictionnaire::NoeudDictionnaire * Dictionnaire::_min(NoeudDictionnaire *p_root) const{
        //Condition d'arret: si l'enfant de gauche du noeud courant est vide
        if (p_root->gauche == 0) {
            return p_root;
        }
        //Appel recursif sinon
        return _min(p_root->gauche);
    }
    //ATTENTION fonction trouvee ici https://www.techiedelight.com/find-similarity-between-two-strings-in-cpp/ (legerement modifiee)
    int Dictionnaire::_getEditDistance(const std::string &mot1, const std::string &mot2){
        int m = mot1.length();
        int n = mot2.length();

        int T[m + 1][n + 1];
        for (int i = 1; i <= m; i++) {
            T[i][0] = i;
        }

        for (int j = 1; j <= n; j++) {
            T[0][j] = j;
        }

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                int weight = mot1[i - 1] == mot2[j - 1] ? 0: 1;
                T[i][j] = std::min(std::min(T[i-1][j] + 1, T[i][j-1] + 1), T[i-1][j-1] + weight);
            }
        }

        return T[m][n];
    }
}//Fin du namespace

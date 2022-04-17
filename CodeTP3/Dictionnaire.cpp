/**
 * \file Dictionnaire.cpp
 * \brief Ce fichier contient une implantation des méthodes de la classe Dictionnaire
 * \author IFT-2008, Guillaume Marseille
 * \version 0.1
 * \date avril 2022
 *
 */

#include "Dictionnaire.h"

// Limite du nombre de suggestions
#define LIMITE_SUGGESTIONS 5

namespace TP3 {
    /**
     * \brief Constructeur par défaut
     * \post Une instance de la classe Dictionnaire est initialisée
     */
    Dictionnaire::Dictionnaire() : racine(0), cpt(0) {}

    /**
     * \brief Constructeur par avec un fichier
     * \param[in] fichier le fichier dictionnaire
     * \post Une instance de la classe Dictionnaire est initialisée
     */
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

    /**
     *  \brief Destructeur de la classe Dictionnaire
     *  \post L'instance de Dictionnaire est détruite
     */
    Dictionnaire::~Dictionnaire() {
        _detruire(racine);
    }

    /**
     * \brief Ajoute un mot en gardant le dictionnaire balance
     * \param[in] motOriginal le mot a ajouter en anglais
     * \param[in] motTraduit la traduction du mot a ajouter
     * \post L'élément et sa traduction est ajouté, si le mot existait deja, seul la traduction est ajoutee
     */
    void Dictionnaire::ajouteMot(const std::string &motOriginal, const std::string &motTraduit) {
        //Si le mot existe deja, on ne fait qu'ajouter la traduction au vectuer de ce mot
        if (appartient(motOriginal)){
            NoeudDictionnaire *& mot = _trouverMot(racine, motOriginal);
            mot->traductions.push_back(motTraduit);
        } else{ //Sinon on ajoute le mot et sa traduction a l'arbre
            _insererAVL(racine, motOriginal, motTraduit);
        }
    }

    /**
     * \brief Enlever un élément en gardant l'arbre AVL
     * \param[in] motOriginal le mot a enlever
     * \pre L'élément est pas dans l'arbre
     * \post L'élément est enlevé
     */
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
    /**
    * \brief Compare 2 strings et retourne la similarite entre les 2 en pourcentage
    * Une partie de cette fonction a ete prise ici: https://www.techiedelight.com/find-similarity-between-two-strings-in-cpp/
    * \param[in] mot1 le premier mot a comparer
    * \param[in] mot2 le deuxieme mot a comparer
    * \return un double representant le pourcentage de similarite (1 etant 2 mots identiques)
    */
    double Dictionnaire::similitude(const std::string &mot1, const std::string &mot2) const {
        double max_length = std::max(mot1.length(), mot2.length());
        if (max_length > 0) {
            return (max_length - _getEditDistance(mot1, mot2)) / max_length;;
        }
        return 1.0;
    }

    /**
    * \brief cree un vecteur de suggestions lorsqu'un mot de figure pas dans le dictionnaire
    * \param[in] motMalEcrit le mot inconnu
    * \return un vecteur contenant au plus 5 suggestions
    */
    std::vector<std::string> Dictionnaire::suggereCorrections(const std::string &motMalEcrit) {
        std::vector<std::pair <double, std::string>> comparaison;
        std::vector<std::string> suggestions;
        _parcousSuggestion(racine, comparaison, motMalEcrit);
        sort(comparaison.rbegin(), comparaison.rend());

        int i = 0;
        while (i < comparaison.size() and suggestions.size() < LIMITE_SUGGESTIONS){
            suggestions.push_back(comparaison[i].second);
            i++;
        }
        return suggestions;
    }

    /**
    * \brief cree un vecteur de traductions pour un mot donnee
    * \param[in] mot le mot a traduire
    * \return un vecteur contenant au plus 5 suggestions
    */
    std::vector<std::string> Dictionnaire::traduit(const std::string &mot) {
        std::vector<std::string> traductions;
        NoeudDictionnaire* noeudMot = _trouverMot(racine, mot);
        traductions = noeudMot->traductions;
        return traductions;
    }

    /**
    * \brief verifie si un mot appartient au dictionnaire
    * \param[in] mot le mot a verifier
    * \return un bool a vrai si il s'y trouve
    */
    bool Dictionnaire::appartient(const std::string &mot) {
        return (_appartient(racine, mot) != 0);
    }

    /**
    * \brief verifie si un dictionnaire est vide
    * \return un bool a vrai si il l'est
    */
    bool Dictionnaire::estVide() const {
        return cpt == 0;
    }

    // Complétez ici l'implémentation avec vos méthodes privées.

    /**
     * \brief Détruire un sous-arbre. Fonction récursive auxiliaire pour le destructeur
     * \post Le sous-arbre est détruit
     * \param[in] p_root Le sous-arbre a detruire
     */
    void Dictionnaire::_detruire(NoeudDictionnaire *&p_root) {
        if (p_root != 0) { // si l'arbre existe.
            _detruire(p_root->gauche); //recursive call gauche
            _detruire(p_root->droite); //recursive call droit
            delete p_root;
            p_root = 0;

        }
    }

    /**
     * \brief Fonction récursive pour déterminer si un élément est dans l'arbre
     * \param[in] p_root Le sous-arbre de la récursion
     * \param[in] mot L'élément à rechercher
     * \return Pointeur sur le noeud contenant la donnée
     * \post L'arbre est inchangée
     */
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

    /**
     * \brief Mettre à jour la hauteur d'un noeud
     * \param[in] p_root Le noeud à mettre à jour
     * \post La hauteur du noeud est mise à jour
     */
    void Dictionnaire::_majHauteur(NoeudDictionnaire *&p_root) {
        //Si l'arbre n'est pas vide, hauteur est 1 + hauteur de la plus longue branch du noeud.
        if (p_root != 0) {
            p_root->hauteur = 1 + std::max(_hauteur(p_root->gauche), _hauteur(p_root->droite));
        }
    }

    /**
     * \brief Retourne la hauteur d'un sous-arbre
     * \param[in] p_root Un sous-arbre
     * \return La hauteur du sous-arbre
     * \post L'arbre est inchangé
     */
    int Dictionnaire::_hauteur(NoeudDictionnaire * &p_root) const
    {
        //Cas ou l'arbre est vide
        if (p_root == 0) {
            return -1;
        }
        //Sinon simplement retourner l'attribut heuteur
        return p_root->hauteur;
    }

    /**
     * \brief Effectuer la transformation zig-zig gauche
     * \param[in] p_noeudCritique Le noeud critique du zig-zig
     */
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

    /**
     * \brief Effectuer la transformation zig-zag gauche
     * \param[in] p_noeudCritique Le noeud critique du zig-zag
     */
    void Dictionnaire::_zigZagGauche(NoeudDictionnaire * &p_noeudCritique)
    {
        //On fait une rotation a droite
        _zigZigDroite(p_noeudCritique->gauche);
        //Ensuite rotation a gauche
        _zigZigGauche(p_noeudCritique);
    }

    /**
     * \brief Effectuer la transformation zig-zig droite
     * \param[in] p_noeudCritique Le noeud critique du zig-zig
     */
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

    /**
     * \brief Effectuer la transformation zig-zag droite
     * \param[in] p_noeudCritique Le noeud critique du zig-zag
     */
    void Dictionnaire::_zigZagDroite(NoeudDictionnaire * &p_noeudCritique)
    {
        //Rotation a gauche suivi de rotation a droite
        _zigZigGauche(p_noeudCritique->droite);
        _zigZigDroite(p_noeudCritique);
    }

    /**
     * \brief Mettre à jour la hauteur d'un noeud
     * \param[in] p_root Le noeud à mettre à jour
     * \post La hauteur du noeud est mise à jour
     */
    void Dictionnaire::_miseAJourHauteurNoeud(NoeudDictionnaire * &p_root)
    {
        if (p_root != 0) {//Si la racine n'est pas un arbre vide
            p_root->hauteur = 1//Initialiser la hauteur a 1
                               + std::max(_hauteur(p_root->gauche),
                                          _hauteur(p_root->droite));//on va chercher la hauteur du noeud le plus haut
                                                                        //et on l'ajoute au 1.
        }
    }

    /**
     * \brief Balancer un sous-arbre
     * \param[in] p_root Le sous-arbre à balancer
     * \post Le sous-arbre est balancé
     */
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
            }
            else {
                _zigZigDroite(p_root);
            }
        }
    }

    /**
     * \brief Indique si le sous-arbre est débalancé à gauche
     * \param[in] p_root Un sous-arbre
     * \return Bool indiquant si le sous-arbre est débalancé à gauche
     * \post L'arbre est inchangé
     */
    bool Dictionnaire::_debalancementAGauche(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {//Si arbre vide
            return false;
        }
        //Compare hauteur gauche et verifie si la difference est >= 2
        return _hauteur(p_root->gauche) - _hauteur(p_root->droite) >= 2;
    }

    /**
     * \brief Indique si le sous-arbre est débalancé à droite
     * \param[in] p_root Un sous-arbre
     * \return Bool indiquant si le sous-arbre est débalancé à droite
     * \post L'arbre est inchangé
     */
    bool Dictionnaire::_debalancementADroite(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {
            return false;
        }
        return _hauteur(p_root->droite) - _hauteur(p_root->gauche) >= 2;
    }

    /**
     * \brief Indique si le sous-arbre penche à gauche
     * \param[in] p_root Un sous-arbre
     * \return Bool indiquant si le sous-arbre penche à gauche
     * \post L'arbre est inchangé
     */
    bool Dictionnaire::_sousArbrePencheAGauche(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {//Si arbre vide
            return false;
        }
        //Compare hauteur arbre gauche et droite, retourne vrai si gauche > droite
        return (_hauteur(p_root->gauche) > _hauteur(p_root->droite));
    }

    /**
     * \brief Indique si le sous-arbre penche à droite
     * \param[in] p_root Un sous-arbre
     * \return Bool indiquant si le sous-arbre penche à droite
     * \post L'arbre est inchangé
     */
    bool Dictionnaire::_sousArbrePencheADroite(NoeudDictionnaire * &p_root) const
    {
        if (p_root == 0) {
            return false;
        }
        return (_hauteur(p_root->droite) > _hauteur(p_root->gauche));
    }

    /**
     * \brief Fonction récursive pour insérer un élément en gardant l'arbre AVL
     * \param[in] p_root Le sous arbre actuel
     * \param[in] motOriginal L'élément à ajouter
     * \param[in] motTraduit La traduction a ajouter
     * \post L'élément et sa traduction sont ajoutés
     */
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

    /**
     * \brief Fonction récursive pour trouver un mot dans le dictionnaire et le retourner
     * \param[in] p_root Le sous arbre actuel
     * \param[in] mot L'élément à trouver
     * \return L'élément trouve
     */
    Dictionnaire::NoeudDictionnaire *& Dictionnaire::_trouverMot(NoeudDictionnaire *&p_root, const std::string &mot) const
    {
        //Condition d'arret
        if (p_root->mot == mot) {
            return p_root;
        }
        //Cas mot plus petit que mot racine. Debut recursivite.
        if (p_root->mot > mot) {
            return _trouverMot(p_root->gauche, mot);
        }
            //Cas mot plus grand que mot racine. Debut recursivite.
        else {
            return _trouverMot(p_root->droite, mot);
        }
    }

    /**
     * \brief Fonction récursive pour enlever un élément en gardant le dictionnaire AVL
     * \param[in] p_root Le sous arbre de la récursion
     * \param[in] motASupprimer L'élément à enlever
     * \post L'élément est enlevé
     */
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

    /**
     * \brief Indique si le sous-arbre à deux fils
     * \param[in] p_root Un sous-arbre
     * \return Bool indiquant si le sous-arbre à deux fils
     * \post L'arbre est inchangé
     */
    bool Dictionnaire::_aDeuxfils(NoeudDictionnaire * &p_root) const{
        //retourne vrai si aucun fils n'est vide
        return (p_root->gauche != 0 && p_root->droite != 0);
    }

    /**
     * \brief Fonction récursive pour atteindre la donnée minimum en partant
     *        du noeud passé en paramètre
     * \param[in] p_root Le noeud de départ
     * \return Pointeur sur le noeud atteint
     * \post L'arbre est inchangé
     */
    Dictionnaire::NoeudDictionnaire * Dictionnaire::_min(NoeudDictionnaire *p_root) const{
        //Condition d'arret: si l'enfant de gauche du noeud courant est vide
        if (p_root->gauche == 0) {
            return p_root;
        }
        //Appel recursif sinon
        return _min(p_root->gauche);
    }

    /**
     * \brief Methode pour avoir la distance de Levenshtein entre 2 strings
     *        Code source pris ici : https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance
     * \param[in] first la premiere string
     * \param[in] second la deuxieme string
     * \return la distance Levenshtein
     * \post L'arbre est inchangé
     */
    unsigned int Dictionnaire::_getEditDistance(const std::string &first, const std::string &second) const
    {
        const std::size_t len1 = first.size(), len2 = second.size();
        std::vector<std::vector<unsigned int>> d(len1 + 1, std::vector<unsigned int>(len2 + 1));

        d[0][0] = 0;
        for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
        for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

        for(unsigned int i = 1; i <= len1; ++i)
            for(unsigned int j = 1; j <= len2; ++j)
                // note that std::min({arg1, arg2, arg3}) works only in C++11,
                // for C++98 use std::min(std::min(arg1, arg2), arg3)
                d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (first[i - 1] == second[j - 1] ? 0 : 1) });
        return d[len1][len2];
    }

    /**
    * \brief Methode qui parcours le dictionnaire et verifie la similitude de chaque mots avec celle du mot mal ecrit
     * \param[in] p_root Le noeud de départ
     * \param[in] p_vectPair un vecteur de pair double / string
     * \param[in] p_motMalEcrit le mot mal ecrit
     * \post Le vecteur contient tous les mots qui on au moins 0.5 de similarite avec le mot mal ecrit
     */
    void Dictionnaire::_parcousSuggestion(NoeudDictionnaire *p_root,
                                          std::vector<std::pair <double, std::string>> &p_vectPair,
                                          std::string p_motMalEcrit) const
    {
        if (p_root != 0) {
            double simi = similitude(p_motMalEcrit, p_root->mot); //T'es ici jai switch les 2 pour tester
            if (simi >= 0.5)
            {
                p_vectPair.push_back(std::make_pair(simi,p_root->mot));
            }
            _parcousSuggestion(p_root->gauche, p_vectPair, p_motMalEcrit);
            _parcousSuggestion(p_root->droite, p_vectPair, p_motMalEcrit);
        }
    }

    /**
     * \brief Methode qui verifie si l'arbre est balance
     * \return vrai si arbre balance
     */
    bool Dictionnaire::estBalancee() const {
        if (racine == 0) {//Si arbre vide
            return false;
        }
        //Compare hauteur gauche et droite
        return !(abs(_hauteur(racine->gauche) - _hauteur(racine->droite)) >= 2);
    }
}//Fin du namespace

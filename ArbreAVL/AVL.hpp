/**
 * \file AVL.hpp
 * \brief Le code des opérateurs d'un arbre AVL.
 * \author Ludovic Trottier
 * \author Jean-Francis Roy
 * \version 0.3
 * \date mai 2014
 *
 * Implémentation de la classe générique Arbre AVL par chaînage
 *
 */
#include <cmath>
#include <iostream>
#include "ContratException.h"

/**
 * \brief Namespace du laboratoire 8
 */
namespace labArbreAVL {
/**
 * \brief Constructeur par défaut
 * \post Une instance de la classe Arbre est initialisée
 */
template<typename E>
Arbre<E>::Arbre() :
		m_racine(0), m_cardinalite(0) {
}
/**
 * \brief Constructeur de copie
 *
 * \pre Il faut qu'il y ait suffisamment de mémoire
 * \post une copie profonde de l'arbre source est instanciée
 * \param[in] p_source L'arbre source à copier
 */
template<typename E>
Arbre<E>::Arbre(const Arbre& p_source) {
	_auxCopier(p_source.m_racine, m_racine);
	m_cardinalite = p_source.m_cardinalite;
}
/**
 * \brief Surcharger l'opérateur =
 *
 * \pre Il y a assez de mémoire pour l'opération
 * \post Une copie de l'arbre est effectuée
 * \param[in] p_source L'arbre source à copier
 * \return Une référence constante sur l'objet courant
 */
template<typename E>
Arbre<E>& Arbre<E>::operator =(const Arbre & p_source) {
	if (this != &p_source) {
		_auxDetruire(m_racine);
		_auxCopier(p_source.m_racine, m_racine);
		m_cardinalite = p_source.m_cardinalite;
	}

	return *this;
}
/**
 *  \brief Destructeur
 *  \post L'instance de Graphe est détruite
 */
template<typename E>
Arbre<E>::~Arbre() {
	_auxDetruire(m_racine);
}
/**
 * \brief Détruire un sous-arbre. Fonction récursive auxiliaire pour le destructeur
 * \post Le sous-arbre est détruit
 * \param[in] p_arb Le sous-arbre à détruire
 */
template<typename T>
void Arbre<T>::_auxDetruire(Noeud *& p_arb) {
	if (p_arb != 0) {
		_auxDetruire(p_arb->m_gauche);
		_auxDetruire(p_arb->m_droite);
		delete p_arb;
		p_arb = 0;
	}
}
/**
 * \brief Copier deux sous-arbres. Fonction récursive auxiliaire pour le constructeur de copie
 *
 * \pre Il y a assez de mémoire
 * \post Le sous-arbre source est copié sur la destination
 * \param[in] p_source Le sous-arbre à copier
 * \param[out] p_destination La destination où copier
 */
template<typename T>
void Arbre<T>::_auxCopier(Noeud * p_source, Noeud * & p_destination) {
	if (p_source == 0) {
		p_destination = 0;
		return;
	}

	p_destination = new Noeud(p_source->m_data);
	p_destination->m_hauteur = p_source->m_hauteur;
	_auxCopier(p_source->m_gauche, p_destination->m_gauche);
	_auxCopier(p_source->m_droite, p_destination->m_droite);
}
/**
 *  \brief Indiquer si l'arbre est vide
 *  \post L'arbre est inchangée
 *  \return Bool indiquant si l'arbre est vide
 */
template<typename E>
bool Arbre<E>::estVide() const {
	return m_cardinalite == 0;
}

/**
 * \brief Vérifier les invariant de la classe
 * \post La classe est inchangée.
 * \post L'arbre est inchangée
 */
template<typename E>
void Arbre<E>::verifieInvariant() const {
	INVARIANT(m_cardinalite >= 0);
}
/**
 * \brief Déterminer si un élément est dans l'arbre
 * \param[in] p_data L'élément à rechercher
 * \return Bool indiquant si l'élément est dans l'arbre
 * \post L'arbre est inchangée
 */
template<typename E>
bool Arbre<E>::appartient(const E & p_data) const {
	return (_appartient(m_racine, p_data) != 0);
}
/**
 * \brief Fonction récursive pour déterminer si un élément est dans l'arbre
 * \param[in] p_arb Le sous-arbre de la récursion
 * \param[in] p_data L'élément à rechercher
 * \return Pointeur sur le noeud contenant la donnée
 * \post L'arbre est inchangée
 */
template<typename E>
typename Arbre<E>::Noeud* Arbre<E>::_appartient(Noeud* const & p_arb,
		const E & p_data) const {
	if (p_arb == 0) {
		return 0;
	}
	if (p_arb->m_data == p_data) {
		return p_arb;
	}

	if (p_arb->m_data > p_data) {
		return _appartient(p_arb->m_gauche, p_data);
	} else {
		return _appartient(p_arb->m_droite, p_data);
	}
}
/**
 * \brief Effectuer un parcours pre-ordre sur l'arbre et lister la visite des noeuds.
 * \post L'arbre est inchangée
 * \return Vector contenant la visite des noeuds
 */
template<typename E>
std::vector<E> Arbre<E>::listerPreOrdre() const {
	std::vector<E> v;
	_listerPreOrdre(m_racine, v);
	return v;
}

/**
 * \brief Fonction auxiliaire récursive pour le parcours.
 * \post L'arbre est inchangée
 * \param[in] p_arb Racine du sous-arbre
 * \param[out] p_v Vector contenant la visite des noeuds
 */
template<typename E>
void Arbre<E>::_listerPreOrdre(Noeud * p_arb, std::vector<E> & p_v) const {
	if (p_arb != 0) {
		p_v.push_back(p_arb->m_data);
		_listerPreOrdre(p_arb->m_gauche, p_v);
		_listerPreOrdre(p_arb->m_droite, p_v);
	}
}
/**
 * \brief Effectuer un parcours en-ordre sur l'arbre et lister la visite des noeuds.
 * \post L'arbre est inchangée
 * \return Vector contenant la visite des noeuds
 */
template<typename E>
std::vector<E> Arbre<E>::listerEnOrdre() const {
	std::vector<E> v;
	_listerEnOrdre(m_racine, v);
	return v;
}
/**
 * \brief Fonction auxiliaire récursive pour le parcours.
 * \post L'arbre est inchangée
 * \param[in] p_arb Racine du sous-arbre
 * \param[out] p_v Vector contenant la visite des noeuds
 */
template<typename E>
void Arbre<E>::_listerEnOrdre(Noeud * p_arb, std::vector<E> & p_v) const {
	if (p_arb != 0) {
		_listerEnOrdre(p_arb->m_gauche, p_v);
		p_v.push_back(p_arb->m_data);
		_listerEnOrdre(p_arb->m_droite, p_v);
	}
}
/**
 * \brief Effectuer un parcours post-ordre sur l'arbre et lister la visite des noeuds.
 * \post L'arbre est inchangé
 * \return Vector contenant la visite des noeuds
 */
template<typename E>
std::vector<E> Arbre<E>::listerPostOrdre() const {
	std::vector<E> v;
	_listerPostOrdre(m_racine, v);
	return v;
}
/**
 * \brief Fonction auxiliaire récursive pour le parcours.
 * \post L'arbre est inchangée
 * \param[in] p_arb Racine du sous-arbre
 * \param[out] p_v Vector contenant la visite des noeuds
 */
template<typename E>
void Arbre<E>::_listerPostOrdre(Noeud * p_arb, std::vector<E> & p_v) const {
	if (p_arb != 0) {
		_listerPostOrdre(p_arb->m_gauche, p_v);
		_listerPostOrdre(p_arb->m_droite, p_v);
		p_v.push_back(p_arb->m_data);
	}
}
/**
 * \brief Effectuer un parcours par niveau sur l'arbre et lister la visite des noeuds.
 * \post L'arbre est inchangée
 * \return Vector contenant la visite des noeuds
 */
template<typename E>
std::vector<E> Arbre<E>::listerParNiveau() const {
	std::queue<Noeud*> file;
	Noeud * temp;
	std::vector<E> v;

	file.push(m_racine);
	while (!file.empty()) {
		temp = file.front();
		v.push_back(temp->m_data);
		file.pop();
		if (temp->m_gauche != 0) {
			file.push(temp->m_gauche);
		}
		if (temp->m_droite != 0) {
			file.push(temp->m_droite);
		}
	}
	return v;
}

/**
 * \brief Insérer un élément en gardant l'arbre AVL
 * \param[in] p_data L'élément à ajouter
 * \pre L'élément n'est pas dans l'arbre
 * \post L'élément est ajouté
 */
template<typename E>
void Arbre<E>::insererAVL(const E & p_data) {
	PRECONDITION(!appartient(p_data));

	_insererAVL(m_racine, p_data);

	POSTCONDITION(appartient(p_data));
	INVARIANTS();
}
/**
 * \brief Fonction récursive pour insérer un élément en gardant l'arbre AVL
 * \param[in] p_arb Le sous arbre actuel
 * \param[in] p_data L'élément à ajouter
 * \post L'élément est ajouté
 */
template<typename E>
void Arbre<E>::_insererAVL(Noeud *& p_arb, const E & p_data) {
	if (p_arb == 0) {
		p_arb = new Noeud(p_data);
		m_cardinalite++;
		return;
	} else if (p_arb->m_data < p_data) {
		_insererAVL(p_arb->m_droite, p_data);
	} else {
		_insererAVL(p_arb->m_gauche, p_data);
	}

	_miseAJourHauteurNoeud(p_arb);
	_balancerUnNoeud(p_arb);
}
/**
 * \brief Retourne la hauteur d'un sous-arbre
 * \param[in] p_arb Un sous-arbre
 * \return La hauteur du sous-arbre
 * \post L'arbre est inchangé
 */
template<typename E>
int Arbre<E>::_hauteur(Noeud * & p_arb) const {
	if (p_arb == 0) {
		return -1;
	}
	return p_arb->m_hauteur;
}
/**
 * \brief Indique si le sous-arbre est débalancé à gauche
 * \param[in] p_arb Un sous-arbre
 * \return Bool indiquant si le sous-arbre est débalancé à gauche
 * \post L'arbre est inchangé
 */
template<typename E>
bool Arbre<E>::_debalancementAGauche(Noeud *& p_arb) const {
	if (p_arb == 0) {
		return false;
	}

	return _hauteur(p_arb->m_gauche) - _hauteur(p_arb->m_droite) >= 2;
}
/**
 * \brief Indique si le sous-arbre est débalancé à droite
 * \param[in] p_arb Un sous-arbre
 * \return Bool indiquant si le sous-arbre est débalancé à droite
 * \post L'arbre est inchangé
 */
template<typename E>
bool Arbre<E>::_debalancementADroite(Noeud * & p_arb) const {
	if (p_arb == 0) {
		return false;
	}

	return _hauteur(p_arb->m_droite) - _hauteur(p_arb->m_gauche) >= 2;
}
/**
 * \brief Indique si le sous-arbre penche à gauche
 * \param[in] p_arb Un sous-arbre
 * \return Bool indiquant si le sous-arbre penche à gauche
 * \post L'arbre est inchangé
 */
template<typename E>
bool Arbre<E>::_sousArbrePencheAGauche(Noeud *& p_arb) const {
	if (p_arb == 0) {
		return false;
	}

	return (_hauteur(p_arb->m_gauche) > _hauteur(p_arb->m_droite));
}
/**
 * \brief Indique si le sous-arbre penche à droite
 * \param[in] p_arb Un sous-arbre
 * \return Bool indiquant si le sous-arbre penche à droite
 * \post L'arbre est inchangé
 */
template<typename E>
bool Arbre<E>::_sousArbrePencheADroite(Noeud *& p_arb) const {
	if (p_arb == 0) {
		return false;
	}

	return (_hauteur(p_arb->m_droite) > _hauteur(p_arb->m_gauche));
}
/**
 * \brief Effectuer la transformation zig-zig gauche
 * \param[in] p_noeudCritique Le noeud critique du zig-zig
 */
template<typename E>
void Arbre<E>::_zigZigGauche(Noeud * & p_noeudCritique) {
	Noeud * noeudSousCritique = p_noeudCritique->m_gauche;
	p_noeudCritique->m_gauche = noeudSousCritique->m_droite;
	noeudSousCritique->m_droite = p_noeudCritique;

	_miseAJourHauteurNoeud(p_noeudCritique);
	_miseAJourHauteurNoeud(noeudSousCritique);

	p_noeudCritique = noeudSousCritique;
}
/**
 * \brief Effectuer la transformation zig-zag gauche
 * \param[in] p_noeudCritique Le noeud critique du zig-zag
 */
template<typename E>
void Arbre<E>::_zigZagGauche(Noeud * & p_noeudCritique) {
	_zigZigDroite(p_noeudCritique->m_gauche);
	_zigZigGauche(p_noeudCritique);
}
/**
 * \brief Effectuer la transformation zig-zig droite
 * \param[in] p_noeudCritique Le noeud critique du zig-zig
 */
template<typename E>
void Arbre<E>::_zigZigDroite(Noeud *& p_noeudCritique) {
	Noeud * noeudSousCritique = p_noeudCritique->m_droite;
	p_noeudCritique->m_droite = noeudSousCritique->m_gauche;
	noeudSousCritique->m_gauche = p_noeudCritique;

	_miseAJourHauteurNoeud(p_noeudCritique);
	_miseAJourHauteurNoeud(noeudSousCritique);

	p_noeudCritique = noeudSousCritique;
}
/**
 * \brief Effectuer la transformation zig-zag droite
 * \param[in] p_noeudCritique Le noeud critique du zig-zag
 */
template<typename E>
void Arbre<E>::_zigZagDroite(Noeud *& p_noeudCritique) {
	_zigZigGauche(p_noeudCritique->m_droite);
	_zigZigDroite(p_noeudCritique);
}
/**
 * \brief Enlever un élément en gardant l'arbre AVL
 * \param[in] p_data L'élément à enlever
 * \pre L'élément est pas dans l'arbre
 * \post L'élément est enlevé
 */
template<typename E>
void Arbre<E>::enleverAVL(const E & p_data) {
	PRECONDITION(appartient(p_data));

	_enleverAVL(m_racine, p_data);

	POSTCONDITION(!appartient(p_data));
	INVARIANTS();
}
/**
 * \brief Fonction récursive pour enlever un élément en gardant l'arbre AVL
 * \param[in] p_arb Le sous arbre de la récursion
 * \param[in] p_data L'élément à enlever
 * \post L'élément est enlevé
 */
template<typename E>
void Arbre<E>::_enleverAVL(Noeud *& p_arb, const E& p_data) {
	if (p_arb == 0) {
		// Si on se rend ici, il y a une erreur (noeud inexistant).
		throw std::logic_error("enleverAVL: élément inexistant.");
	}

	// Appels récursifs à gauche et à droite.
	if (p_arb->m_data < p_data) {
		_enleverAVL(p_arb->m_droite, p_data);
	}
	else if (p_arb->m_data > p_data) {
		_enleverAVL(p_arb->m_gauche, p_data);
	}
	else {
        // On a trouvé.
        // Si on est une feuille, on supprime simplement.
        if (_hauteur(p_arb) == 0) {
            delete p_arb;
            p_arb = 0;
            m_cardinalite--;
        }
        else if (!_aDeuxfils(p_arb)) {
            // Si on a juste un fils, on interchange et on supprime le fils (récursivement).
            if (p_arb->m_gauche != 0) {
                std::swap(p_arb->m_data, p_arb->m_gauche->m_data);
                _enleverAVL(p_arb->m_gauche, p_data);
            }
            else {
                std::swap(p_arb->m_data, p_arb->m_droite->m_data);
                _enleverAVL(p_arb->m_droite, p_data);
            }
        }
        else {
            // On a deux fils: cas compliqué.
            // On interchange la valeur du noeud avec celle de son successeur (minimum dans le sous-arbre de droite).
            Noeud * minDroite = _min(p_arb->m_droite);
            std::swap(p_arb->m_data, minDroite->m_data);

            // Puis, on appelle récursivement sur le sous-arbre de droite.
            _enleverAVL(p_arb->m_droite, p_data);
        }
    }

	_miseAJourHauteurNoeud(p_arb);
	_balancerUnNoeud(p_arb);
}
/**
 * \brief Indique si le sous-arbre à deux fils
 * \param[in] p_arb Un sous-arbre
 * \return Bool indiquant si le sous-arbre à deux fils
 * \post L'arbre est inchangé
 */
template<typename E>
bool Arbre<E>::_aDeuxfils(Noeud *& p_arb) const {
	return (p_arb->m_gauche != 0 && p_arb->m_droite != 0);
}
/**
 * \brief Fonction récursive pour atteindre la donnée minimum en partant
 *        du noeud passé en paramètre
 * \param[in] p_arb Le noeud de départ
 * \return Pointeur sur le noeud atteint
 * \post L'arbre est inchangé
 */
template<typename E>
typename Arbre<E>::Noeud * Arbre<E>::_min(Noeud * p_arb) const {
	if (p_arb->m_gauche == 0) {
		return p_arb;
	}
	return _min(p_arb->m_gauche);
}
/**
 * \brief Indique si l'arbre est AVL
 * \post L'arbre est inchangé
 * \return Bool indiquant si l'arbre est AVL
 */
template<typename E>
bool Arbre<E>::estAVL() const {
	if (estVide()) {
		return true;
	}
	std::queue<Noeud*> Q;
	Noeud * temp;

	Q.push(m_racine);
	while (!Q.empty()) {
		temp = Q.front();
		if (_amplitudeDuDebalancement(temp) > 1) {
			return false;
		}
		Q.pop();
		if (temp->m_gauche != 0)
			Q.push(temp->m_gauche);
		if (temp->m_droite != 0)
			Q.push(temp->m_droite);
	}

	return true;
}
/**
 * \brief Retourne l'amplitude du débalancement d'un sous-arbre
 * \param[in] p_arb Le sous-arbre
 * \return Un entier indiquant l'amplitude du débalancement
 * \post L'arbre est inchangé
 */
template<typename E>
int Arbre<E>::_amplitudeDuDebalancement(Noeud * p_arb) const {
	return std::abs(_hauteur(p_arb->m_gauche) - _hauteur(p_arb->m_droite));
}
/**
 * \brief Balancer un sous-arbre
 * \param[in] p_arb Le sous-arbre à balancer
 * \post Le sous-arbre est balancé
 */
template<typename E>
void Arbre<E>::_balancerUnNoeud(Noeud *& p_arb) {
	if (p_arb == 0) {
		return;
	}
	if (_debalancementAGauche(p_arb)) {
		// IMPORTANT
		// Lorsque le débalancement est à gauche,
		// on fait un zigZig lorsque le sousArbre
		// penche à gauche OU est balancé.
		// L'ordre est important
		if (_sousArbrePencheADroite(p_arb->m_gauche)) {
			_zigZagGauche(p_arb);
		} else {
			_zigZigGauche(p_arb);
		}
	} else if (_debalancementADroite(p_arb)) {
		// IMPORTANT
		// Lorsque le débalancement est à droite,
		// on fait un zigZig lorsque le sousArbre
		// penche à droite OU est balancé.
		// L'ordre est important
		if (_sousArbrePencheAGauche(p_arb->m_droite)) {
			_zigZagDroite(p_arb);
		} else {
			_zigZigDroite(p_arb);
		}
	}
	POSTCONDITION(_amplitudeDuDebalancement(p_arb) < 2);
}
/**
 * \brief Mettre à jour la hauteur d'un noeud
 * \param[in] p_arb Le noeud à mettre à jour
 * \post La hauteur du noeud est mise à jour
 */
template<typename E>
void Arbre<E>::_miseAJourHauteurNoeud(Noeud * & p_arb) {
	if (p_arb != 0) {
		p_arb->m_hauteur = 1
				+ std::max(_hauteur(p_arb->m_gauche),
						_hauteur(p_arb->m_droite));
	}
}

} //Fin du namespace

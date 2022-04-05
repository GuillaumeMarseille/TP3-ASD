/**
 * \file AVLTesteur.cpp
 * \brief Tests unitaires pour Arbre AVL.
 * \author Ludovic Trottier
 * \version 0.3
 * \date mai 2014
 *
 * Tests unitaires googletest pour Arbre AVL.
 */
#include "AVL.h"
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace labArbreAVL;

/*
 * \brief Afficher un vector. Utile pour le listage.
 * \param[in] v Le vecteur à afficher
 */
template<typename E>
void afficherVector(const vector<E> & v) {
	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v[i] << ',';
	}
	cout << endl;
}

/*
 * Afin d'éviter que la construction de la fixture plante,
 * on teste ici que l'insertion ne fait pas de throw.
 */
TEST(ArbreVide, InsererAVLNoThrow) {
	Arbre<int> ab;
	ASSERT_NO_THROW(ab.insererAVL(5));
	ASSERT_NO_THROW(ab.insererAVL(3));
	ASSERT_NO_THROW(ab.insererAVL(1));
	ASSERT_NO_THROW(ab.insererAVL(7));
	ASSERT_NO_THROW(ab.insererAVL(6));
	ASSERT_NO_THROW(ab.insererAVL(2));
	ASSERT_NO_THROW(ab.insererAVL(8));
}

class ArbreTest: public ::testing::Test {
protected:
	virtual void SetUp() {

		/*
		 * L'arbre ressemble à ceci à la fin.
		 *                    3
		 *            1               6
		 *               2        5       7
		 *                                    8
		 */
		ab.insererAVL(5);
		ab.insererAVL(3);
		ab.insererAVL(1); // balancement #1
		ab.insererAVL(7);
		ab.insererAVL(6); // balancement #2
		ab.insererAVL(2);
		ab.insererAVL(8);
	}
	// virtual void TearDown() {}
	Arbre<int> ab;
};

TEST_F(ArbreTest, InsererAVLElementPresent) {
	EXPECT_TRUE(ab.appartient(5));
	EXPECT_TRUE(ab.appartient(3));
	EXPECT_TRUE(ab.appartient(1));
	EXPECT_TRUE(ab.appartient(7));
	EXPECT_TRUE(ab.appartient(6));
	EXPECT_TRUE(ab.appartient(2));
	EXPECT_TRUE(ab.appartient(8));
}

TEST_F(ArbreTest, InsererAVLPasDeDebalancement) {
	EXPECT_TRUE(ab.estAVL());
}

TEST_F(ArbreTest, InsererAVLEnOrdreOk) {
	vector<int> v(ab.listerEnOrdre());
	EXPECT_EQ(1, v[0]);
	EXPECT_EQ(2, v[1]);
	EXPECT_EQ(3, v[2]);
	EXPECT_EQ(5, v[3]);
	EXPECT_EQ(6, v[4]);
	EXPECT_EQ(7, v[5]);
	EXPECT_EQ(8, v[6]);
}
TEST_F(ArbreTest, InsererAVLPreOrdreOk) {
	vector<int> v(ab.listerPreOrdre());
	EXPECT_EQ(3, v[0]);
	EXPECT_EQ(1, v[1]);
	EXPECT_EQ(2, v[2]);
	EXPECT_EQ(6, v[3]);
	EXPECT_EQ(5, v[4]);
	EXPECT_EQ(7, v[5]);
	EXPECT_EQ(8, v[6]);
}

TEST_F(ArbreTest, InsererAVLPostOrdreOk) {
	vector<int> v(ab.listerPostOrdre());
	EXPECT_EQ(2, v[0]);
	EXPECT_EQ(1, v[1]);
	EXPECT_EQ(5, v[2]);
	EXPECT_EQ(8, v[3]);
	EXPECT_EQ(7, v[4]);
	EXPECT_EQ(6, v[5]);
	EXPECT_EQ(3, v[6]);
}
TEST_F(ArbreTest, InsererAVLParNiveauOk) {
	vector<int> v(ab.listerParNiveau());
	EXPECT_EQ(3, v[0]);
	EXPECT_EQ(1, v[1]);
	EXPECT_EQ(6, v[2]);
	EXPECT_EQ(2, v[3]);
	EXPECT_EQ(5, v[4]);
	EXPECT_EQ(7, v[5]);
	EXPECT_EQ(8, v[6]);
}

TEST_F(ArbreTest, InsererAVLThrowSiPresent) {
	EXPECT_THROW(ab.insererAVL(3), PreconditionException);
	EXPECT_THROW(ab.insererAVL(8), PreconditionException);
}

TEST_F(ArbreTest, EnleverAVLNoThrow) {
	EXPECT_NO_THROW(ab.enleverAVL(3));
	EXPECT_NO_THROW(ab.enleverAVL(6));
	EXPECT_NO_THROW(ab.enleverAVL(1));
	EXPECT_NO_THROW(ab.enleverAVL(5));
	EXPECT_NO_THROW(ab.enleverAVL(2));
	EXPECT_NO_THROW(ab.enleverAVL(8));
}

TEST_F(ArbreTest, EnleverAVLEnleverOk) {
	ab.enleverAVL(3);
	EXPECT_TRUE(!ab.appartient(3));
	ab.enleverAVL(6);
	EXPECT_TRUE(!ab.appartient(6));
	ab.enleverAVL(1);
	EXPECT_TRUE(!ab.appartient(1));
	ab.enleverAVL(5);
	EXPECT_TRUE(!ab.appartient(5));
	ab.enleverAVL(2);
	EXPECT_TRUE(!ab.appartient(2));
	ab.enleverAVL(7);
	EXPECT_TRUE(!ab.appartient(7));
	ab.enleverAVL(8);
	EXPECT_TRUE(!ab.appartient(8));
}

TEST_F(ArbreTest, EnleverAVLResteTjrsAVL) {
	ab.enleverAVL(3);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(6);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(1);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(5);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(2);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(7);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(8);
	EXPECT_TRUE(ab.estAVL());
}
TEST_F(ArbreTest, EnleverAVLThrowSiPasPresent) {
	EXPECT_THROW(ab.enleverAVL(1231), PreconditionException);
	EXPECT_THROW(ab.enleverAVL(-1337), PreconditionException);
}

TEST_F(ArbreTest, InsererEnleverRafale) {
	ab.insererAVL(14);
	EXPECT_TRUE(ab.estAVL());
	ab.insererAVL(4);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(6);
	EXPECT_TRUE(ab.estAVL());
	ab.insererAVL(13);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(1);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(5);
	EXPECT_TRUE(ab.estAVL());
	ab.insererAVL(9);
	EXPECT_TRUE(ab.estAVL());
	ab.insererAVL(15);
	EXPECT_TRUE(ab.estAVL());
	ab.insererAVL(16);
	EXPECT_TRUE(ab.estAVL());
	ab.insererAVL(17);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(2);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(3);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(4);
	EXPECT_TRUE(ab.estAVL());
	ab.enleverAVL(13);
	EXPECT_TRUE(ab.estAVL());

	vector<int> v(ab.listerPreOrdre());
	EXPECT_EQ(14, v[0]);
	EXPECT_EQ(8, v[1]);
	EXPECT_EQ(7, v[2]);
	EXPECT_EQ(9, v[3]);
	EXPECT_EQ(16, v[4]);
	EXPECT_EQ(15, v[5]);
	EXPECT_EQ(17, v[6]);

	v = ab.listerPostOrdre();
	EXPECT_EQ(7, v[0]);
	EXPECT_EQ(9, v[1]);
	EXPECT_EQ(8, v[2]);
	EXPECT_EQ(15, v[3]);
	EXPECT_EQ(17, v[4]);
	EXPECT_EQ(16, v[5]);
	EXPECT_EQ(14, v[6]);

	v = ab.listerParNiveau();
	EXPECT_EQ(14, v[0]);
	EXPECT_EQ(8, v[1]);
	EXPECT_EQ(16, v[2]);
	EXPECT_EQ(7, v[3]);
	EXPECT_EQ(9, v[4]);
	EXPECT_EQ(15, v[5]);
	EXPECT_EQ(17, v[6]);

}

TEST_F(ArbreTest, InsererEnleverRafalePlus) {
	srand(time(NULL));
	int v;
	for (int i = 0; i < 1000; ++i) {
		v = rand() % 20;
		if (ab.appartient(v)) {
			ab.enleverAVL(v);
		} else {
			ab.insererAVL(v);
		}
		EXPECT_TRUE(ab.estAVL());

		// Décommentez ce bout de code pour voir tout les arbres créés.
		// Vous pouvez faire un ctrl-f dans la console pour trouver la séquence
		// des diaises qui vous donnera l'arbre qui fait défaut.
		// Pensez aussi à commenter srand(time(NULL)) pour garder le flux pseudo-alétoire fixe.
		//
		// Assurez-vous que la console n'ait pas de limite d'affichage en faisant
		//
		// Cliquez bouton de droite de la souris dans l'espace blanc de la console,
		// Preferences...
		// Décochez limit console output
		/*
		 if (!ab.estAVL()) {
		 cout << "########################" << endl;
		 afficherVector(ab.listerPostOrdre());
		 afficherVector(ab.listerPreOrdre());
		 afficherVector(ab.listerParNiveau());
		 cout << "########################" << endl;
		 } else {
		 cout << "---------------------" << endl;
		 afficherVector(ab.listerPostOrdre());
		 afficherVector(ab.listerPreOrdre());
		 afficherVector(ab.listerParNiveau());
		 cout << "---------------------" << endl;
		 }
		 */
	}

}

TEST_F(ArbreTest, ConstructeurCopieOk) {
	Arbre<int> ab2(ab);
	EXPECT_EQ(ab.listerEnOrdre(), ab2.listerEnOrdre());
	EXPECT_EQ(ab.listerParNiveau(), ab2.listerParNiveau());
	EXPECT_EQ(ab.listerPostOrdre(), ab2.listerPostOrdre());
	EXPECT_EQ(ab.listerPreOrdre(), ab2.listerPreOrdre());
}

TEST_F(ArbreTest, OperatorAsignationOk) {
	Arbre<int> ab2 = ab;
	EXPECT_EQ(ab.listerEnOrdre(), ab2.listerEnOrdre());
	EXPECT_EQ(ab.listerParNiveau(), ab2.listerParNiveau());
	EXPECT_EQ(ab.listerPostOrdre(), ab2.listerPostOrdre());
	EXPECT_EQ(ab.listerPreOrdre(), ab2.listerPreOrdre());
}

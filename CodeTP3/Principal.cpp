/**
 * \file Principal.cpp
 * \brief Fichier de chargement pour le dictionnaire, avec interface de traduction
 * \author IFT-2008, Étudiant(e)
 * \version 0.1
 * \date avril 2022
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Dictionnaire.h"
#include "algorithm"

using namespace std;
using namespace TP3;

int main()
{

	try
	{

		string reponse = "";
		ifstream englishFrench;

		bool reponse_lue = false; //Booléen pour savoir que l'utilisateur a saisi une réponse

		//Tant que la réponse est vide ou non valide, on redemande.
		while (!reponse_lue)
		{
			cout << "Entrez le nom du fichier du dictionnaire Anglais-Francais : ";
			getline(cin, reponse);
			englishFrench.open(reponse.c_str());
			if (englishFrench) {
				cout << "Fichier '" << reponse << "' lu!" << endl;
				reponse_lue = true;
			}
			else {
				cout << "Fichier '" << reponse << "' introuvable! Veuillez entrer un nom de fichier, ou son chemin absolu." << endl;
				cin.clear();
				cin.ignore();
			}
		}

		//Initialisation d'un ditionnaire, avec le constructeur qui utilise un fstream
		Dictionnaire dictEnFr(englishFrench);
		englishFrench.close();


	    // Affichage du dictionnaire niveau par niveau
	    cout << dictEnFr << endl;

        //Verification que l'arbre est bien balance
        if (dictEnFr.estBalancee()){
            cout << "*L'arbre est balancé*" << endl;
        }

		vector<string> motsAnglais; //Vecteur qui contiendra les mots anglais de la phrase entrée

		//Lecture de la phrase en anglais
		cout << "Entrez un texte en anglais (pas de majuscules ou de ponctuation/caracteres speciaux):" << endl;
		getline(cin, reponse);

		stringstream reponse_ss(reponse);
		string str_temp;

		//On ajoute un mot au vecteur de mots tant qu'on en trouve dans la phrase (séparateur = espace)
		while (reponse_ss >> str_temp)
		{
			motsAnglais.push_back(str_temp);
		}

		vector<string> motsFrancais; //Vecteur qui contiendra les mots traduits en français

		for (vector<string>::const_iterator i = motsAnglais.begin(); i != motsAnglais.end(); i++)
			// Itération dans les mots anglais de la phrase donnée
		{

            if (dictEnFr.appartient(*i))
            {
                vector<string> traduction = dictEnFr.traduit(*i);
                if (traduction.size() > 1){
                    cout << "Plusieurs traductions sont possibles pour le mot '" << *i << "'. Veuillez en choisir une parmi les suivantes :" << endl;
                    for (int i = 0; i < traduction.size(); i++){
                        cout << i + 1 << ". " << traduction[i] << "." << endl;
                    }
                    cout << "Votre choix: ";
                    size_t index;
                    cin >> index;
                    motsFrancais.push_back(traduction[index - 1]);
                }
                else{
                    motsFrancais.push_back(traduction[0]);
                }


            }
            else
            {
                vector<string> suggestion = dictEnFr.suggereCorrections(*i);
                if (suggestion.size() > 0)
                {
                    cout << "Le mot '" << *i << "' n'existe pas dans le dictionnaire. Veuillez choisir une des suggestions suivantes :" << endl;
                    for (int i = 0; i < suggestion.size(); i++){
                        cout << i + 1 << ". " << suggestion[i] << "." << endl;}

                    cout << "Votre choix: ";
                    size_t index;
                    cin >> index;
                    string motCorrigeAnglais = suggestion[index - 1];

                    vector<string> traduction = dictEnFr.traduit(motCorrigeAnglais);
                    if (traduction.size() > 1){
                        cout << "Plusieurs traductions sont possibles pour le mot '" << motCorrigeAnglais << "'. Veuillez en choisir une parmi les suivantes :" << endl;
                        for (int i = 0; i < traduction.size(); i++){
                            cout << i + 1 << ". " << traduction[i] << "." << endl;
                        }
                        cout << "Votre choix: ";
                        size_t index;
                        cin >> index;
                        motsFrancais.push_back(traduction[index - 1]);
                    }
                    else{
                        motsFrancais.push_back(traduction[0]);
                    }

                }
                else {
                    cout << "Le mot '" << *i << "' n'existe pas dans le dictionnaire et nous n'avons trouvé aucun mot similaire." << endl;
                }

            }
		}

		stringstream phraseFrancais; // On crée un string contenant la phrase,
									 // À partir du vecteur de mots traduits.
		for (vector<string>::const_iterator i = motsFrancais.begin(); i != motsFrancais.end(); i++)
		{
			phraseFrancais << *i << " ";
		}

		cout << "La phrase en francais est :" << endl << phraseFrancais.str() << endl;

	}
	catch (exception & e)
	{
		cerr << e.what() << endl;
	}

	return 0;
}
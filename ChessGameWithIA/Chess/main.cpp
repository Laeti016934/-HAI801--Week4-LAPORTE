#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "play.h"
using namespace sf;
int main(){
	
	// Initialisation de SFML et autres éléments graphiques
	play p;

	// Création de l'objet ChessBoard
	ChessBoard chessGame;

	// Génération d'un état initial aléatoire
	chessGame.generateRandomState();

	// Utilisation de l'algorithme Hill Climbing pour optimiser l'état du jeu
	ChessState finalState = chessGame.hillClimbing(1000); // Nombre maximum d'itérations

	return 0;
}
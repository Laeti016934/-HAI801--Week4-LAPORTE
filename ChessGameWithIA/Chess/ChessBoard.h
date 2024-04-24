#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "utilityBox.h"
#include "save.h"
#include <fstream>

using namespace sf;
using namespace std;

int spritepositions[64]={
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64,
    48,49,50,51,52,53,54,55,
    56,57,58,59,60,61,62,63};
int board[64]={
   -1,-2,-3,-4,-5,-3,-2,-1,
   -6,-6,-6,-6,-6,-6,-6,-6,
    0, 0, 0, 0,0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 6, 6, 6, 6, 6, 6,
    1, 2, 3, 4, 5, 3, 2, 1};

// Structure pour représenter un état du jeu d'échecs
struct ChessState {
	int board[64]; // Tableau pour représenter la disposition des pièces sur le plateau
	int currentPlayer; // Joueur dont c'est le tour de jouer
};

class ChessBoard{
  private:
    const float WIDTH = 650;
    const float HEIGTH = 650;
  public:
    ChessState currentState; // État actuel du jeu

    // Méthodes publiques pour la manipulation de l'état du jeu
    void generateRandomState();
    vector<ChessState> generateNeighbors(const ChessState& state);
    int evaluateState(const ChessState& state);
    ChessState hillClimbing(int maxIterations);
    vector<int> generateLegalMoves(const ChessState& state, int position);

    void loadtextures(Texture texture[64]);
    void loadboard(Texture texture[64],RectangleShape rectangle[64],Sprite sprite[64]);
    void MainFunctions(int u);
};

// Génère un état du jeu d'échecs aléatoire
void ChessBoard::generateRandomState() {
	// Initialisation de la disposition initiale des pièces
    int initialBoard[64] = {
        -1, -2, -3, -4, -5, -3, -2, -1,
        -6, -6, -6, -6, -6, -6, -6, -6,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         6,  6,  6,  6,  6,  6,  6,  6,
         1,  2,  3,  4,  5,  3,  2,  1
    };

    // Copie de la disposition initiale sur le plateau actuel
    for (int i = 0; i < 64; ++i) {
        currentState.board[i] = initialBoard[i];
    }

    // Mélange aléatoire des positions des pièces sur le plateau
    random_shuffle(currentState.board, currentState.board + 64);

    // Définition aléatoire du joueur dont c'est le tour de jouer
    currentState.currentPlayer = (rand() % 2 == 0) ? -1 : 1;
}

vector<int> ChessBoard::generateLegalMoves(const ChessState& state, int position) {
    vector<int> legalMoves;

    // Récupérer les coordonnées de la position
    int row = position / 8;
    int col = position % 8;

    // Vérifier le type de pièce sur la position actuelle
    int piece = state.board[position];
    bool isWhitePiece = (piece > 0);

    // Générer les mouvements légaux en fonction du type de pièce
    switch (abs(piece)) {
        case 1: // Roi
            // Générer les mouvements vers toutes les cases adjacentes
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    int newRow = row + dr;
                    int newCol = col + dc;
                    if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 && !(dr == 0 && dc == 0)) {
                        int newPosition = newRow * 8 + newCol;
                        if (state.board[newPosition] == 0 || (isWhitePiece != (state.board[newPosition] > 0))) {
                            legalMoves.push_back(newPosition);
                        }
                    }
                }
            }
            break;
        case 2: // Reine
            // Générer les mouvements horizontaux, verticaux et diagonaux
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr != 0 || dc != 0) {
                        for (int i = 1; i < 8; ++i) {
                            int newRow = row + dr * i;
                            int newCol = col + dc * i;
                            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                                int newPosition = newRow * 8 + newCol;
                                if (state.board[newPosition] == 0) {
                                    legalMoves.push_back(newPosition);
                                } else {
                                    if (isWhitePiece != (state.board[newPosition] > 0)) {
                                        legalMoves.push_back(newPosition);
                                    }
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case 3: // Tour
            // Générer les mouvements horizontaux et verticaux
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if ((dr == 0 || dc == 0) && (dr != 0 || dc != 0)) {
                        for (int i = 1; i < 8; ++i) {
                            int newRow = row + dr * i;
                            int newCol = col + dc * i;
                            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                                int newPosition = newRow * 8 + newCol;
                                if (state.board[newPosition] == 0) {
                                    legalMoves.push_back(newPosition);
                                } else {
                                    if (isWhitePiece != (state.board[newPosition] > 0)) {
                                        legalMoves.push_back(newPosition);
                                    }
                                    break;
                                }
                            } else {generateLegalMoves
                        }
                    }
                }
            }
            break;
        case 4: // Fou
            // Générer les mouvements diagonaux
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr != 0 && dc != 0) {
                        for (int i = 1; i < 8; ++i) {
                            int newRow = row + dr * i;
                            int newCol = col + dc * i;
                            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                                int newPosition = newRow * 8 + newCol;
                                if (state.board[newPosition] == 0) {
                                    legalMoves.push_back(newPosition);
                                } else {
                                    if (isWhitePiece != (state.board[newPosition] > 0)) {
                                        legalMoves.push_back(newPosition);
                                    }
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case 5: // Cavalier
            // Générer les mouvements du cavalier (en L)
            int delta[8][2] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}};
            for (int i = 0; i < 8; ++i) {
                int newRow = row + delta[i][0];
                int newCol = col + delta[i][1];
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                    int newPosition = newRow * 8 + newCol;
                    if (state.board[newPosition] == 0 || (isWhitePiece != (state.board[newPosition] > 0))) {
                        legalMoves.push_back(newPosition);
                    }
                }
            }
            break;
        case 6: // Pion
            // Générer les mouvements du pion
            int forward = isWhitePiece ? -1 : 1;
            int startRow = isWhitePiece ? 6 : 1;
            // Avancer d'une case
            int newRow = row + forward;
            int newCol = col;
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 && state.board[newRow * 8 + newCol] == 0) {
                legalMoves.push_back(newRow * 8 + newCol);
                // Avancer de deux cases à partir de la position initiale
                newRow += forward;
                if (row == startRow && state.board[newRow * 8 + newCol] == 0) {
                    legalMoves.push_back(newRow * 8 + newCol);
                }
            }
            // Capturer en diagonale
            int deltaCol[2] = {-1, 1};
            for (int dc : deltaCol) {
                newRow = row + forward;
                newCol = col + dc;
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 &&
                    state.board[newRow * 8 + newCol] * state.currentPlayer < 0) {
                    legalMoves.push_back(newRow * 8 + newCol);
                }
            }
            break;
        default:
            // Cas non géré
            break;
    }

    return legalMoves;
}


// Génère les voisins d'un état du jeu
vector<ChessState> ChessBoard::generateNeighbors(const ChessState& state) {
  
    vector<ChessState> neighbors;

    // Parcourir toutes les cases du plateau
    for (int i = 0; i < 64; ++i) {
        // Vérifier si la case contient une pièce du joueur actuel
        if (state.board[i] != 0 && state.board[i] * state.currentPlayer > 0) {
            // Générer les mouvements légaux possibles pour cette pièce
            vector<int> legalMoves = generateLegalMoves(state, i);
            
            // Pour chaque mouvement légal, générer un nouvel état
            for (int move : legalMoves) {
                ChessState newState = state;
                newState.board[i] = 0; // Enlever la pièce de sa position actuelle
                newState.board[move] = state.board[i]; // Déplacer la pièce vers la nouvelle position
                newState.currentPlayer *= -1; // Passer au joueur suivant
                neighbors.push_back(newState); // Ajouter le nouvel état à la liste des voisins
            }
        }
    }

    return neighbors;
}

// Évaluer la sécurité du roi
bool isKingInCheck(const ChessState& state) {
    int kingPosition = -1;
    for (int i = 0; i < 64; ++i) {
        if (state.board[i] == 6 || state.board[i] == -6) { // Si c'est un roi
            kingPosition = i;
            break;
        }
    }

    if (kingPosition == -1) {
        // Le roi n'est pas sur l'échiquier, situation invalide
        return false;
    }

    // Vérifier s'il y a une menace pour le roi
    for (int i = 0; i < 64; ++i) {
        int piece = state.board[i];
        if (piece != 0 && (piece * state.board[kingPosition] < 0)) { // Si la pièce est ennemie
            vector<int> legalMoves = generateLegalMoves(state, i);
            for (int move : legalMoves) {
                if (move == kingPosition) {
                    // Le roi est menacé
                    return true;
                }
            }
        }
    }

    // Le roi n'est pas menacé
    return false;
}



// Évalue la qualité d'un état du jeu
int ChessBoard::evaluateState(const ChessState& state) {
    int score = 0;

    // Évaluer la mobilité des pièces
    vector<int> whiteMoves;
    vector<int> blackMoves;
    for (int i = 0; i < 64; ++i) {
        if (state.board[i] > 0) {
            vector<int> legalMoves = generateLegalMoves(state, i);
            whiteMoves.insert(whiteMoves.end(), legalMoves.begin(), legalMoves.end());
        } else if (state.board[i] < 0) {
            vector<int> legalMoves = generateLegalMoves(state, i);
            blackMoves.insert(blackMoves.end(), legalMoves.begin(), legalMoves.end());
        }
    }
    score += (whiteMoves.size() - blackMoves.size());

    // Évaluer la position des pièces
    static const int pieceSquareTable[6][64] = {
        // Pour les pions
        {
            0,  0,  0,  0,  0,  0,  0,  0,
            50, 50, 50, 50, 50, 50, 50, 50,
            10, 10, 20, 30, 30, 20, 10, 10,
             5,  5, 10, 25, 25, 10,  5,  5,
             0,  0,  0, 20, 20,  0,  0,  0,
             5, -5,-10,  0,  0,-10, -5,  5,
             5, 10, 10,-20,-20, 10, 10,  5,
             0,  0,  0,  0,  0,  0,  0,  0
        },
        // Pour les cavaliers
        {
            -50,-40,-30,-30,-30,-30,-40,-50,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -30,  0, 10, 15, 15, 10,  0,-30,
            -30,  5, 15, 20, 20, 15,  5,-30,
            -30,  0, 15, 20, 20, 15,  0,-30,
            -30,  5, 10, 15, 15, 10,  5,-30,
            -40,-20,  0,  5,  5,  0,-20,-40,
            -50,-40,-30,-30,-30,-30,-40,-50
        },
        // Pour les fous
        {
            -20,-10,-10,-10,-10,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  5,  5, 10, 10,  5,  5,-10,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -10, 10, 10, 10, 10, 10, 10,-10,
            -10,  5,  0,  0,  0,  0,  5,-10,
            -20,-10,-10,-10,-10,-10,-10,-20
        },
        // Pour les tours
        {
            0,  0,  0,  0,  0,  0,  0,  0,
            5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            0,  0,  0,  5,  5,  0,  0,  0
        },

        // Pour les rois
        {
            20, 30, 10,  0,  0, 10, 30, 20,
            20, 20,  0,  0,  0,  0, 20, 20,
            -10,-20,-20,-20,-20,-20,-20,-10,
            -20,-30,-30,-40,-40,-30,-30,-20,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30
        }
    };
      for (int i = 0; i < 64; ++i) {
        int piece = state.board[i];
        if (piece != 0) {
          int pieceType = abs(piece) - 1;
          int sign = (piece > 0) ? 1 : -1;
          score += sign * pieceSquareTable[pieceType][i];
        }
      }

  // Évaluer la sécurité du roi
    if (isKingInCheck(state)) {
        // Si le roi est en échec, réduire le score pour refléter cela
        score -= 100; // Valeur arbitraire pour indiquer un roi en échec
    }

  return score;
}

// Algorithme Hill Climbing pour optimiser l'état du jeu
ChessState ChessBoard::hillClimbing(int maxIterations) {
	ChessState current = currentState;
	int currentScore = evaluateState(current);

	for (int i = 0; i < maxIterations; ++i) {
    	vector<ChessState> neighbors = generateNeighbors(current);
    	bool improved = false;

    	for (const auto& neighbor : neighbors) {
        	int neighborScore = evaluateState(neighbor);
        	if (neighborScore > currentScore) {
            	current = neighbor;
            	currentScore = neighborScore;
            	improved = true;
            	break; // Pas besoin de chercher d'autres voisins
        	}
    	}

    	if (!improved) {
        	// Aucun voisin améliorant trouvé, on a atteint un maximum local
        	break;
    	}
	}

	return current;
}

void ChessBoard::loadtextures(Texture texture[64]){
    for(int i=0;i<64;i++){
      if(spritepositions[i]==0 || spritepositions[i]==7)
        texture[i].loadFromFile("images/blackRook.png");
      if(spritepositions[i]==1 || spritepositions[i]==6)
        texture[i].loadFromFile("images/blackKnight.png");
      if(spritepositions[i]==2 || spritepositions[i]==5)
        texture[i].loadFromFile("images/blackBishop.png");
      if(spritepositions[i]==3)
        texture[i].loadFromFile("images/blackQueen.png");
      if(spritepositions[i]==4)
        texture[i].loadFromFile("images/blackKing.png");
      if(spritepositions[i]>=8 and spritepositions[i]<=15)
        texture[i].loadFromFile("images/blackPawn.png");
      if(spritepositions[i]==63 || spritepositions[i]==56)
        texture[i].loadFromFile("images/whiteRook.png");
      if(spritepositions[i]==62 || spritepositions[i]==57)
         texture[i].loadFromFile("images/whiteKnight.png");
      if(spritepositions[i]==61 || spritepositions[i]==58)
         texture[i].loadFromFile("images/whiteBishop.png");
      if(spritepositions[i]==59)
         texture[i].loadFromFile("images/whiteQueen.png");
      if(spritepositions[i]==60)
         texture[i].loadFromFile("images/whiteKing.png");
      if(spritepositions[i]>=48 and spritepositions[i]<=55)
         texture[i].loadFromFile("images/whitePawn.png");     
    }
  }
void ChessBoard::loadboard(Texture texture[64],RectangleShape rectangle[64],Sprite sprite[64]){
    for(int j=0;j<64;j++){
        sprite[j].setTexture(texture[j]);
        sprite[j].setScale(1.7f,1.7f);
    }
   for (int j = 0; j < 64; ++j)
    {
        rectangle[j].setSize(sf::Vector2f(WIDTH/8.0f, HEIGTH/8.0f));
    }
    int counter=0;
    for (int i = 0; i < 8; ++i)
    {
      for (int j = 0; j < 8; ++j)
      {
        rectangle[counter].setPosition(j * rectangle[counter].getSize().y, i * rectangle[counter].getSize().x);  ///x,y
        sprite[counter].setPosition(j * rectangle[counter].getSize().y, i * rectangle[counter].getSize().x);
        if((i+j) % 2 == 0)
          rectangle[counter].setFillColor(sf::Color::White);
        else
          rectangle[counter].setFillColor(sf::Color::Blue);
        counter++;
      }
    }
  }
void ChessBoard::MainFunctions(int u)
  {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGTH), "Chess The Game Of Kings!");
    sf::RectangleShape rectangle[64];
    sf::Texture texture[65];
    sf::Sprite sprite[65];
    loadtextures(texture);
    loadboard(texture,rectangle,sprite);
    Identity box;
    bool isMove;
    float dx=0,dy=0;
    Vector2f oldPos,newPos;
    int n;
    int cc,turn=1;
    int alience,position;
    Vector2f firstpos,secondpos;
    int v;int q[64];
    static int cap=0;
    for (int j = 0; j < 64; ++j)
      q[j]=64;
    while (window.isOpen())
    {   Vector2i pos= Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
              save s;
              window.close();
              if(s.smain()){
              ofstream out,out2;
              out.open("spritepositions.txt");
              out2.open("boardpositions.txt");
              for(int i=0;i<64;i++){
                out<<spritepositions[i]<<",";
                out2<<board[i]<<",";
              }
              out.close();
              out2.close();}
              }
            if(u!=0){
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
              for (int j = 0; j < 64; ++j){
                if(turn%2==0 and board[j]<0){
                  if (rectangle[j].getGlobalBounds().contains(pos.x,pos.y))
                      {
                        n=j;
                        firstpos=rectangle[j].getPosition();
                        v=spritepositions[j];
                        rectangle[n].setFillColor(sf::Color::Red);
                        if(spritepositions[n]!=64)
                        
                        cap++;
                  }
              }
            }
              for (int j = 0; j < 64; ++j){
                if(turn%2!=0 and board[j]>0){
                  if (rectangle[j].getGlobalBounds().contains(pos.x,pos.y))
                      {
                        n=j;
                        firstpos=rectangle[j].getPosition();
                        v=spritepositions[j];
                        rectangle[n].setFillColor(sf::Color::Red);
                        if(spritepositions[n]!=64)
                        cap++;
                  }
              }
            }

           }
           if(cap!=0)
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
              for (int j = 0; j < 64; ++j){
                  //cout<<box.identifier(n,j,board[j],board)<<"=identity"<<endl;
                  if (rectangle[j].getGlobalBounds().contains(pos.x,pos.y)){
                        //if(n==8 and j==16 and board[n]==-6){ 
                        isMove=box.identifier(n,j,board[n],board);//<<"=identity"<<endl;
                        secondpos=rectangle[j].getPosition();
                        int spritepos=spritepositions[n];
                        //cout<<"shaheer"<<endl;
                        if(isMove){
                        turn++;
                        cc=q[j]=spritepositions[j];
                        if(j!=n){
                        sprite[spritepos].setPosition(secondpos);
                        sprite[cc].setPosition(100000000,100000000);
                        int suppos=spritepositions[j]; 
                        spritepositions[j]=spritepositions[n];
                        spritepositions[n]=64;
                        if(board[j]==-5 || board[j]==5){
                          save s;
                          window.close();
                          if(s.smain()){
                            ofstream out,out2;
                            out.open("spritepositions.txt");
                            out2.open("boardpositions.txt");
                            for(int i=0;i<64;i++){
                              out<<spritepositions[i]<<",";
                              out2<<board[i]<<",";
                            }
                            out.close();
                            out2.close();}
                        }
                        if(j<=63 and j>=56 and board[n]==-6){
                          board[j]=-4;
                        }
                        else if(j>=0 and j<=7 and board[n]==6){
                          board[j]=4;
                        }
                        else{
                        board[j]=board[n];
                        board[n]=0;}
                        n=j;}
                      }
                        int counter=0;
                        for (int i = 0; i < 8; ++i){
                          for (int j = 0; j < 8; ++j){
                            if((i+j) % 2 == 0)
                              rectangle[counter].setFillColor(sf::Color::White);
                            else
                              rectangle[counter].setFillColor(sf::Color::Blue);
                              counter++;
                              
                              }

                          }

                      }
              }
              cap=0;
            
            }
        }
      }

        window.clear();
        for (int j = 0; j < 64; ++j)
         window.draw(rectangle[j]);
      for (int j = 0; j < 65; j++){
          if(q[j]==64)
          window.draw(sprite[j]);
      }
        
        window.display();
    }
}

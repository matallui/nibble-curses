#ifndef _NIBBLES_H_
#define _NIBBLES_H_
/**
 * @file CNibbles.h
 * @author Luís Taniça
 */
#include <cstdlib>	// for srand
#include <iostream>
#include "CMultNet.h"
#include "CException.h"

/**
 * Class used for playing the Nibbles game.
 */
class CNibbles
{
public:
  CNibbles ( void );
  ~CNibbles ( void );
  void Start ( void );
  
protected:
  int m_maxLines;		///< screen lines
  int m_maxCols;		///< screen columns
  int m_level;			///< current level
  int m_num_obstacles;		///< current number of obstacles
  int m_nplayers;		///< number of players
  int m_mode;			///< 1- Single Player; 2- Multi same PC; 3- Multi over Network
  bool m_master;		///< true if this PC received the connection
  int m_speed;			///< game speed
  int m_score;			///< global score (sum of player scores)
  int m_state;			///< current game state - used to detect errors or game events (e.g. collisions)
  CObstacle * m_obstacles;	///< obstacle array
  CSnake ** m_snakes;		///< snake pointer array
  CPoint m_xyfood;		///< current food coordinates
  int m_nfood;			///< current food number
  CSocket * m_socket;		///< socket pointer
  CException m_exception;	///< used to handle exceptions
  
  void DrawMenu ( char menu[5][30], char title[20], int const & size, int const & item );
  void Menu ( void );
  void PresentLevel ( void );
  void PlayerDies ( int const & player ) const;
  bool GameOver_Restart ( void ) const;
  bool Winner_Restart (  void ) const;
  void PopUp ( char * title1, char * title2 ) const;
  
  void Restart ( void );
  void InitLevel ( void );
  void PutFood ( void );
  void UpdateScores ( void );
  bool Step ( void );
};


#endif
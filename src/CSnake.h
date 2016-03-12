/**
 * @file CSnake.h
 * @author Luís Taniça
 */
#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "CObstacle.h"

/**
 * Virtual Class to handle snakes.
 */
class CSnake
{
public:
  CSnake ( void );
  virtual ~CSnake ( void );
  virtual void Reset ( void );
  virtual void Initialize ( int const & nlevel ) = 0;								///< virtual method
  virtual bool Collision ( CObstacle * const obstacles, int const & num_obstacles, CSnake * sfriend ) = 0;	///< virtual method
  bool hasPoint ( CPoint const & p ) const;
  virtual bool Eat ( CPoint const & food, int const & nfood, int const & level );
  virtual bool Move ( void ) = 0;										///< virtual method
  int getLives ( void );
  int getScore ( void );
  
protected:
  CPoint * m_body;	///< holds snake body coordinates
  int m_max_size;	///< m_body array size
  int m_size;		///< current snake size
  int m_head;		///< head position on array
  int m_tail;		///< tail position on array
  int m_score;		///< player score
  int m_lives;		///< player lives
  char m_direction;	///< current direction
  int m_delay;		///< used to increment snake size
  int m_cp_draw;	///< color pair to draw snake
  int m_cp_erase;  	///< color pair to erase snake
};

#endif

/**
 * @file CSnake.cpp
 * Contains the declarations for the class CSnake.
 * This is a virtual class.
 * @author Luís Taniça
 * @see CSingle CMultPC CMultNet
 */
#include "CSnake.h"

/** Constructor
 */
CSnake::CSnake ( void )
{
  m_size = 0;
  m_max_size = 150;
  m_body = new CPoint [150];
  m_head = 0;
  m_tail = 0;
  m_score = 0;
  m_delay = 0;
  m_lives = 5;
  m_direction = '-';
}

/** Destructor
 */
CSnake::~CSnake ( void )
{
  delete [] m_body;
}

/** Resets player lives and score
 */
void CSnake::Reset ( void )
{
  m_lives = 5;
  m_score = 0;
}

/** Check if point p intersects the snake body
 */
bool CSnake::hasPoint ( CPoint const & p ) const
{ 
  if ( !m_lives ) return false;
  
  int i = m_head;
  for ( int k = 0; k < m_size; k++ )
  {
    if ( m_body[i] == p ) return true;
    i = (i + (m_max_size - 1)) % m_max_size;
  }
  return false;
}

/** Check if the snake intersects the food.
 * If yes, increments score and body length.
 */
bool CSnake::Eat ( CPoint const & food, int const & nfood, int const & level )
{
  if ( !m_lives ) return false;
  
  if ( m_body[m_head] == food )
  {
    m_delay += (3 * nfood);
    m_score += ( 100*level + 100*nfood );
    return true;
  }
  return false;
}

/** Getter for snake lives
 */
int CSnake::getLives ( void )
{
  return m_lives;
}

/** Getter for snake score
 */
int CSnake::getScore ( void )
{
  return m_score;
}


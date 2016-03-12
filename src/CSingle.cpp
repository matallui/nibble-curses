/**
 * @file CSingle.cpp
 * Contains the decarations for the class CSingle
 * @author Luís Taniça
 * @see CSnake CMultPC CMultNet
 */
#include "CSingle.h"
#include "CSnake.h"

/** Contructor
 */
CSingle::CSingle ( void ) {
  m_cp_draw = 4;
  m_cp_erase = 1;
}

/** Destructor
 */
CSingle::~CSingle ( void ) {}

/**
 * Initializes and draws the snake according to the game level.
 */
void CSingle::Initialize ( int const & nlevel )
{
  if ( !m_lives ) return;
  
  m_size = 3;
  m_head = 2;
  m_tail = 0;
  m_delay = 0;
  
  switch ( nlevel )
  {
    case 1:
      m_body[2].Update ( 17, 80 );
      m_body[1].Update ( 17, 81 );
      m_body[0].Update ( 17, 82 );
      m_direction = 'L';
      break;
    case 2:
      m_body[2].Update ( 10, 80 );
      m_body[1].Update ( 10, 81 );
      m_body[0].Update ( 10, 82 );
      m_direction = 'L';
      break;
    case 3:
    case 4:
    case 6:
    case 7:
      m_body[2].Update ( 4, 88 );
      m_body[1].Update ( 4, 89 );
      m_body[0].Update ( 4, 90 );
      m_direction = 'L';
      break;
    case 5:
      m_body[2].Update ( 18, 88 );
      m_body[1].Update ( 18, 89 );
      m_body[0].Update ( 18, 90 );
      m_direction = 'L';
      break;
    case 8:
    case 10:
      m_body[2].Update ( 5, 94 );
      m_body[1].Update ( 4, 94 );
      m_body[0].Update ( 3, 94 );
      m_direction = 'D';
      break;
    case 9:
      m_body[2].Update ( 1, 88 );
      m_body[1].Update ( 1, 89 );
      m_body[0].Update ( 1, 90 );
      m_direction = 'L';
  }
}

/**
 * Detects if the snake collides with obstacle or itself.
 * @param obstacles obstacles array
 * @param num_obstacles size of obstacles array
 * @param sfriend should be NULL
 */
bool CSingle::Collision ( CObstacle * const obstacles, int const & num_obstacles, CSnake * sfriend )
{
  if ( !m_lives ) return false;
  
  // collision with obstacles
  for ( int i = 0; i < num_obstacles; i++ )
    if ( obstacles[i].Intersection ( m_body[m_head] ) )
    {
      m_lives --;
      m_score -= 1000;
      return true;
    }
  
  // collision with body
  int i = m_tail;
  for ( int k = 0; k < m_size-3; k++ )
  {
    if ( m_body[m_head] == m_body[i] )
    {
      m_lives --;
      m_score -= 1000;
      return true;
    }
    i = (i + 1) % m_max_size;
  }
  
  return false;
}

/**
 * Moves the snake by one step.
 * Detects keyboard input.
 */
bool CSingle::Move ( void )
{
  if ( !m_lives ) return true;
  
  int next_head = (m_head + 1) % m_max_size;
  
  int key = getch();
  switch ( key )
  {
    case KEY_UP:
      if ( m_direction != 'D' ) m_direction = 'U';
      break;
    case KEY_DOWN:
      if ( m_direction != 'U' ) m_direction = 'D';
      break;
    case KEY_LEFT:
      if ( m_direction != 'R' ) m_direction = 'L';
      break;
    case KEY_RIGHT:
      if ( m_direction != 'L' ) m_direction = 'R';
      break;
  }
  
  // snake has to increase
  if ( m_delay > 0 )
  {
    m_delay --;
    m_size ++;
  }
  else
  {
    attrset ( COLOR_PAIR(m_cp_erase) );
    m_body[m_tail].Draw( ' ' );
    m_tail = (m_tail + 1) % m_max_size;
  }
  
  // update new head coordinates
  m_body[next_head] = m_body[m_head];
  m_head = next_head;
  
  switch ( m_direction )
  {
    case 'U':
      m_body[m_head].decX(1);
      break;
    case 'D':
      m_body[m_head].incX(1);
      break;
    case 'L':
      m_body[m_head].decY(1);
      break;
    case 'R':
      m_body[m_head].incY(1);
      break;
  }
  
  attrset ( COLOR_PAIR(m_cp_draw) );
  m_body[m_head].Draw ( '@' );
  refresh();
  return true;
}


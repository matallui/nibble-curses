/**
 * @file CMultPC.cpp
 * Contains the declarations for the class CMultPC.
 * @author Luís Taniça
 * @see CSnake CSingle CMultNet
 */
#include "CMultPC.h"

/** Contructor.
 * Defines the colors according to player id.
 * @param id 1 or 2
 */
CMultPC::CMultPC ( int const & id )
	: m_playerid (id), m_friend_alive ( true )
{
  if ( m_playerid == 1 ) 	m_cp_draw = 4;
  else				m_cp_draw = 5;
  m_cp_erase = 1;
}

/** Destructor
 */
CMultPC::~CMultPC ( void ) {}

/** Resets snake lives and score.
 */
void CMultPC::Reset ( void )
{
  m_lives = 5;
  m_score = 0;
  m_friend_alive = true;
}

/** Initializes the snake on the game board according to the level.
 * Player 1 and 2 have different start points.
 */
void CMultPC::Initialize ( int const & nlevel )
{
  if ( !m_lives ) return;
  if ( m_playerid == 1 ) 
  {
    CSingle::Initialize ( nlevel );
    return;
  }
  
  m_size = 3;
  m_head = 2;
  m_tail = 0;
  m_delay = 0;
  
  switch ( nlevel )
  {
    case 1:
      m_body[2].Update ( 19, 20 );
      m_body[1].Update ( 19, 19 );
      m_body[0].Update ( 19, 18 );
      m_direction = 'R';
      break;
    case 2:
      m_body[2].Update ( 26, 20 );
      m_body[1].Update ( 26, 19 );
      m_body[0].Update ( 26, 18 );
      m_direction = 'R';
      break;
    case 3:
    case 4:
    case 6:
    case 7:
      m_body[2].Update ( 31, 20 );
      m_body[1].Update ( 31, 19 );
      m_body[0].Update ( 31, 18 );
      m_direction = 'R';
      break;
    case 5:
      m_body[2].Update ( 19, 20 );
      m_body[1].Update ( 19, 19 );
      m_body[0].Update ( 19, 18 );
      m_direction = 'R';
      break;
    case 8:
    case 10:
      m_body[2].Update ( 30, 5 );
      m_body[1].Update ( 31, 5 );
      m_body[0].Update ( 32, 5 );
      m_direction = 'U';
      break;
    case 9:
      m_body[2].Update ( 34, 20 );
      m_body[1].Update ( 34, 19 );
      m_body[0].Update ( 34, 18 );
      m_direction = 'R';
  }
}

/**
 * Detects if the snake collides with obstacle, itself or with the other player.
 * @param obstacles obstacle array
 * @param num_obstacles size of obstacle array
 * @param sfriend pointer to the other player's snake
 */
bool CMultPC::Collision ( CObstacle * const obstacles, int const & num_obstacles, CSnake * sfriend )
{
  if ( !m_lives ) return false;
  
  // collision with obstacles
  for ( int i = 0; i < num_obstacles; i++ )
    if ( obstacles[i].Intersection ( m_body[m_head] ) )
    {
      Die ( dynamic_cast <CMultPC*> (sfriend) );
      m_score -= 1000;
      return true;
    }
  
  // collision with itself
  int i = m_tail;
  for ( int k = 0; k < m_size-3; k++ )
  {
    if ( m_body[m_head] == m_body[i] )
    {
      Die ( dynamic_cast <CMultPC*> (sfriend) );
      m_score -= 1000;
      return true;
    }
    i = (i + 1) % m_max_size;
  }
  
  // collision with friend
  if ( sfriend ) 
    if ( sfriend->hasPoint ( m_body[m_head] ) )
    {
      Die ( dynamic_cast <CMultPC*> (sfriend) );
      m_score -= 1000;
      return true;
    }
  
  return false;
}

/** Moves the snake by one step.
 * Detects keyboard input. If any key belonging to the other player is detected,
 * puts it into the keyboard buffer again (unless the other player is dead).
 */
bool CMultPC::Move ( void )
{
  if ( !m_lives ) return true;
  
  int next_head = (m_head + 1) % m_max_size;
  int key = getch();
  
  if ( m_playerid == 1 )
  {
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
      case 'W':
      case 'w':
      case 'S':
      case 's':
      case 'A':
      case 'a':
      case 'D':
      case 'd':
	if ( m_friend_alive )
	  ungetch ( key );
	break;
    }
  }
  else
  {
    switch ( key )
    {
      case 'W':
      case 'w':
	if ( m_direction != 'D' ) m_direction = 'U';
	break;
      case 'S':
      case 's':
	if ( m_direction != 'U' ) m_direction = 'D';
	break;
      case 'A':
      case 'a':
	if ( m_direction != 'R' ) m_direction = 'L';
	break;
      case 'D':
      case 'd':
	if ( m_direction != 'L' ) m_direction = 'R';
	break;
      case KEY_UP:
      case KEY_DOWN:
      case KEY_LEFT:
      case KEY_RIGHT:
	if ( m_friend_alive )
	  ungetch ( key );
	break;
    }
  }
  
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

/** Decrements the snake's lives by one.
 * If last life, adverstises the other player.
 */
void CMultPC::Die ( CMultPC * sfriend )
{
  if ( m_lives > 0 )
  {
    m_lives --;
    if ( !m_lives )
      sfriend->ManDown();
  }
}

/** Acknowledges friend's death.
 */
void CMultPC::ManDown ( void )
{
  m_friend_alive = false;
}
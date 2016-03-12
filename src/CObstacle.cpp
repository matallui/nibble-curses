/**
 * @file CObstacle.cpp
 * Contains the declarations for the class CObstacle
 * @author Luís Taniça
 */

#include "CObstacle.h"

/** Contructor
 * @param ox X coordinate where the obstacle begins
 * @param oy Y coordinate where the obstacle begins
 * @param length obstacle length
 * @param mode define if the obstacle will be vertical (='V'), horizontal (='H'), diagonal (='D') or Interlaced (='I')
 */
CObstacle::CObstacle ( int const & ox, int const & oy, int const & length, char const & mode )
		      : m_orig ( ox, oy )
{
  m_length = length;
  m_mode = mode;
  m_cp_draw = 2;
  m_cp_erase = 1;
}

/** 
 * Updates object variables
 */
void CObstacle::Update ( int const & ox, int const & oy, int const & length, char const & mode )
{
  m_orig.setX ( ox );
  m_orig.setY ( oy );
  m_length = length;
  m_mode = mode;
}

/**
 * Set the color in wich the obstacle shall be drawn
 */
void CObstacle::SetColors ( int const & cp_draw, int const & cp_erase )
{
  m_cp_draw = cp_draw;
  m_cp_erase = cp_erase;
}

/**
 * Paints the obstacle into the screen
 */
void CObstacle::Paint ( void ) const
{
  int x = m_orig.getX();
  int y = m_orig.getY();
  
  switch ( m_mode )
  {
    case 'V':					// draw vertical object starting on the upper point
      for ( int i = 0; i < m_length; i++ )
      {
	move ( x+i, y );
	addch ( ' ' );
      }
      break;
    case 'H':					// draw horizontal object starting on the left
      for ( int i = 0; i < m_length; i++ )
      {
	move ( x, y+i );
	addch ( ' ' );
      }
      break;
    case 'D':					// draw diagonal object starting on the upper-left points
      for ( int i = 0; i < m_length; i++ )
      {
	move ( x+i, y+i );
	addch ( ' ' );
      }
      break;
    case 'I':					// draw dashed vertical object starting from upper point
      for ( int i = 0; i < m_length; i+=2 )
      {
	move ( x+i, y );
	addch ( ' ' );
      }
      break;
  }
  
  refresh();
}

/**
 * Draw the obstacle into the screen
 */
void CObstacle::Draw ( void ) const
{
  attrset ( COLOR_PAIR(m_cp_draw) );
  Paint ();
}

/**
 * Erases the obstacle from the screen
 */
void CObstacle::Erase ( void ) const
{
  attrset ( COLOR_PAIR(m_cp_erase) );
  Paint ();
}

/**
 * Determines if the point p intersects the obstacle
 */
bool CObstacle::Intersection ( CPoint const & p ) const
{
  int px = p.getX();
  int py = p.getY();
  int ox = m_orig.getX();
  int oy = m_orig.getY();
  bool res;
  
  switch ( m_mode )
  {
    case 'V':
      res = ( (py == oy) && (px >= ox) && (px < (ox+m_length)) );
      break;
    case 'H':
      res = ( (px == ox) && (py >= oy) && (py < (oy+m_length)) );
      break;
    case 'D':
      res = ( (px >= ox) && (px < (ox+m_length)) && (py >= oy) && (py < (oy+m_length)) && ((py-px) == (oy-ox)) );
      break;
    case 'I':
      res = ( (py == oy) && (px >= ox) && (px < (ox+m_length)) && ((px%2) == (ox%2)) );
  }
  return res;
}
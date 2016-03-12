/**
 * @file CPoint.cpp
 * Contains the declarations for the class CPoint
 * @author Luís Taniça
 */

#include "CPoint.h"

/**
 * Constructor
 */
CPoint::CPoint ( int const & x, int const & y )
{
  m_x = x;
  m_y = y;
}

/** 
 * Destructor
 */
CPoint::CPoint ( CPoint const & src )
{
  m_x = src.getX();
  m_y = src.getY();
}

/**
 * Assignem operator
 */
CPoint & CPoint::operator = ( CPoint const & src )
{
  if ( &src == this ) return *this;
  
  m_x = src.getX();
  m_y = src.getY();
  return *this;
}

/**
 * Getter for X coordinate
 */
int CPoint::getX ( void ) const
{
  return m_x;
}

/**
 * Getter for Y coordinate
 */
int CPoint::getY ( void ) const
{
  return m_y;
}

/**
 * Setter for X coordinate
 */
void CPoint::setX ( int const & x )
{
  m_x = x;
}

/**
 * Setter for Y coordinate
 */
void CPoint::setY ( int const & y )
{
  m_y = y;
}

/**
 * Increments X coordinate by one
 */
void CPoint::incX ( int const & n )
{
  m_x += n;
}

/**
 * Increments Y coordinate by one
 */
void CPoint::incY ( int const & n )
{
  m_y += n;
}

/**
 * Decrements X coordinate by one
 */
void CPoint::decX ( int const & n )
{
  m_x -= n;
}

/**
 * Decrements Y coordinate by one
 */
void CPoint::decY ( int const & n )
{
  m_y -= n;
}

/**
 * Updates X and Y coordinates
 */
void CPoint::Update ( int const & x, int const & y )
{
  setX ( x );
  setY ( y );
}

/**
 * Compares two CPoint objects
 */
bool CPoint::operator == ( CPoint const & a )
{
  return ( (m_x == a.m_x) && (m_y == a.m_y) );
}

/**
 * Draws ch into the screen on the object coordinates
 */
void CPoint::Draw ( char const & ch ) const
{
  move ( m_x, m_y );
  addch ( ch );
}
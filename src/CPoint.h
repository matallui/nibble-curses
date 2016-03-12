/**
 * @file CPoint.h
 * @author Luís Taniça
 */
#ifndef _CPOINT_H_
#define _CPOINT_H_

#include <ncurses.h>
using namespace std;

/**
 * Class to handle 2 coordinate points.
 */
class CPoint
{
public:
  CPoint ( int const & x = 0, int const & y = 0 );
  CPoint ( CPoint const & src );
  CPoint & operator = ( CPoint const & src );
  int getX ( void ) const;
  int getY ( void ) const;
  void setX ( int const & x );
  void setY ( int const & y );
  void incX ( int const & n );
  void incY ( int const & n );
  void decX ( int const & n );
  void decY ( int const & n );
  void Update ( int const & x, int const & y );
  bool operator == ( CPoint const & b );
  void Draw ( char const & ch ) const;
  
protected:
  int m_x;	///< X coordinate
  int m_y;	///< Y coordinate
  
};

#endif
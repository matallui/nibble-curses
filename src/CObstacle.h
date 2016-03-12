/**
 * @file CObstacle.h
 * @author Luís Taniça
 */
#ifndef _COBSTACLE_H_
#define _COBSTACLE_H_

#include "CPoint.h"

/**
 * Class to handle obstacles.
 */
class CObstacle
{
public:
  CObstacle ( int const & ox = 0, int const & oy = 0, int const & length = 0, char const & mode = 'H' );
  void Update ( int const & ox, int const & oy, int const & length, char const & mode );
  void SetColors ( int const & cp_draw, int const & cp_erase );
  void Draw ( void ) const;
  void Erase ( void ) const;
  bool Intersection ( CPoint const & p ) const;
  
protected:
  CPoint m_orig;	///< obstacle origin coordinates
  int m_length;		///< obstacle length
  char m_mode;		///< 'V' vertical, 'H' horizontal, 'D' diagonal or 'I' interleaved
  int m_cp_draw;	///< color pair number for draw
  int m_cp_erase;	///< colot pair number for erase
  
  void Paint ( void ) const;
};

#endif
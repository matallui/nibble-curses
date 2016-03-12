/**
 * @file CSingle.h
 * @author Luís Taniça
 */
#ifndef _CSINGLE_H_
#define _CSINGLE_H_

#include "CSnake.h"

/**
 * Class to handle single player mode snakes.
 */
class CSingle : public CSnake
{
public:
  CSingle ( void );
  virtual ~CSingle ( void );
  virtual void Initialize ( int const & nlevel );
  virtual bool Collision ( CObstacle * const obstacles, int const & num_obstacles, CSnake * sfriend = NULL );
  virtual bool Move ( void );
};

#endif
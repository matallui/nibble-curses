/**
 * @file CMultPC.h
 * @author Luís Taniça
 */
#ifndef _CMULTPC_H_
#define _CMULTPC_H_

#include "CSingle.h"

/**
 * Class to handle multi player (on the same PC) snakes.
 */
class CMultPC : public CSingle
{
public:
  CMultPC ( int const & id );
  virtual ~CMultPC ( void );
  virtual void Reset ( void );
  virtual void Initialize ( int const & nlevel );
  virtual bool Collision ( CObstacle * const obstacles, int const & num_obstacles, CSnake * sfriend = NULL );
  virtual bool Move ( void );
  
protected:
  int m_playerid;		///< player ID (1 or 2)
  bool m_friend_alive;		///< true if other player is still alive
  
  void Die ( CMultPC * sfriend );
  void ManDown ( void );
};

#endif
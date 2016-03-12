/**
 * @file CMultNet.h
 * @author Luís Taniça
 */
#ifndef _CMULTNET_H_
#define _CMULTNET_H_

#include "CMultPC.h"
#include "CSocketServer.h"
#include "CSocketClient.h"

/**
 * Class to handle multi player (over network) snakes.
 */
class CMultNet : public CMultPC
{
public:
  CMultNet ( int const & id, CSocket * socket, bool const & host );
  virtual ~CMultNet ( void );
  virtual bool Move ( void );

protected:
  bool m_isHost;		///< true if controled by keyboard input, false if remote player
  CSocket * m_socket;		///< communication socket pointer
};

#endif
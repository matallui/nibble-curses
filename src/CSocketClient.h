/**
 * @file CSocketClient.h
 * @author Luís Taniça
 */
#ifndef _CSOCKETCLIENT_H_
#define _CSOCKETCLIENT_H_

#include "CSocket.h"

/**
 * Class used to handle client side sockets.
 */
class CSocketClient : public CSocket
{
public:
  CSocketClient ( void );
  virtual ~CSocketClient ( void );
  virtual bool Initialize ( int const & portno, char * hostname = NULL );
  virtual bool EstablishConnection ( void );
  
protected:
  struct hostent * m_server;	///< variable meant to hold server info
};

#endif
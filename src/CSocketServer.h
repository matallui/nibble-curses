/**
 * @file CSocketServer.h
 * @author Luís Taniça
 */
#ifndef _CSOCKETSERVER_H_
#define _CSOCKETSERVER_H_

#include "CSocket.h"

/**
 * Class used to handle server side sockets.
 */
class CSocketServer : public CSocket
{
public:
  CSocketServer ( void );
  virtual ~CSocketServer ( void );
  virtual bool Initialize ( int const & portno, char * hostname = NULL );
  virtual bool EstablishConnection ( void );
  
protected:
  int m_sockfd;				///< socket file descriptor
  struct sockaddr_in m_clientAddr;	///< structure for client data
  socklen_t m_clientLength;		///< size of m_clientAddr
};

#endif
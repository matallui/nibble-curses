/**
 * @file CSocket.h
 * @author Luís Taniça
 */
#ifndef _CSOCKET_H_
#define _CSOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/**
 * Virtual class to be used as base class for handling sockets.
 */
class CSocket
{
public:
  CSocket ( void );
  virtual ~CSocket ( void );
  virtual bool Initialize ( int const & portno, char * hostname = NULL ) = 0;		///< virtual method
  virtual bool EstablishConnection ( void ) = 0;					///< virtual method
  virtual bool Send ( char * buffer, int const & buffsize );
  virtual bool Recv ( char * buffer, int const & buffsize );
  
protected:
  int m_newsockfd;			///< communication socket file descriptor
  int m_portno;				///< communication port number
  struct sockaddr_in m_serverAddr;	///< structure for server info
  bool m_connected;			///< determines if socket is connected or not
};

#endif
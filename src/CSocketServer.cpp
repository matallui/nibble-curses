/**
 * @file CSocketServer.cpp
 * Used to initialize, destroy and communicate through socket.
 * It is initialized as server socket
 * @author Luís Taniça
 * @see CSocket CSocketClient
 */
#include "CSocketServer.h"

/**
 * Constructor
 */
CSocketServer::CSocketServer ( void ) {}

/** 
 * Destructor
 */
CSocketServer::~CSocketServer ( void )
{
  close ( m_sockfd );
}

/** Initializes server socket
 * @param portno port number where the server will be listening
 * @param hostname should be NULL, only used for CSocketClient
 */
bool CSocketServer::Initialize ( int const & portno, char * hostname )
{
  m_sockfd = socket ( AF_INET, SOCK_STREAM, 0 );
  if ( m_sockfd < 0 ) return false;
  
  bzero ( (char*) (&m_serverAddr), sizeof(m_serverAddr) );
  m_portno = portno;
  m_serverAddr.sin_family = AF_INET;
  m_serverAddr.sin_addr.s_addr = INADDR_ANY;
  m_serverAddr.sin_port = htons ( m_portno );
  
  int s;
  s = bind ( m_sockfd, (struct sockaddr*) (&m_serverAddr), sizeof( m_serverAddr ) );
  if ( s < 0 ) return false;
  listen ( m_sockfd, 5 );
  return true;
}

/** Tries to accept connection from client
 */
bool CSocketServer::EstablishConnection ( void )
{
  m_clientLength = sizeof ( m_clientAddr );
  m_newsockfd = accept ( m_sockfd, (struct sockaddr*) (&m_clientAddr), &m_clientLength );
  if ( m_newsockfd < 0 ) return false;
  
  m_connected = true;
  return true;
}
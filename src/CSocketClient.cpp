/**
 * @file CSocketClient.cpp
 * Used to initialize, destroy and communicate through socket.
 * It initializes the socket as client
 * @author Luís Taniça
 * @see CSocket CSocketServer
 */
#include "CSocketClient.h"

/** Constructor
 */
CSocketClient::CSocketClient ( void )
{
  m_server = NULL;
}

/** Destructor
 */
CSocketClient::~CSocketClient ( void ) {}

/** Initializes socket as client
 * @param portno port number to communicate with server socket
 * @param hostname server hostname
 */
bool CSocketClient::Initialize ( int const & portno, char * hostname )
{
  m_portno = portno;
  m_newsockfd = socket ( AF_INET, SOCK_STREAM, 0 );
  if ( m_newsockfd < 0 ) return false;
  m_server = gethostbyname ( hostname );
  if ( m_server == NULL ) return false;
  
  bzero ( (char*) (&m_serverAddr), sizeof( m_serverAddr ) );
  m_serverAddr.sin_family = AF_INET;
  bcopy ( (char*) m_server->h_addr, (char*) (&m_serverAddr.sin_addr.s_addr), m_server->h_length );
  m_serverAddr.sin_port = htons ( m_portno );
  
  return true;
}

/** Tries to establish connection with server
 */
bool CSocketClient::EstablishConnection ( void )
{
  int s;
  s = connect ( m_newsockfd, (struct sockaddr*) (&m_serverAddr), sizeof(m_serverAddr) );
  if ( s < 0 ) return false;
  m_connected = true;
  return true;
}

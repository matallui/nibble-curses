/**
 * @file CSocket.cpp
 * Contains declarations for the class CSocket
 * @brief Virtual class used to initialize, destroy and communicate through socket
 * @author Luís Taniça
 * @see CSocketClient CSocketServer
 */
#include "CSocket.h"

/** Constructor
 */
CSocket::CSocket ( void )
{
  m_connected = false;
}

/** Destructor
 */
CSocket::~CSocket ( void )
{
  if ( m_connected )
    close( m_newsockfd );
}

/** Sends char array through the socket
 */
bool CSocket::Send ( char * buffer, int const & buffsize )
{
  if ( !m_connected ) return false;
  
  int n;
  n = write ( m_newsockfd, buffer, buffsize );
  if ( n < 0 ) return false;
  return true;
}

/** Receives char array through the socket
 */
bool CSocket::Recv ( char * buffer, int const & buffsize )
{
  if ( !m_connected ) return false;
  
  bzero ( buffer, buffsize );
  int n;
  n = read ( m_newsockfd, buffer, buffsize );
  if ( n < 0 ) return false;
  return true;
}
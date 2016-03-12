/**
 * @file CMultNet.cpp
 * Contains the declarations for the class CMultNet.
 * @author Luís Taniça
 * @see CSnake CSingle CMultPC
 */
#include "CMultNet.h"

/** Contructor.
 * @param id player id ( 1 or 2 )
 * @param socket pointer to an already initialized socket
 * @param host determines if the snake is being controlled by keyboard input (true) or by socket (false)
 */
CMultNet::CMultNet ( int const & id, CSocket * socket, bool const & host )
	    : CMultPC ( id )
{
  m_socket = socket;
  m_isHost = host;
}

/** Destructor.
 */
CMultNet::~CMultNet ( void ) {}

/** Moves the snake by one step, either by keyboard detection or socket command.
 */
bool CMultNet::Move ( void )
{
  if ( !m_lives ) return true;
  
  char buffer[10];
  bzero ( buffer, 10 );
  
  if ( m_isHost )	// the snake controlled by this computer
  {
    CSingle::Move();
    sprintf ( buffer, "M%c", m_direction );
    if ( ! m_socket->Send ( buffer, 3 ) ) return false;
    bzero ( buffer, 3 );
    if ( ! m_socket->Recv ( buffer, 3 ) ) return false;
    if ( buffer[0] != 'O' || buffer[1] != 'K' ) return false;
  }
  else			// the snake controlled by remote computer
  {
    if ( ! m_socket->Recv ( buffer, 3 ) ) return false;
    if ( buffer[0] != 'M' ) return false;
    m_direction = buffer[1];
    
    switch ( m_direction )
    {
      case 'U':
	ungetch ( KEY_UP );
	break;
      case 'D':
	ungetch ( KEY_DOWN );
	break;
      case 'L':
	ungetch ( KEY_LEFT );
	break;
      case 'R':
	ungetch ( KEY_RIGHT );
	break;
      default:
	return false;
	break;
    }
    CSingle::Move();
    sprintf ( buffer, "OK" );
    if ( ! m_socket->Send ( buffer, 3 ) ) return false;
  }
  return true;
}
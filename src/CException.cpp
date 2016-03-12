/**
 * @file CException.cpp
 * Contains the declarations for the class CException
 * @brief This class is used to throw exceptions when some problem occurs, specially in socket communication.
 * @author Luís Taniça
 */
#include "CException.h"

/** Default Constructor.
 */
CException::CException ( void )
		      : m_error("")
{}

/** Destructor.
 */
CException::CException ( std::string const & error )
		      : m_error ( error )
{ }

/** Copy constructor.
 */
CException::CException ( CException const & src )
{
  m_error = src.m_error;
}

/** Assignment operator.
 */
CException & CException::operator = ( CException const & src )
{
  if ( &src == this ) return *this;
  m_error = src.m_error;
  return *this;
}

/** Prints exceptions error message into ostream os.
 */
ostream & operator << ( ostream & os, CException const & e )
{
  os << e.m_error << std::endl;
  return os;
}
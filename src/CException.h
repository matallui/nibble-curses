/**
 * @file CException.h
 * @author Luís Taniça
 */
#ifndef _CEXCEPTION_H
#define _CEXCEPTION_H

#include <iostream>
using namespace std;

/** Class used to handle exceptions.
 */
class CException
{
public:
  CException ( void );
  CException ( std::string const & error );
  CException ( CException const & src );
  CException & operator = ( CException const & src );
  friend ostream & operator << ( ostream & os, CException const & e );
protected:
  string m_error;	///< contains exception error message
};
#endif
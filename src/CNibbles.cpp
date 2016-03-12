/**
 * @file CNibbles.cpp
 * Contains the declarations of the class CNibles.
 * @brief This class is used to initialize, play and destroy my version of the Nibbles game.
 * @author Luís Taniça
 */
#include "CNibbles.h"

/** Constructor.
 * Initializes the screen dimensions, player structures and obstacle structures.
 * NCurses is initialized here as well.
 */
CNibbles::CNibbles ( void )
	: m_exception ( "" )
{
  m_maxLines = 36;
  m_maxCols = 100;
  m_level = 0;
  m_num_obstacles = 0;
  m_nplayers = 0;
  m_mode = 0;				// 1-Single Player , 2-Multi-player on PC, 3-Multi-player over Network
  m_speed = 0;				// game speed
  m_nfood = 0;				// food number
  m_score = 0;				// global score ( sum of all player scores )
  m_snakes = new CSnake* [2];		// pointer array to control game players
  m_snakes[0] = m_snakes[1] = NULL;
  m_socket = NULL;			// pointer to socket
  m_master = true;			// determines who is the server / player 1 ( = true )
  
  srand(time(NULL));	// initialize seed
  
  // initialize ncurses
  initscr ();
  nodelay ( stdscr, true );
  if ( getenv("ESCDELAY") == NULL ) ESCDELAY = 25;
  keypad ( stdscr, true );
  noecho ();
  curs_set (0);
  start_color();
  init_pair ( 1, COLOR_BLACK, COLOR_BLACK );	// for erasing
  init_pair ( 2, COLOR_YELLOW, COLOR_YELLOW );	// for obstacles
  init_pair ( 3, COLOR_WHITE, COLOR_BLACK );	// for scores
  init_pair ( 4, COLOR_GREEN, COLOR_BLACK );	// for player 1
  init_pair ( 5, COLOR_BLUE, COLOR_BLACK );	// for player 2
  init_pair ( 6, COLOR_RED, COLOR_WHITE );	// for pop-up windows
  
  // allocate memory for obstacles and set their colors
  m_obstacles = new CObstacle [20];
  for ( int i = 0; i < 20; i++ )
    m_obstacles[i].SetColors( 2, 1 );
}

/** Destructor.
 * Frees player, obstacle and socket structures (if they were created).
 * Terminates NCurses.
 * Checks if the execution ended unexpectly.
 */
CNibbles::~CNibbles ( void )
{
  for ( int i = 0; i < 2; i++ )
    if ( m_snakes[i] ) delete m_snakes[i];
  delete [] m_snakes;
  delete [] m_obstacles;
  if ( m_mode == 3 && m_socket ) delete m_socket;
  nodelay( stdscr, false );
  endwin ();
  
  if ( m_state == 5 )
  {
    cout << "\nThe program has aborted!\nReason:" << endl;
    cout << " - " << m_exception << endl;
  }
}

/** Starts the Nibbles game.
 */
void CNibbles::Start ( void )
{
  try
  {
    Menu ();
    
    m_state = 2; // next level;
    for ( m_level = 1; m_level <= 10; m_level++ )
    {
      InitLevel ( );
      m_state = 0; // normal game curse
      while ( Step() )	usleep ( m_speed );
      // - same level (player dies)   	-> 1
      // - next level			-> 2
      // - game over			-> 3
      // - winner				
      // - Abort Execution		-> 5
      if ( m_state == 1 )		m_level--;
      else if ( m_state == 3 )
      {
	if ( GameOver_Restart() ) 	Restart ();
	else				break;
      }
      if ( m_level == 10 )
      {
	if ( Winner_Restart() )	Restart ();
	else 			break;
      }
	
    }
  }
  catch (CException e)
  {
    m_exception = e;
    m_state = 5;
  }
  
}

/** Draws interactive menu on the screen.
 * @param menu contains the menu options
 * @param title contains the menu title
 * @param size contains the number of available options (max 5)
 * @param item determines wich options is highlighted
 */
void CNibbles::DrawMenu ( char menu[5][30], char title[30], int const & size, int const & item )
{
  // clean and draw frame
  bkgd ( COLOR_PAIR (1) );
  attrset ( COLOR_PAIR (4) );
  for ( int j = 0; j < m_maxCols; j++ )
  {
    mvaddch ( 0, j, '+' );
    mvaddch ( m_maxLines-1, j, '+' );
  }
  for ( int i = 0; i < m_maxLines; i++ )
  {
    mvaddch ( i, 0, '+' );
    mvaddch ( i, m_maxCols-1, '+' );
  }
  
  // draw menu
  attrset ( COLOR_PAIR(5) );
  mvaddstr ( 8, 20, title );
  attrset ( COLOR_PAIR(3) );
  
  for ( int c = 0; c < size; c++ )
  {
    if ( c == item )
      attron ( A_REVERSE );
    mvaddstr( 12+(c*2), 25, menu[c] );
    attroff ( A_REVERSE );
  }
  
  refresh();
}

/** Dispays Menu to initialize the Nibbles game.
 * Sets number of players, game mode and difficulty.
 * If needed, the socket is initialized.
 */
void CNibbles::Menu ( void )
{ 
  nodelay ( stdscr, false );
  
  // Mode menu
  char title[30] = "Select the Mode:\0";
  char menu[5][30] = {
			      "       Single Player       \0",
			      "  Multi-Player on same PC  \0",
			      " Multi-Player over Network \0",
			      "\0","\0"
  }; 

  int key, item;
  item = 0;
  DrawMenu ( menu, title, 3, item );
  do
  {
    key = getch();
    switch ( key )
    {
      case KEY_DOWN:
	item = (item+1) % 3;
	break;
      case KEY_UP:
	item = (item+2) % 3;
	break;
    }
    DrawMenu ( menu, title, 3, item );
  }while ( key != '\n' );
  
  switch ( item )
  {
    case 0:
      m_nplayers = 1;
      m_mode = 1;
      break;
    case 1:
      m_nplayers = 2;
      m_mode = 2;
      break;
    case 2:
      m_nplayers = 2;
      m_mode = 3;
      
      sprintf ( title, "Select the Connection:" );
      sprintf ( menu[0], "    Receive Connection     ");
      sprintf ( menu[1], "       Conncect To         ");
      item = 0; 
      DrawMenu ( menu, title, 2, item );
      do
      {
	key = getch();
	switch ( key )
	{
	  case KEY_DOWN:
	    item = (item+1) % 2;
	    break;
	  case KEY_UP:
	    item = (item+1) % 2;
	    break;
	}
	DrawMenu ( menu, title, 2, item );
      }while ( key != '\n' );
      
      int port;
      if ( item == 0 )
      {
	port = (rand()%1000) + 50000;
	sprintf ( title, "Waiting in Port: %d", port );
	DrawMenu ( menu, title, 0, item );
	m_socket = new CSocketServer;
	if ( ! m_socket->Initialize ( port, NULL ) )	throw CException( "Unable to initialize socket." );
	  
	if ( ! m_socket->EstablishConnection () )	throw CException( "Unable to establish connection." );
      }
      else
      {
	char hostname[50];
	bzero ( hostname, 50 );
	m_master = false;
	sprintf ( title, " " );
	DrawMenu ( menu, title, 0, item );
	echo();
	mvaddstr ( 12, 20, "Hostame Address: " );
	scanw ( (char*)"%s", hostname );
	mvaddstr ( 14, 20, "Insert Port: " );
	scanw ( (char*)"%d", &port );
	m_socket = new CSocketClient;
	if ( ! m_socket->Initialize ( port, hostname ) )	throw CException ( "Unable to Initialize socket." );
	if ( ! m_socket->EstablishConnection() )		throw CException ( "Unable to establish connection." );
      }
  }

  if ( m_master )
  {
    // Difficulty menu
    sprintf ( title, "Select the Difficulty:" );
    sprintf ( menu[0], "         Beginner          ");
    sprintf ( menu[1], "           Easy            ");
    sprintf ( menu[2], "          Normal           ");
    sprintf ( menu[3], "           Hard            ");
    sprintf ( menu[4], "          Expert           ");
    
    item = 0;
    DrawMenu ( menu, title, 5, item );
    do
    {
      key = getch();
      switch ( key )
      {
	case KEY_DOWN:
	  item = (item+1) % 5;
	  break;
	case KEY_UP:
	  item = (item+4) % 5;
	  break;
      }
      DrawMenu ( menu, title, 5, item );
    }while ( key != '\n' );
    
    switch ( item )
    {
      case 0:
	m_speed = 135000; 
	break;
      case 1:
	m_speed = 115000;
	break;
      case 2:
	m_speed = 90000;
      case 3:
	m_speed = 65000;
	break;
      case 4:
	m_speed = 50000;
	break;
    }
  }
  else
  {
    sprintf ( title, "Wait for Start ..." );
      DrawMenu ( menu, title, 0, item );
  }

  // allocate player snake(s) according to mode
  switch ( m_mode )
  {
    case 1:
      m_snakes[0] = new CSingle;
      break;
    case 2:
      m_snakes[0] = new CMultPC (1);
      m_snakes[1] = new CMultPC (2);
      break;
    case 3:
      m_snakes[0] = new CMultNet ( 1, m_socket, m_master );
      m_snakes[1] = new CMultNet ( 2, m_socket, !m_master );
      
      char buffer[3];
      bzero ( buffer, 3 );
      
      if ( m_master )
      {
	if ( ! m_socket->Send ( (char*) &m_speed, sizeof(m_speed) ) )		throw CException( "Problems with communication." );
	if ( ! m_socket->Recv ( buffer, 3 ) )					throw CException( "Problems with communication." );
	if ( buffer[0] != 'O' || buffer[1] != 'K' )				throw CException( "Problems with communication." );
      }
      else
      {
	if ( ! m_socket->Recv ( (char*) &m_speed, sizeof(m_speed) ) )		throw CException( "Problems with communication." );
	sprintf ( buffer, "OK" );
	if ( m_speed >= 50000 && m_speed <= 135000 )
	  m_socket->Send ( buffer, 3 );
	else
	  throw CException( "Problems with communication." );
      }
  }
  
  noecho();
  nodelay ( stdscr, true );
}

/** Displays level presentation.
 * Ends when the master player presses space.
 */
void CNibbles::PresentLevel ( void )
{
  bkgd ( COLOR_PAIR (1) );
  attrset ( COLOR_PAIR(2) );
  for ( int j = 0; j < m_maxCols; j++ )
  {
    mvaddch ( 0, j, '*' );
    mvaddch ( m_maxLines-1, j, '*' );
  }
  for ( int i = 0; i < m_maxLines; i++ )
  {
    mvaddch ( i, 0, '*' );
    mvaddch ( i, m_maxCols-1, '*' );
  }
  
  attrset ( COLOR_PAIR(3) );
  move ( 15, 39 );
  attron ( A_BOLD );
  printw ( "LEVEL %d / 10", m_level );
  attroff ( A_BOLD );
  if ( m_mode == 3 )
  {
    char buffer[3];
    bzero ( buffer, 3 );
    if (  (m_master && m_snakes[0]->getLives() ) || ( !m_master && !m_snakes[0]->getLives() ) )
    {
      move ( 19, 32 );
      printw ( "* Press SPACE to continue... *" );
      refresh();
      nodelay( stdscr, false );
      while ( getch() != ' ' );
      nodelay( stdscr, true );
      sprintf ( buffer, "GO" );
      if ( ! m_socket->Send ( buffer, 3 ) )		throw CException( "Problems with communication." );
      bzero ( buffer, 3 );
      if ( ! m_socket->Recv ( buffer, 3 ) )		throw CException( "Problems with communication." );
      if ( buffer[0] != 'O' || buffer[1] != 'K' )	throw CException( "Problems with communication." );
    }
    else
    {
      move ( 21, 32 );
      printw ( "* Waiting for answer... *" );
      refresh();
      if ( ! m_socket->Recv ( buffer, 3 ) )		throw CException( "Problems with communication." );
      if ( buffer[0] !='G' || buffer[1] != 'O' )	throw CException( "Problems with communication." );
      sprintf ( buffer, "OK" );
      if ( ! m_socket->Send ( buffer, 3 ) )		throw CException( "Problems with communication." );
    }
    flushinp();
  }
  else
  {
    move ( 19, 32 );
    printw ( "* Press SPACE to continue... *" );
    refresh();
    nodelay( stdscr, false );
    while ( getch() != ' ' );
    nodelay( stdscr, true );
  }
}

/** Draws pop-up window.
 * Used when some player dies, game over or winner situations.
 */
void CNibbles::PopUp ( char * title1, char * title2 ) const
{
  attrset ( COLOR_PAIR(6) );
  mvaddstr ( 14, 35, "******************************" );
  mvaddstr ( 15, 35, "*                            *" );
  mvaddstr ( 16, 35, title1 );
  mvaddstr ( 17, 35, "*                            *" );
  mvaddstr ( 18, 35, title2 );
  mvaddstr ( 19, 35, "******************************" );
  refresh();
}

/** Function called when some player dies.
 * Prints the pop-up screen and treats all the necessary communication.
 */
void CNibbles::PlayerDies ( int const & player ) const
{
  nodelay ( stdscr, false );
  
  char title[31];
  sprintf ( title, "*       PLAYER %d dies!!      *", player );
  
  if ( m_mode == 3 )
  {
    char buffer[3];
    bzero ( buffer, 3 );
    
    if ( (m_master && player == 1) || (!m_master && player == 2) )
    {
      PopUp ( (char*)title, (char*)"*   press space to continue  *" );
      while ( getch () != ' ' );
      sprintf ( buffer, "GO" );
      if ( ! m_socket->Send ( buffer, 3 ) )		throw CException( "Problems with communication." );
      bzero ( buffer, 3 );
      if ( ! m_socket->Recv ( buffer, 3 ) )		throw CException( "Problems with communication." );
      if ( buffer[0] != 'O' || buffer[1] != 'K' )	throw CException( "Problems with communication." );
    }
    else
    {
      PopUp ( (char*)title, (char*)"*   waiting to continue ...  *" );
      if ( ! m_socket->Recv ( buffer, 3 ) )		throw CException( "Problems with communication." );
      if ( buffer[0] != 'G' || buffer[1] != 'O' )	throw CException( "Problems with communication." );
      sprintf ( buffer, "OK" );
      if ( ! m_socket->Send ( buffer, 3 ) )		throw CException( "Problems with communication." );
      flushinp();
    }
  }
  else
  {
    PopUp ( (char*)title, (char*)"*   press space to continue  *" );
    while ( getch () != ' ' );
  }
  nodelay ( stdscr, true );
  touchwin ( stdscr );
}

/** Function called when all players loose all lives.
 * Asks for restart.
 */
bool CNibbles::GameOver_Restart ( void ) const
{
  bool restart;
  nodelay ( stdscr, false );
  
  if ( m_mode == 3 )
  {
    char buffer[3];
    bzero ( buffer, 3 );
    
    if ( m_master )
    {
      PopUp( (char*)"*          GAME OVER!        *", (char*)"*      Try again? [Y/N]      *" );
      while (1)
      {
	char key = getch ();
	if ( key == 'Y' || key == 'y' )
	{
	  sprintf ( buffer, "RY" );
	  restart = true;
	  break;
	}
	else if ( key == 'N' || key == 'n' )
	{
	  sprintf ( buffer, "RN" );
	  restart = false;
	  break;
	}
      }
      if ( ! m_socket->Send ( buffer, 3 ) )		throw CException( "Problems with communication." );
      bzero ( buffer, 3 );
      if ( ! m_socket->Recv ( buffer, 3 ) )		throw CException( "Problems with communication." );
      if ( buffer[0] != 'O' || buffer[1] != 'K' )	throw CException( "Problems with communication." );
      
    }
    else
    {
      PopUp( (char*)"*          GAME OVER!        *", (char*)"*   waiting for answer...    *" );
      if ( ! m_socket->Recv ( buffer, 3 ) )	throw CException( "Problems with communication." );
      if ( buffer[0] != 'R' )			throw CException( "Problems with communication." );
      switch ( buffer[1] )
      {
	case 'Y':
	  restart = true;
	  sprintf ( buffer, "OK" );
	  if ( ! m_socket->Send ( buffer, 3 ) )	throw CException( "Problems with communication." );
	  break;
	case 'N':
	  restart = false;
	  sprintf ( buffer, "OK" );
	  if ( ! m_socket->Send ( buffer, 3 ) )	throw CException( "Problems with communication." );
	  break;
	default:
	  throw CException( "Problems with communication." );
      }
    }
  }
  else
  {
    PopUp( (char*)"*          GAME OVER!        *", (char*)"*      Try again? [Y/N]      *" );
    while (1)
    {
      int key = getch ();
      if ( key == 'Y' || key == 'y' )
      {
	restart = true;
	break;
      }
      else if ( key == 'N' || key == 'n' )
      {
	restart = false;
	break;
      }
    }
  }
  nodelay ( stdscr, true );
  return restart;
}

/** Fuction called when the game was won.
 * Asks for restart.
 */
bool CNibbles::Winner_Restart ( void ) const
{
  bool restart;
  nodelay ( stdscr, false );
  
  if ( m_mode == 3 )
  {
    char buffer[3];
    bzero ( buffer, 3 );
    
    if ( m_master )
    {
      PopUp( (char*)"*        !! YOU WON !!       *", (char*)"*      Try again? [Y/N]      *" );
      while (1)
      {
	char key = getch ();
	if ( key == 'Y' || key == 'y' )
	{
	  sprintf ( buffer, "RY" );
	  restart = true;
	  break;
	}
	else if ( key == 'N' || key == 'n' )
	{
	  sprintf ( buffer, "RN" );
	  restart = false;
	  break;
	}
      }
      if ( ! m_socket->Send ( buffer, 3 ) )		throw CException( "Problems with communication." );
      bzero ( buffer, 3 );
      if ( ! m_socket->Recv ( buffer, 3 ) )		throw CException( "Problems with communication." );
      if ( buffer[0] != 'O' || buffer[1] != 'K' )	throw CException( "Problems with communication." );
    }
    else
    {
      PopUp( (char*)"*        !! YOU WON !!       *", (char*)"*   waiting for answer...    *" );
      if ( ! m_socket->Recv ( buffer, 3 ) )	throw CException( "Problems with communication." );
      if ( buffer[0] != 'R' )			throw CException( "Problems with communication." );
      switch ( buffer[1] )
      {
	case 'Y':
	  restart = true;
	  sprintf ( buffer, "OK" );
	  if ( ! m_socket->Send ( buffer, 3 ) )	throw CException( "Problems with communication." );
	  break;
	case 'N':
	  restart = false;
	  sprintf ( buffer, "OK" );
	  if ( ! m_socket->Send ( buffer, 3 ) )	throw CException( "Problems with communication." );
	  break;
	default:
	  throw CException( "Problems with communication." );
      }
    }
  }
  else
  {
    PopUp( (char*)"*        !! YOU WON !!       *", (char*)"*      Try again? [Y/N]      *" );
    while (1)
    {
      int key = getch ();
      if ( key == 'Y' || key == 'y' )
      {
	restart = true;
	break;
      }
      else if ( key == 'N' || key == 'n' )
      {
	restart = false;
	break;
      }
    }
  }
  nodelay ( stdscr, true );
  return restart;
}

/** Restarts the game, reseting all snakes, global score and level.
 */
void CNibbles::Restart ( void )
{
  for ( int i = 0; i < m_nplayers; i++ )
    m_snakes[i]->Reset();
  m_score = 0;
  m_state = 2;
  m_level = 0;  
}

/** Initiates a new level.
 */
void CNibbles::InitLevel ( void )
{
  if ( m_state == 2 ) PresentLevel();
  
  bkgd ( COLOR_PAIR (1) );
  
  m_nfood = 1;
  
  switch ( m_level )
  {
    case 1:
      m_num_obstacles = 4;
      m_obstacles[0].Update ( 0, 0, m_maxCols, 'H' );			// superior horizontal
      m_obstacles[1].Update ( m_maxLines-1, 0, m_maxCols, 'H' );		// inferior horizontal
      m_obstacles[2].Update ( 1, 0, m_maxLines-2, 'V' );			// left vertical
      m_obstacles[3].Update ( 1, m_maxCols-1, m_maxLines-2, 'V' );		// right vertical
      break;
    case 2:
      m_num_obstacles = 5;
      m_obstacles[4].Update ( 17, 25, 50, 'H' );
      break;
    case 3:
      m_num_obstacles = 6;
      m_obstacles[4].Update ( 6, 30, 24, 'V' );
      m_obstacles[5].Update ( 6, 69, 24, 'V' );
      break;
    case 4:
      m_num_obstacles = 8;
      m_obstacles[4].Update ( 1, 30, 16, 'V' );
      m_obstacles[5].Update ( 20, 70, 16, 'V' );
      m_obstacles[6].Update ( 25, 1, 45, 'H' );
      m_obstacles[7].Update ( 11, 55, 45, 'H' );
      break;
    case 5:
      m_num_obstacles = 12;
      m_obstacles[4].Update ( 1, 20, 15, 'V' );
      m_obstacles[5].Update ( 1, 40, 15, 'V' );
      m_obstacles[6].Update ( 1, 60, 15, 'V' );
      m_obstacles[7].Update ( 1, 80, 15, 'V' );
      m_obstacles[8].Update ( 21, 20, 15, 'V' );
      m_obstacles[9].Update ( 21, 40, 15, 'V' );
      m_obstacles[10].Update ( 21, 60, 15, 'V' );
      m_obstacles[11].Update ( 21, 80, 15, 'V' );
      break;
    case 6:
      m_num_obstacles = 8;
      m_obstacles[4].Update ( 12, 25, 12, 'V' );
      m_obstacles[5].Update ( 12, 75, 12, 'V' );
      m_obstacles[6].Update ( 10, 27, 47, 'H' );
      m_obstacles[7].Update ( 25, 27, 47, 'H' );
      break;
    case 7:
      m_num_obstacles = 5;
      m_obstacles[4].Update ( 1, 50, 34, 'I' );
      break;
    case 8:
      m_num_obstacles = 10;
      m_obstacles[4].Update ( 1, 14, 28, 'V' );
      m_obstacles[5].Update ( 7, 28, 28, 'V' );
      m_obstacles[6].Update ( 1, 42, 28, 'V' );
      m_obstacles[7].Update ( 7, 56, 28, 'V' );
      m_obstacles[8].Update ( 1, 70, 28, 'V' );
      m_obstacles[9].Update ( 7, 84, 28, 'V' );
      break;
    case 9:
      m_num_obstacles = 6;
      m_obstacles[4].Update ( 2, 2, 32, 'D' );
      m_obstacles[5].Update ( 2, 66, 32, 'D' );
      break;
    case 10:
      m_num_obstacles = 10;
      m_obstacles[4].Update ( 1, 14, 34, 'I' );
      m_obstacles[5].Update ( 2, 28, 34, 'I' );
      m_obstacles[6].Update ( 1, 42, 34, 'I' );
      m_obstacles[7].Update ( 2, 56, 34, 'I' );
      m_obstacles[8].Update ( 1, 70, 34, 'I' );
      m_obstacles[9].Update ( 2, 84, 34, 'I' );
      break;
  }
  
  for ( int i = 0; i < m_num_obstacles; i++ )
    m_obstacles[i].Draw ();
  
  for ( int i = 0; i < m_nplayers; i++ )
    m_snakes[i]->Initialize ( m_level );
  
  UpdateScores();
  PutFood();
  
  refresh();
}

/** Displays updated scores on the screen
 */
void CNibbles::UpdateScores ( void )
{
  // Display scores
  attrset ( COLOR_PAIR(3) );
  if ( m_snakes[0] )
  {
  move ( 36, 2 );
  printw ("PLAYER 1 -> Lives: %d , Score: %d  ", m_snakes[0]->getLives(), m_snakes[0]->getScore());
  }
  if ( m_snakes[1] )
  {
  move ( 36, 62 );
  printw ("PLAYER 2 -> Lives: %d , Score: %d  ", m_snakes[1]->getLives(), m_snakes[1]->getScore());
  }
  move ( 36, 46);
  printw ( "%d  ", m_score ); // global score
  refresh();
}

/** Puts piece of food on the game board.
 */
void CNibbles::PutFood ( void )
{
  if ( m_master )
  {
    bool free = false;
    
    while ( !free )
    {
      m_xyfood.setX ( rand() % m_maxLines );
      m_xyfood.setY ( rand() % m_maxCols );
      
      if ( m_snakes[0]->hasPoint ( m_xyfood ) ) continue;
      if ( m_snakes[1] && m_snakes[1]->hasPoint ( m_xyfood ) ) continue;
      
      free = true;
      for ( int i = 0; i < m_num_obstacles; i++ )
	if ( m_obstacles[i].Intersection ( m_xyfood ) ) free = false;	
    }
  }
  
  if ( m_mode == 3 )
  {
    if ( m_master )
    {
      if ( ! m_socket->Send ( (char*) &m_xyfood, sizeof(m_xyfood) ) )
	throw CException( "Problems with communication." );
    }
    else
    {
      if ( ! m_socket->Recv ( (char*) &m_xyfood, sizeof(m_xyfood) ) )
	throw CException( "Problems with communication." );
      int x, y;
      x = m_xyfood.getX();
      y = m_xyfood.getY();
      if ( x < 0 || x > m_maxLines || y < 0 || y > m_maxCols )
	throw CException( "Problems with communication." );
    }
  }
  
  attrset ( COLOR_PAIR(3) );
  move ( m_xyfood.getX(), m_xyfood.getY() );
  addch ( (char) (m_nfood+48) );
  refresh();
}

/** Realizes one game step, moving the snakes, analizing collisions and putting food (if necessary).
 * @return false if collision or level passed, true otherwise.
 */
bool CNibbles::Step ( void )
{
  if ( m_nplayers == 1)
  {
    // move snake
    m_snakes[0]->Move();
    // check collisions
    if ( m_snakes[0]->Collision ( m_obstacles, m_num_obstacles, NULL ) )
    {
      m_score -= 1000;
      UpdateScores();
      if ( m_snakes[0]->getLives() > 0 ) 	
      {
	m_state = 1;	// restart level
	PlayerDies(1);
      }
      else					m_state = 3;	// game over
      return false;
    }
    // check if eats food
    if ( m_snakes[0]->Eat ( m_xyfood, m_nfood, m_level ) )
    {
      m_score += (100*m_level + 100*m_nfood);
      m_nfood ++;
      UpdateScores();
      if ( m_nfood == 10 )
      {
	m_state = 2;	// Next Level
	return false;
      }
      PutFood();
    }
  }
  else
  {
    // move snakes
    if ( ! m_snakes[0]->Move() )	throw CException( "Problems with communication." );
    if ( ! m_snakes[1]->Move() )	throw CException( "Problems with communication." );
    
    // check for collisions
    if ( m_snakes[0]->Collision ( m_obstacles, m_num_obstacles, m_snakes[1] ) )
    {
      m_score -= 1000;
      UpdateScores();
      if ( m_snakes[0]->getLives() > 0 || m_snakes[1]->getLives() > 0 )
      {
	m_state = 1;
	PlayerDies(1);
      }
      else	m_state = 3;
      return false;
    }
    else if ( m_snakes[1]->Collision ( m_obstacles, m_num_obstacles, m_snakes[0] ) )
    {
      m_score -= 1000;
      UpdateScores();
      if ( m_snakes[0]->getLives() > 0 || m_snakes[1]->getLives() > 0 )
      {
	m_state = 1;
	PlayerDies(2);
      }
      else	m_state = 3;
      return false;
    }
    // check if someone eats food
    if ( m_snakes[0]->Eat ( m_xyfood, m_nfood, m_level ) || m_snakes[1]->Eat ( m_xyfood, m_nfood, m_level ) )
    {
      m_score += (100*m_level + 100*m_nfood);
      m_nfood ++;
      UpdateScores();
      if ( m_nfood == 10 )
      {
	m_state = 2;
	return false;
      }
      PutFood();
    } 
  }

  return true;
}

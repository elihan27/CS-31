
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;
const int MAXCOLS = 20;
const int MAXRATS = 100;

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH =2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;

class Rat
{
public:
    // Constructor
    Rat(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int m_pellet1;
    int m_pellet2;
    int m_turn;
};

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    string dropPoisonPellet();
    string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
    
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     ratCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRatsAt(int r, int c) const;
    void    display(string msg) const;
    
    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRat(int r, int c);
    bool addPlayer(int r, int c);
    void moveRats();
    
private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rat*    m_rats[MAXRATS];
    int     m_nRats;
    int     m_turns;
    
    // Helper functions
    void checkPos(int r, int c) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rat must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
        
        
    {
        cout << "***** Rat created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_pellet1 = 0; //Rat currently has zero pellets
    m_pellet2 = 0;
    m_turn = 0;
}

int Rat::row() const
{
    return m_row;
}

int Rat::col() const
{
    return m_col;
}

bool Rat::isDead() const
{
    
    return (m_pellet1 != 0  && m_pellet2 != 0); //if rat has two pellets, rat is dead
}

void Rat::move()
{
    
    if (m_pellet1 != 0) //rat has eaten one pellet
    {    m_turn++;      //once it does, begin keep track of turns
        if (m_turn % 2 != 0) //if turn is odd number, rat does not move.
            return;
    }
    
    int way = randInt(NORTH, WEST);
    attemptMove(*m_arena, way, m_row, m_col); //rat attempts to move in random direction
    
    
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
    {
        if (m_pellet1 != 0) //pellets are eaten
            m_pellet2 = 1;
        else if (m_pellet1 == 0)
            m_pellet1 = 1;
        m_arena->setCellStatus(m_row, m_col, EMPTY);
    }
    
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
    
}

int Player::col() const
{
    
    return m_col;
}

string Player::dropPoisonPellet()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poison pellet at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
    
    if (dir == NORTH && m_row != 1) //check to see if moving is impossible
    {
        m_row = m_row-1;
        if (m_arena->numberOfRatsAt(m_row, m_col) > 0)
            return "Player walked into a rat and died.";
        return "Player moved north.";
    }
    
    else if (dir == EAST && m_col != MAXCOLS)
    {
        m_col = m_col+1;
        if (m_arena->numberOfRatsAt(m_row, m_col) > 0)
            return "Player walked into a rat and died.";
        return "Player moved east.";
    }
    else if (dir == SOUTH && m_row != MAXROWS)
    {
        m_row = m_row+1;
        if (m_arena->numberOfRatsAt(m_row, m_col) > 0)
            return "Player walked into a rat and died.";
        return "Player moved south.";
    }
    else if (dir == WEST && m_col != 1)
    {
        m_col = m_col -1;
        if (m_arena->numberOfRatsAt(m_row, m_col) > 0)
            return "Player walked into a rat and died.";
        return "Player moved west.";
    }
    
    else
        return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    if (m_arena->numberOfRatsAt(m_row, m_col) > 0 || m_dead == true)
        return true;
    else
        return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRats = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}


Arena::~Arena()
{
    delete m_player;
    int i = 0;
    while (i< m_nRats)
    {
        delete m_rats[i];
        i++;
    }
    
}

int Arena::rows() const
{
    
    return m_rows;
}

int Arena::cols() const
{
    
    return m_cols;
    
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::ratCount() const
{
    
    return m_nRats;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
    int i = 0;
    int x = 0;
    while (x < m_nRats) //run through entire array of rats to figure out which
    {                   //have row r and column c
        if (m_rats[x]->row() == r && m_rats[x]->col() ==c)
            i++; //accounts for if gridpoint has no rat
        x++;
        
    }
    return i;
}



void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill displayGrid with dots (empty) and stars (poison pellets)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
    

    for (r = 1; r <= rows(); r++) //run through gridpoints
    {
        for (c = 1; c <= cols(); c++)
        {
            if (numberOfRatsAt(r, c)== 1)
                displayGrid[r-1][c-1] = 'R';
            else if (numberOfRatsAt(r, c)== 2)
                displayGrid[r-1][c-1] = '2';
            else if (numberOfRatsAt(r, c)== 3)
                displayGrid[r-1][c-1] = '3';
            else if (numberOfRatsAt(r, c)== 4)
                displayGrid[r-1][c-1] = '4';
            else if (numberOfRatsAt(r, c)== 5)
                displayGrid[r-1][c-1] = '5';
            else if (numberOfRatsAt(r, c)== 6)
                displayGrid[r-1][c-1] = '6';
            else if (numberOfRatsAt(r, c)== 7)
                displayGrid[r-1][c-1] = '7';
            else if (numberOfRatsAt(r, c)== 8)
                displayGrid[r-1][c-1] = '8';
            else if (numberOfRatsAt(r, c) >= 9)
                displayGrid[r-1][c-1] = '9';
        }
    }
    
    
    // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');
    
    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
    
    // Write message, rat, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << ratCount() << " rats remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addRat(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a rat on a spot with a poison pellet
    if (getCellStatus(r, c) != EMPTY)
        return false;
    
    // Don't add a rat on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    if (m_nRats == MAXRATS)
        return false; //  If there are MAXRATS existing rats, return false.
    
    m_rats[m_nRats] = new Rat(this, r, c);
    m_nRats++; //there is one more rat every time newRat is called
    
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Don't add a player on a spot with a rat
    if (numberOfRatsAt(r, c) > 0)
        return false;
    
    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRats()
{
    
    int i = 0;
    while (i < m_nRats)
    {
        m_rats[i]->move();
        if (m_rats[i]->row() == m_player->row() && m_rats[i]->col() == m_player->row())
            m_player->setDead(); //player is dead
        if (m_rats[i]->isDead())
        {
            delete m_rats[i];
            
            
            int shift = i;
            while (shift < m_nRats-1)
            {
                m_rats[shift] = m_rats[shift+1];
                shift++;
            }
            m_nRats--; //there is now at least one less dead rat

        }
            i++;
    }
    
    
    m_turns++; // Another turn has been taken
    
    
    
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
        << c << ")" << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
    if (nRats < 0)
    {
        cout << "***** Cannot create Game with negative number of rats!" << endl;
        exit(1);
    }
    if (nRats > MAXRATS)
    {
        cout << "***** Trying to create Game with " << nRats
        << " rats; only " << MAXRATS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRats > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with rats
    while (nRats >0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRat(r, c);
        nRats--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);
        
        Player* player = m_arena->player();
        int dir;
        
        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonPellet();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonPellet();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->ratCount() > 0)
    {
        string msg = takePlayerTurn();
        Player* player = m_arena->player();
        if (player->isDead())
        {
            cout << msg << endl;
            break;
        }
        m_arena->moveRats();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
        default:  return false;
        case 'n': dir = NORTH; break;
        case 'e': dir = EAST;  break;
        case 's': dir = SOUTH; break;
        case 'w': dir = WEST;  break;
    }
    return true;
}


bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    if  (                           //check if moving a step would run off arena edge
         (dir == NORTH && r == 1) ||
         (dir == EAST && c == a.cols()) ||
         (dir == SOUTH && r == a.rows()) ||
         (dir == WEST && c == 1)
         )
        return false;
    else
    {
        if (dir == NORTH)
            r = r-1;
        if (dir == EAST)
            c = c+1;
        if (dir == SOUTH)
            r = r+1;
        if (dir == WEST)
            c = c-1;
    }
    return true;
    
}


bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    
    
    int row[NUMDIRS] = {r-1, r, r+1, r};
    int col[NUMDIRS] = {c, c+1, c, c-1};
    
    
    int northpoint = -1;
    if (r!=1)
        northpoint = a.numberOfRatsAt(row[NORTH], col[NORTH]);
    int eastpoint = -1;
    if (c!= a.cols())
        eastpoint = a.numberOfRatsAt(row[EAST], col[EAST]);
    int southpoint =-1;
    if (r!= a.rows())
        southpoint = a.numberOfRatsAt(row[SOUTH], col[SOUTH]);
    int westpoint = -1;
    if (c!= 1)
        westpoint = a.numberOfRatsAt(row[WEST], col[WEST]);
    
    
    if (
        (
         ((northpoint == 0 || northpoint < 0) &&
          (eastpoint ==0 || eastpoint < 0) &&
          (southpoint ==0 || southpoint <0) &&
          (westpoint == 0 || westpoint <0)) ||
         
         ((northpoint > 0 || northpoint <0) &&
          (eastpoint > 0 || eastpoint <0) &&
          (southpoint > 0 || southpoint <0) &&
          (westpoint > 0 || westpoint <0))
         )
        )
        return false; //Drop pellet if there are no rats around or there is nowhere to move
    else
    {
        int possible[NUMDIRS] = {-1, -1, -1, -1};
        if (northpoint == 0)
            possible[NORTH]= NORTH;
        if (eastpoint ==0)
            possible[EAST] = EAST;
        if (southpoint ==0)
            possible[SOUTH] = SOUTH;
        if (westpoint ==0)
            possible [WEST] = WEST;
        
        //if it's possible to move in a certain direction, set corresponding position in array to the direction. Leave as -1 if you cannot move
        
        int i = 0;
        int surroundCount[NUMDIRS] = {0, 0, 0, 0};
        
       
        //if player were to move in a certain direction, count the number of rats that would be surrounding them if they did
        
        while (i < NUMDIRS)
        {
            if (possible[i] == -1) // means that this doesn't apply, since player cannot move in that
                surroundCount[i]= -1; //direction to begin with
            if (possible[i] != -1)
            {
                if (row[i] != 1)
                    surroundCount[i] += a.numberOfRatsAt(row[i]-1, col[i]);
                if (col[i]!=a.cols())
                    surroundCount[i] += a.numberOfRatsAt(row[i], col[i]+1 );
                if (row[i] != a.rows())
                    surroundCount[i] += a.numberOfRatsAt(row[i]+1, col[i]);
                if (col[i] != 1)
                    surroundCount[i] += a.numberOfRatsAt(row[i], col[i]-1);
            }
            i++;
        }
       
        
        
        
        int looptrack = 0;
        int track = 1;
        int k =0;
        
        //run through surroundCount array to try and find the smallest number of Rats
        while (looptrack < NUMDIRS-1)
        {
            if (surroundCount[k] == -1)
            {
                k++;
                looptrack++;
            }
            else if (surroundCount[k] != -1)
            {
                if (surroundCount[k] <= surroundCount[k+track])
                {
                    track++;
                    looptrack++;
                }
                else if (surroundCount[k] > surroundCount[k+track])
                {
                    if (surroundCount[k+track] == -1)
                    {
                        track++;
                        looptrack++;
                    }
                    else
                    {
                        k = k+track;
                        track = 1;
                        looptrack++;
                    }
                }
            }
            
        }
        bestDir = k;
        
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:
    //Game g(3, 5, 2);
    Game g(10, 12, 40);
    
    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

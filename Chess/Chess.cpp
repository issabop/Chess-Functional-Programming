#include <iostream>
#include<fstream>
#include <string>
#include <windows.h>
#include <conio.h>
using namespace std;

int white = 0, black = 1;
int dim = 8;
int kw, kb, r1w, r2w, r1b, r2b;

void load(char B[][8], string fn,int &turn)
{
    ifstream Rdr(fn);
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Rdr >> B[r][c];
        }
    }
    Rdr >> turn;
}

void load_replay(char B[][8], ifstream &Rdr)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Rdr >> B[r][c];
        }
    }
}

void save(char B[][8], ofstream& Writer)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Writer << B[r][c];
        }
        Writer << endl;
    }
}

void save_board(char B[][8], ofstream& Writer)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Writer << B[r][c];
        }
        Writer << endl;
    }
}

void save_moves(string M[][2], int& moves,int sri,int sci, int mri, int mci)
{
    M[moves][0] = sri;
    M[moves][0] += sci;
    M[moves][1] = mri;
    M[moves][1] += mci;
    moves++;
}

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void print_box(int sr, int sc, int rdim, int cdim, char sym)
{
    for (int r = 0; r < rdim; r++)
    {
        for (int c = 0; c < cdim; c++)
        {
            gotoRowCol(sr + r, sc + c);
            cout << sym;
        }
    }
}

void initialize(char arr[][8], int& turn, string name[])
{
    kw = kb = r1w = r2w = r1b = r2b = 0;
    for (int row = 1; row < 7; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            if (row == 1)
            {
                arr[row][col] = 'P';
            }
            else if (row == 6)
            {
                arr[row][col] = 'p';
            }
            else
            {
                arr[row][col] = '-';
            }
        }
    }
    arr[0][0] = arr[0][7] = 'R';
    arr[0][1] = arr[0][6] = 'H';
    arr[0][2] = arr[0][5] = 'B';
    arr[0][3] = 'Q';
    arr[0][4] = 'K';
    arr[7][0] = arr[7][7] = 'r';
    arr[7][1] = arr[7][6] = 'h';
    arr[7][2] = arr[7][5] = 'b';
    arr[7][3] = 'q';
    arr[7][4] = 'k';
    turn = white;
    cout << "Player 1 Enter Name: "; cin >> name[white];
    cout << "Player 2 Enter Name: "; cin >> name[black];
}

void display(char arr[][8])
{
    system("cls");
    cout << "    A   B   C   D   E   F   G   H" << endl;
    for (int row = 0; row < dim; row++)
    {
        cout << row + 1;
        for (int col = 0; col < dim; col++)
        {
            cout << "   " << arr[row][col];
        }
        cout << endl;
    }
}

void turn_msg(string name)
{
    cout << endl << name << "'s turn" << endl;
}

void turn_change(int &turn)
{
    turn = (turn + 1) % 2;
}

void copy_arr(char TB[][8], char B[][8])
{
    for (int r = 0; r < dim; r++)
    {
        for (int c = 0; c < dim; c++)
        {
            TB[r][c] = B[r][c];
        }
    }
}

void coords(int& row, int& col)
{
    getRowColbyLeftClick(row, col);
    row = row - 1;
    col = ((col - 4) / 4);
}

bool checkpiece(int turn, char choice)
{
    if (turn == white)
    {
        if (choice >= 'A' && choice <= 'Z')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (choice >= 'a' && choice <= 'z')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool checkvalidity(int row, int col)
{
    if (row > 7 || row < 0 || col > 7 || col < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void update_board(char board[][8], int sri, int sci, int mri, int mci)
{
    board[mri][mci] = board[sri][sci];
    board[sri][sci] = '-';
}

void update_console(char board[][8], int sri, int sci, int mri, int mci)
{
    gotoRowCol(sri + 1, (sci * 4) + 4);
    cout << board[sri][sci];
    gotoRowCol(mri + 1, (mci * 4) + 4);
    cout << board[mri][mci];
}

bool h_move(int sri, int sci, int mri, int mci)
{
    return sri == mri;
}

bool v_move(int sri, int sci, int mri, int mci)
{
    return sci == mci;
}

bool d_move(int sri, int sci, int mri, int mci)
{
    return abs(sri - mri) == abs(sci - mci);
}

bool h_path_c(char B[][8], int sri, int sci, int mri, int mci)
{
    if (mci > sci)
    {
        for (int c = sci + 1; c < mci; c++)
        {
            if (B[sri][c] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        for (int c = sci - 1; c > mci; c--)
        {
            if (B[sri][c] != '-')
            {
                return false;
            }
        }
        return true;
    }
}

bool v_path_c(char B[][8], int sri, int sci, int mri, int mci)
{
    if (mri > sri)
    {
        for (int r = sri + 1; r < mri; r++)
        {
            if (B[r][sci] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        for (int r = sri - 1; r > mri; r--)
        {
            if (B[r][sci] != '-')
            {
                return false;
            }
        }
        return true;
    }
}

bool d_path_c(char B[][8], int sri, int sci, int mri, int mci)
{
    int t;
    if ((mri < sri) && (mci < sci))
    {
        t = sri - mri;
        for (int m = 1; m < t; m++)
        {
            if (B[sri - m][sci - m] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else if ((mri < sri) && (mci > sci))
    {
        t = sri - mri;
        for (int m = 1; m < t; m++)
        {
            if (B[sri - m][sci + m] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else if ((mri > sri) && (mci < sci))
    {
        t = mri - sri;
        for (int m = 1; m < t; m++)
        {
            if (B[sri + m][sci - m] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else 
    {
        t = mri -sri;
        for (int m = 1; m < t; m++)
        {
            if (B[sri + m][sci + m] != '-')
            {
                return false;
            }
        }
        return true;
    }
}

bool rook_valid(char B[][8], int sri, int sci, int mri, int mci)
{
    return ((h_move(sri, sci, mri, mci) && h_path_c(B, sri, sci, mri, mci)) || (v_move(sri, sci, mri, mci) && v_path_c(B, sri, sci, mri, mci)));
}

bool bishop_valid(char B[][8], int sri, int sci, int mri, int mci)
{
    return ((d_move(sri, sci, mri, mci) && d_path_c(B, sri, sci, mri, mci)));
}

bool queen_valid(char B[][8], int sri, int sci, int mri, int mci)
{
    return (rook_valid(B, sri, sci, mri, mci) || bishop_valid(B, sri, sci, mri, mci));
}

bool king_valid(char B[][8], int sri, int sci, int mri, int mci)
{   
    if (abs(mci - sci) == 2)
    {
        return (h_move(sri,sci,mri,mci) && h_path_c(B,sri,sci,mri,mci));
    }
    else
    {
        return (queen_valid(B, sri, sci, mri, mci) && (abs(sri - mri) <= 1) && (abs(sci - mci) <= 1));
    }
}

bool horse_valid(char B[][8], int sri, int sci, int mri, int mci)
{
    return ((abs(sri - mri) == 2 && abs(sci - mci) == 1)) || ((abs(sri - mri) == 1 && abs(sci - mci) == 2));
}

bool enpassant_w(char B[][8], int sri, int sci, int mri, int mci, int turn)
{
    char T[8][8];
    if (sri == 4 && B[4][sci -1] == 'p')
    {
        load(T, "Last.txt", turn);
        if (T[sri + 2][sci - 1] == 'p')
        {
            return ((mri - sri == 1) && (sci - mci == 1));
        }
    }
    if (sri == 4 && B[4][sci + 1] == 'p')
    {
        load(T, "Last.txt", turn);
        if (T[sri + 2][sci + 1] == 'p')
        {
            return ((mri - sri == 1) && (mci - sci == 1));
        }
    }
    return false;
}

bool enpassant_b(char B[][8], int sri, int sci, int mri, int mci, int turn)
{
    char T[8][8];
    if (sri == 3 && B[3][sci - 1] == 'P')
    {
        load(T, "Last.txt", turn);
        if (T[sri - 2][sci - 1] == 'P')
        {
            return ((sri - mri == 1) && (sci - mci == 1));
        }
    }
    if (sri == 3 && B[3][sci + 1] == 'P')
    {
        load(T, "Last.txt", turn);
        if (T[sri - 2][sci + 1] == 'P')
        {
            return ((sri - mri == 1) && (mci - sci == 1));
        }
    }
    return false;
}

bool pawn_valid_b(char B[][8], int sri, int sci, int mri, int mci, int turn)
{
    if ((mri < sri) && (sci == mci))
    {
        if (sri == 6)
        {
            return v_move(sri, sci, mri, mci) && v_path_c(B, sri, sci, mri, mci) && (sri - mri <= 2) && (B[mri][mci] == '-');
        }
        else
        {
            return v_move(sri, sci, mri, mci) && v_path_c(B, sri, sci, mri, mci) && (sri - mri == 1) && (B[mri][mci] == '-');
        }
    }
    else
    {
        if (checkpiece((turn + 1) % 2, B[mri][mci]))
        {
            return ((sri - mri == 1) && (abs(mci - sci) == 1));
        }
        else
        {
            return enpassant_b(B, sri, sci, mri, mci, turn);
        }
    }
}

bool pawn_valid_w(char B[][8], int sri, int sci, int mri, int mci, int turn)
{
    if ((mri > sri) && (sci == mci))
    {
        if (sri == 1)
        {
            return v_move(sri, sci, mri, mci) && v_path_c(B, sri, sci, mri, mci) && (mri - sri <= 2) && (B[mri][mci] == '-');
        }
        else
        {
            return v_move(sri, sci, mri, mci) && v_path_c(B, sri, sci, mri, mci) && (mri - sri == 1) && (B[mri][mci] == '-');
        }
    }
    else
    {
        if (checkpiece((turn + 1) % 2, B[mri][mci]))
        {
            return ((mri - sri == 1) && (abs(mci - sci) == 1));
        }
        else
        {
            return enpassant_w(B, sri, sci, mri, mci, turn);
        }
    }
}

bool legit_move(char B[][8], int sri, int sci, int mri, int mci, int turn)
{
    switch (B[sri][sci])
    {
        case 'r':
        case 'R':
            return rook_valid(B, sri, sci, mri, mci);
        case 'b':
        case 'B':
            return bishop_valid(B, sri, sci, mri, mci);
        case 'q':
        case 'Q':
            return queen_valid(B, sri, sci, mri, mci);
        case 'k':
        case 'K':
            return king_valid(B, sri, sci, mri, mci);
        case 'h':
        case 'H':
            return horse_valid(B, sri, sci, mri, mci);
        case 'p':
            return pawn_valid_b(B, sri,sci,mri,mci,turn);
        case 'P':
            return pawn_valid_w(B, sri, sci, mri, mci, turn);
        default:
            return false;
    }
}

void findking(char B[][8], int turn, int& kr, int& kc)
{
    char ks = (turn == black) ? 'k' : 'K';
    for (int r = 0;r < dim; r++)
    {
        for (int c = 0; c < dim; c++)
        {
            if (B[r][c] == ks)
            {
                kr = r;
                kc = c;
                return;
            }
        }
    }
}

bool check(char B[][8], int turn)
{
    turn_change(turn);
    int kr, kc;
    findking(B, turn, kr, kc);
    turn_change(turn);
    for (int r = 0; r < dim; r++)
    {
        for (int c = 0; c < dim; c++)
        {
            if (checkpiece(turn, B[r][c]) && legit_move(B, r, c, kr, kc, turn))
            {
                return true;
            }
        }
    }
    return false;
}

bool self_check(char B[][8], int turn, int sri, int sci, int mci, int mri)
{
    char TB[8][8];
    copy_arr(TB, B);
    update_board(TB, sri, sci, mci, mri);
    turn_change(turn);
    return check(TB, turn);
}

bool castle_valid(char B[][8], int turn, int sri, int sci, int mri, int mci)
{
    if ((abs(sci - mci) == 2) && (sri - mri == 0))
    {
        if (mci > sci)
        {
            if (B[sri][sci] == 'K')
            {
                if ((!check(B, (turn+1)%2)) && (!self_check(B, turn, sri, sci, sri, sci + 1)) && (!self_check(B, turn, sri, sci, sri, sci + 2)))
                {
                    return(h_path_c(B, sri, sci, 0, 7) && (kw == 0) && (r2w == 0));
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if ((!check(B, (turn+1)%2)) && (!self_check(B, turn, sri, sci, sri, sci + 1)) && (!self_check(B, turn, sri, sci, mri, mci)))
                {
                    return(h_path_c(B, sri, sci, 7, 7) && (kb == 0) && (r2b == 0));
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            if (B[sri][sci] == 'K')
            {
                if ((!check(B, (turn + 1) % 2)) && (!self_check(B, turn, sri, sci, sri, sci - 1)) && (!self_check(B, turn, sri, sci, mri, mci)))
                {
                    return(h_path_c(B, sri, sci, 0, 0) && (kw == 0) && (r1w == 0));
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if ((!check(B, (turn + 1) % 2)) && (!self_check(B, turn, sri, sci, sri, sci - 1)) && (!self_check(B, turn, sri, sci, mri, mci)))
                {
                    return(h_path_c(B, sri, sci, 7, 0) && (kb == 0) && (r1b == 0));
                }
                else
                {
                    return false;
                }
            }
        }
    }
    else
    {
        return false;
    }
}

void highlight(char B[][8], char H[][8], int sri, int sci, int turn)
{
    for (int r = 0; r < dim; r++)
    {
        for (int c = 0; c < dim; c++)
        {
            if (!((!checkvalidity(r, c)) || (checkpiece(turn, B[r][c]))) && legit_move(B, sri, sci, r,c, turn))
            {
                if (!self_check(B, turn, sri, sci, r, c))
                {
                    if ((B[sri][sci] == 'K' || B[sri][sci] == 'k') && (abs(sri - r) == 0) && (abs(sci - c) == 2))
                    {
                        if (castle_valid(B,turn,sri,sci,r,c))
                        {
                            H[r][c] = 'X';
                            gotoRowCol(r + 1, (c * 4) + 4);
                            cout << 'X';
                        }
                    }
                    else
                    {
                        H[r][c] = 'X';
                        gotoRowCol(r + 1, (c * 4) + 4);
                        cout << 'X';
                    }
                }
            }
            else
            {
                H[r][c] = '-';
            }
        }
    }
}

void unhighlight(char B[][8], char H[][8])
{
    for (int r = 0; r < dim; r++)
    {
        for (int c = 0; c < dim; c++)
        {
            if (H[r][c] == 'X')
            {
                gotoRowCol(r + 1, (c * 4) + 4);
                cout << B[r][c];
            }
        }
    }
}

bool canImove(char B[][8], int turn)
{
    for (int sr = 0; sr < dim; sr++)
    {
        for (int sc = 0; sc < dim; sc++)
        {
            if ((checkpiece(turn, B[sr][sc])))
            {
                for (int dr = 0; dr < dim; dr++)
                {
                    for (int dc = 0; dc < dim; dc++)
                    {
                        if (!checkpiece(turn, B[dr][dc]))
                        {
                            if ((legit_move(B, sr, sc, dr, dc, turn) && !self_check(B, turn, sr, sc, dr, dc)))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool checkmate(char B[][8], int turn) 
{
    return check(B, (turn+1) % 2) && !canImove(B, turn);
}

bool stalemate(char B[][8], int turn)
{
    return !check(B, (turn + 1) % 2) && !canImove(B, turn);
} 

bool repetition(char B[][8], ifstream & Rdr)
{
    char T[8][8];
    int repeat = 0;
    int count = 0;
    while (!Rdr.eof())
    {
        load_replay(T, Rdr);
        for (int r = 0; r < dim; r++)
        {
            for (int c = 0; c < dim; c++)
            {
                if (T[r][c] == B[r][c])
                {
                    count++;
                }
            }
        }
        if (count == 64)
        {
            repeat++;
        }
        count = 0;
    }
    return repeat == 2;
}

void pawn_promotion(char B[][8])
{
    for (int c = 0; c < dim; c++)
    {
        char promote;
        if (B[7][c] == 'P')
        {
            do
            {
                gotoRowCol(15, 0);
                cout << "Enter What Piece You Want To Promote To (Q / B / H / R): ";
                cin >> promote;
            } while (promote != 'Q' && promote && 'B' && promote != 'H' && promote != 'R');
            B[7][c] = promote;
            gotoRowCol(15, 0);
            cout << "                                                         ";
        }
        else if (B[0][c] == 'p')
        {
            do
            {
                gotoRowCol(15, 0);
                cout << "Enter What Piece You Want To Promote To (q / b / h / r): ";
                cin >> promote;
            } while (promote != 'q' && promote != 'b' && promote != 'h' && promote != 'r');
            B[0][c] = promote;
            gotoRowCol(15, 0);
            cout << "                                                         ";
        }
    }
}

void check_print(char B[][8], int turn)
{
    gotoRowCol(13, 0);
    if (check(B,turn) == true)
    {
        if (turn == white)
        {
            cout << "Check_White = F, Check_Black = T";
        }
        else
        {
            cout << "Check_White = T, Check_Black = F";
        }
    }
    else
    {
        cout << "Check_White = F, Check_Black = F";
    }
}

void checkmate_print(bool mate, string name)
{
    if (mate)
    {
        gotoRowCol(15, 0);
        cout << name << " has been Checkmated!";
    }
}

void stalemate_print(bool draw)
{
    if (draw)
    {
        gotoRowCol(15, 0);
        cout << "Stalemate!";
    }
}

void resign_print(string name, bool &win)
{
    gotoRowCol(15, 0);
    cout << name << " has Resigned";
    win = true;
}

void offer_draw(string name1, string name2, bool& draw)
{
    char inp;
    do
    {
        gotoRowCol(15, 0);
        cout << name1 << " has Offered a draw " << name2 << " enter Y to accept or N to deny: ";
        cin >> inp;
        if (inp == 'Y')
        {
            draw = true;
            gotoRowCol(15, 0);
            cout << name2 << " accepted the draw                                                        ";
            return;
        }
        else if (inp == 'N')
        {
            gotoRowCol(15, 0);
            cout << "                                                                                   ";
            return;
        }
    } while (1);
}

void repetition_print(bool draw)
{
    if (draw)
    {
        gotoRowCol(15, 0);
        cout << "Threefold Repetition!";
    }
}

void load_board(char board[][8], int& turn)
{
    load(board, "Save.txt", turn);

}

void replay(ifstream& Rdr)
{
    char T[8][8];
    while (!Rdr.eof())
    {
        load_replay(T, Rdr);
        display(T);
        system("pause");
    }
}

void update_enpassant(char B[][8], int sri, int sci, int mri, int mci)
{
    if ((abs(sri - mri) == 1 && abs(sci - mci) == 1) && (B[sri][sci] == 'p' || B[sri][sci] == 'P') && B[mri][mci] == '-')
    {
        B[sri][mci] = '-';
        gotoRowCol(sri+1, ((mci*4) + 4));
        cout << '-';
    }
}

void update_castling(char B[][8], int sri, int sci, int mri, int mci)
{
    if ((abs(sci - mci)) == 2 && (B[sri][sci] == 'K' || B[sri][sci] == 'k') && sri - mri == 0)
    {
        if (mci > sci)
        {
            B[sri][mci - 1] = B[sri][7];
            B[sri][7] = '-';
            gotoRowCol(sri + 1, ((7 * 4) + 4));
            cout << '-';
            gotoRowCol(sri + 1, (((mci-1) * 4) + 4));
            cout << B[sri][mci -1];
        }
        else
        {
            B[sri][mci + 1] = B[sri][0];
            B[sri][0] = '-';
            gotoRowCol(sri + 1, ((0 * 4) + 4));
            cout << '-';
            gotoRowCol(sri + 1, (((mci + 1) * 4) + 4));
            cout << B[sri][mci + 1];
        }
    }
}

void king_moved(char B[][8], int sri, int sci)
{
    if (B[sri][sci] == 'K')
    {
        kw = 1;
    }
    else if (B[sri][sci] == 'k')
    {
        kb = 1;
    }
}

void castle_moved(char B[][8], int sri, int sci)
{
    if (B[sri][sci] == 'R' && sci == 0)
    {
       r1w = 1;
    }
    else if (B[sri][sci] == 'R' && sci == 7)
    {
        r2w = 1;
    }
    else if (B[sri][sci] == 'r' && sci == 0)
    {
        r1b = 1;
    }
    else if (B[sri][sci] == 'r' && sci == 7)
    {
        r2b = 1;
    }
}

void display_buttons()
{
    print_box(5, 80, 3, 10, -37);
    gotoRowCol(6, 83);
    cout << "New";
    print_box(10, 80, 3, 10, -37);
    gotoRowCol(11, 83);
    cout << "Load";
    print_box(15, 80, 3, 10, -37);
    gotoRowCol(16, 82);
    cout << "Replay";
}

void display_buttons2()
{
    print_box(5, 80, 3, 10, -37);
    gotoRowCol(6, 83);
    cout << "Undo";
    print_box(10, 80, 3, 10, -37);
    gotoRowCol(11, 83);
    cout << "Draw";
    print_box(15, 80, 3, 10, -37);
    gotoRowCol(16, 82);
    cout << "Resign";
}

void Undo(char B[][8], int& turn, ifstream &Reader)
{
    load_replay(B, Reader);
    for (int x = 0; x < dim; x++)
    {
        for (int y = 0; y < dim; y++)
        {
            gotoRowCol(x + 1, (y * 4) + 4);
            cout << B[x][y];
        }
    }
    turn = (turn + 1) % 2;
} 

void button_click(ifstream& Rdr, char B[][8], int &turn)
{
    int r, c;
    while (1)
    {
        getRowColbyLeftClick(r, c);
        if (r >= 5 && r < 8 && c >= 80 && c < 90)
        {
            return;
        }
        else if (r >= 10 && r < 13 && c >= 80 && c < 90)
        {
            load_board(B, turn);
            return;
        }
        else if (r >= 15 && r < 18 && c >= 80 && c < 90)
        {
            replay(Rdr);
            return;
        }
    }
}

void button_click2(char B[][8], int& turn, string name[], bool &win, bool &draw, ifstream &Reader)
{
    int r, c;
    gotoRowCol(15, 0);
    cout << "Click anywhere to switch turns";
    while (1)
    {
        getRowColbyLeftClick(r, c);
        if (r >= 5 && r < 8 && c >= 80 && c < 90)
        {
            Undo(B, turn, Reader);
            return;
        }
        else if (r >= 10 && r < 13 && c >= 80 && c < 90)
        {
            offer_draw(name[(turn + 1) % 2], name[turn], draw);
            return;
        }
        else if (r >= 15 && r < 18 && c >= 80 && c < 90)
        {
            resign_print(name[(turn + 1) % 2], win);
            return;
        }
        else
        {
            gotoRowCol(15, 0);
            cout << "                                  ";
            return;
        }
    }
}

int main()
{
    char board[8][8], H[8][8];
    string moves[167][2];
    int move = 0;
    string name[2];
    bool mate, draw;
    draw = false;
    int sri, sci, mri, mci, turn;
    initialize(board, turn, name);
    display_buttons();
    ifstream Rdr("Replay.txt");
    button_click(Rdr, board, turn);
    display(board);
    ofstream Replay("Replay.txt");
    ofstream Writer("Save.txt");
    save(board, Writer);
    save_board(board, Replay);
    do
    {
        display_buttons2();
        gotoRowCol(9, 0);
        turn_msg(name[turn]);
        do
        {
            unhighlight(board, H);
            do
            {
                gotoRowCol(11, 0);
                cout << "Select Your Piece: " << endl;
                coords(sri, sci);
            } while ((!checkvalidity(sri, sci)) || (!checkpiece(turn, board[sri][sci])));
            highlight(board, H, sri, sci, turn);
            gotoRowCol(11, 0);
            cout << "Select Destination: " << endl;
            coords(mri, mci);
        } while (!(H[mri][mci] == 'X'));
        unhighlight(board, H);
        ofstream Check("Last.txt");
        save_board(board, Check);
        save_moves(moves, move, sri, sci, mri, mci);
        update_enpassant(board,sri,sci,mri,mci);
        update_castling(board, sri, sci, mri, mci);
        king_moved(board, sri, sci);
        castle_moved(board, sri, sci);
        update_board(board, sri, sci, mri, mci);
        update_console(board, sri, sci, mri, mci);
        pawn_promotion(board);
        check_print(board, turn);
        turn_change(turn);
        mate = checkmate(board, turn);
        checkmate_print(mate, name[turn]);
        draw = stalemate(board, turn);
        stalemate_print(draw);
        ifstream Rdr("Replay.txt");
        draw = repetition(board, Rdr);
        repetition_print(draw);
        ifstream Reader("Save.txt");
        button_click2(board, turn, name, mate, draw, Reader);
        ofstream Writer("Save.txt");
        save(board, Writer);
        Writer << turn;
        save_board(board, Replay);
    } while (mate == false && draw == false);
}



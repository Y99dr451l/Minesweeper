#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime> 
#include <cstdlib>
#include <cmath>
//g++ minesweeper.cpp  -o minesweeper -static-libgcc -static-libstdc++ -static -lpthread

using namespace std;

void promptarg(int& dimx, int& dimy, int& mines);
void promptclic(vector<vector<int>> mask, int dimx, int dimy, int& cx, int& cy);
void printfield(vector<vector<int>> field, vector<vector<int>> mask, int dimx, int dimy, int toggle);
void onclic(vector<vector<int>> field, vector<vector<int>>& mask, int dimx, int dimy, int cx, int cy);
void reveal(vector<vector<int>> field, vector<vector<int>>& mask, int dimx, int dimy, int cx, int cy);
void findfreecell(vector<vector<int>> field, int dimx, int dimy, int& cx, int& cy);

main(int argc, char *argv[]) {
// INPUTS
    int dimx, dimy, mines;
    if (argc!=4) {
        cout << endl << "wrong number of arguments";
        promptarg(dimx, dimy, mines);
    } else {
        dimx = strtol(argv[1], NULL, 10);
        dimy = strtol(argv[2], NULL, 10);
        mines = strtol(argv[3], NULL, 10);
        if (dimx<0 || dimy<0 || mines<0 || mines>dimx*dimy-1) {
            cout << endl << "wrong values: " << dimx << " " << dimy << " " << mines;
            promptarg(dimx, dimy, mines);
        }
    }
// FIELD
    vector<vector<int>> field(dimx+2, vector<int> (dimy+2, 0));
    vector<vector<int>> mask(dimx+2, vector<int> (dimy+2, 0));
    srand(time(0));
    int i(0);
    while (i<mines) {
        int rx(rand()%dimx+1);
        int ry(rand()%dimy+1);
        if (field[rx][ry]!=9) {
            field[rx][ry]=9;
            i++;
        }
    }
    for (int i(1); i < dimx+1; i++) {
        for (int j(1); j < dimy+1; j++) {
            if (field[i][j]!=9) {
                int mcount(0);
                for (int k(i-1); k < i+2; k++) {
                    for (int l(j-1); l < j+2; l++) {
                        if (field[k][l]==9) mcount++;
                    }
                }
                field[i][j]=mcount;
            }   
        }
    }
    printfield(field, mask, dimx, dimy, 0);
// CLIC
    int cx(rand()%dimx+1);
    int cy(rand()%dimy+1);
    findfreecell(field, dimx, dimy, cx, cy);
    onclic(field, mask, dimx, dimy, -1, -1);
    while (field[cx][cy]!=9) { // ADD WIN
        promptclic(mask, dimx, dimy, cx, cy);
        onclic(field, mask, dimx, dimy, cx, cy);
    }
    return 0;
}

void promptarg(int& dimx, int& dimy, int& mines) {
    cout << endl << "enter dimx: ";
    cin >> dimx;
    while (dimx<0) {
        cout << endl << "wrong value; cant be negative";
        cout << endl << "enter dimx: ";
        cin >> dimx; 
    }
    cout << endl << "enter dimy: ";
    cin >> dimy;
    while (dimy<0) {
        cout << endl << "wrong value; cant be negative";
        cout << endl << "enter dimy: ";
        cin >> dimy; 
    }
    cout << endl << "enter mines: ";
    cin >> mines;
    while (mines<0 || mines > dimx*dimy-1) {
        cout << endl << "wrong value; cant exceed dimx*dimy-1 (=" << dimx*dimy-1 <<") or be negative";
        cout << endl << "enter mines: ";
        cin >> mines; 
    }
}

void promptclic(vector<vector<int>> mask, int dimx, int dimy, int& cx, int& cy) {
    cout << endl << "next click: ";
    cin >> cx >> cy;
    if (cx!=-1 && cy!=-1) {
        while (cx<1 || cx>dimx) {
            cout << endl << "cx out of bounds";
            cout << endl << "enter cx: ";
            cin >> cx;
        }
        while (cy<1 || cy>dimy) {
            cout << endl << "cy out of bounds";
            cout << endl << "enter cy: ";
            cin >> cy;
        }
        if (mask[cx][cy]==1) {
            cout << endl << "cell already uncovered";
            promptclic(mask, dimx, dimy, cx, cy);
        }
    } 
}

void printfield(vector<vector<int>> field, vector<vector<int>> mask, int dimx, int dimy, int toggle) {
    cout << endl;
    int spacing(ceil(log10(dimy))+1);
    for (int i(0); i < spacing+1; i++) cout << " ";
    for (int i(0); i < (dimx-(dimx%10))/10; i++) {
        cout << setw(10) << "|";
    }
    for (int i(1); i < dimy+1; i++) {
        cout << endl << setw(spacing) << i << " ";
        for (int j(1); j < dimx+1; j++) {
            if (mask[i][j]==1 || toggle==0) {
                if (field[i][j]==9) cout << "*";
                else if (field[i][j]==0) cout << " ";
                else cout << field[i][j];
            //} else cout << "☻";
            } else cout << "X";
        }
        cout << " " << i;
    }
    cout << endl;
}

void onclic(vector<vector<int>> field, vector<vector<int>>& mask, int dimx, int dimy, int cx, int cy) {
    //cout << endl << "onclic";
    if (cx==-1 && cy==-1) {
        findfreecell(field, dimx, dimy, cx, cy);
        reveal(field, mask, dimx, dimy, cx, cy);
    } else {
        mask[cx][cy]=1;
        if (field[cx][cy]==0) {
            reveal(field, mask, dimx, dimy, cx, cy);
        } else if (field[cx][cy]==9) {
            cout << endl << "++++++++++ DEAD ++++++++++";
            // END GAME
            // PROMPT RESTART
        }
    }
    printfield(field, mask, dimx, dimy, 1);
}

void reveal(vector<vector<int>> field, vector<vector<int>>& mask, int dimx, int dimy, int cx, int cy) {
    mask[cx][cy]=1;
    for (int i(cx-1); i< cx+2; i++) {
        for (int j(cy-1); j<cy+2; j++) {
            if (i>0 && j>0 && i<dimx+1 && j<dimy+1 && mask[i][j]==0){
                mask[i][j]=1;
                if (field[i][j]==0) reveal(field, mask, dimx, dimy, i, j);
            }
           
        }
    }
}

void findfreecell(vector<vector<int>> field, int dimx, int dimy, int& cx, int& cy) {
    do {
        cx = rand()%dimx+1;
        cy = rand()%dimy+1;
    } while (field[cx][cy]!=0);
}

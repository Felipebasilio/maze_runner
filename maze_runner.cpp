#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

vector<vector<char> > maze;
bool foundExit = false;
int startX, startY;
mutex mtx; 


void loadMaze(const string& filename) {
    ifstream file("data/" + filename);

    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo de labirinto." << endl;
        exit(1);
    }

    int rows, cols;
    file >> rows >> cols;
    file.ignore();

    maze.resize(rows, vector<char>(cols));

    for (int i = 0; i < rows; ++i) {
        string line;
        getline(file, line);
        for (int j = 0; j < cols; ++j) {
            maze[i][j] = line[j];
        }
    }

    file.close();
}


void findEntrance() {
    bool entranceFound = false;
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == 'e') {
                startX = i;
                startY = j;
                entranceFound = true;
                return;
            }
        }
    }

    if (!entranceFound) {
        cout << "Erro: Entrada do labirinto ('e') não encontrada." << endl;
        exit(1);
    }
}


bool isValid(int x, int y) {
    if (x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size()) {
        return false;
    }
    return maze[x][y] == 'x' || maze[x][y] == 's';
}


void printMaze() {
    for (const auto& row : maze) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
    cout << endl;
}


void exploreMaze(int x, int y) {
    {
        lock_guard<mutex> lock(mtx); 
        if (foundExit) return; 

        if (maze[x][y] == 's') {
            foundExit = true;
            cout << "Saída encontrada na posição: (" << x << ", " << y << ")" << endl;
            return;
        }

        if (maze[x][y] != 'e') {
            maze[x][y] = 'o'; 
        }
        printMaze();
    }

    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    vector<thread> threads;
    
    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        {
            lock_guard<mutex> lock(mtx);
            if (!isValid(newX, newY)) continue;

            if (maze[newX][newY] != 's') {
                maze[newX][newY] = '.';
            }
        }

        threads.emplace_back([newX, newY]() {
            exploreMaze(newX, newY);
        });
    }

    for (auto& t : threads) {
        t.join(); 
    }

    {
        lock_guard<mutex> lock(mtx);
        if (maze[x][y] != 'e' && maze[x][y] != 's') {
            maze[x][y] = '.'; 
        }
        printMaze();
    }
}

int main() {
    cout << "Carregando o labirinto..." << endl;
    loadMaze("maze.txt");

    cout << "Procurando a entrada..." << endl;
    findEntrance();
    cout << "Entrada encontrada em: (" << startX << ", " << startY << ")" << endl;

    cout << "Explorando o labirinto..." << endl;
    thread mainThread(exploreMaze, startX, startY);
    mainThread.join();

    if (!foundExit) {
        cout << "Não há caminho para a saída." << endl;
    }
    
    else {
        cout << "Saída encontrada." << endl;
        return 1;
    }

    return 0;
}

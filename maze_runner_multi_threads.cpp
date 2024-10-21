#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

vector<vector<char> > maze;
bool foundExit = false;
int startX, startY;
mutex mtx; // Mutex para proteger a impressão e a variável foundExit

// Função para carregar o labirinto do arquivo
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

// Função para encontrar a entrada no labirinto
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

// Função para verificar se a posição é válida
bool isValid(int x, int y) {
    if (x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size()) {
        return false;
    }
    return maze[x][y] == 'x' || maze[x][y] == 's';
}

// Função para imprimir o labirinto
void printMaze() {
    this_thread::sleep_for(chrono::milliseconds(200));
    for (const auto& row : maze) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
    cout << endl;
}

// Função recursiva para explorar o labirinto com múltiplas threads
void exploreMaze(int x, int y) {
    if (foundExit) return; // Se a saída já foi encontrada, sair

    {
        lock_guard<mutex> lock(mtx); // Bloqueia a região crítica
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

    // Movimentos possíveis: cima, baixo, esquerda, direita
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
        t.join(); // Espera todas as threads terminarem
    }

    {
        lock_guard<mutex> lock(mtx);
        if (maze[x][y] != 'e') {
            maze[x][y] = '.';
        }
        printMaze();
    }
}

int main() {
    cout << "Carregando o labirinto..." << endl;
    loadMaze("maze4.txt");

    cout << "Procurando a entrada..." << endl;
    findEntrance();
    cout << "Entrada encontrada em: (" << startX << ", " << startY << ")" << endl;

    cout << "Explorando o labirinto..." << endl;
    thread mainThread(exploreMaze, startX, startY);
    mainThread.join();

    if (!foundExit) {
        cout << "Não há caminho para a saída." << endl;
    }

    return 0;
}

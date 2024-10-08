#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<vector<char> > maze;
bool foundExit = false;
int startX, startY;

// Função para carregar o labirinto do arquivo
void loadMaze(const string& filename) {
    ifstream file("data/"+filename);

    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo de labirinto." << endl;
        exit(1); // Encerra o programa caso o arquivo não seja encontrado
    }

    int rows, cols;
    file >> rows >> cols;
    file.ignore(); // Ignora o resto da linha

    maze.resize(rows, vector<char>(cols));

    // Lê o labirinto
    for (int i = 0; i < rows; ++i) {
        string line;
        getline(file, line);
        if (line.size() != cols) {
            cout << "Erro: número de colunas no arquivo de labirinto é inconsistente." << endl;
            exit(1); // Encerra o programa se houver inconsistência
        }
        for (int j = 0; j < cols; ++j) {
            maze[i][j] = line[j];
        }
    }

    file.close();
}

// Função para encontrar a posição da entrada no labirinto
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
        exit(1); // Encerra o programa se a entrada não for encontrada
    }
}


// Função para verificar se a posição é válida
bool isValid(int x, int y) {
    // Verifica se está dentro dos limites da matriz
    if (x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size()) {
        return false;
    }

    // Verifica se a posição é um caminho válido ('x') ou a saída ('s')
    return maze[x][y] == 'x' || maze[x][y] == 's';
}

// Função para imprimir o labirinto atualizado
void printMaze() {
    for (const auto& row : maze) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
    cout << endl;
}

// Função recursiva para explorar o labirinto
bool exploreMaze(int x, int y) {
    // Verifica se a posição atual é a saída
    if (maze[x][y] == 's') {
        foundExit = true;
        return true;
    }

    // Marca a posição atual como explorada
    maze[x][y] = 'o';
    printMaze();

    // Tenta explorar as direções: cima, baixo, esquerda, direita
    int dx[] = {-1, 1, 0, 0}; // Movimentos verticais
    int dy[] = {0, 0, -1, 1}; // Movimentos horizontais

    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        // Verifica se a nova posição é válida para explorar
        if (isValid(newX, newY)) {
            if (exploreMaze(newX, newY)) {
                return true;
            }
        }
    }

    // Marca a posição atual como explorada (ponto)
    maze[x][y] = '.';
    printMaze();

    return false;
}

int main() {
    // Carrega o labirinto do arquivo
    cout << "Carregando o labirinto..." << endl;
    loadMaze("maze.txt");

    // Encontra a posição de entrada
    cout << "Procurando a entrada..." << endl;
    findEntrance();
    cout << "Entrada encontrada em: (" << startX << ", " << startY << ")" << endl;

    // Inicia a busca pelo caminho para a saída
    cout << "Explorando o labirinto..." << endl;
    if (exploreMaze(startX, startY)) {
        cout << "Saída encontrada!" << endl;
    } else {
        cout << "Não há caminho para a saída." << endl;
    }

    return 0;
}

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class MazeSolver {
    private static char[][] maze;
    private static boolean foundExit = false;
    private static int startX, startY;

    public static void main(String[] args) {
        try {
            // Carrega o labirinto do arquivo
            loadMaze("maze5.txt");

            findEntrance();

            if (exploreMaze(startX, startY)) {
                System.out.println("Saída encontrada!");
            } else {
                System.out.println("Não há caminho para a saída.");
            }
        } catch (FileNotFoundException e) {
            System.out.println("Arquivo de labirinto não encontrado.");
        }
    }

    private static void loadMaze(String filename) throws FileNotFoundException {
        Scanner scanner = new Scanner(new File(filename));

        int rows = scanner.nextInt();
        int cols = scanner.nextInt();
        scanner.nextLine();

        maze = new char[rows][cols];

        for (int i = 0; i < rows; i++) {
            String line = scanner.nextLine();
            maze[i] = line.toCharArray();
        }
    }

    private static void findEntrance() {
        for (int i = 0; i < maze.length; i++) {
            for (int j = 0; j < maze[i].length; j++) {
                if (maze[i][j] == 'e') {
                    startX = i;
                    startY = j;
                    return;
                }
            }
        }
    }

    private static boolean exploreMaze(int x, int y) {
        if (maze[x][y] == 's') {
            foundExit = true;
            return true;
        }

        maze[x][y] = 'o';
        printMaze();

        int[] dx = {-1, 1, 0, 0};
        int[] dy = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValid(newX, newY)) {
                if (exploreMaze(newX, newY)) {
                    return true;
                }
            }
        }

        maze[x][y] = '.';
        printMaze();

        return false;
    }

    private static boolean isValid(int x, int y) {
        if (x < 0 || x >= maze.length || y < 0 || y >= maze[0].length) {
            return false;
        }

        return maze[x][y] == 'x' || maze[x][y] == 's';
    }

    private static void printMaze() {
        for (char[] row : maze) {
            for (char c : row) {
                System.out.print(c);
            }
            System.out.println();
        }
        System.out.println();
    }
}

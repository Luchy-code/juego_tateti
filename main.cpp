#include <windows.h>
#include <tchar.h>
#include <cstdio>

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 100
#define GRID_SIZE 3

// Declaración de la función de la ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ResetGame();
void CheckWin();
void ComputerMove();

char grid[GRID_SIZE][GRID_SIZE];
int playerWins = 0;
int computerWins = 0;
HWND hwndButtons[GRID_SIZE][GRID_SIZE];
HWND hwndPlayerWins;
HWND hwndComputerWins;
HWND hwndResetButton;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Tateti";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Calcular el tamaño de la ventana
    int windowWidth = GRID_SIZE * BUTTON_WIDTH + 10; // +20 para margen
    int windowHeight = GRID_SIZE * BUTTON_HEIGHT + 100; // +100 para etiquetas y margen inferior

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "TaTeTi",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int turn = 1; // 1 for player, -1 for computer

    switch (uMsg) {
        case WM_CREATE: {
            // Crear botones
            for (int i = 0; i < GRID_SIZE; ++i) {
                for (int j = 0; j < GRID_SIZE; ++j) {
                    hwndButtons[i][j] = CreateWindow(
                        "BUTTON",
                        "",
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                        j * BUTTON_WIDTH, i * BUTTON_HEIGHT,
                        BUTTON_WIDTH, BUTTON_HEIGHT,
                        hwnd,
                        (HMENU)(i * GRID_SIZE + j),
                        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                        NULL
                    );
                }
            }

            // Crear labels para el conteo de partidas ganadas y perdidas
            hwndPlayerWins = CreateWindow(
                "STATIC",
                "Player Wins: 0",
                WS_VISIBLE | WS_CHILD,
                0, GRID_SIZE * BUTTON_HEIGHT+20,
                150, 20,
                hwnd,
                NULL,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            hwndComputerWins = CreateWindow(
                "STATIC",
                "Computer Wins: 0",
                WS_VISIBLE | WS_CHILD,
                0, GRID_SIZE * BUTTON_HEIGHT + 45,
                150, 20,
                hwnd,
                NULL,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            // Crear un botón de reinicio
            hwndResetButton = CreateWindow(
                "BUTTON",
                "Reiniciar",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                215,
                GRID_SIZE * BUTTON_HEIGHT+20,
                80,
                40,
                hwnd,
                (HMENU)100,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            ResetGame();
            break;
        }
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            if (id == 100) { // ID del botón de reinicio
                ResetGame();
                break;
            }

            int row = id / GRID_SIZE;
            int col = id % GRID_SIZE;

            if (grid[row][col] == ' ') {
                grid[row][col] = 'X';
                SetWindowText(hwndButtons[row][col], "X");
                CheckWin();
                turn = -1;
                ComputerMove();
                CheckWin();
                turn = 1;
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void ResetGame() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = ' ';
            SetWindowText(hwndButtons[i][j], "");
        }
    }
}

void CheckWin() {
    // Check rows, columns, and diagonals
    for (int i = 0; i < GRID_SIZE; ++i) {
        if (grid[i][0] != ' ' && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2]) {
            if (grid[i][0] == 'X') {
                //MessageBox(NULL, "Player wins!", "Game Over", MB_OK);
                playerWins++;
                char buf[32];
                snprintf(buf, sizeof(buf), "Player Wins: %d", playerWins);
                SetWindowText(hwndPlayerWins, buf);
            } else {
                //MessageBox(NULL, "Computer wins!", "Game Over", MB_OK);
                computerWins++;
                char buf[32];
                snprintf(buf, sizeof(buf), "Computer Wins: %d", computerWins);
                SetWindowText(hwndComputerWins, buf);
            }
            ResetGame();
            return;
        }
    }

    for (int i = 0; i < GRID_SIZE; ++i) {
        if (grid[0][i] != ' ' && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i]) {
            if (grid[0][i] == 'X') {
                //MessageBox(NULL, "Player wins!", "Game Over", MB_OK);
                playerWins++;
                char buf[32];
                snprintf(buf, sizeof(buf), "Player Wins: %d", playerWins);
                SetWindowText(hwndPlayerWins, buf);
            } else {
                //MessageBox(NULL, "Computer wins!", "Game Over", MB_OK);
                computerWins++;
                char buf[32];
                snprintf(buf, sizeof(buf), "Computer Wins: %d", computerWins);
                SetWindowText(hwndComputerWins, buf);
            }
            ResetGame();
            return;
        }
    }

    if (grid[0][0] != ' ' && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
        if (grid[0][0] == 'X') {
            //MessageBox(NULL, "Player wins!", "Game Over", MB_OK);
            playerWins++;
            char buf[32];
            snprintf(buf, sizeof(buf), "Player Wins: %d", playerWins);
            SetWindowText(hwndPlayerWins, buf);
        } else {
            //MessageBox(NULL, "Computer wins!", "Game Over", MB_OK);
            computerWins++;
            char buf[32];
            snprintf(buf, sizeof(buf), "Computer Wins: %d", computerWins);
            SetWindowText(hwndComputerWins, buf);
        }
        ResetGame();
        return;
    }

    if (grid[0][2] != ' ' && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
        if (grid[0][2] == 'X') {
            //MessageBox(NULL, "Player wins!", "Game Over", MB_OK);
            playerWins++;
            char buf[32];
            snprintf(buf, sizeof(buf), "Player Wins: %d", playerWins);
            SetWindowText(hwndPlayerWins, buf);
        } else {
            //MessageBox(NULL, "Computer wins!", "Game Over", MB_OK);
            computerWins++;
            char buf[32];
            snprintf(buf, sizeof(buf), "Computer Wins: %d", computerWins);
            SetWindowText(hwndComputerWins, buf);
        }
        ResetGame();
        return;
    }

    // Check for tie
    bool tie = true;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == ' ') {
                tie = false;
            }
        }
    }

    if (tie) {
        MessageBox(NULL, "It's a tie!", "Game Over", MB_OK);
        ResetGame();
    }
}

void ComputerMove() {
    // Simple AI: choose the first empty spot
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == ' ') {
                grid[i][j] = 'O';
                SetWindowText(hwndButtons[i][j], "O");
                return;
            }
        }
    }
}


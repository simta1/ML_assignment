#include <windows.h>
#include <conio.h> // _kbhit, _getch
#include "game.h"
#include "agent.h"
#include "userControlKey.h"

void consoleInit() {
    // console title
    SetConsoleTitle(TEXT("tetris"));

    // cursor
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void runWithPlayer();
void runWithAgent();

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
	if (argc < 2) {
        cerr << "Usage: main.exe [--player | --agent]\n";
		return 1;
	}

    consoleInit();

	string mode = argv[1];
    if (mode == "--player") runWithPlayer();
    else if (mode == "--agent") runWithAgent();
    else {
        cerr << "Usage: main.exe [--player | --agent]\n";
		return 1;
	}

    return 0;
}

void runWithPlayer() {
    Game<20, 10> game("data.txt");
    game.start();

    while (1) {
        if (_kbhit()) {
            char key = _getch();

            if (key == KEY_PAUSE_GAME) game.pause();
            else if (key == KEY_REPLAY_GAME) game.replay();
            else if (key == KEY_MOVE_LEFT) game.applyMove(Move::LEFT);
            else if (key == KEY_MOVE_RIGHT) game.applyMove(Move::RIGHT);
            else if (key == KEY_ROTATE_LEFT) game.applyMove(Move::ROTATE_CCW);
            else if (key == KEY_ROTATE_RIGHT) game.applyMove(Move::ROTATE_CW);
            else if (key == KEY_DROP_SOFT) game.applyMove(Move::DOWN);
            else if (key == KEY_DROP_HARD) game.applyMove(Move::HARDDROP);
            else if (key == KEY_HOlD) game.applyMove(Move::HOLD);
            else if (key == KEY_CHANGE_THEMA) game.changeThema();
        }

        game.run();
    }
}

void runWithAgent() {
    Game<20, 10> game;
    game.start();
    
    string dataPath = "data.txt";
    // string dataPath = "model_junho.txt";
    // string dataPath = "model_always_left.txt"; // 항상 왼쪽 벽 끝에 테트로미노 둠

    Agent agent(dataPath);
    
    while (1) {
		Move move = agent.predict(game);
		game.applyMove(move);
		game.run();
        Sleep(10);
    }
}
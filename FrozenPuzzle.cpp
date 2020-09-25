
#include <iostream>
#include <bangtal.h>
#include <ctime>




using namespace bangtal;
using namespace std;


bool PuzzleGame(ScenePtr scene, int num);



int main()
{
    srand((unsigned int)time(NULL));

    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);


    // 장면생성
    auto scene1 = Scene::create("", "images/시작.png");
    auto scene2 = Scene::create("", "images/background.png");


    //  Scene 1: 시작화면
    auto start_button = Object::create("images/start.png", scene1, 520, 300);

    start_button->setScale(2.0f);

    start_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
        scene2->enter();

        return true;
        });

    bool completed_1;

    PuzzleGame(scene2, 1);
    completed_1 = PuzzleGame(scene2, 1);

    if (completed_1 == true)
    {
        endGame();
    }

    startGame(scene1);

    return 0;


    // Scene 2: 퍼즐맞추기 게임



}

    bool PuzzleGame(ScenePtr scene, int num){

    time_t start_time;

    ObjectPtr game_board[16];
    ObjectPtr init_board[16];

    bool completed_ = false;
    int blank = 15;
    for (int i = 0; i < 16; i++) {
        string filename = "images/piece" + to_string(num)+ "-"  + to_string(i + 1) + ".png";
        cout << filename << endl;

        game_board[i] = Object::create(filename, scene, (i % 4) * 320, 540 - (i / 4) * 180);
        init_board[i] = game_board[i];
        
        game_board[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool {

            int j;
            for (j = 0; j < 16; j++) {

                if (game_board[j] == object)
                    break;
            }

            cout << j << endl;

            if ((j > 0 && j < 16 && (j % 4 > 0 && j - 1 == blank) || j % 4 < 3 && j + 1 == blank || j > 3 && j - 4 == blank || j < 12 && j + 4 == blank)) {

                cout << "move " << blank << " <-> " << j << endl;

                game_board[j]->locate(scene, (blank % 4) * 320, 540 - (blank / 4) * 180);
                game_board[blank]->locate(scene, (j % 4) * 320, 540 - (j / 4) * 180);


                game_board[j] = game_board[blank];
                game_board[blank] = object;
                blank = j;


                int k;

                for (k = 0; k < 16; k++) {

                    if (game_board[k] != init_board[k]) break;

                }

                if (k == 16) {

                    auto end_time = time(NULL);
                    cout << difftime(end_time, start_time) << endl;

                    string buf = to_string(difftime(end_time, start_time)) + "걸렸습니다!!!", showMessage(buf.c_str());
                    completed_ = true;
                
                
                }
            }



            return true;
        });
    }

    game_board[blank]->hide();


    auto count = 0;
    auto timer = Timer::create(0.1);
    timer->setOnTimerCallback([&](TimerPtr t)->bool {

        cout << "timeout!!" << count << endl;

        int j = 0;

        do {
            switch (rand() % 4) {

            case 0: j = blank - 1; break;
            case 1: j = blank + 1; break;
            case 2: j = blank - 4; break;
            case 3: j = blank + 4; break;


            }
        }

        while ((j < 0 || j > 15 || !((j % 4 > 0 && j - 1 == blank) || j % 4 < 3 && j + 1 == blank || j > 3 && j - 4 == blank || j < 12 && j + 4 == blank)));

             cout << "move " << blank << " <-> " << j << endl;

             game_board[j]->locate(scene, (blank % 4) * 320, 540 - (blank / 4) * 180);
             game_board[blank]->locate(scene, (j % 4) * 320, 540 - (j / 4) * 180);


             auto object = game_board[j];
             game_board[j] = game_board[blank];
             game_board[blank] = object;
             blank = j;

            count++;
            if (count < 100) {
                t->set(0.1f);
                t->start();
            }

            return true;

        });

    timer->start();


    start_time = time(NULL);


    return completed_;
}


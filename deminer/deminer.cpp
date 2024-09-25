#include <iostream>
#include <windows.h>
#include <sstream>
#include <vector>
#include <ctime>
#include <conio.h>
#include<stack>
void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
enum ConsoleColor
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARKGRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15 
};
const int border = 100;
const int emptiness = 0;
const int mine = 10;
void setColor(int background, int text)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
// Функция для сохранения текущих параметров цвета консоли
WORD getConsoleTextAttribute() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.wAttributes;
}
class Map {
private:
    int size;
    int numMines;
    std::vector<std::vector<int>> map;
    std::vector<std::vector<int>> mask;
public:
    void e_size()
    {
        do
        {
            std::cout << "Enter the size of the card (5 to 20):";
            std::cin >> size;
            system("cls");
        } while (size <= 0);
    }
    void e_mines() {
        do {
            std::cout << "Enter the number of min (max " << (size - 2) * (size - 2) << "): ";
            std::cin >> numMines;
            system("cls");
        } while (numMines < 1 || numMines >= (size - 2) * (size - 2));
    }
    Map() {
        map_filling();
    }

    bool is_border(int x, int y) {
        if (x < 0 || x >= size || y < 0 || y >= size) {
            return false;
        }
        if (map[x][y] == border)
            return true;
        else
            return false;
    }
    bool check_win() {
        for (int i = 1; i < size - 1; i++) {
            for (int j = 1; j < size - 1; j++) {
                if (map[j][i] != mine && mask[j][i] == emptiness) {
                    return false; // Если есть неоткрытая клетка без мины, игра не выиграна
                }
            }
        }
        return true; // Все клетки без мин открыты
    }
    void show() {
        system("cls");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (mask[j][i] == emptiness)
                {
                    std::cout << ".";
                    continue;
                }
                if (map[j][i] == border)
                {
                    cout_color('#', LIGHT_BLUE);
                }
                else if (map[j][i] == emptiness)
                    std::cout << " ";
                else if (map[j][i] == mine)
                {
                    cout_color('*', RED);
                }
                else if (map[j][i] == 1)
                {
                    cout_color('1', BROWN);
                }
                else if (map[j][i] == 2)
                {
                    cout_color('2', LIGHT_GREEN);
                }
                else if (map[j][i] == 3)
                {
                    cout_color('3', LIGHT_CYAN);
                }
                else if (map[j][i] == 4)
                {
                    cout_color('4', LIGHT_MAGENTA);
                }
                else if (map[j][i] == 5)
                {
                    cout_color('5', GREEN);
                }
                else if (map[j][i] == 6)
                {
                    cout_color('6', CYAN);
                }
                else if (map[j][i] == 7)
                {
                    cout_color('7', MAGENTA);
                }
                else if (map[j][i] == 8)
                {
                    cout_color('8', LIGHT_GRAY);
                }
            }
            std::cout << "\n";
        }
    }

    void init_vector(std::vector<std::vector<int>> &vec) {

        for (int i = 0; i < size; i++) {
            std::vector<int> temp;
            for (int j = 0; j < size; j++) {
                if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
                    temp.push_back(border);
                else
                    temp.push_back(emptiness);
            }
            vec.push_back(temp);
        }
    }
    void map_filling()
    {
        init_vector(map);
    }
    void cout_color(char ch, int color) {
        WORD defaultAttributes = getConsoleTextAttribute(); // Сохранение текущих параметров цвета консоли
        setColor(BLACK, color); // Установка нужного цвета
        std::cout << ch;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultAttributes); // Восстановление параметров цвета консоли
    }
    void min_setting() {
        if (numMines >= (size - 2) * (size - 2)) {
            std::cout << "ERROR" << std::endl;
            return;
        }
        for (int i = 0; i < numMines; i++) {
            int x = 0, y = 0;
            do {
                x = rand() % (size - 2) + 1;
                y = rand() % (size - 2) + 1;
            } while (map[x][y] == mine);
            map[x][y] = mine;
        }
    }

    void num_setting() {
        for (int i = 1; i < size - 1; i++) {
            int temp = 0;
            for (int j = 1; j < size - 1; j++) {
                if (map[j][i] == mine)
                    continue;
                if (map[j][i + 1] == mine)
                    temp++;
                if (map[j][i - 1] == mine)
                    temp++;
                if (map[j + 1][i + 1] == mine)
                    temp++;
                if (map[j + 1][i - 1] == mine)
                    temp++;
                if (map[j - 1][i + 1] == mine)
                    temp++;
                if (map[j - 1][i - 1] == mine)
                    temp++;
                if (map[j + 1][i] == mine)
                    temp++;
                if (map[j - 1][i] == mine)
                    temp++;
                map[j][i] = temp;
                temp = 0;
            }
        }
    }
    void mask_filling() {
        init_vector(mask);
    }
    int open_cell(int x, int y)
    {
        int result = 1;
        mask[x][y] = 1;
        if (map[x][y]==mine)
        {
            result=mine;
        }
        if (map[x][y] == emptiness)
        {
            result = emptiness;
        }
        show();
        return result;;
    }
    void game_over()
{
    Sleep(500);
    system("cls"); // Очистка экрана
    std::cout << "Game Over";
    Sleep(2000); // Задержка на 2 секунды для отображения сообщения
    system("pause"); // Пауза, чтобы пользователь мог увидеть сообщение
}

    void game_won() {
        std::cout << "Congratulations! You won!";
        Sleep(2000);
        system("pause");
    }
    void fill(int px,int py)
    {
        std::stack <int> stk;
        stk.push(px);
        stk.push(py);
        int x = 0, y = 0;
        while (true)
        {
            y = stk.top();
            stk.pop();
            x = stk.top();
            stk.pop();
            
            if (map[x][y+1]==emptiness && mask[x][y + 1] == 0)
            {
                stk.push(x);
                stk.push(y + 1);
            }
            mask[x][y + 1] = 1;
            
            if (map[x][y - 1] == emptiness && mask[x][y - 1] == 0)
            {
                stk.push(x);
                stk.push(y - 1);
            }
            mask[x][y - 1] = 1;
            if (map[x+1][y + 1] == emptiness && mask[x+1][y + 1] == 0)
            {
                stk.push(x+1);
                stk.push(y + 1); 
            }
            mask[x+1][y + 1] = 1;
            
            if (map[x+1][y - 1] == emptiness && mask[x+1][y - 1] == 0)
            {
                stk.push(x+1);
                stk.push(y - 1);   
            }
            mask[x+1][y - 1] = 1;
            
            if (map[x - 1][y + 1] == emptiness && mask[x-1][y + 1] == 0)
            {
                stk.push(x - 1);
                stk.push(y + 1);
            }
            mask[x - 1][y + 1] = 1;
            
            if (map[x - 1][y - 1] == emptiness && mask[x-1][y - 1] == 0)
            {
                stk.push(x - 1);
                stk.push(y - 1);
            }
            mask[x - 1][y - 1] = 1;
            
            if (map[x + 1][y] == emptiness && mask[x][y + 1] == 0)
            {
                stk.push(x + 1);
                stk.push(y);
            }
            mask[x + 1][y] = 1;
            
            if (map[x - 1][y] == emptiness && mask[x-1][y] == 0)
            {
                stk.push(x - 1);
                stk.push(y);
            }
            mask[x - 1][y] = 1;
            
            if (stk.empty())
            {
                break;
            }
        }
    }
};

class keyboard {
private:
    int ch = 0;

public:
    keyboard() {
        ch = 0;
    }

    void wait_key() {
        ch = _getch();
    }

    int get_key() {
        return ch;
    }
};

class cursor {
private:
    int x = 1;
    int y = 1;
    int temp_x = 1;
    int temp_y = 1;

public:
    void save() {
        temp_x = x;
        temp_y = y;
    }

    void undo() {
        x = temp_x;
        y = temp_y;
    }

    void incX() {
        x++;
    }

    void decX() {
        x--;
    }

    void incY() {
        y++;
    }

    void decY() {
        y--;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    void move() {
        gotoxy(x, y);
    }
};

class user {
private:
    keyboard kb;
    cursor cr;
    Map map;
public:
    user() {
        map.e_size();
        map.e_mines();
        map.map_filling();
        map.mask_filling();
        map.min_setting();
        map.num_setting();
        map.show();
    }

    void run() {
        cr.move();
        bool exit = false;
        while (!exit) {
            kb.wait_key();
            cr.save();
            switch (kb.get_key()) {
            case 77: cr.incX(); break; // Вправо
            case 80: cr.incY(); break; // Вниз
            case 75: cr.decX(); break; // Влево
            case 72: cr.decY(); break; // Вверх
            case 13:
                int result = map.open_cell(cr.getX(), cr.getY());
                if (result == mine)
                {
                    map.show();
                    map.game_over();
                    exit = true;
                }
                if (result == emptiness)
                {
                    map.fill(cr.getX(), cr.getY());
                    map.show();
                }
                if (map.check_win()) 
                {
                    map.show();
                    map.game_won();
                    exit = true;
                }
                map.show();
                break;

            }
            if (map.is_border(cr.getX(), cr.getY())) {
                cr.undo();
            }
            cr.move();
        }
    }
};

class Game {
private:
    void showlogo() {
        // ASCII-арт логотип
        const char* logo = R"(
$$$$$$$\  $$$$$$$$\ $$\      $$\ $$$$$$\ $$\   $$\ $$$$$$$$\ $$$$$$$\  
$$  __$$\ $$  _____|$$$\    $$$ |\_$$  _|$$$\  $$ |$$  _____|$$  __$$\ 
$$ |  $$ |$$ |      $$$$\  $$$$ |  $$ |  $$$$\ $$ |$$ |      $$ |  $$ |
$$ |  $$ |$$$$$\    $$\$$\$$ $$ |  $$ |  $$ $$\$$ |$$$$$\    $$$$$$$  |
$$ |  $$ |$$  __|   $$ \$$$  $$ |  $$ |  $$ \$$$$ |$$  __|   $$  __$$< 
$$ |  $$ |$$ |      $$ |\$  /$$ |  $$ |  $$ |\$$$ |$$ |      $$ |  $$ |
$$$$$$$  |$$$$$$$$\ $$ | \_/ $$ |$$$$$$\ $$ | \$$ |$$$$$$$$\ $$ |  $$ |
\_______/ \________|\__|     \__|\______|\__|  \__|\________|\__|  \__| 
        )";

        // Получаем размер консольного окна
        CONSOLE_SCREEN_BUFFER_INFO console_status;
        int columns, rows;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_status);
        columns = console_status.srWindow.Right - console_status.srWindow.Left + 1;
        rows = console_status.srWindow.Bottom - console_status.srWindow.Top + 1;

        // Вычисляем начальную позицию для центрирования логотипа
        int logo_width = 68; // Примерная ширина логотипа
        int logo_height = 8; // Примерная высота логотипа

        int start_col = (columns - logo_width) / 2;
        int start_row = (rows - logo_height) / 2;

        // Печатаем пустые строки, чтобы переместить логотип в центр по вертикали
        for (int i = 0; i < start_row; ++i) {
            std::cout << std::endl;
        }

        // Печатаем логотип с пробелами перед каждой строкой для горизонтального центрирования
        std::istringstream logo_stream(logo);
        std::string line;
        while (std::getline(logo_stream, line)) {
            std::cout << std::string(start_col, ' ') << line << std::endl;
        }

        Sleep(3000); // Задержка на 3 секунды
        system("cls"); // Очистка экрана
    }

public:
    void run_game() {
        showlogo();
        user us;
        us.run();
    }
};

int main() {
    srand(time(0));
    Game game;
    game.run_game();
    return 0;
}

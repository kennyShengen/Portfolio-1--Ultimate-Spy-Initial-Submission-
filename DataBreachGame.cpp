#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Guard
{
public:
    int row;
    int col;
    char direction;

    Guard(int r, int c, char d)
    {
        row = r;
        col = c;
        direction = d;
    }
};

class Level
{
private:
    vector<string> map;
    vector<Guard> guards;

    int playerRow;
    int playerCol;

    bool won;
    bool lost;

public:
    Level(vector<string> layout)
    {
        map = layout;
        won = false;
        lost = false;

        for (int r = 0; r < map.size(); r++)
        {
            for (int c = 0; c < map[r].size(); c++)
            {
                char ch = map[r][c];

                if (ch == '@')
                {
                    playerRow = r;
                    playerCol = c;
                    map[r][c] = ' ';
                }

                if (ch == '<' || ch == '>' || ch == '^' || ch == 'v')
                {
                    guards.push_back(Guard(r, c, ch));
                    map[r][c] = ' ';
                }
            }
        }
    }

    void display()
    {
        vector<string> displayMap = map;

        displayMap[playerRow][playerCol] = '@';

        for (Guard g : guards)
        {
            displayMap[g.row][g.col] = g.direction;
        }

        cout << endl;

        for (string row : displayMap)
        {
            cout << row << endl;
        }

        cout << endl;
    }

    bool isBlocked(int r, int c)
    {
        if (map[r][c] == '#')
            return true;

        for (Guard g : guards)
        {
            if (g.row == r && g.col == c)
                return true;
        }

        return false;
    }

    void checkGuardVision()
    {
        for (Guard g : guards)
        {
            int dr = 0;
            int dc = 0;

            if (g.direction == '^') dr = -1;
            if (g.direction == 'v') dr = 1;
            if (g.direction == '<') dc = -1;
            if (g.direction == '>') dc = 1;

            int r = g.row + dr;
            int c = g.col + dc;

            while (r >= 0 && r < map.size() &&
                   c >= 0 && c < map[0].size())
            {
                if (map[r][c] == '#')
                    break;

                bool guardFound = false;

                for (Guard other : guards)
                {
                    if (other.row == r && other.col == c)
                    {
                        guardFound = true;
                        break;
                    }
                }

                if (guardFound)
                    break;

                if (playerRow == r && playerCol == c)
                {
                    lost = true;
                    return;
                }

                if (map[r][c] == '$')
                    break;

                r += dr;
                c += dc;
            }
        }
    }

    void moveGuards()
    {
        for (auto &g : guards)
        {
            int dr = 0;
            int dc = 0;

            if (g.direction == '^') dr = -1;
            if (g.direction == 'v') dr = 1;
            if (g.direction == '<') dc = -1;
            if (g.direction == '>') dc = 1;

            int newRow = g.row + dr;
            int newCol = g.col + dc;

            bool blocked = false;

            if (map[newRow][newCol] == '#')
                blocked = true;

            if (newRow == playerRow && newCol == playerCol)
                blocked = true;

            for (Guard other : guards)
            {
                if (&other != &g &&
                    other.row == newRow &&
                    other.col == newCol)
                {
                    blocked = true;
                }
            }

            if (blocked)
            {
                if (g.direction == '^') g.direction = 'v';
                else if (g.direction == 'v') g.direction = '^';
                else if (g.direction == '<') g.direction = '>';
                else if (g.direction == '>') g.direction = '<';

                dr = 0;
                dc = 0;

                if (g.direction == '^') dr = -1;
                if (g.direction == 'v') dr = 1;
                if (g.direction == '<') dc = -1;
                if (g.direction == '>') dc = 1;

                newRow = g.row + dr;
                newCol = g.col + dc;

                if (!isBlocked(newRow, newCol))
                {
                    g.row = newRow;
                    g.col = newCol;
                }
            }
            else
            {
                g.row = newRow;
                g.col = newCol;
            }
        }
    }

    void movePlayer(char input)
    {
        input = toupper(input);

        int dr = 0;
        int dc = 0;

        if (input == 'W') dr = -1;
        else if (input == 'S') dr = 1;
        else if (input == 'A') dc = -1;
        else if (input == 'D') dc = 1;
        else return;

        int newRow = playerRow + dr;
        int newCol = playerCol + dc;

        if (map[newRow][newCol] == '#')
        {
            cout << "Firewall detected. Move blocked.\n";
            return;
        }

        for (Guard g : guards)
        {
            if (g.row == newRow && g.col == newCol)
            {
                lost = true;
                return;
            }
        }

        playerRow = newRow;
        playerCol = newCol;

        if (map[playerRow][playerCol] == '$')
        {
            won = true;
            return;
        }

        moveGuards();

        for (Guard g : guards)
        {
            if (g.row == playerRow && g.col == playerCol)
            {
                lost = true;
                return;
            }
        }

        checkGuardVision();
    }

    bool hasWon()
    {
        return won;
    }

    bool hasLost()
    {
        return lost;
    }
};

string toLowerCase(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void playLevel(vector<string> layout)
{
    Level level(layout);

    while (!level.hasWon() && !level.hasLost())
    {
        level.display();

        cout << "Move (W/A/S/D): ";

        string input;
        getline(cin, input);

        if (!input.empty())
        {
            level.movePlayer(input[0]);
        }
    }

    level.display();

    if (level.hasWon())
    {
        cout << "MISSION COMPLETE!\n";
        cout << "Data successfully extracted.\n\n";
    }
    else
    {
        cout << "ACCESS DENIED!\n";
        cout << "Security drones detected you.\n\n";
    }
}

int main()
{
    vector<string> level1 =
    {
        "############",
        "#      v   #",
        "# #######  #",
        "#          #",
        "# @      $ #",
        "############"
    };

    vector<string> level2 =
    {
        "##############",
        "# <          #",
        "# #######    #",
        "#       ^    #",
        "#            #",
        "# @        $ #",
        "##############"
    };

    while (true)
    {
        cout << "\n================================\n";
        cout << "     CYBERGHOST: DATA BREACH\n";
        cout << "================================\n\n";

        cout << "1) First Breach\n";
        cout << "2) Root Access\n";
        cout << "3) Quit\n\n";

        cout << "Select mission: ";

        string choice;
        getline(cin, choice);

        choice = toLowerCase(choice);

        if (choice == "1" || choice == "first breach")
        {
            playLevel(level1);
        }
        else if (choice == "2" || choice == "root access")
        {
            playLevel(level2);
        }
        else if (choice == "3" || choice == "quit")
        {
            break;
        }
        else
        {
            cout << "Invalid selection.\n";
        }
    }

    return 0;
}
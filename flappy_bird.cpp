// TODO: clean this up later

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>
#include <fstream>

int main()
{
    HANDLE consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE consoleOutpuHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (consoleInputHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "error" << std::endl;
        return 1;
    }
    if (consoleOutpuHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "error" << std::endl;
        return 1;
    }

    DWORD mode = 0;
    DWORD mode2 = 0;
    DWORD mode3 = 0;
    if (!GetConsoleMode(consoleInputHandle, &mode))
    {
        std::cerr << "error" << std::endl;
        return 1;
    }
    mode2 = mode;
    mode2 &= ~ENABLE_LINE_INPUT;
    mode2 &= ~ENABLE_ECHO_INPUT;
    if (!SetConsoleMode(consoleInputHandle, mode2))
    {
        std::cerr << "error" << std::endl;
        return 1;
    }
    if (GetConsoleMode(consoleOutpuHandle, &mode3)) SetConsoleMode(consoleOutpuHandle,
                                                                   mode3 | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // bird
    float birdYPosition = 9.0f;
    float birdVelocity = 0.0f;
    int birdTop = 0;
    int birdBottom = 0;
    int birdLeft = 10;
    int birdRight = 10 + 2 - 1;
    int dead = 0;
    float time = 0.0f;
    unsigned long long score = 0;
    unsigned long long bestScore = 0;
    // hud padding
    int leftPadding = 0;
    int rightPadding = 0;
    // pipe data
    std::vector<float> pipeXPositions;
    std::vector<int> pipeGaps;
    std::vector<int> pipeScored;

    // rng
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> gapPosition(2, 20 - 6 - 2);

    INPUT_RECORD record;
    DWORD ne = 0;

    // load best score
    std::ifstream fin("best-score.txt");
    if (!fin) { bestScore = 0; }
    if (fin) { fin >> bestScore; }
    fin.close();

    auto prev = std::chrono::steady_clock::now();

    // main loop
    while (dead == 0)
    {
        // delta time
        auto now = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(now - prev).count();
        prev = now;
        if (dt > 0.1f) dt = 0.1f; // clamp delta time

        // read input events
        DWORD nEvents = 0;
        if (!GetNumberOfConsoleInputEvents(consoleInputHandle, &nEvents))
        {
            SetConsoleMode(consoleInputHandle, mode);
            return 1;
        }

        for (DWORD i = 0; i < nEvents; ++i) // loop through events
        {
            if (!ReadConsoleInput(consoleInputHandle, &record, 1, &ne))
            {
                std::cerr << "Failed to read console input." << std::endl;
                SetConsoleMode(consoleInputHandle, mode);
                return 1;
            } // end if ReadConsoleInput

            if (record.EventType == KEY_EVENT)
            {
                KEY_EVENT_RECORD k = record.Event.KeyEvent;
                if (k.bKeyDown == TRUE)
                {
                    if (k.wVirtualKeyCode == VK_RETURN)
                    {
                        birdVelocity = -14.0f;
                    } // end if enter
                } // end if key down
            } // end if key event
        } // end for each event

        birdVelocity = birdVelocity + 42.0f * dt;
        birdYPosition = birdYPosition + birdVelocity * dt;

        time = time + dt;
        if (time >= 1.4f)
        {
            time = time - 1.4f;
            pipeXPositions.push_back(50.0f);
            pipeGaps.push_back(gapPosition(rng));
            pipeScored.push_back(0);
        }

        for (int i = 0; i < (int)pipeXPositions.size(); i++) // loop over all pipes
        {
            pipeXPositions[i] = pipeXPositions[i] - 18.0f * dt; // move pipe left

            int pipeRight = (int)std::floor(pipeXPositions[i]) + 6 - 1;
            if (pipeScored[i] == 0 && pipeRight < 10)
            {
                pipeScored[i] = 1;
                score = score + 1;
                if (score > bestScore) bestScore = score;
            }
        }

        for (int i = (int)pipeXPositions.size() - 1; i >= 0; i--)
        {
            if (pipeXPositions[i] + 6.0f < 0.0f)
            {
                pipeXPositions.erase(pipeXPositions.begin() + i);
                pipeGaps.erase(pipeGaps.begin() + i);
                pipeScored.erase(pipeScored.begin() + i);
            }
        }

        // collision
        birdTop = (int)std::floor(birdYPosition);
        birdBottom = birdTop + 2 - 1;
        birdLeft = 10; // same every frame
        birdRight = 10 + 2 - 1; // same every frame
        // check wall 
        if (birdTop < 0 || birdBottom >= 20) { dead = 1; }

        if (!dead)
        {
            for (int i = 0; i < (int)pipeXPositions.size(); i++)
            {
                int pl = (int)std::floor(pipeXPositions[i]);
                int pr = pl + 6 - 1;

                if (birdRight >= pl && birdLeft <= pr)
                {
                    for (int y = birdTop; y <= birdBottom; y++)
                    {
                        if (y < pipeGaps[i] || y >= pipeGaps[i] + 6)
                        {
                            dead = 1;
                            break;
                        }
                    }
                }

                if (dead != 0) break;
            }
        }

        if (dead != 0) break;

        std::vector<std::string> frame(20, std::string(50, ' '));

        for (int i = 0; i < (int)pipeXPositions.size(); i++)
        {
            int pl = (int)std::floor(pipeXPositions[i]);
            for (int dx = 0; dx < 6; dx++)
            {
                int x = pl + dx;
                if (x < 0 || x >= 50) continue;
                for (int y = 0; y < 20; y++)
                {
                    if (!(y >= pipeGaps[i] && y < pipeGaps[i] + 6))
                    {
                        frame[y][x] = 'P';
                    }
                }
            }
        }

        for (int dy = 0; dy < 2; dy++)
        {
            int y = birdTop + dy;
            if (y < 0 || y >= 20) continue;
            for (int dx = 0; dx < 2; dx++)
            {
                int x = 10 + dx;
                if (x >= 0 && x < 50) frame[y][x] = 'B';
            }
        }

        std::string scoreText = "Score: " + std::to_string(score) + "   Best: " + std::to_string(bestScore);
        if (scoreText.size() > 50) scoreText = scoreText.substr(0, 50);
        leftPadding = (int)((50 - (int)scoreText.size()) / 2);
        rightPadding = 50 - leftPadding - (int)scoreText.size();

        std::cout << "\x1b[2J\x1b[H";
        std::cout << "+" << std::string(50, '-') << "+" << "\n";
        for (int y = 0; y < 20; y++)
        {
            std::cout << "|";
            for (int x = 0; x < 50; x++)
            {
                char c = frame[y][x];
                if (c == 'P')
                {
                    std::cout << "\x1b[32mP\x1b[0m";
                }
                else if (c == 'B')
                {
                    std::cout << "\x1b[33mB\x1b[0m";
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << "|\n";
        }
        std::cout << "+" << std::string(50, '-') << "+" << "\n";
        std::cout << "+" << std::string(50, '-') << "+" << "\n";
        std::cout << "|" << std::string(leftPadding, ' ') << scoreText << std::string(rightPadding, ' ') << "|\n";
        std::cout << "+" << std::string(50, '-') << "+" << "\n";
        std::cout.flush();

        float ft = std::chrono::duration<float>(std::chrono::steady_clock::now() - now).count();
        if (ft < 1.0f / 30.0f)
        {
            std::this_thread::sleep_for(std::chrono::duration<float>(1.0f / 30.0f - ft));
        }
    }

    // save best score to file
    std::ofstream fout("best-score.txt", std::ios::trunc);
    if (fout) fout << bestScore; // write best score
    fout.close(); // close the file

    // restore original console mode
    SetConsoleMode(consoleInputHandle, mode);
    return 0;
}

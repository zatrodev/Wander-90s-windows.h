#pragma once
#include <conio.h>
#include "welcome.h"

#define EASY 10
#define NORMAL 5
#define HARD 0

struct Logic
{
    int mode;

    bool checkWin = true;
    bool distanceScore = true;
    bool shoot = false;
    bool gameOver = false;
    bool enemyShoot = false;

    void controller(Ship &ship, int &bulletCount)
    {
        if (wanderMode)
        {
            static bool start = false;
            ship.shipSpeed = 0.5;
            static bool isUp = true;

            if (kbhit())
            {
                switch (getch())
                {
                case 32:
                    start = true;

                    isUp = isUp ? false : true;
                    break;
                case 'p':
                    if (bulletCount != 0 && !shoot)
                    {
                        shoot = true;
                        --bulletCount;
                    }

                    break;
                }
            }

            if (!isUp && start)
            {
                ship._delete();
                ship.shipY += ship.shipSpeed;
                ship.draw();
            }

            else if (isUp && start)
            {
                ship._delete();
                ship.shipY -= ship.shipSpeed;
                ship.draw();
            }
        }

        else
        {
            if (kbhit())
            {
                switch (getch())
                {
                case 'w':
                    ship._delete();
                    ship.shipY -= ship.shipSpeed;
                    ship.draw();
                    break;
                case 'a':
                    ship._delete();
                    ship.shipX -= ship.shipSpeed + 4;
                    ship.draw();
                    break;
                case 's':
                    ship._delete();
                    ship.shipY += ship.shipSpeed;
                    ship.draw();
                    break;
                case 'd':
                    ship._delete();
                    ship.shipX += ship.shipSpeed + 4;
                    ship.draw();
                    break;
                case 'p':
                    if (bulletCount != 0 && !shoot)
                    {
                        shoot = true;
                        --bulletCount;
                    }
                }
            }
        }
    }

    void check_collision(int rows, Ship &ship, vector<Pipe> &pipes, vector<Bullet> &enemyBullets)
    {
        for (vector<float> ship_coord : ship.ship_coords)
        {
            for (int i = 0; i < pipes.size(); ++i)
            {
                for (vector<int> pipe_coord : pipes[i].pipe_coords)
                {
                    if ((ship_coord[0] == pipe_coord[0] && ship_coord[1] == pipe_coord[1]) || (ship_coord[0] == 0 || ship_coord[1] == 1 || ship_coord[1] == rows - 1))
                        gameOver = true;
                }

                if (pipes[0].pipeX < PIPE_THRESHOLD)
                {
                    pipes[0]._delete();
                    pipes.erase(pipes.begin());
                }
            }

            for (int i = 0; i < enemyBullets.size(); ++i)
            {
                if (ship_coord[0] == enemyBullets[i].bulletX && ship_coord[1] == enemyBullets[i].bulletY)
                    gameOver = true;

                if (enemyBullets[0].bulletX < BULLET_THRESHOLD)
                {
                    enemyBullets[0]._delete();
                    enemyBullets.erase(enemyBullets.begin());
                }
            }
        }
    }

    void check_bullet_collision(vector<Bullet> &bullets, vector<Pipe> &pipes, vector<Ship> &enemyShip, vector<Bullet> &enemyBullets, int columns)
    {
        for (int i = 0; i < pipes.size(); ++i)
        {
            for (vector<int> pipe_coord : pipes[i].pipe_coords)
            {
                if ((bullets[0].bulletX == pipe_coord[0] || bullets[0].bulletX + 1 == pipe_coord[0] || bullets[0].bulletX + 2 == pipe_coord[0]) && bullets[0].bulletY == pipe_coord[1])
                {
                    pipes[i]._delete();
                    bullets[0]._delete();
                    pipes.erase(pipes.begin() + i);
                    bullets.clear();
                    shoot = false;
                }

                if (bullets[0].bulletX + 2 > columns - 4)
                {
                    bullets[0]._delete();
                    bullets.clear();
                    shoot = false;
                }
            }
        }

        if (enemyShip.size() != 0)
        {
            if ((bullets[0].bulletX == round(enemyShip[0].shipX) || bullets[0].bulletX + 1 == round(enemyShip[0].shipX) || bullets[0].bulletX + 2 == round(enemyShip[0].shipX)) && (bullets[0].bulletY == round(enemyShip[0].shipY) || bullets[0].bulletY == round(enemyShip[0].shipY + 1)))
            {
                bullets[0]._delete();
                bullets.clear();
                enemyShip[0]._delete();
                enemyShip.erase(enemyShip.begin());
                shoot = false;
            }

            for (int i = 0; i < enemyBullets.size(); ++i)
            {
                if ((bullets[0].bulletX == round(enemyBullets[i].bulletX)|| bullets[0].bulletX + 1 == round(enemyBullets[i].bulletX) || bullets[0].bulletX + 2 == round(enemyBullets[i].bulletX)) && (bullets[0].bulletY == round(enemyBullets[i].bulletY) || bullets[0].bulletY == round(enemyBullets[i].bulletY + 1)))
                {
                    bullets[0]._delete();
                    enemyBullets[i]._delete();
                    bullets.clear();
                    enemyBullets.erase(enemyBullets.begin() + i);
                    shoot = false;
                }
            }
        }
    }

    void check_win(Ship ship, int columns, int rows)
    {
        for (vector<float> ship_coord : ship.ship_coords)
        {
            for (int i = 0; i < rows; ++i)
            {
                if (ship_coord[0] == columns && ship_coord[1] == i)
                {
                    gameOver = true;
                }
            }
        }
    }

    void set_mode()
    {
        if (easyMode)
            mode = EASY;
        else if (normalMode)
            mode = NORMAL;
        else if (hardMode)
            mode = HARD;
        else if (wanderMode)
        {
            mode = EASY + 10;
            checkWin = false;
            distanceScore = false;
        }
    }
};

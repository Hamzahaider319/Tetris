#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>
using namespace std;

int const startX = 690, endX = 1230, startY = 72, length = 54, centre = 960 - (54), vel = 54;

class Piece {
protected:
    int arr[4][4], id;
    sf::Sprite sprite; // Sprite for rendering each block
    sf::Texture* texture; // Reference to texture


public:
    Piece(sf::Texture* tex) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                arr[i][j] = 0;
            }
        }
        texture = tex;
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(0, 0, 54, 54));
        id = -1;
    }


    void draw(sf::RenderWindow& window, int x, int y) {
        sprite.setPosition(x, y);
        window.draw(sprite);
        std::cout << "Drawing at: " << x << ", " << y << std::endl;


    }
    const int* getArr(int i) {
        return arr[i];
    }
    Piece* generateRandomShape(sf::Texture* tex, int& num);
    int getshapeId() {
        return id;
    }
    virtual ~Piece() = default;
 
};
//the 7 shapes in tetis, each inherits from the base piece class:
class Ishape :public Piece {
public:
    Ishape(sf::Texture* tex) :Piece(tex) {
        for (int i = 0; i < 4; i++) {
            arr[i][1] = 1;
        }
        id = 0;
    }
};
class Zshape :public Piece {
private:
    int c;
public:
    Zshape(sf::Texture* tex) :Piece(tex) {
        c = 2;
        for (int i = 0; i < 3; i++) {
            arr[i][c] = 1;
            if (i == 1) {
                c--;
                arr[i][c] = 1;
            }
        }
        id = 1;
    }
};
class Jshape :public Piece {
private:
    int c;
public:
    Jshape(sf::Texture* tex) :Piece(tex) {
        c = 0;
        for (int i = 0; i < 2; i++) {
            arr[i][c] = 1;
        }
        for (int i = 1; i < 3; i++) {
            arr[1][i] = 1;
        }
        id = 2;
    }
};

class Lshape :public Piece {
private:
    int c;
public:
    Lshape(sf::Texture* tex) :Piece(tex) {
        c = 2;
        for (int i = 0; i < 2; i++) {
            arr[i][c] = 1;
        }
        c = 1;
        for (int i = 0; i < 2; i++) {
            arr[c][i] = 1;
        }
        id = 3;
    }
};

class Oshape :public Piece {
private:

public:
    Oshape(sf::Texture* tex) :Piece(tex) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                arr[i][j] = 1;
            }
        }
        id = 4;
    }
};

class Sshape :public Piece {
private:
    int c;
public:
    Sshape(sf::Texture* tex) :Piece(tex) {
        c = 0;
        for (int i = 0; i < 2; i++) {
            arr[i][c] = 1;
        }
        c = 1;
        for (int i = 1; i < 3; i++) {
            arr[i][c] = 1;
        }
        id = 5;
    }
};

class Tshape :public Piece {
private:
    int c;
public:
    Tshape(sf::Texture* tex) :Piece(tex) {
        arr[0][1] = 1;
        c = 1;
        for (int i = 0; i < 3; i++) {
            arr[c][i] = 1;
        }
        id = 6;
    }
};


class Current {//class responsible for the current shape being used
protected:
    int x[4], y[4], bx[4], by[4];//x and y array store coordinate of each block in the shape, bx and by store the coordinates before a move so that they may be reverted if needed
    Piece* CurrentShape;
public:
    Current(Piece& shape) {
        CurrentShape = &shape;
        int c = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (*(CurrentShape->getArr(i) + j) == 1) {//initializing the spawn coordinates of each block
                    x[c] = centre + j * 54;
                    y[c] = 54 * i;
                    bx[c] = x[c];
                    by[c] = y[c];
                    c++;
                }
            }
        }
    }
    int* getcurX() {
        return x;
    }
    int* getcurY() {
        return y;
    }
    int* getcurBackupX() {
        return bx;
    }
    int* getcurBackupY() {
        return by;
    }
    void render(sf::RenderWindow& window) {//for rendering the current shape
        for (int i = 0; i < 4; i++) {
            CurrentShape->draw(window, x[i], y[i]); // Draw each block
        }
    }
    void renderNext(sf::RenderWindow& window) {//for rendering the upcoming shape
        for (int i = 0; i < 4; i++) {
            // Draw each block
            CurrentShape->draw(window, x[i] + 550, y[i] + 500);
        }
    }
    void assigncurr(Piece& newShape) {//
        CurrentShape = &newShape;
    }

    int getId() {
        return CurrentShape->getshapeId();
    }
};

class Board {//Board class that controls the game and executes all the movements
private:
    int field[20][10];
    Current* CurrObj;
    sf::Sprite sprite; // Sprite for rendering each block
    sf::Texture* texture; // Reference to texture
public:
    Board(sf::Texture* tex) {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                field[i][j] = 0;
            }
        }
        texture = tex;
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(0, 0, 54, 54));
    }
    void assignCurrentObject(Current& obj) {
        // delete CurrObj;
        CurrObj = &obj;
    }
    void storeBackup() {//function executed after a successful move to copy the current coodinates into the backup coordinates
        for (int i = 0; i < 4; i++) {
            *(CurrObj->getcurBackupX() + i) = *(CurrObj->getcurX() + i);
            *(CurrObj->getcurBackupY() + i) = *(CurrObj->getcurY() + i);
        }
    }
    bool check() {//function that checks whether coordinates after a move are valid and reverts the coordinate to their backup coordinates if found invalid
        for (int i = 0; i < 4; i++) {
            if (*(CurrObj->getcurX() + i) < startX || *(CurrObj->getcurX() + i) >= endX || *(CurrObj->getcurY() + i) >= 1080 || field[*(CurrObj->getcurY() + i) / 54][(*(CurrObj->getcurX() + i) - startX) / 54]) {
                for (int i = 0; i < 4; i++) {
                    *(CurrObj->getcurX() + i) = *(CurrObj->getcurBackupX() + i);
                    *(CurrObj->getcurY() + i) = *(CurrObj->getcurBackupY() + i);
                }
                return false;
            }
        }
        storeBackup();
        return true;
    }
    void moveDown() {
        for (int i = 0; i < 4; i++) {
            *(CurrObj->getcurY() + i) += 54; // Move all blocks downward
        }
        check();
    }
    void moveRight() {
        for (int i = 0; i < 4; i++) {
            *(CurrObj->getcurX() + i) += 54;
        }
        check();
    }
    void moveLeft() {
        for (int i = 0; i < 4; i++) {
            *(CurrObj->getcurX() + i) -= 54;
        }
        check();
    }
    bool checkEnd() {//function that checks if the current shape has reached the bottom or a block
        for (int i = 0; i < 4; i++) {
            if (field[(*(CurrObj->getcurY() + i) / 54) + 1][(*(CurrObj->getcurX() + i) - startX) / 54] || *(CurrObj->getcurY() + i) >= 1080 - 54) {
                for (int i = 0; i < 4; i++) {
                    field[(*(CurrObj->getcurY() + i) / 54)][(*(CurrObj->getcurX() + i) - startX) / 54] = 1;//changes the field to 1 at that point
                }
                return true;
                cout << "ended\n";
            }
        }
        return false;
    }

    void drawField(sf::RenderWindow& window) {//function responsible for drawing the blocks where the field is 1
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                if (field[i][j]) {
                    sprite.setPosition(startX + j * 54, i * 54);
                    window.draw(sprite);
                }
            }
        }
    }
    void rotate() {
        int xOrig = CurrObj->getcurX()[2];  //setting origins/pivot of each shape at (2,2) 
        int yOrig = CurrObj->getcurY()[2];
        for (int i = 0; i < 4; i++) {//formula for 90 degree counterclockwise rotation around a pivot point
            int oldX = CurrObj->getcurX()[i];
            int oldY = CurrObj->getcurY()[i];

            CurrObj->getcurX()[i] = xOrig + (oldY - yOrig);
            CurrObj->getcurY()[i] = yOrig - (oldX - xOrig);
        }

        check();
        sf::sleep(sf::milliseconds(200));
    }
    void clearRows(int& scor) {//function responsible for clearing the rows when they are completely filled
        for (int row = 19; row >= 0; row--) { //Start checking from the bottom row
            bool fullRow = true;

            for (int col = 0; col < 10; col++) {
                if (field[row][col] == 0) {
                    fullRow = false; // If any block is empty, row is not full
                    break;
                }
            }

            if (fullRow) {
                scor++;//increase score if a full row is found
                // Clear the row by shifting everything down
                for (int i = row; i > 0; i--) {
                    for (int j = 0; j < 10; j++) {
                        field[i][j] = field[i - 1][j];
                    }
                    sf::sleep(sf::milliseconds(50));
                }

                // Set top row to empty after shifting everything down
                for (int j = 0; j < 10; j++) {
                    field[0][j] = 0;
                }

                row++; // Recheck the same row after clearing since rows shift down
            }
        }
    }

    bool GameEnd() {//function that checks whether the game has ended
        for (int i = 0; i < 4; i++) {
            if (CurrObj->getcurY()[i] <= 0) {
                cout << "Game Ended\n";
                return true;//return true if a block has reached the top of the screen
            }
        }
        return false;
    }
    void Restart() {//function that restarts the game once it has ended
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                field[i][j] = 0;//set field to zero again
            }
        }
    }
};

Piece* Piece::generateRandomShape(sf::Texture* tex, int& num) {//function that generates a random shape
    switch (num) {
    case 0: return new Ishape(tex);
    case 1: return new Zshape(tex);
    case 2: return new Jshape(tex);
    case 3: return new Lshape(tex);
    case 4: return new Oshape(tex);
    case 5: return new Sshape(tex);
    case 6: return new Tshape(tex);
    default: return new Ishape(tex);
    }
}

int main()
{
    const int ScreenWidth = 1920, ScreenHeight = 1080; //play size:540x1080 with tile size 54x54;

    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "kuch bikhre huwe blocks");
    sf::Font font;
    if (!font.loadFromFile("Scream Again.ttf"))//!
    {
        cout << "Font not loaded\n";
    }
    int upcoming = 0, Curscore = 0;
    sf::Text text;
    text.setFont(font);
    text.setString("Game Ended\n Press 'R' to Restart\n");
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::Red);
    text.setPosition({ 490,340 });
    sf::Text score;
    score.setFont(font);
    score.setString(std::to_string(Curscore));
    score.setCharacterSize(60);
    score.setFillColor(sf::Color::Blue);
    score.setPosition({ 372 + 100,120 });
    sf::Text sc = score;
    sc.setString("Score:");
    sc.setPosition({ 10 + 100,120 });
    score.setCharacterSize(200);
    score.setPosition({ 150,300 });
    sf::Text Nextshape = text;
    Nextshape.setString("Upcoming Shape:");
    Nextshape.setPosition({ 960 + 300, 300 });
    Nextshape.setCharacterSize(30);




    srand(time(0));
    sf::Texture blockTexture;
    if (!blockTexture.loadFromFile("imgTetris.png")) {//!
        std::cerr << "Error loading texture!" << std::endl;
    }
    int randShape = 0;
    randShape = rand() % 7;
    Piece* shape = nullptr;
    Current* Curr;
    shape = shape->generateRandomShape(&blockTexture, randShape);
    randShape = rand() % 7;//randshape tells what will be the next shape;
    Piece* nextShape = nullptr;
    nextShape = nextShape->generateRandomShape(&blockTexture, randShape);
    Current* nextCurr;
    nextCurr = new Current(*nextShape);
    nextCurr->renderNext(window);
    Curr = new Current(*shape);
    Board board(&blockTexture);
    board.assignCurrentObject(*Curr);
    sf::RectangleShape rectLeft({ 1,1080 });
    rectLeft.setPosition(startX, 0);
    sf::RectangleShape rectRight({ 1,1080 });
    rectRight.setPosition(endX, 0);
    sf::Clock clock;
    bool ended = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::D) {//move Right
                    board.moveRight();
                    cout << "move right\n";
                }
                else if (event.key.code == sf::Keyboard::A) {//move Left
                    board.moveLeft();
                    cout << "moving left\n";
                }
                else if (event.key.code == sf::Keyboard::W && shape->getshapeId() != 4) {//rotate
                    board.rotate();
                    cout << "Rotate\n";
                }
                else if (event.key.code == sf::Keyboard::Space) {//instantly move the shape down
                    for (int i = 0; i < 20; i++) {
                        board.moveDown();
                    }
                }
                else if (event.key.code == sf::Keyboard::S) {//makes the shape move down one block(use to move faster)
                    board.moveDown();
                    clock.restart();
                }
            }

        }

        if (board.checkEnd()) {//if a shape reaches the end:
            board.clearRows(Curscore);
            score.setString(std::to_string(Curscore));
            if (board.GameEnd()) {//if a block reaches the top
                ended = true;
                window.clear();
                window.draw(score);
                window.draw(sc);
                window.draw(text);
                window.display();
                if (event.key.code == sf::Keyboard::R) {//press R to restart
                    cout << "Restarting\n";
                    board.Restart();
                    ended = false;
                    Curscore = 0;
                    score.setString(std::to_string(Curscore));
                }

            }
            if (!ended) {//deletes previous shape and makes new one
                delete shape;
                delete Curr;
                shape = nullptr;
                shape = shape->generateRandomShape(&blockTexture, randShape);//new shape generated
                randShape = rand() % 7;
                cout << randShape << endl;
                delete nextShape;
                delete nextCurr;
                nextShape = nullptr;
                nextShape = nextShape->generateRandomShape(&blockTexture, randShape);//next/upcoming shape generated
                nextCurr = new Current(*nextShape);
                nextCurr->renderNext(window);//draw the next shape
                Curr = new Current(*shape);
                board.assignCurrentObject(*Curr);//Assign the new current shape
                cout << "ended\n";
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 1000) {//move the pieces down after a certain time
            board.moveDown();
            clock.restart();
        }
        if (!ended) {//drawing and displaying:
            window.clear();
            board.drawField(window);
            Curr->render(window);

            window.draw(rectLeft);
            window.draw(rectRight);
            window.draw(score);
            window.draw(sc);
            window.draw(Nextshape);
            nextCurr->renderNext(window);
            window.display();
            sf::sleep(sf::milliseconds(200));
        }
    }

    return 0;
}
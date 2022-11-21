#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include "shape.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;
//https://github.com/mikepoirier/clion-glfw
GLdouble width, height;
int wd;
const int SIDE_LENGTH = 20;
const color white(1, 1, 1);
const color red(1, 0, 0);
color green(0, 1, 0);
const color black(0, 0, 0);
const color cyan (0, 1, 1);


Rect user;
vector<Circle> snow;
vector<Rect> maze;
enum screens{first = 1, middle = 2, lastWin = 3, lastLost =4};
screens screen = first;
string startScreen = "Starting Screen: please press s to START the Mountain Maze Game!";
string endScreenLose = "You lost!";
string endScreenWin = "You Won!";
string endScreenWrong = "Something went wrong!";

void initUser() {
    // centered in the top left corner of the graphics window
    user.setCenter(390, 250);
    user.setSize(10, 10);
    user.setColor(cyan);
}

void initSnow() {
    for (int i = 0; i < 150; ++i) {
        snow.push_back(Circle(1, 1, 1, 1, rand() % int(width), -(rand() % int(height)), rand() % 5 + 1));
    }
}

void initMaze() {
    string filename = "../maze2.txt";
    ifstream inFile(filename);
    inFile >> noskipws;
        int xCoord = 0, yCoord = 0;
        char letter;
        color blockColor;
        bool draw;
        while (inFile >> letter) {
            draw = true;
            switch (letter) {
                case 'b':
                    blockColor = black;
                    break;
                case 'r':
                    blockColor = red;
                    break;
                case 'g':
                    blockColor = green;
                    break;
                case 'w':
                    blockColor = white;
                    break;
                case ' ':
                    draw = false;
                    xCoord += SIDE_LENGTH;
                    yCoord += 0;
                    break;

                default: // newline
                    draw = false;
                    xCoord = 0;
                    yCoord += SIDE_LENGTH;
            }
            if (draw) {
                //create rectangle object
                //stores x and y cord
                maze.push_back(Rect(blockColor,xCoord, yCoord));
                xCoord += SIDE_LENGTH;
            }

        }
        inFile.close();
    }

    void determineWin() {

        ////TODO: ISOVERLAPPING
        //GET FROM RUNNER determine if user rectangle is not overlapping with the maze
        //detect if user


        for (int i = 0; i < maze.size(); ++i) {
            if ((maze[i].isOverlapping(user))) {
                screen = lastLost;
            }
            if(user.getTopY()== 705 && user.getLeftX()== 85){
                screen = lastWin;
            }
        }
    }





void mazeDraw() {
    for (int i = 0; i < maze.size(); ++i) {
        int xCoord = maze[i].getLeftX();
        int yCoord = maze[i].getTopY();
        //color drawColor = maze[i].getColor();
        //replay with rectangle draw
        glBegin(GL_QUADS);
        glColor3f(maze[i].getColor().red, maze[i].getColor().green, maze[i].getColor().blue);
        glVertex2i(xCoord, yCoord);
        glVertex2i(xCoord + SIDE_LENGTH, yCoord);
        glVertex2i(xCoord + SIDE_LENGTH, yCoord + SIDE_LENGTH);
        glVertex2i(xCoord, yCoord + SIDE_LENGTH);
        glEnd();

    }

}
void init() {
    width = 800;
    height = 800;
    srand(time(0));
    initSnow();
    initMaze();
    initUser();

}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(cyan.red, cyan.green, cyan.blue, 1.0f);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)
    
    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE
    
    /*
     * Draw here
     */

    //Screen switch
    switch(screen) {
        case (first):
            glColor3f(1, 1, 1);
            glRasterPos2i((width / 4) - 50, (height / 2));
            for (const char &letter: startScreen) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

            glFlush();
            break;

        case (middle):
            // Add logic to draw the clouds, mountain, the user, the maze.
            for (Circle &flake: snow) {
                flake.draw();
            }

            //draw mountain
            glBegin(GL_TRIANGLES);
            glClearColor(white.red, white.green, white.blue, 1.0f);
            glVertex2i(0, 800);
            glVertex2i(400, 200);
            glVertex2i(800, 800);
            glEnd();


            mazeDraw();
            user.draw();
            determineWin();

            glFlush();

                 // Render now
            break;

            //maybe lastWin and lastLose
        case (lastWin):
            glClear(GL_COLOR_BUFFER_BIT);
            glColor3f(1, 1, 1);
            glRasterPos2i((width / 3), (height / 2));

            for (const char &message: endScreenWin) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, message);
            }
            glFlush();

            break;

        case (lastLost):
            glClear(GL_COLOR_BUFFER_BIT);
            glColor3f(1, 1, 1);
            glRasterPos2i((width / 3), (height / 2));
            for (const char &message: endScreenLose) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, message);
            }
            glFlush();
            break;
    }
}



// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    if (key == 115) {
        screen = middle;
    }
    if (key == 117) {
        screen = lastLost;
    }
    
    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {

        case GLUT_KEY_DOWN:
            user.move(0, 5);
            break;
        case GLUT_KEY_LEFT:
            user.move(-5, 0);
            break;
        case GLUT_KEY_RIGHT:
            user.move(5, 0);
            break;
        case GLUT_KEY_UP:
            user.move(0, -5);
            break;
    }
    
    glutPostRedisplay();
}

void cursor(int x, int y) {
    // TODO: Set the user's center point to be the coordinates
    // passed in as parameters to this function. This will make
    // the user block move with the mouse.
    //user.setCenter(x,y);
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    glutPostRedisplay();
}
void timerSnow(int dummy) {
    for (Circle &flake : snow) {
        flake.moveY(flake.getRadius());
        if (flake.getTopY() > height) {
            flake.setCenter(rand() % int(width), -flake.getRadius());
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timerSnow, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Mountain Maze!");
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    
    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);


    // handles mouse movement
    glutPassiveMotionFunc(cursor);
    
    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timerSnow, 0);

    // Enter the event-processing loop
    glutMainLoop();




    return 0;
}

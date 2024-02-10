#include <GL/glut.h>
#include <math.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
    for(int i=0; i<360; i++) {
        float rad = i * M_PI / 180.0;
        glVertex2f(cos(rad), sin(rad));
    }
    glEnd();

    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL Circle");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
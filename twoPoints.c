#include <stdlib.h>
 //#include "SimpleNurbs.h"
#include <math.h>

#include <limits.h>

#include <GL/glut.h>

#include <GL/gl.h>

#include <GL/glu.h>

// Global variable
GLfloat angle = 0.0f; // Current rotational angle of the shapes

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

/* Called back when there is no other event to be handled */
void idle() {
    glutPostRedisplay(); // Post a re-paint request to activate display()
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(0.4f, 1.0f, 0.0f);
    glVertex3f(0.4f, -1.0f, 0.0f);

    glEnd();

    glPointSize(10);
    glBegin(GL_POINTS);

    glColor3f(1, 0, 0);
    //glVertex3f(0.1f, 0.0f,0.0f); 
    //gl_PointSize = gl_Normal.x;
    // gl_Position = ftransform();

    glVertex3f(-0.2f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(-0.8f, 0.0f, 0.0f);
    glVertex3f(-1.1f, 0.0f, 0.0f);
    glVertex3f(-1.4f, 0.0f, 0.0f);
    glColor3f(0, 1, 0);
    //glVertex3f(0.1f, 0.2f,0.0f); 
    glVertex3f(-0.2f, 0.2f, 0.0f);
    glVertex3f(-0.5f, 0.2f, 0.0f);
    glVertex3f(-0.8f, 0.2f, 0.0f);
    glVertex3f(-1.1f, 0.2f, 0.0f);
    glVertex3f(-1.4f, 0.2f, 0.0f);
    //delay(10);
    //cleardivaice();

    glEnd();

    // Clear the color buffer
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity(); // Reset the model-view matrix

    glPushMatrix(); // Save model-view matrix setting
    glTranslatef(-0.1f, 0.0f, 0.0f); // Translate
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_POINTS);

    glColor3f(1, 0, 0);
    glVertex3f(0.9f, 0.0f, 0.0f);

    glEnd();
    glPopMatrix(); // Restore the model-view matrix

    glPushMatrix();

    glTranslatef(-0.1f, 0.0f, 0.0f); // Translate
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_POINTS);

    glColor3f(0, 1, 0);
    glVertex3f(1.0f, 0.2f, 0.0f);

    glEnd();
    glPopMatrix(); // Restore the model-view matrix

    glPushMatrix();

    /// new point /////////////////

    // Save model-view matrix setting

    glutSwapBuffers(); // Double buffered - swap the front and back buffers

    // Change the rotational angle after each display()
    angle += 0.2f;
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) { // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat) width / (GLfloat) height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char ** argv) {
    glutInit( & argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(640, 480); // Set the window's initial width & height - non-square
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("Animation via Idle Function"); // Create window with the given title
    glutDisplayFunc(display); // Register callback handler for window re-paint event
    glutReshapeFunc(reshape); // Register callback handler for window re-size event
    glutIdleFunc(idle); // Register callback handler if no other event
    initGL(); // Our own OpenGL initialization
    glutMainLoop(); // Enter the infinite event-processing loop
    return 0;
}



/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * Andrew Miller <amiller@dappervision.com>
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

#include "libfreenect.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <pthread.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <stdlib.h>
#include <math.h>

freenect_context* f_ctx;
freenect_device* f_dev;

int window;
GLuint gl_rgb_tex;
int mx=-1,my=-1;        // Prevous mouse coordinates
int rotangles[2] = {0}; // Panning angles
float zoom = 1;         // zoom factor
int color = 1;          // Use the RGB texture or just draw it as color


pthread_t freenect_thread;
volatile int die = 0;
pthread_mutex_t gl_backbuf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;
int got_rgb = 0;
int got_depth = 0;

// back: owned by libfreenect (implicit for depth)
// mid: owned by callbacks, "latest frame ready"
// front: owned by GL, "currently being drawn"
uint16_t *depth_mid, *depth_front;
uint8_t *rgb_back, *rgb_mid, *rgb_front;


// Do the projection from u,v,depth to X,Y,Z directly in an opengl matrix
// These numbers come from a combination of the ros kinect_node wiki, and
// nicolas burrus' posts.
void LoadVertexMatrix()
{
    const float fx = 594.21f;
    const float fy = 591.04f;
    const float a = -0.0030711f;
    const float b = 3.3309495f;
    const float cx = 339.5f;
    const float cy = 242.7f;
    GLfloat mat[16] = {
        1/fx,     0,  0, 0,
        0,    -1/fy,  0, 0,
        0,       0,  0, a,
        -cx/fx, cy/fy, -1, b
    };
    glMultMatrixf(mat);
}


// This matrix comes from a combination of nicolas burrus's calibration post
// and some python code I haven't documented yet.
void LoadRGBMatrix()
{
    float mat[16] = {
        5.34866271e+02,   3.89654806e+00,   0.00000000e+00,   1.74704200e-02,
        -4.70724694e+00,  -5.28843603e+02,   0.00000000e+00,  -1.22753400e-02,
        -3.19670762e+02,  -2.60999685e+02,   0.00000000e+00,  -9.99772000e-01,
        -6.98445586e+00,   3.31139785e+00,   0.00000000e+00,   1.09167360e-02
    };
    glMultMatrixf(mat);
}

void mouseMoved(int x, int y)
{
    if (mx>=0 && my>=0) {
        rotangles[0] += y-my;
        rotangles[1] += x-mx;
    }
    mx = x;
    my = y;
}

void mousePress(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mx = x;
        my = y;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mx = -1;
        my = -1;
    }
}

void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
    pthread_mutex_lock(&gl_backbuf_mutex);
    memcpy((void*) depth_mid, v_depth, 640 * 480 * sizeof(uint16_t));
    got_depth++;
    pthread_cond_signal(&gl_frame_cond);
    pthread_mutex_unlock(&gl_backbuf_mutex);
}

void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
    pthread_mutex_lock(&gl_backbuf_mutex);

    // swap buffers
    assert (rgb_back == rgb);
    rgb_back = rgb_mid;
    freenect_set_video_buffer(dev, rgb_back);
    rgb_mid = (uint8_t*)rgb;

    got_rgb++;
    pthread_cond_signal(&gl_frame_cond);
    pthread_mutex_unlock(&gl_backbuf_mutex);
}

void *freenect_threadfunc(void *arg)
{
    int accelCount = 0;

    freenect_set_led(f_dev, LED_GREEN);
    freenect_set_depth_callback(f_dev, depth_cb);
    freenect_set_video_callback(f_dev, rgb_cb);
    freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB));
    freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
    freenect_set_video_buffer(f_dev, rgb_back);

    freenect_start_depth(f_dev);
    freenect_start_video(f_dev);

    while (!die && freenect_process_events(f_ctx) >= 0)
    {
    }

    freenect_set_led(f_dev, LED_RED);

    printf("\nshutting down streams...\n");

    freenect_stop_depth(f_dev);
    freenect_stop_video(f_dev);

    freenect_close_device(f_dev);
    freenect_shutdown(f_ctx);

    printf("-- done!\n");
    return NULL;
}


void DrawGLScene()
{
    uint32_t ts;

    pthread_mutex_lock(&gl_backbuf_mutex);

    if (!got_depth || !got_rgb)
    {
        pthread_mutex_unlock(&gl_backbuf_mutex);
        return;
    }

    if (got_depth) {
        uint16_t *tmp;
        tmp = depth_front;
        depth_front = depth_mid;
        depth_mid = tmp;
        got_depth = 0;
    }
    if (got_rgb) {
        uint8_t *tmp;
        tmp = rgb_front;
        rgb_front = rgb_mid;
        rgb_mid = tmp;
        got_rgb = 0;
    }

    pthread_mutex_unlock(&gl_backbuf_mutex);


    static unsigned int indices[480][640];
    static short xyz[480][640][3];
    int i,j;
    for (i = 0; i < 480; i++) {
        for (j = 0; j < 640; j++) {
            xyz[i][j][0] = j;
            xyz[i][j][1] = i;
            xyz[i][j][2] = depth_front[i*640+j];
            indices[i][j] = i*640+j;
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    glScalef(zoom,zoom,1);
    glTranslatef(0,0,-3.5);
    glRotatef(rotangles[0], 1,0,0);
    glRotatef(rotangles[1], 0,1,0);
    glTranslatef(0,0,1.5);

    LoadVertexMatrix();

    // Set the projection from the XYZ to the texture image
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1/640.0f,1/480.0f,1);
    LoadRGBMatrix();
    LoadVertexMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPointSize(1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, xyz);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_SHORT, 0, xyz);

    if (color)
        glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front);

    glPointSize(2.0f);
    glDrawElements(GL_POINTS, 640*480, GL_UNSIGNED_INT, indices);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y)
{
    if (key == 27) {
		die = 1;
		pthread_join(freenect_thread, NULL);
		glutDestroyWindow(window);
		free(depth_mid);
		free(depth_front);
		free(rgb_back);
		free(rgb_mid);
		free(rgb_front);
		// Not pthread_exit because OSX leaves a thread lying around and doesn't exit
		exit(0);
    }
    if (key == 'w')
        zoom *= 1.1f;
    if (key == 's')
        zoom /= 1.1f;
    if (key == 'c')
        color = !color;
}

void ReSizeGLScene(int Width, int Height)
{
    glViewport(0,0,Width,Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 4/3., 0.3, 200);
    glMatrixMode(GL_MODELVIEW);
}

void InitGL(int Width, int Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glGenTextures(1, &gl_rgb_tex);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ReSizeGLScene(Width, Height);
}

int main(int argc, char **argv)
{
    if (freenect_init(&f_ctx, NULL) < 0) {
        printf("freenect_init() failed\n");
        return 1;
    }

    freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
    freenect_select_subdevices(f_ctx, (freenect_device_flags) FREENECT_DEVICE_CAMERA);

    int nr_devices = freenect_num_devices (f_ctx);
    printf ("Number of devices found: %d\n", nr_devices);

    int user_device_number = 0;
    if (argc > 1)
        user_device_number = atoi(argv[1]);

    if (nr_devices < 1) {
        freenect_shutdown(f_ctx);
        return 1;
    }

    if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
        printf("Could not open device\n");
        freenect_shutdown(f_ctx);
        return 1;
    }

    depth_mid = (uint16_t*)malloc(640*480*3);
    depth_front = (uint16_t*)malloc(640*480*3);
    rgb_back = (uint8_t*)malloc(640*480*3);
    rgb_mid = (uint8_t*)malloc(640*480*3);
    rgb_front = (uint8_t*)malloc(640*480*3);


    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);

    window = glutCreateWindow("LibFreenect");

    glutDisplayFunc(&DrawGLScene);
    glutIdleFunc(&DrawGLScene);
    glutReshapeFunc(&ReSizeGLScene);
    glutKeyboardFunc(&keyPressed);
    glutMotionFunc(&mouseMoved);
    glutMouseFunc(&mousePress);

    InitGL(640, 480);

    glutMainLoop();

    return 0;
}


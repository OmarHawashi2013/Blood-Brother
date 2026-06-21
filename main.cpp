/*  
    ================================================
    =   /----\  \       /|  ------  |-----|        =
    =  |      | | \    / | /      \ |      |       =
    =  |      | |  \  /  | |------| |-----|        =
    =  |      | |   \/   | |      | |     \        =
    =  \ ____ / |        | |      | |      \       =
    =  [ --> OFFSET: 0x0000 | STATUS: OK ]         =
    =                                              =
    =  [ OMAR ENGINE - Custom Win32 Framework ]    =
    =  [ Developed by: Omar Hawash | (C) 2026 ]    =
    ================================================

*/ 

#define STB_IMAGE_IMPLEMENTATION
#include <windows.h>
#include <chrono>
#include <math.h>
#include <GL/gl.h>
#include "inc/stb_image.h"
#include <GL/glu.h>


float rotation = 0;

#define screenX 1280
#define screenY 720

float vertices[] = {
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f
};


constexpr double MS_PER_FRAME = 1000.0 / 60.0;

HDC backDC;
HBITMAP backBMP;

GLuint tex;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg) 
    {
        case WM_SIZE: {
            int w = LOWORD(lParam), h = HIWORD(lParam);

            if (h == 0) {
                h = 1;
            }
            if (w == 0) {
                w = 1;
            }

            glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            gluPerspective(45.0, double(screenX) / double(screenY), 0.1, 100.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            break;
        }
    	case WM_DESTROY:
            //DeleteObject(hFont);
            //DeleteObject(hFontSmall);
            //mciSendString("close bgm", NULL, 0, NULL);
            //mciSendString("close sndJump", NULL, 0, NULL);
            //mciSendString("close sndShoot", NULL, 0, NULL);
            //mciSendString("close sndEnemyJump", NULL, 0, NULL);

            DeleteObject(backBMP);
            DeleteDC(backDC);

            PostQuitMessage(0);
            return 0;
 
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam); 
    }
    return 0; 
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow){
	WNDCLASS wc = {};

	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = "3D ENGINE";

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
	    0,
	    "3D ENGINE",
	    "3D ENGINE",
	    WS_OVERLAPPEDWINDOW,

	    CW_USEDEFAULT, CW_USEDEFAULT, screenX, screenY,

	    NULL,
	    NULL,
	    hInstance,
	    NULL
	);

	if (hwnd == NULL){
	    return 0;
	}

	ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    HDC hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, double(screenX) / double(screenY), 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);

    int w, h, c;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("textures/good_woden.png", &w, &h, &c, 0);



    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data != NULL && w > 0 && h > 0) {
        GLenum format = (c == 4) ? GL_RGBA : GL_RGB;
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    /*x
    init
    */

    //mciSendString("open \"sfx/jump.wav\" type waveaudio alias sndJump", NULL, 0, NULL);
    //mciSendString("open \"sfx/shoot.wav\" type waveaudio alias sndShoot", NULL, 0, NULL);
    //mciSendString("open \"sfx/ejump.wav\" type waveaudio alias  sndEnemyJump", NULL, 0, NULL);
    

    //mciSendString("open music/main_menu.wav type mpegvideo alias bgm", NULL, 0, NULL);
    //mciSendString("play bgm repeat", NULL, 0, NULL);



    //AddFontResourceEx("Minecraft.ttf", FR_PRIVATE, 0);

    /*
    init
    */


    MSG msg;

    while (true){
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            if(msg.message == WM_QUIT) return msg.wParam;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        auto frameStart = std::chrono::high_resolution_clock::now();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -3.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 5,  0, 0, 0,  0, 1, 0);



// --------------- CUBE -------------------

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glRotatef(rotation, 1.0f, 1.0f, 0.0f);
    rotation += 1.0f;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

// --------------- CUBE -------------------


        SwapBuffers(hdc);



        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frameTime = frameEnd - frameStart;

        if (frameTime.count() < MS_PER_FRAME) {
            Sleep(static_cast<DWORD>(MS_PER_FRAME - frameTime.count()));
        }
    }
}

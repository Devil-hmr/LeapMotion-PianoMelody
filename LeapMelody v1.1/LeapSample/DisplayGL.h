#pragma once
#include <iostream>
#include <vector>
#include <windows.h>

#include <minwinbase.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glaux.h" // GLaux ���ͷ�ļ�
#include "GL/glut.h"
#include "CPianoMelody.h"
#define MAX_TEXTURE  3   // ��������������Ŀ
#define MAX_KEY_NUM 7
using namespace std;
//start���������ģ�end��ָ��
struct SStick
{
	float start_x;
	float start_y;
	float start_z;
	float end_x;
	float end_y;
	float end_z;
};
extern CRITICAL_SECTION g_csPiano;
extern CRITICAL_SECTION g_csStick;
extern vector<SStick> g_StickVector;
extern CPianoMelody	  *g_pianoMelody;
AUX_RGBImageRec *LoadImage(char *Filename);
BOOL	LoadTextureGL(); 
void	glDrawCube(GLfloat width, GLfloat height, GLfloat deep);
void	DrawGLScene(void);
void	ReSizeFunc(int width,int height);
int		Init();
void	KeyBoardFunc(unsigned char key, int x, int y);
bool	SetKeyPress(int musicNum);


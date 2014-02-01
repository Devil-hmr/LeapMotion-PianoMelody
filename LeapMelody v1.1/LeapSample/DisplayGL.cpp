#include "stdafx.h"
#include "DisplayGL.h"
CRITICAL_SECTION g_csPiano;
CRITICAL_SECTION g_csStick;
GLuint			texture[3];   
static int		g_PressWhichKey = -1;
vector<SStick>	g_StickVector;
CPianoMelody	*g_pianoMelody;
AUX_RGBImageRec *LoadImage(char *Filename)  // ����һ��ͼƬ   
{   
	FILE *File = NULL;                      // �ļ����   
	if(!Filename)                          // ȷ���ļ����Ѿ��ṩ   
	{   
		return NULL;                        // ���û���򷵻�NULL   
	}   
	File=fopen(Filename,"r");               // ����ļ��Ƿ����   

	if(File)                               // �ļ�������   
	{   
		fclose(File);                       // �ر�File�ļ����   
		return auxDIBImageLoad(Filename);   // ����ͼƬ��������ָ��   
	}   
	return NULL;                            // ������ش����򷵻�NULL   
}   
BOOL LoadTextureGL()                                            // ����ͼƬ��ת��Ϊ����(new)   
{   
	BOOL State=FALSE;                                           // ״ָ̬ʾ   

	AUX_RGBImageRec *TextureImage[MAX_TEXTURE];                  // Ϊ�����ٴ洢�ռ�   

	memset(TextureImage,0,sizeof(void *)*MAX_TEXTURE);           //  ���ͼ���¼��ȷ��������Ϊ�ղ�ʹָ��ָ��NULL   
	//����ͼƬ������Ƿ���� �����ͼƬ�������򷵻�      
	if ((TextureImage[0] = LoadImage("Resources/white.bmp"))   
		&& (TextureImage[1] = LoadImage("Resources/black.bmp")) 
		&& (TextureImage[2] = LoadImage("Resources/piano.bmp")))
	{      
		State = TRUE;                                           // ����״̬����ΪTRUE   
		glGenTextures(MAX_TEXTURE, &texture[0]);                 // ����Ψһ��������������ʶ����,������texture��   

		// ��ͼƬ���ݲ�������   
		for (int loop=0; loop<MAX_TEXTURE; loop++)                // ѭ������MAX_TEXTURE������   
		{   
			glBindTexture(GL_TEXTURE_2D, texture[loop]);   
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);   
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
		}   
	}   
	for (int loop=0; loop<MAX_TEXTURE; loop++)   
	{   
		if (TextureImage[loop])                  
		{   
			if (TextureImage[loop]->data)        // ����������   
			{   
				free(TextureImage[loop]->data);  // �ͷ�����洢�ռ�   
			}   
			free(TextureImage[loop]);            
		}   
	}   
	return State;                                // ����State   
}   
// ����һ��������ĳ�����   
void glDrawCube(GLfloat width, GLfloat height, GLfloat deep)   
{   
	glBegin(GL_QUADS);   
	// ǰ��   
	glNormal3f( 0.0f, 0.0f, 1.0f);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2, deep/2);   
	// ����   
	glNormal3f( 0.0f, 0.0f,-1.0f);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2,-deep/2);   
	// ����   
	glNormal3f( 0.0f, 1.0f, 0.0f);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2, height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	// ����   
	glNormal3f( 0.0f,-1.0f, 0.0f);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2,-height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	// ����   
	glNormal3f( 1.0f, 0.0f, 0.0f);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	// ����   
	glNormal3f(-1.0f, 0.0f, 0.0f);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glEnd();   
}   

void DrawGLScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// �����������Ȼ���   
	glLoadIdentity();                                   // ���õ�ǰ����   
	// �ڴ˴���Ӵ�����л���:
	glTranslatef(0.0f, 0.5f,-2.0f);
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	//glRotatef(-20, 0.0f, 1.0f, 0.0f);
	//�Ȱ�n����ָ������ g_StickVector
	SStick *sTemp;
	double factor = 200.0;
	
	GLUquadricObj *qobj = gluNewQuadric();
	EnterCriticalSection(&g_csStick);
	for(int i=0;i<g_StickVector.size();++i)
	{
		glColor3f(0,1,0);
		sTemp = &g_StickVector[i];

		//һЩ����ĵ������¸��汾��ʹ��1:1�ı��������ƣ����OpenGL�����Ǳ��˵ģ��Ҳ�δ����̫���޸ģ�
		sTemp->start_x /= factor*1.2;
		sTemp->start_y /= factor*1.5;
		sTemp->start_z /= factor;
		sTemp->end_x /= factor*1.2;
		sTemp->end_y /= factor*1.5;
		sTemp->end_z /= factor;

		sTemp->start_x += 0.1;
		sTemp->end_x += 0.1;

		sTemp->start_z -= 0.2;
		sTemp->end_z -= 0.2;

		sTemp->start_y -= 0.7;
		sTemp->end_y -= 0.7;

		glPushMatrix();////////////////////////////////
		//��ʼ��
		glLineWidth(3);
		glPointSize(10);

		//�����ǻ��ߣ�����ѡ��򿪿���Ч��
// 		glBegin(GL_LINES);
// 		glVertex3f(sTemp->start_x,sTemp->start_y,sTemp->start_z);
// 		glVertex3f(sTemp->end_x,sTemp->end_y,sTemp->end_z);
// 		glEnd();

		glPopMatrix();//////////////////////////

		glColor3f(1,0,0);
		glPushMatrix();
		glTranslatef(sTemp->end_x,sTemp->end_y,sTemp->end_z);
		gluSphere(qobj,0.03,100,100);
		glPopMatrix();
	}
	LeaveCriticalSection(&g_csStick);

	glColor3f(1,1,1);
	// �����һ����߸���ɫ����
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glTranslatef(-0.8f,-0.5f,0.0f);

	// ��һ����ɫ����   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (0 == g_PressWhichKey)
		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// �ڶ�����ɫ����   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (1 == g_PressWhichKey)
	{
	 	glTranslatef(0.0f, -0.04f, 0.0f);
	}
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// ��������ɫ����   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (2 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// ���ĸ���ɫ����   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (3 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// �������ɫ����   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (4 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// ��������ɫ����   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (5 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// ���߸���ɫ����   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (6 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	glPopMatrix();                      // ��ɫ�����������   

	// ѭ�����ƺ�ɫ����   
	glBindTexture(GL_TEXTURE_2D, texture[1]);   
	glPushMatrix();   
	glTranslatef(-0.7f, -0.46f, -0.2f);   
	for (int loop=0; loop<7; loop++)   
	{   
		glTranslatef(0.2f, 0.0f, 0.0f);   
		glDrawCube(0.1f, 0.1f, 0.4f);   
	}   
	glPopMatrix();   

	// �����������٣��������⣩   
	glBindTexture(GL_TEXTURE_2D, texture[2]);   
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.8f);
	glDrawCube(1.8f, 0.1f, 1.0f);
	glPopMatrix();

	glPushMatrix();   
	glTranslatef(0.0f, 0.0f, -0.8f);   
	glTranslatef(0.0f, -0.65f, 0.0f);   
	glDrawCube(1.6f, 1.2f, 0.8f);   
	glPopMatrix();   

	glPushMatrix();   
	glTranslatef(-0.75f, -0.8f, 0.0f);   
	glDrawCube(0.1f, 0.9f, 0.8f);   
	glTranslatef(1.5f, 0.0f, 0.0f);   
	glDrawCube(0.1f,0.9f, 0.8f);   
	glPopMatrix();   

	glPushMatrix();   
	glTranslatef(0.0f, -1.3f, -0.4f);   
	glDrawCube(1.8f, 0.1f, 1.7f);   
	glPopMatrix();   

	glutSwapBuffers(); 
}
void ReSizeFunc(int width,int height)
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));              // ���õ�ǰ�ӿڴ�С   
	glMatrixMode(GL_PROJECTION);                                        // �л���ͶӰ����ģʽ   
	glLoadIdentity();                                                   // ����ͶӰ����   
	gluPerspective(45, (float)width/(float)height, 0.1, 1000);           // ����͸��ͶӰ   
	glMatrixMode(GL_MODELVIEW);                                         // �л���ģ����ͼ����   
	glLoadIdentity();                                                   // ����ģ����ͼ����   
}

int Init()
{
	InitializeCriticalSection(&g_csPiano);
	InitializeCriticalSection(&g_csStick);

	glClearColor(0,0,0,0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);//  ������Ӱƽ��
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);//  ������ϸ��͸������
	if (!LoadTextureGL())  // ����LoadTextureGL()����   
	{   
		return FALSE;      // ���������ش����򷵻�FALSE   
	}   
	glEnable(GL_TEXTURE_2D);// ����2D����ӳ��   
	//�����
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	return 1;
}
void KeyBoardFunc(unsigned char key, int x, int y)
{
//	printf("%x\n",key);
	if(VK_ESCAPE == key)
	{
		exit(0);
	}
	if(('A' == key)||('a' == key))
	{
		g_PressWhichKey = 0;
	}
	else if(('S' == key)||('s' == key))
	{
		g_PressWhichKey = 1;
	}
	else if(('D' == key)||('d' == key))
	{
		g_PressWhichKey = 2;
	}
	else if(('F' == key)||('f' == key))
	{
		g_PressWhichKey = 3;
	}
	else if(('G' == key)||('g' == key))
	{
		g_PressWhichKey = 4;
	}
	else if(('H' == key)||('h' == key))
	{
		g_PressWhichKey = 5;
	}
	else if(('J' == key)||('j' == key))
	{
		g_PressWhichKey = 6;
	}
	EnterCriticalSection(&g_csPiano);
	g_pianoMelody->Play(g_PressWhichKey);
	LeaveCriticalSection(&g_csPiano);
	DrawGLScene();
}

bool SetKeyPress(int musicNum)
{
	if((musicNum >= MAX_KEY_NUM) || (musicNum < 0))
	{
		return false;
	}
	g_PressWhichKey = musicNum;
	EnterCriticalSection(&g_csPiano);
	g_pianoMelody->Play(musicNum);
	LeaveCriticalSection(&g_csPiano);
	DrawGLScene();
	return true;
}

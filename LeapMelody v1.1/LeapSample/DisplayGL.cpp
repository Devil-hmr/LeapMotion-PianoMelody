#include "stdafx.h"
#include "DisplayGL.h"
CRITICAL_SECTION g_csPiano;
CRITICAL_SECTION g_csStick;
GLuint			texture[3];   
static int		g_PressWhichKey = -1;
vector<SStick>	g_StickVector;
CPianoMelody	*g_pianoMelody;
AUX_RGBImageRec *LoadImage(char *Filename)  // 加载一个图片   
{   
	FILE *File = NULL;                      // 文件句柄   
	if(!Filename)                          // 确保文件名已经提供   
	{   
		return NULL;                        // 如果没有则返回NULL   
	}   
	File=fopen(Filename,"r");               // 检查文件是否存在   

	if(File)                               // 文件存在吗？   
	{   
		fclose(File);                       // 关闭File文件句柄   
		return auxDIBImageLoad(Filename);   // 载入图片并返回其指针   
	}   
	return NULL;                            // 如果加载错误则返回NULL   
}   
BOOL LoadTextureGL()                                            // 加载图片并转换为纹理(new)   
{   
	BOOL State=FALSE;                                           // 状态指示   

	AUX_RGBImageRec *TextureImage[MAX_TEXTURE];                  // 为纹理开辟存储空间   

	memset(TextureImage,0,sizeof(void *)*MAX_TEXTURE);           //  清除图像记录，确保其内容为空并使指针指向NULL   
	//加载图片并检查是否出错 ，如果图片不存在则返回      
	if ((TextureImage[0] = LoadImage("Resources/white.bmp"))   
		&& (TextureImage[1] = LoadImage("Resources/black.bmp")) 
		&& (TextureImage[2] = LoadImage("Resources/piano.bmp")))
	{      
		State = TRUE;                                           // 设置状态变量为TRUE   
		glGenTextures(MAX_TEXTURE, &texture[0]);                 // 返回唯一的纹理名字来标识纹理,保存在texture中   

		// 用图片数据产生纹理   
		for (int loop=0; loop<MAX_TEXTURE; loop++)                // 循环处理MAX_TEXTURE张纹理   
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
			if (TextureImage[loop]->data)        // 如果纹理存在   
			{   
				free(TextureImage[loop]->data);  // 释放纹理存储空间   
			}   
			free(TextureImage[loop]);            
		}   
	}   
	return State;                                // 返回State   
}   
// 绘制一个带纹理的长方体   
void glDrawCube(GLfloat width, GLfloat height, GLfloat deep)   
{   
	glBegin(GL_QUADS);   
	// 前面   
	glNormal3f( 0.0f, 0.0f, 1.0f);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2, deep/2);   
	// 后面   
	glNormal3f( 0.0f, 0.0f,-1.0f);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2,-deep/2);   
	// 顶面   
	glNormal3f( 0.0f, 1.0f, 0.0f);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2, height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	// 底面   
	glNormal3f( 0.0f,-1.0f, 0.0f);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2,-height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	// 右面   
	glNormal3f( 1.0f, 0.0f, 0.0f);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	// 左面   
	glNormal3f(-1.0f, 0.0f, 0.0f);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glEnd();   
}   

void DrawGLScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清除场景和深度缓存   
	glLoadIdentity();                                   // 重置当前矩阵   
	// 在此处添加代码进行绘制:
	glTranslatef(0.0f, 0.5f,-2.0f);
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	//glRotatef(-20, 0.0f, 1.0f, 0.0f);
	//先把n个手指画出来 g_StickVector
	SStick *sTemp;
	double factor = 200.0;
	
	GLUquadricObj *qobj = gluNewQuadric();
	EnterCriticalSection(&g_csStick);
	for(int i=0;i<g_StickVector.size();++i)
	{
		glColor3f(0,1,0);
		sTemp = &g_StickVector[i];

		//一些坐标的调整（下个版本将使用1:1的比例来绘制，这个OpenGL程序是别人的，我并未进行太多修改）
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
		//初始化
		glLineWidth(3);
		glPointSize(10);

		//下面是画线，可以选择打开看看效果
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
	// 从左到右绘制七个白色按键
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glTranslatef(-0.8f,-0.5f,0.0f);

	// 第一个白色按键   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (0 == g_PressWhichKey)
		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// 第二个白色按键   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (1 == g_PressWhichKey)
	{
	 	glTranslatef(0.0f, -0.04f, 0.0f);
	}
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// 第三个白色按键   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (2 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// 第四个白色按键   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (3 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// 第五个白色按键   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (4 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// 第六个白色按键   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (5 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	// 第七个白色按键   
	glTranslatef(0.2f, 0.0f, 0.0f);   
	glPushMatrix();   
	if (6 == g_PressWhichKey)
	 		glTranslatef(0.0f, -0.04f, 0.0f);   
	glDrawCube(0.2f, 0.1f, 0.8f);   
	glPopMatrix();   

	glPopMatrix();                      // 白色按键绘制完毕   

	// 循环绘制黑色按键   
	glBindTexture(GL_TEXTURE_2D, texture[1]);   
	glPushMatrix();   
	glTranslatef(-0.7f, -0.46f, -0.2f);   
	for (int loop=0; loop<7; loop++)   
	{   
		glTranslatef(0.2f, 0.0f, 0.0f);   
		glDrawCube(0.1f, 0.1f, 0.4f);   
	}   
	glPopMatrix();   

	// 绘制整个钢琴（除按键外）   
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
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));              // 重置当前视口大小   
	glMatrixMode(GL_PROJECTION);                                        // 切换到投影矩阵模式   
	glLoadIdentity();                                                   // 重置投影矩阵   
	gluPerspective(45, (float)width/(float)height, 0.1, 1000);           // 设置透视投影   
	glMatrixMode(GL_MODELVIEW);                                         // 切换到模型视图矩阵   
	glLoadIdentity();                                                   // 重置模型视图矩阵   
}

int Init()
{
	InitializeCriticalSection(&g_csPiano);
	InitializeCriticalSection(&g_csStick);

	glClearColor(0,0,0,0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);//  启用阴影平滑
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);//  真正精细的透视修正
	if (!LoadTextureGL())  // 跳到LoadTextureGL()函数   
	{   
		return FALSE;      // 如果纹理加载错误则返回FALSE   
	}   
	glEnable(GL_TEXTURE_2D);// 开启2D纹理映射   
	//抗锯齿
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

// LeapSample.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include "Leap.h"
#include "DisplayGL.h"
using namespace Leap;
using namespace std;
//extern CPianoMelody		*g_pianoMelody;
//extern vector<SStick>	g_StickVector;
static bool				g_bStartBackgroundMusic = true;
class PianoListener : public Listener {
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
};

void PianoListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void PianoListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
//	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
//	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
//	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void PianoListener::onDisconnect(const Controller& controller) {
	//Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void PianoListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void PianoListener::onFrame(const Controller& controller) {
	// ��ȡ���µ�һ֡�����ҷ���һЩ������Ϣ
	const Frame frame = controller.frame();

	if(4 < frame.fingers().count())
	{
		if(g_bStartBackgroundMusic)
		{
		//	g_bStartBackgroundMusic = false;
			EnterCriticalSection(&g_csPiano);
			g_pianoMelody->StopPlayingBackgroundMusic();
			LeaveCriticalSection(&g_csPiano);
		}
	}

	//��OpenGL����ʾ��
	EnterCriticalSection(&g_csStick);
	g_StickVector.clear();
	for(int i=0;i<frame.fingers().count();++i)
	{
		SStick sTemp;
		sTemp.start_x = frame.fingers()[i].hand().palmPosition().x;
		sTemp.start_y = frame.fingers()[i].hand().palmPosition().y;
		sTemp.start_z = frame.fingers()[i].hand().palmPosition().z;
		sTemp.end_x = frame.fingers()[i].tipPosition().x;
		sTemp.end_y = frame.fingers()[i].tipPosition().y;
		sTemp.end_z = frame.fingers()[i].tipPosition().z;
		g_StickVector.push_back(sTemp);
	}
	LeaveCriticalSection(&g_csStick);
	// ��ȡ����
	const GestureList gestures = frame.gestures();
	for (int g = 0; g < gestures.count(); ++g) {
		Gesture gesture = gestures[g];
		switch (gesture.type()) {
		case Gesture::TYPE_SWIPE:
			{
				SwipeGesture swipe = gesture;
				std::cout << "Swipe id: " << gesture.id()
					<< ", state: " << gesture.state()
					<< ", direction: " << swipe.direction()
					<< ", speed: " << swipe.speed() << std::endl;
				break;
			}
		case Gesture::TYPE_KEY_TAP:
			{
				KeyTapGesture tap = gesture;
				EnterCriticalSection(&g_csPiano);
				bool flag = g_pianoMelody->OccurGesture(tap);
				LeaveCriticalSection(&g_csPiano);
				break;
			}
		case Gesture::TYPE_SCREEN_TAP:
			{
				ScreenTapGesture screentap = gesture;
				std::cout << "Screen Tap id: " << gesture.id()
					<< ", state: " << gesture.state()
					<< ", position: " << screentap.position()
					<< ", direction: " << screentap.direction()<< std::endl;
				break;
			}
		default:
			std::cout << "Unknown gesture type." << std::endl;
			break;
		}
	}

	if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
		//std::cout << std::endl;
	}
	 // Sleep(100);//�����ˢ��̫����������2��һ��2����
}

void PianoListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void PianoListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

int main(int argc, char *argv[]){

	//������Leap Motion����

	// �������Ӽ����ߺͿ�����
	PianoListener listener;
	Controller controller;

	// �ü����߻�ȡ�����������ݲ����з���
	controller.addListener(listener);
	//����ʹ��new������ʹ�ö��󣬷�����ָ���޷���ȡ���õ����⡣�������ǳ��֣��漰�����߳��Լ�STL������
	g_pianoMelody = new CPianoMelody;
	g_pianoMelody->LoadMusic("piano-files.txt");
	// �ó���һֱ���У�ֱ������س����Ž���

	//������Ӻü��ױ������֣������������ͬʱ���пս�txt�ļ�ɾ�����ĳ�ֱ�ӱ����ļ��е�����
	g_bStartBackgroundMusic = false;
	if(g_bStartBackgroundMusic)
	{
		srand(time(0));
		int randNum = rand()%3;
		if(0 == randNum)
		{
			g_pianoMelody->PlayBackgroundMusic("OpeningMusic\\�ǿ�.mp3");//֧�����ֺ��пո�����
		}
		else if(1 == randNum)
		{
			g_pianoMelody->PlayBackgroundMusic("OpeningMusic\\���ӡ��.mp3");
		}
		else
		{
			g_pianoMelody->PlayBackgroundMusic("OpeningMusic\\Variations on the Canon.mp3");
		}
	}

	//OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	//glutInitDisplayMode(GLUT_RGB );
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("����(Piano Melody)by���� v1.1");
	glutReshapeFunc(ReSizeFunc);
	glutDisplayFunc(DrawGLScene);
	glutIdleFunc(DrawGLScene);
	glutKeyboardFunc(KeyBoardFunc);
	Init();
	//glutTimerFunc(40,OnTimer,1);
	glutMainLoop();
	
	//std::cout << "Press Enter to quit..." << std::endl;
	//std::cin.get();
	// �Ƴ�������
	controller.removeListener(listener);
	return 0;
}
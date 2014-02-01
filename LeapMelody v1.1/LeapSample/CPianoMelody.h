/**********************************
CPianoMelody.h
verson 1.0����ͨ��KeyTap�������������ֵĲ���
***********************************/
#pragma once
//#include <deque>
#include "Leap.h"
#include "MusicPlayer.h"
using namespace std;
using namespace Leap;
//�����Ҫ�¼�������
class CPianoMelody
{
public:
	CPianoMelody();
	bool			UpdateData(const Frame &frame);//���µ�ǰ��ָ
	bool			OccurGesture(const KeyTapGesture &keyTap);
	void			PlayBackgroundMusic(char fileName[]);
	void			StopPlayingBackgroundMusic();
	bool			LoadMusic(char fileName[]);
	bool			Play(int musicNum);
	//bool			OccurGesture(SwipeGesture &swip);
	
private:
	bool			m_bFingersInited;//ȷ����ָ�Ѿ�������
	int				m_currentFinger;
	int				m_currentPianoArea;
	bool			m_keyTapOccured;
	bool			m_swipOccured;
	FingerList		m_fullFingerList;
	CMusicPlayer	m_musicPlayer;
};
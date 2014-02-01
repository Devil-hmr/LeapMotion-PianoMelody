/**********************************
CPianoMelody.h
verson 1.0仅仅通过KeyTap手势来进行音乐的播放
***********************************/
#pragma once
//#include <deque>
#include "Leap.h"
#include "MusicPlayer.h"
using namespace std;
using namespace Leap;
//添加主要事件处理类
class CPianoMelody
{
public:
	CPianoMelody();
	bool			UpdateData(const Frame &frame);//更新当前手指
	bool			OccurGesture(const KeyTapGesture &keyTap);
	void			PlayBackgroundMusic(char fileName[]);
	void			StopPlayingBackgroundMusic();
	bool			LoadMusic(char fileName[]);
	bool			Play(int musicNum);
	//bool			OccurGesture(SwipeGesture &swip);
	
private:
	bool			m_bFingersInited;//确认手指已经更新了
	int				m_currentFinger;
	int				m_currentPianoArea;
	bool			m_keyTapOccured;
	bool			m_swipOccured;
	FingerList		m_fullFingerList;
	CMusicPlayer	m_musicPlayer;
};
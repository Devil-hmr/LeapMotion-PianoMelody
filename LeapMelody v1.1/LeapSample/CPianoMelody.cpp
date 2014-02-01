#include "stdafx.h"
#include "CPianoMelody.h"
#include "DisplayGL.h"
#include <LeapMath.h>
#include <windows.h>
#include <iostream>
using namespace std;
CPianoMelody::CPianoMelody()
{
	m_bFingersInited	= false;
	m_currentFinger		= -1;
	m_keyTapOccured		= false;
	m_currentPianoArea	= -1;
}
bool	CPianoMelody::UpdateData(const Frame &frame)
{
	FingerList fingerList = frame.fingers();
	return true;
}
//截取发生的屏幕点击事件，进行演奏
bool	CPianoMelody::OccurGesture(const KeyTapGesture &keyTap)
{
	Pointable pt = keyTap.pointable();
	Vector v= pt.tipVelocity();
	Vector pos = pt.tipPosition();
	int xPos = pos.x;
	if(xPos <= -200)
	{
		xPos = -200;
	}
	else if(xPos >= 200)
	{
		xPos = 200;
	}
	int musicNum = (pos.x + 200)/50;

	SetKeyPress(musicNum);
	
	return false;
}
void CPianoMelody::PlayBackgroundMusic(char fileName[])
{
	m_musicPlayer.Play(fileName);
}
void CPianoMelody::StopPlayingBackgroundMusic()
{
	m_musicPlayer.Stop();
}
bool CPianoMelody::LoadMusic(char fileName[])
{
	return m_musicPlayer.LoadMusic(fileName);
}
bool CPianoMelody::Play(int musicNum)
{
	if((musicNum >= MAX_KEY_NUM) || (musicNum < 0))
	{
		return false;
	}
	m_musicPlayer.Play(musicNum);
	return true;
}
#include "stdafx.h"
#include "MusicPlayer.h"
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
using namespace std;
deque<string>	m_openStrings;
deque<string>	m_playStrings;
deque<int>		m_vPlayDeque;//���Ŷ���
CRITICAL_SECTION g_cs;
DWORD WINAPI PlayMusicThread(LPVOID p)
{
	char ret[256]={0};
	EnterCriticalSection(&g_cs);//��Ҫʹ���ٽ������ؼ������
	if(m_vPlayDeque.empty())
	{
		LeaveCriticalSection(&g_cs);
		return 0xFFFFFFFF;
	}
	int num = m_vPlayDeque.front();
	m_vPlayDeque.pop_front();
//	cout<<"music number = "<<num<<endl;
	LeaveCriticalSection(&g_cs);

	mciSendString(m_openStrings[num].c_str(),ret,strlen(ret),NULL);
	mciSendString(m_playStrings[num].c_str(),ret,strlen(ret),NULL);
	
	return 0;
}
CMusicPlayer::CMusicPlayer(void)
{
	m_sID = "CurrentName";
	InitializeCriticalSection(&g_cs);/////��ʼ��~!!!!!
}
CMusicPlayer::~CMusicPlayer()
{
	DeleteCriticalSection(&g_cs);
}
bool CMusicPlayer::LoadMusic(char fileName[])
{
	m_openStrings.clear();
	m_playStrings.clear();
	char str[256];
	ifstream inStream(fileName,ios::in);
	inStream.getline(str,256);
	m_nMusicNum = 0;
	for(;strlen(str)!=0;)
	{
		++m_nMusicNum;
		char strNum[5]={'\0'};
		sprintf_s(strNum,"%d",m_nMusicNum);
		string mciOpenStr = "open \"" + string(str) +"\" alias device_music"+strNum;
		inStream.getline(str,256);
		m_openStrings.push_back(mciOpenStr);
		string mciPlayStr = string("play")+" device_music"+strNum+" notify";//notify�����Ի���wait,�ᷢ��������
		m_playStrings.push_back(mciPlayStr);
	}
	cout<<"һ�������ָ��� = "<<m_openStrings.size()<<"  m_nMusicNum= "<<m_nMusicNum<<endl;
	if(!m_nMusicNum)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}
void CMusicPlayer::Play(int musicName)
{
	EnterCriticalSection(&g_cs);
	if((musicName >= m_nMusicNum) || (musicName < 0))
	{
		LeaveCriticalSection(&g_cs);
		return;
	}
	cout<<"CMusicPlayer::Play = "<<musicName<<endl;
	m_vPlayDeque.push_back(musicName);
	LeaveCriticalSection(&g_cs);
	//��������Ϊ��ʱ��������Ϊ���ȷ��ر�������ʧ�ˣ����Զ�ȡ��������
	CreateThread(NULL,0,PlayMusicThread,NULL,0,NULL);//��������û�д���ȥ����������9-9
}
void CMusicPlayer::Play(char fileName[])
{
	//֮����Ҫ�� \"��Ϊ�����ļ����п��ܰ����ո�ֻ�м��ϲ��ܲ���
	string mciOpenStr = "open \"" + string(fileName) +"\" alias "+ m_sID;
	
	string mciPlayStr = string("play ")+m_sID+" repeat";//wait�ĵĻ�������
	//repeat---����ѭ��
	//char ret[256];
	cout<<mciOpenStr<<endl;cout<<mciPlayStr<<endl;
	mciSendString(mciOpenStr.c_str(),NULL,0,NULL);
	mciSendString("play CurrentName notify repeat",NULL,0,NULL);
}
void CMusicPlayer::Stop()
{
	char ss[50];
	string str = " Pause " + m_sID;
	cout<<str<<endl;
	MCIERROR error =mciSendString(str.c_str(),ss,50,NULL);

	 str = "stop " + m_sID;
	cout<<str<<endl;
	mciSendString(str.c_str(),NULL,0,NULL);
	
	
	 str = "close " + m_sID;
	cout<<str<<endl;
	mciSendString(str.c_str(),NULL,0,NULL);
	

	mciGetErrorString(error,ss,50);
	cout<<"ss2="<<ss<<endl;
}
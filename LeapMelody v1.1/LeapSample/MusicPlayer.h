#pragma once
#include <string>
using namespace std;
class CMusicPlayer
{
public:
	CMusicPlayer(void);
	~CMusicPlayer();
	bool	LoadMusic(char fileName[]);
	void	Play(int musicName);
	void    Play(char fileName[]);//Ŀǰ���ṩһ�ױ������ֵĲ���
	void    Stop();
	//~CMusicPlayer(void);
private:
	int	    m_nMusicNum;
	string  m_sID;
};
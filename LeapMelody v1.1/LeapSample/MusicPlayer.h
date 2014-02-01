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
	void    Play(char fileName[]);//目前就提供一首背景音乐的播放
	void    Stop();
	//~CMusicPlayer(void);
private:
	int	    m_nMusicNum;
	string  m_sID;
};
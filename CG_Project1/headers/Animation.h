#pragma once
#include <vector>
#include <string>

class Animation
{
public:
	Animation(const std::string name, std::vector<int> frameIndxs, bool loop);
	~Animation() {};
	int getFrame();
	std::string m_name;
	std::vector<int> m_frameIndexes;
	int m_currentFrame;
	bool m_loop;
	std::string m_nextAnim;
	bool m_readyNext;
private:
	
};


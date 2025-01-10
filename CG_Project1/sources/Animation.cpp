#pragma once
#include "Animation.h"
#include <iostream>

Animation::Animation(const std::string name, std::vector<int> frameIndxs, bool loop) {
	m_name = name;
	m_frameIndexes = frameIndxs;
	m_currentFrame = 0;
	m_loop = loop;

}

int Animation::getFrame() {
	auto frame = m_currentFrame;
	if (m_currentFrame < m_frameIndexes.size() - 1) {
		m_currentFrame = m_currentFrame + 1;
	}
	else {
		if (m_loop) {
			m_currentFrame = m_frameIndexes[0];
		}
	}
	//std::cout << m_frameIndexes[frame] << std::endl;
	return m_frameIndexes[frame];
}





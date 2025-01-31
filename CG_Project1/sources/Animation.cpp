#pragma once
#include "Animation.h"
#include <iostream>

Animation::Animation(const std::string name, std::vector<int> frameIndxs, bool loop) {
	m_name = name;
	m_frameIndexes = frameIndxs;
	m_currentFrame = 0;
	m_loop = loop;
	m_nextAnim = "";
	m_readyNext = false;
}

int Animation::getFrame() {
	auto frame = m_currentFrame;
	if (m_currentFrame < m_frameIndexes.size() - 1) {
		m_currentFrame ++;
	}
	else {
		if (m_loop) {
			m_currentFrame = 0;
		}
		else {
			m_readyNext = true;
		}
	}

	return m_frameIndexes[frame];
}





#pragma once

class ActionTimer
{
	float m_currentTime = 0.0f;
	float m_targetTime = 0.0f;

public:
	void Tick(float delta)
	{
		m_currentTime += delta;
	}

	// Returns true is currentTime is higher than targetTime
	bool Evaluate()
	{
		return m_currentTime > m_targetTime;
	}

	void Reset()
	{
		m_currentTime = 0.0f;
	}

	float GetCurrentTime() const
	{
		return m_currentTime;
	}

	void SetTargetTime(float target)
	{
		m_targetTime = target;
	}
};
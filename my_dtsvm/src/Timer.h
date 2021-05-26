
#include <iostream>
#include <chrono>

#ifndef Included_Timer_H

#define Included_Timer_H

class Timer
{

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
public:
	Timer();
	~Timer();
	void Stop();

};

#endif
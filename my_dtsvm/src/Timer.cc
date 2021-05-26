#include "Timer.h"


Timer::Timer()
{
	m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	Stop();
}

void Timer::Stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::seconds>(m_StartTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::seconds>(endTimepoint).time_since_epoch().count();
	auto duration = end - start;
	//double ms = duration * 0.001;
	std::cout << "\nLama Proses : " << ((double) duration/60) << " menit\n";
}
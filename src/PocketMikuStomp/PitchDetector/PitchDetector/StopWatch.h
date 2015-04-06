//
//  StopWatch.h
//  HelloStopWatch
//
//  Created by Nobuhiro Kuroiwa on 2015/04/06.
//  Copyright (c) 2015年 osakanazabuun. All rights reserved.
//

#ifndef __HelloStopWatch__StopWatch__
#define __HelloStopWatch__StopWatch__

#include <string>
#include <chrono>

class StopWatch
{
public:
	StopWatch(std::string name);
	~StopWatch();
	void CountUp();
	unsigned long long Average();
	unsigned long long Time();
	void Print();
private:
	const std::chrono::system_clock::time_point _startTime;
	unsigned long long _counter;
	const std::string _name;
};

#endif /* defined(__HelloStopWatch__StopWatch__) */

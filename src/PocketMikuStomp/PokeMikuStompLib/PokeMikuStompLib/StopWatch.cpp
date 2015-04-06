//
//  StopWatch.cpp
//  HelloStopWatch
//
//  Created by Nobuhiro Kuroiwa on 2015/04/06.
//  Copyright (c) 2015年 osakanazabuun. All rights reserved.
//

#include "StopWatch.h"
#include <iostream>include

using namespace std;
using namespace chrono;

StopWatch::StopWatch(std::string name)
: _startTime(system_clock::now()), _counter(0), _name(name)
{
	
}

StopWatch::~StopWatch()
{
	
}

void StopWatch::CountUp()
{
	_counter++;
}

unsigned long long StopWatch::Average()
{
	unsigned long long time = Time();
	
	unsigned long long count = _counter ? _counter : 1;
	unsigned long long ave = time / count;
	
	return ave;
}

unsigned long long StopWatch::Time()
{
	auto endTime = system_clock::now();
	typedef duration<unsigned long long, microseconds::period> fp_microsec;
	unsigned long long time = fp_microsec(endTime - _startTime).count();
	
	return time;
}

void StopWatch::Print()
{
	unsigned long long avg = Average();
	cout << _name << " : " << avg << endl;
}

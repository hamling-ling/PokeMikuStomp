#pragma once


class IAutoCorrelation
{
public:
	IAutoCorrelation(int windowSize);
	virtual ~IAutoCorrelation() = 0;
	virtual void Compute(const float* x, float *corr) = 0;
	virtual int WindowSize();
protected:
	const int kWinSize;
	const int kFftSize;
};


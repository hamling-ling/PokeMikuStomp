
#include "IAutoCorrelation.h"

IAutoCorrelation::IAutoCorrelation(int windowSize)
	:
kWinSize(windowSize),
kFftSize(windowSize*2)
{
}

IAutoCorrelation::~IAutoCorrelation()
{
}

int IAutoCorrelation::WindowSize()
{
	return kWinSize;
}

#pragma once

#include <cstdint>

static const float kThreshold = 0.8f;
static const unsigned int kReservedBellNumber = 64;

typedef struct _NsdfPoint {
	float value;
	uint32_t index;
} NsdfPoint;

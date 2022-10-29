#pragma once
#include <random>

class RNG
{
private:
	inline static std::mt19937_64 engine{ std::random_device{}()};

public:
	static int64_t Generate() { return engine(); }
	static int64_t Generate(std::uniform_int_distribution<int64_t> dist) { return dist(engine); }
};
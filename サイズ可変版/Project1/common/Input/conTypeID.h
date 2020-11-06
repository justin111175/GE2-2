#pragma once
#include <type_traits>

//“ü—Í‘•’u
enum class conType
{
	Key,
	Pad,
	Max
};

static conType begin(conType) { return conType::Key; }
static conType end(conType) { return conType::Max; }
static conType operator*(conType key) { return key; }
static conType operator++(conType& key) { return key = conType(std::underlying_type<conType>::type(key) + 1); }
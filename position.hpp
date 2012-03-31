
#pragma once

namespace YPT {

template <typename T = int>
struct Position {
	Position()
		:x(0),
		y(0)
	{}

	Position(const T &x, const T &y)
		:x(x),
		y(y)
	{}

	T x, y;
};

}


#pragma once

namespace Math {
	inline static int Modulo(int x, int N) {
		return (x % N + N) % N;
	}
}
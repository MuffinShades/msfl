#pragma once
#include <iostream>

class ptrconverter {
public:
	template<class _Ty, class TargetType > static TargetType* convertTo(_Ty* from, size_t sz1) {
		TargetType* res = new TargetType[sz1];

		for (int i = 0; i < sz1; i++)
			res[i] = (TargetType)from[i];

		return res;
	};
};
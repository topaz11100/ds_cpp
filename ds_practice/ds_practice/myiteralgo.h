#pragma once

template<class iter, class func>
void foreach(iter& i1, iter& i2, const func& f) {
	for (; i1 != i2; ++i1) f(*i1);
}



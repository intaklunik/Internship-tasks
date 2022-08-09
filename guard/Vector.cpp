#include <iostream>
#include <vector>
#include "Guard.h"

void foo(std::vector<int> &v)
{
	int n = 10;
	int i = 0;
	auto cancel = [&v, &i](){while(i--) v.pop_back();};
	guard g = make_guard(cancel);
	try {
        for(i = 0; i < n; ++i)
            v.push_back(i);
        g.dismiss();
	} catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
	}
}

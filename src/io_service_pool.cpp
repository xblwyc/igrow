#include "io_service_pool.hpp"

io_service_pool::io_service_pool(const size_t num_threads) : w(new work(*this))
{
	reserve(num_threads);
	for (size_t i = 0; i < num_threads; ++i)
	{
		emplace_back(boost::async(boost::launch::async, [&]()
		{
			run();
		}));
	}
}

void io_service_pool::wait()
{
	w.reset();
	for (auto& f : *this)
	{
		f.get();
	}
}

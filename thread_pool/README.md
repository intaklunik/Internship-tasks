# Thread Pool
## Task
Implement simple thread pool. Example of necessary but not sufficient class interface:
```cpp
class SimpleThreadPool {
public:
	explicit SimpleThreadPool(std::size_t threadCount);
	~SimpleThreadPool();
	SimpleThreadPool(const SimpleThreadPool &) = delete;
	SimpleThreadPool & operator=(const SimpleThreadPool &) = delete;
	template<typename Fnc_T>
	auto Post(Fnc_T task) -> std::future<decltype(task())>;
	void WorkOn();
	void Destroy();
private:
	size_t m_threadCount;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	std::condition_variable condition;
	std::mutex mut;
	bool stop;
};
```


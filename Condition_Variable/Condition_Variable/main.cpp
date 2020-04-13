#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <fstream>
#include <string>

std::mutex mtx;
std::condition_variable c_var;

bool flag = false;
bool ret() { return flag; }

void foo()
{
	std::ofstream log;
	std::string str;
	static int numFoo = 0;
	
	while (true)
	{
		std::unique_lock<std::mutex>ulm(mtx);
		c_var.wait(ulm, ret);

		log.open("log.log", std::ios_base::app);
		str = "Is foo: " + std::to_string(++numFoo) + 'n';
		log.write(str.c_str(), str.length());
		log.close();
		std::this_thread::sleep_for(std::chrono::microseconds(5000000));

	}

}

void bar()
{
	while (true)
	{
		char input;
		std::cout << "Input: "; std::cin >> input;
		std::cin.clear();

		if (input == 'y')
		{
			flag = true;
			c_var.notify_one();
		}
		if (input == 'n') flag = false;
	}
}

int main(int argc, char** argv)
{	
	std::thread t1(bar);
	std::thread t2(foo);

	t1.join();
	t2.join();


	system("pause >> NULL");
	return 0;
}
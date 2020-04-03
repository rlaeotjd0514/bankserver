#pragma once
#include <functional>
#include <mutex>

using namespace std;

class HI_timer {
public :		
	enum timer_type{ deadline, event };	
	mutex mtx;
	HI_timer(timer_type type_, uint8_t lt, uint16_t duration_, function<void(void)> dcallback_, function<void(void)> ecallback_)://void(*dcallback_)(void), void(*ecallback_)(void)) :
		left_time(lt),
		valid(true),
		dcallback(dcallback_),
		ecallback(ecallback_),
		current_type(type_),
		duration(duration_)
	{			
		
	}

	void start() {		
		thread th([=]() {
			while (left_time > 0) {
				if (current_type == timer_type::event) {
					if (left_time > duration) {
						mtx.lock();
						left_time -= duration;
						mtx.unlock();
					}
					else {
						std::this_thread::sleep_for(std::chrono::seconds(left_time % duration));
						mtx.lock();
						left_time = 0;
						mtx.unlock();
						break;
					}
					std::this_thread::sleep_for(std::chrono::seconds(duration));
					ecallback();
				}
				else {
					mtx.lock();
					left_time -= 1;
					mtx.unlock();
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			}
			dcallback();
			valid = false;
			});
		th.detach();			
	}

	int get_left_time() {
		return left_time;
	}

	void add_to_left_time(int addtime_) {		
		mtx.lock();
		left_time += addtime_;
		mtx.unlock();
	}	

	HI_timer(const HI_timer& rhs):
		left_time(rhs.left_time),
		valid(true),
		dcallback(rhs.dcallback),
		ecallback(rhs.ecallback),
		current_type(rhs.current_type),
		duration(rhs.duration)
	{

	}

	HI_timer operator=(const HI_timer& rhs) {
		return rhs;
	}

private :
	uint16_t left_time;
	uint16_t duration;
	bool valid;
	function<void(void)> dcallback;
	function<void(void)> ecallback;
	timer_type current_type;
};
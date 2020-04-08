#pragma once
#include <functional>
#include <mutex>
#include <future>
#include <iostream>

using namespace std;

class HI_timer {
private:
	enum timer_type{ deadline, event };	
	thread thd;
	future<void> fobj;
	promise<void> exit_signal;
	uint16_t left_time;
	uint16_t duration;
	bool valid;
	function<void(void)> dcallback;
	function<void(void)> ecallback;
	timer_type current_type;

	friend class session;
public :		
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
		auto& ths = *this;			
		auto& fobj_ = fobj = exit_signal.get_future();
		thd = thread([ths, &fobj_]() {
			while (ths.left_time > 0) {											
				if (ths.current_type == timer_type::event) {
					if (ths.left_time > ths.duration) {
						const_cast<HI_timer&>(ths).mtx.lock();
						const_cast<HI_timer&>(ths).left_time -= ths.duration;
						const_cast<HI_timer&>(ths).mtx.unlock();
					}
					else {
						std::this_thread::sleep_for(std::chrono::seconds(ths.left_time % ths.duration));
						const_cast<HI_timer&>(ths).mtx.lock();
						const_cast<HI_timer&>(ths).left_time = 0;
						const_cast<HI_timer&>(ths).mtx.unlock();
						break;
					}
					std::this_thread::sleep_for(std::chrono::seconds(ths.duration));
					ths.ecallback();
				}
				else {
					const_cast<HI_timer&>(ths).mtx.lock();
					const_cast<HI_timer&>(ths).left_time -= 1;
					const_cast<HI_timer&>(ths).mtx.unlock();
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			}
			ths.dcallback();
			const_cast<HI_timer&>(ths).mtx.lock();
			const_cast<HI_timer&>(ths).valid = false;
			const_cast<HI_timer&>(ths).mtx.unlock();
			});
		thd.detach();		
	}

	int get_left_time() {
		return left_time;
	}

	void add_to_left_time(int addtime_) {		
		mtx.lock();
		left_time += addtime_;
		mtx.unlock();
	}	

	void set_left_time(int left_time_) {
		mtx.lock();
		left_time = left_time_;
		mtx.unlock();
	}

	bool stop_reqed() const {
		if (fobj.wait_for(chrono::milliseconds(0)) == future_status::timeout) return false;
		return true;
	}

	void expire_timer() {
		this->exit_signal.set_value();		
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
};
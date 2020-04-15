/*
1. 특정 시간 이후에 콜백 함수 호출
2. 정해진 시간이 지나기 전에 인터럽 가능
3. 타이머가 동작하는 중에 남은 시간 수정 가능
수정사항::
대기시간의 단위를 작게하면 반복문을 도는 횟수가 많아져 설정한 시간보다 오래대기함 추후 타이머 클래스 재작성 필요
*/
#pragma once
#include "pch.h"

using namespace std;

class HI_timer {
private:
	enum class timer_type{ deadline, event };	
	thread thd;
	future<void> fobj;
	promise<void> exit_signal;
	atomic<uint16_t> left_time;
	atomic<bool> valid;
	uint16_t duration;
	function<void(void)> dcallback;
	function<void(void)> ecallback;
	timer_type current_type;

	friend class session;
public :		
	mutex mtx;
	HI_timer(timer_type type_, uint32_t lt, uint16_t duration_, function<void(void)> dcallback_, function<void(void)> ecallback_)://void(*dcallback_)(void), void(*ecallback_)(void)) :
		left_time(lt),
		valid(true),
		dcallback(dcallback_),
		ecallback(ecallback_),
		current_type(type_),
		duration(duration_)
	{			
		
	}

	void start(HI_timer * self) {		
		auto& ths = *this;			
		thd = thread([&ths](HI_timer * self_) {			
			future<void> fobj_ = self_->exit_signal.get_future();
			while (self_->left_time > 0 && fobj_.wait_for(chrono::seconds(1)) == future_status::timeout) {				
				/*if (ths.current_type == timer_type::event) {
					if (ths.left_time > ths.duration) {
						const_cast<HI_timer&>(ths).mtx.lock();
						const_cast<HI_timer&>(ths).left_time -= ths.duration;
						const_cast<HI_timer&>(ths).mtx.unlock();
					}
					else {
						std::this_thread::sleep_for(std::chrono::milliseconds(ths.left_time % ths.duration));
						const_cast<HI_timer&>(ths).mtx.lock();
						const_cast<HI_timer&>(ths).left_time = 0;
						const_cast<HI_timer&>(ths).mtx.unlock();
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(ths.duration));
					ths.ecallback();
				}*/				
				/*const_cast<HI_timer&>(ths).mtx.lock();
				const_cast<HI_timer&>(ths).left_time -= 1;
				const_cast<HI_timer&>(ths).mtx.unlock();	*/					
				self_->left_time -= 1;				
				//std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			ths.dcallback();			
			self_->valid = false;			
			}, self);		
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

	void expire_timer() {
		this->exit_signal.set_value();
	}

	HI_timer(const HI_timer& rhs) = delete;	
};
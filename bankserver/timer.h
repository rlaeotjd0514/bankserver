/*
1. Ư�� �ð� ���Ŀ� �ݹ� �Լ� ȣ��
2. ������ �ð��� ������ ���� ���ͷ� ����
3. Ÿ�̸Ӱ� �����ϴ� �߿� ���� �ð� ���� ����
��������::
���ð��� ������ �۰��ϸ� �ݺ����� ���� Ƚ���� ������ ������ �ð����� ��������� ���� Ÿ�̸� Ŭ���� ���ۼ� �ʿ�
*/
#pragma once
#include "pch.h"

using namespace std;


///<summary><br>timer is used in setting session's expire time.</br>
///<br>functions asynchronously and calls an event or callback methods when left time has expired</br>
///</summary>
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

	///<summary><br>starts timer</br>
	///<br>self pointer is timer pointer, for saving the timer class values</br></summary>
	void start(HI_timer * self) {		
		auto& ths = *this;			
		thd = thread([&ths](HI_timer * self_) {			
			future<void> fobj_ = self_->exit_signal.get_future();
			while (self_->left_time > 0 && fobj_.wait_for(chrono::seconds(1)) == future_status::timeout) {											
				self_->left_time -= 1;				
			}
			ths.dcallback();			
			self_->valid = false;			
			}, self);		
		thd.detach();
	}

	///<summary>returns timer left time</summary>
	int get_left_time() {
		return left_time.load();
	}

	///<summary>extend left time while running timer</summary>
	void add_to_left_time(int addtime_) {		
		mtx.lock();
		left_time += addtime_;
		mtx.unlock();
	}	

	///<summary>set left time to specific time</summary>
	void set_left_time(int left_time_) {
		mtx.lock();
		left_time = left_time_;
		mtx.unlock();
	}	

	///<summary>expire timer</summary>
	void expire_timer() {
		this->exit_signal.set_value();
	}

	HI_timer(const HI_timer& rhs) {
		this->current_type = rhs.current_type;
		this->dcallback = rhs.dcallback;
		this->duration = rhs.duration;
		this->ecallback = rhs.ecallback;
		this->left_time.store(rhs.left_time.load());
		this->valid.store(rhs.left_time.load());
	}

	void operator=(const HI_timer& rhs) {
		this->current_type = rhs.current_type;
		this->dcallback = rhs.dcallback;
		this->duration = rhs.duration;
		this->ecallback = rhs.ecallback;
		this->left_time.store(rhs.left_time.load());
		this->valid.store(rhs.left_time.load());
	}

	HI_timer() {}
};
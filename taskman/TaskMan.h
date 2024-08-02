/*


*/
#pragma once

#include <inttypes.h>


class Task {
	public:
		Task(uint32_t aperiod=1);
		virtual ~Task();
		
		bool start();
		void stop();
		virtual void run() {};
		
		void period(uint32_t aperiod) { _period = aperiod; }
		uint32_t period() const { return _period; }
		bool isrunning() const { return _isrunning; }
	
	private:
		uint32_t _period = 1;
		bool _isrunning = false;
};


class TaskMan {
  struct taskstruct {
    taskstruct(Task *atask=nullptr, uint32_t atick=0) : task(atask), tick(atick) {}
    Task *task;
    int32_t tick;
  };

	public:
		TaskMan();
		~TaskMan();
		
		void begin(uint8_t anslots = 8, uint32_t now = 0);
		void end();
		
		bool add(Task *atask);
		bool remove(Task *atask);
		
		void process(uint32_t now);
		
		uint8_t nslots() const {return _nslots; }
		uint8_t usedslots() const;
   
	private:
		taskstruct *_tasks=nullptr;
		uint32_t _lastmillis=0;
    uint8_t _nslots=0;
};

extern TaskMan taskman;


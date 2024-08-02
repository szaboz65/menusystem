/*

*/

#include "TaskMan.h"

Task::Task(uint32_t aperiod) : _period(aperiod) {
	
}

Task::~Task() {
	stop();
}

bool Task::start() {
	if( taskman.add(this)) {
	  _isrunning = true;
	}
  return isrunning();
}

void Task::stop() {
	_isrunning = false;
	taskman.remove(this);
}



TaskMan::TaskMan() {
}

TaskMan::~TaskMan() {
	end();
}

void TaskMan::begin(uint8_t anslots, uint32_t now) {
	end();
	_tasks = new taskstruct[anslots];
	_nslots = _tasks ? anslots : 0;
	_lastmillis = now;
}

void TaskMan::end() {
	if( _tasks) {
		delete [] _tasks;
		_tasks = nullptr;
	}
	_nslots = 0;
}

bool TaskMan::add(Task *atask) {
	remove(atask);
	for(uint8_t i=0; i<_nslots; ++i) {
		if( !_tasks[i].task ) {
      _tasks[i] = taskstruct(atask, (int32_t)atask->period());
			return true;
		}
	}
	return false;
}

bool TaskMan::remove(Task *atask){
	for(uint8_t i=0; i<_nslots; ++i) {
		if( _tasks[i].task == atask) {
			_tasks[i] = taskstruct();
			return true;
		}
	}
	return false;
}

uint8_t TaskMan::usedslots() const {
  uint8_t n=0;
  for(uint8_t i=0; i<_nslots; ++i) 
    if( _tasks[i].task) ++n;
  return n;
}

void TaskMan::process(uint32_t now) {
	int32_t dt = now - _lastmillis;
	_lastmillis = now;

	for(uint8_t i=0; i<_nslots; ++i) {
    taskstruct *t = _tasks + i; 
		if( t->task) {
			if(t->tick == 0) {
				t->task->run();
			}
			else if( dt) {
				t->tick -= dt;
				if(t->tick <= 0) {
					while( (t->tick += (int32_t)t->task->period()) <= 0);
					t->task->run();
				}
			}
		}
	}
}


TaskMan taskman;

/*
	OS independent generic rotary encoder model.
  Based on the Observer pattern the rotary model notify the listener object.
  This implementation has only one listener.
  https://en.wikipedia.org/wiki/Observer_pattern
	
*/
#pragma once

#define MOVES_PER_CLICK 4

class RotaryModel;

class RotaryListener {
  public:
    enum class Events {
      Changed,
      Left,
      Right
    };

    RotaryListener(RotaryModel &model);
    virtual ~RotaryListener() {}
    virtual void EventHandler(RotaryListener::Events theevent) = 0;

    RotaryModel &model() const {return _model; }
    
  private:
    RotaryModel &_model;
};


class RotaryModel {
  public:
		enum class Directions {
			Unknow,
			Left,
			Right
		};

		RotaryModel(int moves_per_click = MOVES_PER_CLICK);
    void addlistener(RotaryListener *listener) {
      _listener = listener;
    }
    void notify(RotaryListener::Events theevent);
	
    inline Directions direction() const {return _direction; }
    inline int moves_per_click() const {return _moves_per_click;}
    void moves_per_click(int amoves_per_click);
		
		int position() const { return _position / _moves_per_click; }
		void reset();

    void enable(bool statea = true, bool stateb = true);
    void disable();
    bool enabled() const {return _enabled;}
		void process(bool statea, bool stateb);
	
	private:
    RotaryListener *_listener = nullptr;
    int _prev_state = 0;
		int _moves_per_click = MOVES_PER_CLICK;
		int _position = 0;
    int _last_position = 0;
		Directions _direction = Directions::Unknow;
    bool _enabled = false;

    int make_state(bool statea, bool stateb);
};

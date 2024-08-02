/*
	OS independent generic button model.
  Based on the Observer pattern the button model notify the only one listener object.
  This implementation has only one listener.
  https://en.wikipedia.org/wiki/Observer_pattern
*/
#pragma once

#define DEBOUNCE_MS      50
#define LONGCLICK_MS    250
#define DOUBLECLICK_MS  400

class ButtonModel;

class ButtonListener {
  public:
    enum class Events {
      Changed,
      Pressed,
      Released,
      Click,
      LongClick,
      DoubleClick,
      TripleClick
    };

    ButtonListener(ButtonModel &model);
    virtual ~ButtonListener() {}
    virtual void EventHandler(ButtonListener::Events theevent) = 0;

    ButtonModel &model() const {return _model; }
    
  private:
    ButtonModel &_model;
};


class ButtonModel {
  public:
    ButtonModel() {};
    void addlistener(ButtonListener *listener) {
      _listener = listener;
    }
    void notify(ButtonListener::Events theevent);
    
    unsigned int debounce_time_ms() const {
      return _debounce_time_ms;
    }
    void debounce_time_ms(unsigned int ms) {
      _debounce_time_ms = ms;
    }
    unsigned int longclick_time_ms() const {
      return _longclick_time_ms;
    }
    void longclick_time_ms(unsigned int ms) {
      _longclick_time_ms = ms;
    }
    unsigned int doubleclick_time_ms() const {
      return _doubleclick_time_ms;
    }
    void doubleclick_time_ms(unsigned int ms) {
      _doubleclick_time_ms = ms;
    }
    bool enabled() const {
      return _enabled;
    }

    void enable(bool astate = true);
    void disable();
    void process(bool astate, unsigned long now);

  private:
    ButtonListener *_listener = nullptr;
    unsigned long _click_ms = 0;
    unsigned long _down_ms = 0;
    unsigned int _debounce_time_ms = DEBOUNCE_MS;
    unsigned int _longclick_time_ms = LONGCLICK_MS;
    unsigned int _doubleclick_time_ms = DOUBLECLICK_MS;
    unsigned int _click_count = 0;
    bool _prev_state = true;
    bool _pressed_triggered = false;
    bool _longclick_detected = false;
    bool _enabled = false;

};

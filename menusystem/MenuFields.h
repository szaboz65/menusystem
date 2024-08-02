/*
	Input fields for the menusystem
*/

#pragma once

#include "MenuSystem.h"
#include "WString.h"


// *********************************************************
// FieldList
// *********************************************************
class FieldList : public MenuItem {
  public:
    FieldList(const char * name, const char **items);
    virtual ~FieldList() {};

    //! \brief is_focusable is true if the component can grab the focus
    virtual bool is_focusable() const {
      return true;
    }

    //! \copydoc MenuComponent::render
    virtual void render(MenuRenderer const& renderer) const override;

    //! \copydoc MenuComponent::next
    virtual bool next(bool loop = false) override;

    //! \copydoc MenuComponent::prev
    virtual bool prev(bool loop = false) override;

    //! \copydoc MenuComponent::select
    virtual bool select(int mode = 0) override;

    //! \copydoc MenuComponent::back
    virtual bool back() override;

    //! \copydoc MenuComponent::reset
    virtual void reset() override;

    void selected(int selected);

    int count() const {
      return _nitems;
    }
    int selected() const {
      return _selected_item;
    }
    const char *value() const;

  private:
    const char **_items = nullptr;
    int _original_item = 0;
    int _selected_item = 0;
    int _nitems = 0;

    int count_items() const;

};


// *********************************************************
// FieldEdit
// *********************************************************
class FieldEdit : public MenuItem {

  public:
    enum class CharSet {
      Alpha,
      AlphaCaps,
      AlphaNum,
      Url,
      IntNumber,
      FloatNumber,
      SignNumbers
    };

    FieldEdit(const char * name, uint8_t length);
    virtual ~FieldEdit() {};

    //! \brief is_focusable is true if the component can grab the focus
    virtual bool is_focusable() const {
      return true;
    }

    //! \copydoc MenuComponent::render
    virtual void render(MenuRenderer const& renderer) const override;

    //! \copydoc MenuComponent::next
    virtual bool next(bool loop = false) override;

    //! \copydoc MenuComponent::prev
    virtual bool prev(bool loop = false) override;

    //! \copydoc MenuComponent::select
    virtual bool select(int mode = 0) override;

    //! \copydoc MenuComponent::back
    virtual bool back() override;

    //! \copydoc MenuComponent::reset
    virtual void reset() override;

    //! \brief validate the current input
    //!
    //! \returns true if the input is invalid, false otherwise.
    //!
    //! \note The valid input will be saved
    virtual bool validate() {
      return false;
    }

    void text(String atext);

    int length() const {
      return _length;
    }
    int current_index() const {
      return _current_index;
    }
    String text() const;

    void charset(CharSet acharset);
    void charset(const char *acharset);

    bool is_enabled() const {
      return _enabled;
    }
    void set_enabled(bool en)  {
      _enabled = en;
    }
	
    bool is_password() const {
      return _password;
    }
    void set_password(bool pw)  {
      _password = pw;
    }
	
  protected:
    int _charset_index = 0;

    int find_charset_index(char letter);
    void current_index(int index);

  private:
    String _original_text;
    String _text;
    const char *_charset = alpha;
    int _current_index = 0;
    int _length = 0;
	bool _enabled = true;
	bool _password = false;

    static const char alpha[];
    static const char alphacaps[];
    static const char alphanum[];
    static const char url[];
    static const char intnumbers[];
    static const char floatnumbers[];
    static const char signnumbers[];
};


// *********************************************************
// FieldAlphaCapsEdit
// *********************************************************
class FieldAlphaCapsEdit : public FieldEdit {

  public:
    FieldAlphaCapsEdit(const char * name, uint8_t length);
};


// *********************************************************
// FieldAlphaNumEdit
// *********************************************************
class FieldAlphaNumEdit : public FieldEdit {

  public:
    FieldAlphaNumEdit(const char * name, uint8_t length);
};

// *********************************************************
// FieldUrlEdit
// *********************************************************
class FieldUrlEdit : public FieldEdit {

  public:
    FieldUrlEdit(const char * name, uint8_t length);
};

// *********************************************************
// FieldIntEdit
// *********************************************************
class FieldIntEdit : public FieldEdit {

  public:
    FieldIntEdit(const char * name, int min = 0, int max = 9999);
    virtual ~FieldIntEdit() {};

    //! \copydoc MenuComponent::render
    virtual void render(MenuRenderer const& renderer) const override;

    //! \copydoc MenuComponent::select
    virtual bool validate() override;

    int value() const;
    void value(int val);

    int min() const {
      return _imin;
    }
    int max() const {
      return _imax;
    }

  private:
    int _imin = 0;
    int _imax = 0;

};

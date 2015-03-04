//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 11:15:04 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "keyboard_mouse_driver.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace input
{
namespace pc
{
	
	keyboard_mouse_driver::keyboard_mouse_driver() :
		m_driver_id(0)
	{}
	
	bool keyboard_mouse_driver::initialise(int driver_id)		
	{
		m_driver_id = driver_id;
		return true;
	}

	void keyboard_mouse_driver::update(event_handler *)
	{
	}
	
	int keyboard_mouse_driver::get_num_devices() const
	{
		return 2;
	}
	
	const device_description* keyboard_mouse_driver::get_device_desc(int i) const
	{
		TYCHO_ASSERT(i < 2);
		static device_description desc[2] =  {
			{ make_device_id(m_driver_id, 0), device_keyboard, "PC Keyboard", 0 },
			{ make_device_id(m_driver_id, 1), device_mouse, "Mouse", 0 }
		};
		return &desc[i];			
	}	

} // end namespace
} // end namespace
} // end namespace

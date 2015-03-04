//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 11:15:12 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "xinput_driver.h"
#include "input/types.h"
#include "core/memory.h"
#include "core/debug/utilities.h"


#define TYCHO_TRACE_INPUT(_v) //_v

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace input
{
namespace pc
{

	namespace detail
	{
		float get_normalised_axis(int val, int deadzone)
		{
			// clamp to the deadzone and scale the remaining range into [0,1]
			if(val >= 0 && val < deadzone)
				val = 0;
			else if(val < 0 && val >= -deadzone)
				val = 0;
			else if (val < 0)
				val += deadzone;
			else 
				val -= deadzone;
				
			return ((float)val) / (32768 - deadzone);
		}
	}
	
	/// constructor
	xinput_driver::xinput_driver() :
		m_num_devices(0)
	{
		core::mem_zero(m_devices, sizeof(device) * MaxDevices);
	}
	
	
	/// destructor
	xinput_driver::~xinput_driver()
	{
	}
		
	bool xinput_driver::initialise(int driver_id)
	{
		m_driver_id = driver_id;
		XInputEnable(true);
		enumerate_devices();
		return true;
	}
	
	
	/// called frequently to let the driver push any input events onto the input stream.
	void xinput_driver::update(event_handler *handler)
	{
		for(int i = 0; i < MaxDevices; ++i)
		{
			device &d = m_devices[i];		
			XINPUT_STATE state;
			if(XInputGetState(i, &state) == ERROR_SUCCESS)
			{
				d.m_connected = true;
				
				// don't generate any events for first update just store it.
				if(d.m_packet_num == 0)
				{
					core::mem_cpy(&d.m_device.Gamepad, &state.Gamepad, sizeof(XINPUT_GAMEPAD));
					d.m_packet_num = state.dwPacketNumber;				
				}				
				// compare this state to last state and trigger any events
				else if(d.m_packet_num != (int)state.dwPacketNumber)
				{
					// check to see which button were down and now are up so we can generate a button up event
					struct button_map
					{
						int xinput;
						key_type tycho;
					};
					const button_map ButtonMasks[] = {
						{ XINPUT_GAMEPAD_DPAD_UP, key_button_dpad_up },
						{ XINPUT_GAMEPAD_DPAD_DOWN, key_button_dpad_down },
						{ XINPUT_GAMEPAD_DPAD_LEFT, key_button_dpad_left },
						{ XINPUT_GAMEPAD_DPAD_RIGHT, key_button_dpad_right },
						{ XINPUT_GAMEPAD_START, key_button_start },
						{ XINPUT_GAMEPAD_BACK, key_button_back },
						{ XINPUT_GAMEPAD_LEFT_THUMB, key_button_left_thumb },
						{ XINPUT_GAMEPAD_RIGHT_THUMB, key_button_right_thumb },
						{ XINPUT_GAMEPAD_LEFT_SHOULDER, key_button_left_shoulder },
						{ XINPUT_GAMEPAD_RIGHT_SHOULDER, key_button_right_shoulder },
						{ XINPUT_GAMEPAD_A, key_button_a },
						{ XINPUT_GAMEPAD_B, key_button_b },
						{ XINPUT_GAMEPAD_X, key_button_x },
						{ XINPUT_GAMEPAD_Y, key_button_y },
						{ 0, key_invalid }};
					
					// check buttons
					for(const button_map* m = ButtonMasks; m->xinput; ++m)
					{
						bool is_down = (state.Gamepad.wButtons & m->xinput) > 0;
						bool was_down  = (d.m_device.Gamepad.wButtons & m->xinput) > 0;
						
						// if it wasn't down but is now generate a button press event
						if(!was_down && is_down)
						{
							handler->handle_keyboard_event(d.m_desc.id, make_keyboard_packet(m->tycho, key_state_down));
							TYCHO_TRACE_INPUT(core::debug::write_ln("button pressed : %d", m->tycho));
						}
						// if it was down and now isn't generate a button release event
						else if(was_down && !is_down)
						{
							handler->handle_keyboard_event(d.m_desc.id, make_keyboard_packet(m->tycho, key_state_up));
							TYCHO_TRACE_INPUT(core::debug::write_ln("button released : %d", m->tycho));
						}
					}
					
					// check sticks
					if(d.m_device.Gamepad.sThumbLX != state.Gamepad.sThumbLX)					
					{
						float lthumb_x = detail::get_normalised_axis(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
						handler->handle_axis_event(d.m_desc.id, make_axis_packet(axis_lthumb_x, lthumb_x));														
					}
					if(d.m_device.Gamepad.sThumbLY != state.Gamepad.sThumbLY)					
					{
						float lthumb_y = detail::get_normalised_axis(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
						handler->handle_axis_event(d.m_desc.id, make_axis_packet(axis_lthumb_y, lthumb_y));														
					}
					if(d.m_device.Gamepad.sThumbRX != state.Gamepad.sThumbRX)					
					{					
						float rthumb_x = detail::get_normalised_axis(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
						handler->handle_axis_event(d.m_desc.id, make_axis_packet(axis_rthumb_x, rthumb_x));														
					}
					if(d.m_device.Gamepad.sThumbRY != state.Gamepad.sThumbRY)					
					{										
						float rthumb_y = detail::get_normalised_axis(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);														
						handler->handle_axis_event(d.m_desc.id, make_axis_packet(axis_rthumb_y, rthumb_y));														
					}
					
					// save current state
					core::mem_cpy(&d.m_device.Gamepad, &state.Gamepad, sizeof(XINPUT_GAMEPAD));
					d.m_packet_num = state.dwPacketNumber;
				}				
			}
			else
			{
				d.m_connected = false;
			}
		}
	}
	
	/// \returns the number of devices available from this driver
	int xinput_driver::get_num_devices() const
	{
		return m_num_devices;		
	}
	
	/// \returns the i'th device 
	const device_description* xinput_driver::get_device_desc(int index) const
	{
		int num_connected = -1;
		for(int i = 0; i < MaxDevices; ++i)
		{
			const device &d = m_devices[i];
			if(d.m_connected)
				++num_connected;
			if(num_connected == index)
				return &d.m_desc;
		}
		return 0;
	}
	
	void xinput_driver::enumerate_devices()
	{
		for(int i = 0; i < MaxDevices; ++i)
		{
			device &d = m_devices[i];
			DWORD res = XInputGetCapabilities(i, XINPUT_FLAG_GAMEPAD, &d.m_device);
			d.m_connected = res == ERROR_SUCCESS;
			if(d.m_connected)
			{
				d.m_desc.id = make_device_id(m_driver_id, i);
				d.m_desc.index = i;
				d.m_desc.name  = "XBox 360 Controller";
				d.m_desc.type  = device_xenoncontroller;
				++m_num_devices;
			}
		}		
	}

} // end namespace
} // end namespace
} // end namespace

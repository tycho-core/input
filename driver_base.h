//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 8:41:35 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __DRIVER_BASE_H_0F6E7F35_E1B2_4B77_867B_56AF0BE1D197_
#define __DRIVER_BASE_H_0F6E7F35_E1B2_4B77_867B_56AF0BE1D197_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "input/input_abi.h"
#include "input/types.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace input
{

	/// base of all input drivers. 
	class TYCHO_INPUT_ABI driver_base
    {
    public:
		struct TYCHO_INPUT_ABI event_handler
		{
			virtual void handle_mouse_event(int /*device_id*/, const mouse_packet&) {}
			virtual void handle_keyboard_event(int /*device_id*/, const keyboard_packet&) {}
			virtual void handle_axis_event(int /*device_id*/, const axis_packet&) {}
		};
		
    public:
		/// destructor
		virtual ~driver_base() {}
		
		/// initialise the driver
		virtual bool initialise(int driver_id) = 0;
		
		/// called frequently to let the driver push any input events onto the input stream.
		virtual void update(event_handler *) = 0;				    
		
		/// \returns the number of devices available from this driver
		virtual int get_num_devices() const = 0;
		
		/// \returns the i'th device 
		virtual const device_description* get_device_desc(int i) const = 0;
    };

	/// construct a device id from a driver id and device number
	inline core::int32 make_device_id(int driver_id, int device_num)
	{
		return (driver_id << 16) + device_num;
	}

	/// split a device id into it's constituant parts.
	inline void split_device_id(int device_id, int* driver_id, int* device_num)
	{
		*driver_id = device_id >> 16;
		*device_num = device_id & 0xffff;
	}

} // end namespace
} // end namespace

#endif // __DRIVER_BASE_H_0F6E7F35_E1B2_4B77_867B_56AF0BE1D197_
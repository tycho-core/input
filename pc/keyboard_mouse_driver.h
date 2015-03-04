//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 11:15:03 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __KEYBOARD_MOUSE_DRIVER_H_9B65A040_BEDB_44C3_9D13_566FDFC90F62_
#define __KEYBOARD_MOUSE_DRIVER_H_9B65A040_BEDB_44C3_9D13_566FDFC90F62_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "input/input_abi.h"
#include "input/driver_base.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace input
{
namespace pc
{
 
    class TYCHO_INPUT_ABI keyboard_mouse_driver : public driver_base
    {
    public:
		keyboard_mouse_driver();
		virtual bool initialise(int driver_id);
		virtual void update(event_handler *);				    
		virtual int get_num_devices() const;
		virtual const device_description* get_device_desc(int i) const;    
	private:
		int m_driver_id;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __KEYBOARD_MOUSE_DRIVER_H_9B65A040_BEDB_44C3_9D13_566FDFC90F62_

//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 11:15:11 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __XINPUT_DRIVER_H_5824DE7A_3801_4AF1_8711_232B1B5F366E_
#define __XINPUT_DRIVER_H_5824DE7A_3801_4AF1_8711_232B1B5F366E_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "input/input_abi.h"
#include "input/driver_base.h"
#include "core/pc/safe_windows.h"
#include "d3d/include/XInput.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace input
{
namespace pc
{
 
	/// Windows XInput driver, exposes 360 controllers.
    class TYCHO_INPUT_ABI xinput_driver : public driver_base
    {
    public:
		/// constructor
		xinput_driver();
		
		/// destructor
		virtual ~xinput_driver();
		
		/// \name driver_base interface
		//@{		
		virtual bool initialise(int driver_id);
		virtual void update(event_handler *);				    
		virtual int get_num_devices() const;		
		virtual const device_description* get_device_desc(int i) const;		    
		//@}
		
	private:
		void enumerate_devices();
		
		static const int MaxDevices = 4;
		
		struct device
		{
			device_description  m_desc;
			XINPUT_CAPABILITIES m_device;
			bool				m_connected;
			int					m_packet_num;
			bool				m_gamepad_state_valid;
		};
		int		m_driver_id;
		device	m_devices[MaxDevices];
		int		m_num_devices;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __XINPUT_DRIVER_H_5824DE7A_3801_4AF1_8711_232B1B5F366E_

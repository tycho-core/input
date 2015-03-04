//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 7:44:58 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __EVENT_PACKET_H_91F32CF8_BA69_40BA_BBE1_819F37C4C272_
#define __EVENT_PACKET_H_91F32CF8_BA69_40BA_BBE1_819F37C4C272_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "input/input_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace input
{
	/// input event types. 
	enum event
	{
		/// mouse has been moved
		event_mouse_move,
		
		/// key has been released
		event_key_up,
		
		/// key has been pressed
		event_key_down,
		
		/// translated unicode character
		event_char,
		
		/// 
		event_
	};
	
	struct mouse_packet
	{
		int x;
		int y;
	};
	
	struct float_packet
	{
		float val;
	};
	
	struct key_packet
	{
	
	};

	struct event_packet
	{
	};

} // end namespace
} // end namespace

#endif // __EVENT_PACKET_H_91F32CF8_BA69_40BA_BBE1_819F37C4C272_

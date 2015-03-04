//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 9:25:17 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TYPES_H_EE47A0E5_E049_4BFD_938A_B3834085849A_
#define __TYPES_H_EE47A0E5_E049_4BFD_938A_B3834085849A_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "input/input_abi.h"
#include "core/memory.h"
#include "core/hash.h"
#include "math/vector2.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace input
{

	/// input device type
	enum device_type
	{
		device_unknown = 0,
		device_keyboard,
		device_mouse,
		device_xenoncontroller,
		device_gccontroller,
		device_wiimote,
		device_count
	};
	
	/// device description
	struct device_description
	{
		int			id;		///< unique id of this device
		device_type type;	///< type of the device
		const char* name;	///< user friendly device name 
		int			index;	///< in case of multiple input devices of the same type this is the unique identifier.
	};
	
	/// types of input, these allow the action publisher to specify requirements on
	/// the input type. This allows the binding interface to restrict what is allowed 
	/// per action to sensible options that will work.
	enum event_type
	{
		/// invalid event type
		event_type_invalid = 0,
		
		/// no event type, distinct from invalid so we can distinguish them
		event_type_none,
		
		///< axis distance normalised in range 0 - 1
		event_type_axis,		
		
		///< x and y deltas
		event_type_mouse,	

		///< key up, key down, button press, button release
		event_type_key,		
	};
	
	
	/// input keys
	enum key_type
	{
		key_invalid,
		
		// numeric keys
		key_0,
		key_1,
		key_2,
		key_3,
		key_4,
		key_5,
		key_6,
		key_7,
		key_8,
		key_9,

		// alpha keys
		key_a,
		key_b,
		key_c,
		key_d,
		key_e,
		key_f,
		key_g,
		key_h,
		key_i,
		key_j,
		key_k,
		key_l,
		key_m,
		key_n,
		key_o,
		key_p,
		key_q,
		key_r,
		key_s,
		key_t,
		key_u,
		key_v,
		key_w,
		key_x,
		key_y,
		key_z,
		
		// 
		// windows specific keys
		key_win_lalt,
		key_win_ralt,
		
		// gamepad buttons		
		key_button_a,
		key_button_b,
		key_button_x,
		key_button_y,
		key_button_back,
		key_button_start,
		key_button_left_thumb,
		key_button_right_thumb,
		key_button_right_shoulder,
		key_button_left_shoulder,
		key_button_mouse_left,
		key_button_mouse_middle,
		key_button_mouse_right,
		key_button_dpad_up,
		key_button_dpad_down,
		key_button_dpad_left,
		key_button_dpad_right		
	};
	
	/// input axis
	enum axis_type
	{
		axis_type_invalid = 0,
		axis_lthumb_x,
		axis_lthumb_y,
		axis_rthumb_x,
		axis_rthumb_y,
		axis_rtrigger_x,
		axis_ltrigger_x,				
	};

	/// key states
	enum key_state
	{
		key_state_invalid = 0,
		key_state_up,
		key_state_down
	};
			
	/// published action, this can be associated with a key combination to trigger it.
	/// this combination must contain one source meeting the requirements but can have other
	/// modifiers required to trigger it
	struct action
	{
		const char* name;
		int			id;
		event_type  requirements;
	};
		
	
	/// an input into a key binding
	struct input
	{
		event_type event;
		key_type   key;
		key_state  state;
		axis_type  axis;				
	};
	
	/// input equality function
	/// \warning all unused input fields must be initialised to zero
	inline bool operator==(const input& lhs, const input& rhs)
	{
		return (lhs.event == rhs.event &&
		        lhs.key == rhs.key &&
		        lhs.state == rhs.state &&
		        lhs.axis == rhs.axis);
	}
	
	/// input hashing function
	inline core::uint32 hash(const input& i)
	{
		return core::hash_raw_bytes(i);
	}
	
	/// helper to define an axis input 
	inline input make_axis_input(axis_type t)
		{ input i; core::mem_zero(i); i.event = event_type_axis; i.axis = t; return i; }

	/// helper to define a mouse input
	inline input make_mouse_input()
		{ input i; core::mem_zero(i); i.event = event_type_mouse; return i; }
	
	///  helper to define key input (or button)
	inline input make_keyboard_input(key_type k, key_state s)	
		{ input i; core::mem_zero(i); i.event = event_type_key; i.key = k; i.state = s; return i; }
		
	inline input make_empty_input()
		{ input i; core::mem_zero(i); i.event = event_type_none; return i; }

	/// key binding
	struct binding
	{
		const char* action;
		input		trigger;
	};
						
	/// mouse packet
	struct mouse_packet
	{
		int dx;
		int dy;
	};
	
	enum packet_type
	{
		packet_type_keyboard,
		packet_type_mouse,
		packet_type_axis
	};
		
	/// keyboard packet
	struct keyboard_packet
	{
		key_state state;
		key_type  key;
	};
	
	/// axis packet
	struct axis_packet
	{
		axis_type axis;
		float	  value;
	};
	
	/// helper function to make an axis packet
	inline axis_packet make_axis_packet(axis_type t, float v)
		{ axis_packet p; p.axis = t; p.value = v; return p;	}

	/// helper function to make an mouse packet
	inline mouse_packet make_mouse_packet(int dx, int dy)
		{ mouse_packet p; p.dx = dx; p.dy = dy; return p; }

	/// helper function to make an keyboard packet
	inline keyboard_packet make_keyboard_packet(key_type type, key_state state)
		{ keyboard_packet p; p.state = state; p.key = type; return p;	}
	
	/// input event packet
	struct event_packet
	{	
		int			timestamp;
		int			index;
		packet_type ptype;
		union
		{
			mouse_packet	mouse;
			keyboard_packet keyboard;
			axis_packet		axis;
		};
	};

	/// input handler interface
	class TYCHO_INPUT_ABI input_handler
	{
	public:
		virtual bool handle_mouse(int /*action_id*/, int /*dx*/, int /*dy*/) { return false; }
		virtual bool handle_axis(int /*action_id*/, const float /*value*/) { return false; }
		virtual bool handle_button(int /*action_id*/) { return false; }
		virtual bool handle_key(int /*action_id*/, key_type /*key*/, key_state /*state*/) { return false; }
	};	

	/** \page ihpage Input handlers
		<code>
		action_group g = { 
			"Player", 0, 
			 { 
				{ "TurnLeft",  0, event_type_axis },
			    { "TurnRight", 1, event_type_axis }
			 }
		};
		</code>
	
	*/
} // end namespace
} // end namespace


#endif // __TYPES_H_EE47A0E5_E049_4BFD_938A_B3834085849A_

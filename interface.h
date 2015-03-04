//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 11:00:23 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __INTERFACE_H_871EE0AF_9DED_4FBA_8274_CF60DE815E84_
#define __INTERFACE_H_871EE0AF_9DED_4FBA_8274_CF60DE815E84_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "input/input_abi.h"
#include "input/types.h"
#include "input/driver_base.h"
#include "core/debug/assert.h"
#include "core/containers/scoped_hash_table.h"
#include <vector>
#include <map>
#include <string>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace input
{
	class driver_base;
	
    class TYCHO_INPUT_ABI interface :
		public driver_base::event_handler
    {
    public:		
		typedef std::vector<device_description> devices;
		typedef std::vector<driver_base*> drivers;

		/// constructor
		interface();
		
		/// destructor
		~interface();
		
		/// process all pending input
		void update();
		
		/// add an input driver, this takes ownership of the pointer
		void add_driver(driver_base* driver);
		
		/// \returns list of all available devices available for input
		const devices& get_devices() const;
		
		/// bind a device to an input group
		/// \param device_id obtained from the device_description structure.
		/// \param input_group group to bind to. Must be in range [0,7]
		void bind_device(int input_group, int device_id);
	
		/// find all available controllers
		int enumerate_controllers(device_description const ** out_devices, int output_size) const;
		
		/// push an action group on the stack
		/// \param group	group to push on to the stack
		/// \param handler	object to issue callbacks on when actions are triggered.
		void push_action_group(int group_id, const char* group_name, const action *group, input_handler *handler);
		
		/// pop an action group of the stack
		void pop_action_group(int group_id, const char* group_name, const action *group);
				
		/// register a set of bindings to use when it's corresponding actions are 
		/// push on the stack. caller is responsible for the freeing the bindings.
		void register_bindings(const char* name, const binding* bindings);
		
		/// \name driver_base::event_handler interface
		//@{
		virtual void handle_mouse_event(int device_id, const mouse_packet&);
		virtual void handle_keyboard_event(int device_id, const keyboard_packet&);
		virtual void handle_axis_event(int device_id, const axis_packet&);
		//@}
		
	private:

    private:
		/// information to dispatch a specific input event to user handler
		struct action_handler
		{
			const action*  act;
			input_handler* handler;			
		};

		typedef core::scoped_hash_table<input, const char*, 123>	input_to_action_map;
		typedef core::scoped_hash_table<const char*, action_handler, 123, &core::char_array_equals_fn>	action_to_handler_map;
		typedef std::map<std::string, const binding*> binding_map;
		
		/// group of devices mapped to a single group 
		struct device_group :
			public driver_base::event_handler
		{	
		public:
			device_group();
			void add_device(int device_id);
			void remove_device(int device_id);
			bool contains_device(int device_id);			

			/// map any input to its current handler
			action_handler* map_input_to_action(const input& i);

			/// \name driver_base::event_handler interface
			//@{
			virtual void handle_mouse_event(int device_id, const mouse_packet&);
			virtual void handle_keyboard_event(int device_id, const keyboard_packet&);
			virtual void handle_axis_event(int device_id, const axis_packet&);
			//@}
			
			input_to_action_map		m_input_map;
			action_to_handler_map	m_output_map;			
			
		private:
			/// non copyable
			void operator=(const device_group&);

			static const int MaxDevices = 8;
			int m_device_ids[MaxDevices];
			int m_num_devices;
		};

		/// non copyable
		void operator=(const interface& other); 
							
		/// find the group a device is mapped to
		device_group* get_device_group(int device_id);
					
		/// push a key binding group on the stack, these will get first crack at binding to actions.
		/// caller is responsible for the freeing the bindings.
		void push_bindings(int group_id, const binding* bindings);
		
		/// pop a group of key bindings off the stack
		void pop_bindings(int group_id, const binding* bindings);
					
		static const int MaxGroups = 8;
						
		drivers	m_drivers;		///< input drivers currently in use
		devices	m_devices;		///< devices currently exposed by the drivers
		device_group m_groups[MaxGroups];		///< device group mappings
		binding_map  m_bindings;
		int			 m_cur_driver_id;
    };

} // end namespace
} // end namespace

#endif // __INTERFACE_H_871EE0AF_9DED_4FBA_8274_CF60DE815E84_

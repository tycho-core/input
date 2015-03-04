//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 20 April 2008 11:00:24 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "interface.h"
#include "input/driver_base.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace input
{

	/// constructor
	interface::interface() :
		m_cur_driver_id(0)
	{
	}
	
	/// destructor
	interface::~interface()
	{
		drivers::iterator it = m_drivers.begin();
		drivers::iterator end = m_drivers.end();
		for(; it != end; ++it)
		{
			delete *it;
		}
		m_drivers.clear();
	}
	
	/// process all pending input
	void interface::update()
	{
		drivers::iterator it = m_drivers.begin();
		drivers::iterator end = m_drivers.end();
		for(; it != end; ++it)
		{
			(*it)->update(this);
		}	
	}	
	
	/// bind a device to an input group
	/// \param device_id obtained from the device_description structure.
	/// \param input_group group to bind to. Must be in range [0,7]
	void interface::bind_device(int input_group, int device_id)
	{
		m_groups[input_group].add_device(device_id);
	}
	
	/// add an input driver, this takes ownership of the pointer
	void interface::add_driver(driver_base* driver)
	{
		if(driver->initialise(m_cur_driver_id++))
		{
			m_drivers.push_back(driver);
			for(int i = 0; i < driver->get_num_devices(); ++i)
			{
				m_devices.push_back(*driver->get_device_desc(i));
			}
		}
	}
	
	/// \returns list of all available devices available for input
	const interface::devices& interface::get_devices() const 
	{ 
		return m_devices; 
	}


	void interface::push_action_group(int group_id, const char* group_name, const action *group, input_handler *handler)
	{
		const action* a = group;
		device_group* g = &m_groups[group_id];
		TYCHO_ASSERT(g);
		while(a && a->name)
		{
			action_handler ahandler;
			ahandler.act = a;
			ahandler.handler = handler;
			g->m_output_map.push(a->name, ahandler);
			++a;
		}
		
		// lookup its corresponding binding set and push them on
		binding_map::iterator it = m_bindings.find(group_name);
		if(it != m_bindings.end())
			push_bindings(group_id, it->second);
	}
	
	void interface::pop_action_group(int group_id, const char* group_name, const action *group)
	{
		// lookup its corresponding binding set and push them on
		binding_map::iterator it = m_bindings.find(group_name);
		if(it != m_bindings.end())
			pop_bindings(group_id, it->second);
		const action* a = group;
		device_group* g = &m_groups[group_id];
		TYCHO_ASSERT(g);
		while(a && a->name)
		{
			g->m_output_map.pop(a->name);
			++a;
		}
	}
	
	void interface::register_bindings(const char* name, const binding* bindings)
	{
		TYCHO_ASSERT(m_bindings.find(name) == m_bindings.end());
		m_bindings.insert(std::make_pair(name, bindings));
	}
	
	/// push a key binding group on the stack, these will get first crack at binding to actions
	void interface::push_bindings(int group_id, const binding* bindings)
	{
		const binding* b = bindings;
		device_group* g = &m_groups[group_id];
		TYCHO_ASSERT(g);
		while(b && b->action)
		{
			g->m_input_map.push(b->trigger, b->action);
			++b;
		}
	}
	
	/// pop a group of key bindings off the stack
	void interface::pop_bindings(int group_id, const binding* bindings)
	{
		const binding* b = bindings;
		while(b && b->action)
		{
			device_group* g = &m_groups[group_id];
			TYCHO_ASSERT(g);
			g->m_input_map.pop(b->trigger);
			++b;
		}	
	}
		
	void interface::handle_mouse_event(int device_id, const mouse_packet &pkt)
	{
		device_group* g = get_device_group(device_id);
		if(g)
			g->handle_mouse_event(device_id, pkt);
	}
	
	void interface::handle_keyboard_event(int device_id, const keyboard_packet& pkt)
	{
		device_group* g = get_device_group(device_id);
		if(g)
			g->handle_keyboard_event(device_id, pkt);
	}
	
	void interface::handle_axis_event(int device_id, const axis_packet& pkt)
	{
		device_group* g = get_device_group(device_id);
		if(g)
			g->handle_axis_event(device_id, pkt);
	}

	/// \todo O(N), make O(1)
	interface::device_group* interface::get_device_group(int device_id)
	{
		for(int i = 0; i < MaxGroups; ++i)
		{
			if(m_groups[i].contains_device(device_id))
				return &m_groups[i];
		}
		return 0;
	}

	int interface::enumerate_controllers(device_description const * *out_devices, int output_size) const
	{
		int num_controllers = 0;
		for(size_t i = 0; i < m_devices.size(); ++i)
		{
			if(m_devices[i].type == device_xenoncontroller ||
			   m_devices[i].type == device_gccontroller)
			{
				out_devices[num_controllers++] = &m_devices[i];
				if(num_controllers == output_size)
					break;
			}
		}	
		return num_controllers;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// interface::device_group implementation
	//////////////////////////////////////////////////////////////////////////////

	interface::device_group::device_group() :
		m_num_devices(0)
	{
		for(int i = 0; i < MaxDevices; ++i)
			m_device_ids[i] = -1;
	}
	
	void interface::device_group::add_device(int device_id)
	{
//		TYCHO_ASSERT(!contains_device(device_id));
		m_device_ids[m_num_devices++] = device_id;
	}
	
	void interface::device_group::remove_device(int device_id)
	{
		for(int i = 0; i < MaxDevices; ++i)
		{
			if(m_device_ids[i] == device_id)
			{
				m_device_ids[i] = -1;
				--m_num_devices;
				break;
			}
		}
	}
		
	bool interface::device_group::contains_device(int device_id)
	{
		for(int i = 0; i < MaxDevices; ++i)
		{
			if(m_device_ids[i] == device_id)
			{
				return true;
			}
		}
		return false;	
	}

	/// takes an input and finds the key binding that maps it 
	interface::action_handler* interface::device_group::map_input_to_action(const input& i)
	{
		const char** action_name = m_input_map.find(i);
		if(action_name)
			return m_output_map.find(*action_name);
			
		return 0;
	}

	void interface::device_group::handle_mouse_event(int, const mouse_packet& pkt)
	{
		action_handler* handler = map_input_to_action(make_mouse_input());
		if(handler)
			handler->handler->handle_mouse(handler->act->id, pkt.dx, pkt.dy);			
	}
	
	void interface::device_group::handle_keyboard_event(int, const keyboard_packet& pkt)
	{
		action_handler* handler = map_input_to_action(make_keyboard_input(pkt.key, pkt.state));
		if(handler)
			handler->handler->handle_key(handler->act->id, pkt.key, pkt.state);		
	}
	
	void interface::device_group::handle_axis_event(int, const axis_packet& pkt)
	{
		action_handler* handler = map_input_to_action(make_axis_input(pkt.axis));
		if(handler)
			handler->handler->handle_axis(handler->act->id, pkt.value);	
	}

} // end namespace
} // end namespace


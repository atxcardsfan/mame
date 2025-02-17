// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    RCA Cosmac VIP Expansion Interface emulation

**********************************************************************/

#include "emu.h"
#include "exp.h"



//**************************************************************************
//  MACROS/CONSTANTS
//**************************************************************************

#define LOG 0



//**************************************************************************
//  DEVICE DEFINITIONS
//**************************************************************************

DEFINE_DEVICE_TYPE(VIP_EXPANSION_SLOT, vip_expansion_slot_device, "vip_expansion_slot", "VIP expansion port")



//**************************************************************************
//  DEVICE VIP_EXPANSION CARD INTERFACE
//**************************************************************************

//-------------------------------------------------
//  device_vip_expansion_card_interface - constructor
//-------------------------------------------------

device_vip_expansion_card_interface::device_vip_expansion_card_interface(const machine_config &mconfig, device_t &device)
	: device_interface(device, "vipexp")
{
	m_slot = dynamic_cast<vip_expansion_slot_device *>(device.owner());
}



//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  vip_expansion_slot_device - constructor
//-------------------------------------------------

vip_expansion_slot_device::vip_expansion_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	device_t(mconfig, VIP_EXPANSION_SLOT, tag, owner, clock),
	device_single_card_slot_interface<device_vip_expansion_card_interface>(mconfig, *this),
	m_write_int(*this),
	m_write_dma_out(*this),
	m_write_dma_in(*this), m_card(nullptr)
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void vip_expansion_slot_device::device_start()
{
	m_card = get_card_device();

	// resolve callbacks
	m_write_int.resolve_safe();
	m_write_dma_out.resolve_safe();
	m_write_dma_in.resolve_safe();
}


//-------------------------------------------------
//  program_r - program read
//-------------------------------------------------

uint8_t vip_expansion_slot_device::program_r(offs_t offset, int cs, int cdef, int *minh)
{
	uint8_t data = 0;

	if (m_card != nullptr)
	{
		data = m_card->vip_program_r(offset, cs, cdef, minh);
	}

	return data;
}


//-------------------------------------------------
//  program_w - program write
//-------------------------------------------------

void vip_expansion_slot_device::program_w(offs_t offset, uint8_t data, int cdef, int *minh)
{
	if (m_card != nullptr)
	{
		m_card->vip_program_w(offset, data, cdef, minh);
	}
}


//-------------------------------------------------
//  io_r - io read
//-------------------------------------------------

uint8_t vip_expansion_slot_device::io_r(offs_t offset)
{
	uint8_t data = 0;

	if (m_card != nullptr)
	{
		data = m_card->vip_io_r(offset);
	}

	return data;
}


//-------------------------------------------------
//  io_w - io write
//-------------------------------------------------

void vip_expansion_slot_device::io_w(offs_t offset, uint8_t data)
{
	if (m_card != nullptr)
	{
		m_card->vip_io_w(offset, data);
	}
}


//-------------------------------------------------
//  dma_r - dma read
//-------------------------------------------------

uint8_t vip_expansion_slot_device::dma_r(offs_t offset)
{
	uint8_t data = 0;

	if (m_card != nullptr)
	{
		data = m_card->vip_dma_r(offset);
	}

	return data;
}


//-------------------------------------------------
//  dma_w - dma write
//-------------------------------------------------

void vip_expansion_slot_device::dma_w(offs_t offset, uint8_t data)
{
	if (m_card != nullptr)
	{
		m_card->vip_dma_w(offset, data);
	}
}


//-------------------------------------------------
//  screen_update -
//-------------------------------------------------

uint32_t vip_expansion_slot_device::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	bool value = false;

	if (m_card != nullptr)
	{
		value = m_card->vip_screen_update(screen, bitmap, cliprect);
	}

	return value;
}

int vip_expansion_slot_device::ef1_r()
{
	int state = CLEAR_LINE;
	if (m_card != nullptr)
		state = m_card->vip_ef1_r();
	return state;
}

int vip_expansion_slot_device::ef3_r()
{
	int state = CLEAR_LINE;
	if (m_card != nullptr)
		state = m_card->vip_ef3_r();
	return state;
}

int vip_expansion_slot_device::ef4_r()
{
	int state = CLEAR_LINE;
	if (m_card != nullptr)
		state = m_card->vip_ef4_r();
	return state;
}

void vip_expansion_slot_device::sc_w(offs_t offset, uint8_t data)
{
	if (m_card != nullptr)
		m_card->vip_sc_w(offset, data);
}

void vip_expansion_slot_device::q_w(int state)
{
	if (m_card != nullptr)
		m_card->vip_q_w(state);
}

void vip_expansion_slot_device::tpb_w(int state)
{
	if (m_card != nullptr)
		m_card->vip_tpb_w(state);
}

void vip_expansion_slot_device::run_w(int state)
{
	if (m_card != nullptr)
		m_card->vip_run_w(state);
}



//-------------------------------------------------
//  SLOT_INTERFACE vip_expansion_cards )
//-------------------------------------------------

// slot devices
#include "vp550.h"
#include "vp570.h"
#include "vp575.h"
#include "vp585.h"
#include "vp590.h"
#include "vp595.h"
#include "vp700.h"

void vip_expansion_cards(device_slot_interface &device)
{
	device.option_add("super", VP550);
	//device.option_add("eprom", VP560);
	//device.option_add("eprommer", VP565);
	device.option_add("ram", VP570);
	device.option_add("exp", VP575);
	//device.option_add("exp2", VP576_EXP);
	device.option_add("keypad", VP585);
	device.option_add("color", VP590);
	device.option_add("simple", VP595);
	device.option_add("basic", VP700);
}

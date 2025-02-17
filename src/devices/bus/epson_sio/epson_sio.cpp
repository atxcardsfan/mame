// license:GPL-2.0+
// copyright-holders:Dirk Best
/**********************************************************************

    EPSON SIO port emulation

**********************************************************************/

#include "emu.h"
#include "epson_sio.h"

// supported devices
#include "pf10.h"
#include "tf20.h"


//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

DEFINE_DEVICE_TYPE(EPSON_SIO, epson_sio_device, "epson_sio", "EPSON SIO port")


//**************************************************************************
//  CARD INTERFACE
//**************************************************************************

//-------------------------------------------------
//  device_epson_sio_interface - constructor
//-------------------------------------------------

device_epson_sio_interface::device_epson_sio_interface(const machine_config &mconfig, device_t &device) :
	device_interface(device, "epsonsio")
{
	m_slot = dynamic_cast<epson_sio_device *>(device.owner());
}


//-------------------------------------------------
//  ~device_epson_sio_interface - destructor
//-------------------------------------------------

device_epson_sio_interface::~device_epson_sio_interface()
{
}


//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  epson_sio_device - constructor
//-------------------------------------------------

epson_sio_device::epson_sio_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	device_t(mconfig, EPSON_SIO, tag, owner, clock),
	device_single_card_slot_interface<device_epson_sio_interface>(mconfig, *this),
	m_cart(nullptr),
	m_write_rx(*this),
	m_write_pin(*this)
{
}


//-------------------------------------------------
//  epson_sio_device - destructor
//-------------------------------------------------

epson_sio_device::~epson_sio_device()
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void epson_sio_device::device_start()
{
	m_cart = get_card_device();

	m_write_rx.resolve_safe();
	m_write_pin.resolve_safe();
}


void epson_sio_device::tx_w(int state)
{
	if (m_cart != nullptr)
		m_cart->tx_w(state);
}

void epson_sio_device::pout_w(int state)
{
	if (m_cart != nullptr)
		m_cart->pout_w(state);
}


//**************************************************************************
//  SLOT INTERFACE
//**************************************************************************

void epson_sio_devices(device_slot_interface &device)
{
	device.option_add("pf10", EPSON_PF10);
	device.option_add("tf20", EPSON_TF20);
}

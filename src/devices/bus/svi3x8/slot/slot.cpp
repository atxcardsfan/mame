// license:GPL-2.0+
// copyright-holders:Dirk Best
/***************************************************************************

    SVI 318/328 Expansion Slot

***************************************************************************/

#include "emu.h"
#include "slot.h"


//**************************************************************************
//  SLOT BUS DEVICE
//**************************************************************************

DEFINE_DEVICE_TYPE(SVI_SLOT_BUS, svi_slot_bus_device, "svislotbus", "SVI Slot Bus")

//-------------------------------------------------
//  svi_slot_bus_device - constructor
//-------------------------------------------------

svi_slot_bus_device::svi_slot_bus_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, SVI_SLOT_BUS, tag, owner, clock)
	, m_int_handler(*this)
	, m_romdis_handler(*this)
	, m_ramdis_handler(*this)
{
}

//-------------------------------------------------
//  svi_slot_bus_device - destructor
//-------------------------------------------------

svi_slot_bus_device::~svi_slot_bus_device()
{
}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void svi_slot_bus_device::device_start()
{
	// resolve callbacks
	m_int_handler.resolve_safe();
	m_romdis_handler.resolve_safe();
	m_ramdis_handler.resolve_safe();
}

//-------------------------------------------------
//  add_card - add new card to our bus
//-------------------------------------------------

void svi_slot_bus_device::add_card(device_svi_slot_interface &card)
{
	card.set_bus_device(*this);
	m_dev.emplace_back(card);
}

//-------------------------------------------------
//  mreq_r - memory read from slot
//-------------------------------------------------

uint8_t svi_slot_bus_device::mreq_r(offs_t offset)
{
	uint8_t data = 0xff;

	for (device_svi_slot_interface &entry : m_dev)
		data &= entry.mreq_r(offset);

	return data;
}

//-------------------------------------------------
//  mreq_w - memory write to slot
//-------------------------------------------------

void svi_slot_bus_device::mreq_w(offs_t offset, uint8_t data)
{
	for (device_svi_slot_interface &entry : m_dev)
		entry.mreq_w(offset, data);
}

//-------------------------------------------------
//  iorq_r - memory read from slot
//-------------------------------------------------

uint8_t svi_slot_bus_device::iorq_r(offs_t offset)
{
	uint8_t data = 0xff;

	for (device_svi_slot_interface &entry : m_dev)
		data &= entry.iorq_r(offset);

	return data;
}

//-------------------------------------------------
//  iorq_w - memory write to slot
//-------------------------------------------------

void svi_slot_bus_device::iorq_w(offs_t offset, uint8_t data)
{
	for (device_svi_slot_interface &entry : m_dev)
		entry.iorq_w(offset, data);
}

//-------------------------------------------------
//  bk21_w - signal from host to slots
//-------------------------------------------------

void svi_slot_bus_device::bk21_w(int state)
{
	for (device_svi_slot_interface &entry : m_dev)
		entry.bk21_w(state);
}

//-------------------------------------------------
//  bk22_w - signal from host to slots
//-------------------------------------------------

void svi_slot_bus_device::bk22_w(int state)
{
	for (device_svi_slot_interface &entry : m_dev)
		entry.bk22_w(state);
}

//-------------------------------------------------
//  bk31_w - signal from host to slots
//-------------------------------------------------

void svi_slot_bus_device::bk31_w(int state)
{
	for (device_svi_slot_interface &entry : m_dev)
		entry.bk31_w(state);
}

//-------------------------------------------------
//  bk32_w - signal from host to slots
//-------------------------------------------------

void svi_slot_bus_device::bk32_w(int state)
{
	for (device_svi_slot_interface &entry : m_dev)
		entry.bk32_w(state);
}


//**************************************************************************
//  SVI SLOT DEVICE
//**************************************************************************

DEFINE_DEVICE_TYPE(SVI_SLOT, svi_slot_device, "svislot", "SVI Slot")

//-------------------------------------------------
//  svi_slot_device - constructor
//-------------------------------------------------

svi_slot_device::svi_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, SVI_SLOT, tag, owner, clock)
	, device_single_card_slot_interface<device_svi_slot_interface>(mconfig, *this)
	, m_bus(*this, finder_base::DUMMY_TAG)
{
}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void svi_slot_device::device_start()
{
	device_svi_slot_interface *dev = get_card_device();
	if (dev)
		m_bus->add_card(*dev);
}


//**************************************************************************
//  CARD INTERFACE
//**************************************************************************

//-------------------------------------------------
//  device_svi_slot_interface - constructor
//-------------------------------------------------

device_svi_slot_interface::device_svi_slot_interface(const machine_config &mconfig, device_t &device) :
	device_interface(device, "svi3x8slot"),
	m_bus(nullptr)
{
}

//-------------------------------------------------
//  ~device_svi_slot_interface - destructor
//-------------------------------------------------

device_svi_slot_interface::~device_svi_slot_interface()
{
}

//-------------------------------------------------
//  set_bus_device - set bus we are attached to
//-------------------------------------------------

void device_svi_slot_interface::set_bus_device(svi_slot_bus_device &bus)
{
	assert(!device().started());
	m_bus = &bus;
}

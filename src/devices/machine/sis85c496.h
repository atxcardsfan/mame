// license:BSD-3-Clause
// copyright-holders:R. Belmont
// SiS 85c496 northbridge (PCI & CPU Memory Controller)

#ifndef SIS85C496_H
#define SIS85C496_H

#include "pci.h"
#include "machine/ins8250.h"
#include "machine/ds128x.h"
#include "machine/pic8259.h"
#include "machine/pit8253.h"

#include "bus/ata/ataintf.h"
#include "machine/at_keybc.h"

#include "sound/spkrdev.h"
#include "machine/ram.h"
#include "machine/nvram.h"

#include "machine/pc_lpt.h"
#include "bus/pc_kbd/pc_kbdc.h"

#include "machine/am9517a.h"
#include "cpu/i386/i386.h"
#include "machine/at.h"

#define SIS85C496_HOST(_config, _tag, _cpu_tag, _ram_size)  \
	pci_host_device &pcihost(PCI_HOST(_config, _tag, SIS85C496, 0x10390496, 0x03, 0x00000000)); \
	pcihost.set_cpu_tag(_cpu_tag); \
	pcihost.set_ram_size(_ram_size);

class sis85c496_host_device : public pci_host_device {
public:
	sis85c496_host_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	void set_cpu_tag(const char *tag);
	void set_ram_size(int ram_size);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_add_mconfig(machine_config &config) override;

	void map_bios(address_space *memory_space, uint32_t start, uint32_t end);
	void map_shadowram(address_space *memory_space, offs_t addrstart, offs_t addrend, void *baseptr);

	virtual void reset_all_mappings() override;

	virtual void map_extra(uint64_t memory_window_start, uint64_t memory_window_end, uint64_t memory_offset, address_space *memory_space,
						   uint64_t io_window_start, uint64_t io_window_end, uint64_t io_offset, address_space *io_space) override;

	virtual void config_map(address_map &map) override;

private:
	required_device<cpu_device> m_maincpu;
	required_device<pic8259_device> m_pic8259_master;
	required_device<pic8259_device> m_pic8259_slave;
	required_device<am9517a_device> m_dma8237_1;
	required_device<am9517a_device> m_dma8237_2;
	required_device<pit8254_device> m_pit8254;
	required_device<at_keyboard_controller_device> m_keybc;
	required_device<speaker_sound_device> m_speaker;
	required_device<ds12885_device> m_ds12885;
	required_device<pc_kbdc_device> m_pc_kbdc;

	uint8_t m_at_spkrdata;
	uint8_t m_pit_out2;
	int m_dma_channel;
	bool m_cur_eop;
	uint8_t m_dma_offset[2][4];
	uint8_t m_at_pages[0x10];
	uint16_t m_dma_high_byte;
	uint8_t m_at_speaker;
	bool m_refresh;
	void pc_select_dma_channel(int channel, bool state);
	void at_speaker_set_spkrdata(uint8_t data);
	uint8_t m_channel_check;
	uint8_t m_nmi_enabled;

	int ram_size;
	std::vector<uint32_t> ram;
	uint32_t m_mailbox;
	uint8_t m_bios_config, m_dram_config, m_isa_decoder;
	uint16_t m_shadctrl;
	uint8_t m_smramctrl;

	void internal_io_map(address_map &map);

	uint8_t dram_config_r() { return m_dram_config; }
	void dram_config_w(uint8_t data) { m_dram_config = data; remap_cb(); }
	uint8_t bios_config_r() { return m_bios_config; }
	void bios_config_w(uint8_t data) { m_bios_config = data; remap_cb(); }
	uint32_t mailbox_r()     { return m_mailbox; }
	void mailbox_w(offs_t offset, uint32_t data, uint32_t mem_mask = ~0)    { COMBINE_DATA(&m_mailbox); }
	uint8_t isa_decoder_r() { return m_isa_decoder; }
	void isa_decoder_w(uint8_t data) { m_isa_decoder = data; remap_cb(); }
	uint16_t shadow_config_r() { return m_shadctrl; }
	void shadow_config_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0) { COMBINE_DATA(&m_shadctrl); logerror("SiS496: %04x to shadow control\n", m_shadctrl); remap_cb(); }
	uint8_t smram_ctrl_r() { return m_smramctrl; }
	void smram_ctrl_w(uint8_t data) { m_smramctrl = data; remap_cb(); }

	// southbridge
	uint8_t at_page8_r(offs_t offset);
	void at_page8_w(offs_t offset, uint8_t data);
	uint8_t at_portb_r();
	void at_portb_w(uint8_t data);
	uint8_t get_slave_ack(offs_t offset);
	void at_pit8254_out0_changed(int state);
	void at_pit8254_out1_changed(int state);
	void at_pit8254_out2_changed(int state);
	void pc_dma_hrq_changed(int state);
	uint8_t pc_dma8237_0_dack_r();
	uint8_t pc_dma8237_1_dack_r();
	uint8_t pc_dma8237_2_dack_r();
	uint8_t pc_dma8237_3_dack_r();
	uint8_t pc_dma8237_5_dack_r();
	uint8_t pc_dma8237_6_dack_r();
	uint8_t pc_dma8237_7_dack_r();
	void pc_dma8237_0_dack_w(uint8_t data);
	void pc_dma8237_1_dack_w(uint8_t data);
	void pc_dma8237_2_dack_w(uint8_t data);
	void pc_dma8237_3_dack_w(uint8_t data);
	void pc_dma8237_5_dack_w(uint8_t data);
	void pc_dma8237_6_dack_w(uint8_t data);
	void pc_dma8237_7_dack_w(uint8_t data);
	void at_dma8237_out_eop(int state);
	void pc_dack0_w(int state);
	void pc_dack1_w(int state);
	void pc_dack2_w(int state);
	void pc_dack3_w(int state);
	void pc_dack4_w(int state);
	void pc_dack5_w(int state);
	void pc_dack6_w(int state);
	void pc_dack7_w(int state);
	uint8_t at_dma8237_2_r(offs_t offset);
	void at_dma8237_2_w(offs_t offset, uint8_t data);
	uint8_t at_keybc_r(offs_t offset);
	void at_keybc_w(offs_t offset, uint8_t data);
	void write_rtc(offs_t offset, uint8_t data);
	uint8_t pc_dma_read_byte(offs_t offset);
	void pc_dma_write_byte(offs_t offset, uint8_t data);
	uint8_t pc_dma_read_word(offs_t offset);
	void pc_dma_write_word(offs_t offset, uint8_t data);
	void cpu_int_w(int state);
	void cpu_a20_w(int state);
	void cpu_reset_w(int state);
};

DECLARE_DEVICE_TYPE(SIS85C496, sis85c496_host_device)

#endif

#include "../include/cpu.hpp"
#include "../include/key_presses.hpp"
#include<bitset>
#include <iomanip>



#define $A 1
#define $F 0
#define $B 3
#define $C 2
#define $D 5
#define $E 4
#define $H 7
#define $L 6

#define $AF 0
#define $BC 1
#define $DE 2
#define $HL 3
#define $SP 4
#define $PC 5
 

CPU::CPU(){};
CPU::~CPU(){};

extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;

long CPU::_clock;
uint16_t CPU::_lastTick;

void CPU::incrClockBy(uint8_t incr) { CPU::_clock += incr; }
long CPU::getClock() { return CPU::_clock; }
uint16_t CPU::getLastTick() { return CPU::_lastTick; }
void CPU::incrPCby(uint8_t incr) { Register::write16($PC, (Register::read16($PC) + incr)); }

bool CPU::isBootDone() {
	return Memory::read(0xFF50);
}

void CPU::bootstrap() {
	while (!isBootDone()) {
	  CPU::decode();
          GPU::gpu_step(gRenderer);
	  ISA::handle_interrupts();
	}
}

void CPU::run() {
  KEYS::initialize_keys();
  bool play = true;
  ISA::master_enable = 1;
	while(play){

          const Uint8* keystate = SDL_GetKeyboardState(NULL);
          //continuous-response keys
          if(keystate[SDL_SCANCODE_Z])
            KEYS::key_a = 1;
          if(keystate[SDL_SCANCODE_X])
            KEYS::key_b = 1;
          if(keystate[SDL_SCANCODE_I])
            KEYS::key_up = 1;
          if(keystate[SDL_SCANCODE_K])
            KEYS::key_down = 1;
          if(keystate[SDL_SCANCODE_J])
            KEYS::key_left = 1;
          if(keystate[SDL_SCANCODE_L])
            KEYS::key_right = 1;
          if(keystate[SDL_SCANCODE_S])
            KEYS::key_start = 1;
          if(keystate[SDL_SCANCODE_A])
            KEYS::key_select = 1;
          if(keystate[SDL_SCANCODE_Q])
            play = false;
	  CPU::decode();
	  ISA::handle_interrupts();
          GPU::gpu_step(gRenderer);
          KEYS::reset_keys();
          SDL_PumpEvents();
	}
}


void CPU::decode(){
  uint16_t PC = Register::read16($PC);
	uint8_t instruction = Memory::read(PC);
	uint8_t arg1 = Memory::read(PC + 0x01);
	uint8_t arg2 = Memory::read(PC + 0x02);
	uint16_t arg16 = (arg2 << 8) | arg1;
	incrPCby(DataBank::numbInstructions[instruction]);
	incrClockBy(DataBank::instructionTicks[instruction]);
        CPU::_lastTick = DataBank::instructionTicks[instruction];

	switch(instruction){
		case 0x00 : ISA::nop(); break;
		case 0x01 : ISA::ld_bc_nn(arg16); break;
		case 0x02 : ISA::ld_bcp_a(); break;
		case 0x03 : ISA::inc_bc(); break;
		case 0x04 : ISA::inc_b(); break;
		case 0x05 : ISA::dec_b(); break;
		case 0x06 : ISA::ld_b_n(arg1); break;
		case 0x07 : ISA::rlca(); break;
		case 0x08 : ISA::ld_nnp_sp(arg16); break;
		case 0x09 : ISA::add_hl_bc(); break;
		case 0x0A : ISA::ld_a_bcp(); break;
		case 0x0B : ISA::dec_bc(); break;
		case 0x0C : ISA::inc_c(); break;
		case 0x0D : ISA::dec_c(); break;
		case 0x0E : ISA::ld_c_n(arg1); break;
		case 0x0F : ISA::rrca(); break;
		case 0x10 : ISA::stop(arg1); break;
		case 0x11 : ISA::ld_de_nn(arg16); break;
		case 0x12 : ISA::ld_dep_a(); break;
		case 0x13 : ISA::inc_de(); break;
		case 0x14 : ISA::inc_d(); break;
		case 0x15 : ISA::dec_d(); break;
		case 0x16 : ISA::ld_d_n(arg1); break;
		case 0x17 : ISA::rla(); break;
		case 0x18 : ISA::jr_n(arg1); break;  //FIX 16 to 1
		case 0x19 : ISA::add_hl_de(); break;
		case 0x1A : ISA::ld_a_dep(); break;
		case 0x1B : ISA::dec_de(); break;
		case 0x1C : ISA::inc_e(); break;
		case 0x1D : ISA::dec_e(); break;
		case 0x1E : ISA::ld_e_n(arg1); break;
		case 0x1F : ISA::rra(); break;
		case 0x20 : ISA::jr_nz_n(arg1); break;
		case 0x21 : ISA::ld_hl_nn(arg16); break;
		case 0x22 : ISA::ldi_hlp_a(); break;
		case 0x23 : ISA::inc_hl(); break;
		case 0x24 : ISA::inc_h(); break;
		case 0x25 : ISA::dec_h(); break;
		case 0x26 : ISA::ld_h_n(arg1); break;
		case 0x27 : ISA::daa(); break;
		case 0x28 : ISA::jr_z_n(arg1); break;
		case 0x29 : ISA::add_hl_hl(); break;
		case 0x2A : ISA::ldi_a_hlp(); break;
		case 0x2B : ISA::dec_hl(); break;
		case 0x2C : ISA::inc_l(); break;
		case 0x2D : ISA::dec_l(); break;
		case 0x2E : ISA::ld_l_n(arg1); break;
		case 0x2F : ISA::cpl(); break;
		case 0x30 : ISA::jr_nc_n(arg1); break; //CHANGED TO 1
		case 0x31 : ISA::ld_sp_nn(arg16); break; //CHANGED TO 2
		case 0x32 : ISA::ldd_hlp_a(); break;
		case 0x33 : ISA::inc_sp(); break;
		case 0x34 : ISA::inc_hlp(); break;
		case 0x35 : ISA::dec_hlp(); break;
		case 0x36 : ISA::ld_hlp_n(arg1); break;
		case 0x37 : ISA::scf(); break;
		case 0x38 : ISA::jr_c_n(arg16); break;
		case 0x39 : ISA::add_hl_sp(); break;
		case 0x3A : ISA::ldd_a_hlp(); break;
		case 0x3B : ISA::dec_sp(); break;
		case 0x3C : ISA::inc_a(); break;
		case 0x3D : ISA::dec_a(); break;
		case 0x3E : ISA::ld_a_n(arg1); break;
		case 0x3F : ISA::ccf(); break;
		//TODO WE ARE MISSING LD_B_B TODO TODO TODO
		case 0x41 : ISA::ld_b_c(); break;
		case 0x42 : ISA::ld_b_d(); break;
		case 0x43 : ISA::ld_b_e(); break;
		case 0x44 : ISA::ld_b_h(); break;
		case 0x45 : ISA::ld_b_l(); break;
		case 0x46 : ISA::ld_b_hlp(); break;
		case 0x47 : ISA::ld_b_a(); break;
		case 0x48 : ISA::ld_c_b(); break;
		//TODO WE ARE MISSING LD_C_C TODO TODO TODO
		case 0x4A : ISA::ld_c_d(); break;
		case 0x4B : ISA::ld_c_e(); break;
		case 0x4C : ISA::ld_c_h(); break;
		case 0x4D : ISA::ld_c_l(); break;
		case 0x4E : ISA::ld_c_hlp(); break;
		case 0x4F : ISA::ld_c_a(); break;
		case 0x50 : ISA::ld_d_b(); break;
		case 0x51 : ISA::ld_d_c(); break;
		//TODO MISSING D D
		case 0x53 : ISA::ld_d_e(); break;
		case 0x54 : ISA::ld_d_h(); break;
		case 0x55 : ISA::ld_d_l(); break;
		case 0x56 : ISA::ld_d_hlp(); break;
		case 0x57 : ISA::ld_d_a(); break;
		case 0x58 : ISA::ld_e_b(); break;
		case 0x59 : ISA::ld_e_c(); break;
		case 0x5A : ISA::ld_e_d(); break;
		//TODO MISSING E E
		case 0x5C : ISA::ld_e_h(); break;
		case 0x5D : ISA::ld_e_l(); break;
		case 0x5E : ISA::ld_e_hlp(); break;
		case 0x5F : ISA::ld_e_a(); break;
		case 0x60 : ISA::ld_h_b(); break;
		case 0x61 : ISA::ld_h_c(); break;
		case 0x62 : ISA::ld_h_d(); break;
		case 0x63 : ISA::ld_h_e(); break;
		//TODO MISSING D D
		case 0x65 : ISA::ld_h_l(); break;
		case 0x66 : ISA::ld_h_hlp(); break;
		case 0x67 : ISA::ld_h_a(); break;
		case 0x68 : ISA::ld_l_b(); break;
		case 0x69 : ISA::ld_l_c(); break;
		case 0x6A : ISA::ld_l_d(); break;
		case 0x6B : ISA::ld_l_e(); break;
		case 0x6C : ISA::ld_l_h(); break;
		//TODO MISSING L L
		case 0x6E : ISA::ld_l_hlp(); break;
		case 0x6F : ISA::ld_l_a(); break;
		case 0x70 : ISA::ld_hlp_b(); break;
		case 0x71 : ISA::ld_hlp_c(); break;
		case 0x72 : ISA::ld_hlp_d(); break;
		case 0x73 : ISA::ld_hlp_e(); break;
		case 0x74 : ISA::ld_hlp_h(); break;
		case 0x75 : ISA::ld_hlp_l(); break;
		case 0x76 : ISA::halt(); break;
		case 0x77 : ISA::ld_hlp_a(); break;
		case 0x78 : ISA::ld_a_b(); break;
		case 0x79 : ISA::ld_a_c(); break;
		case 0x7A : ISA::ld_a_d(); break;
		case 0x7B : ISA::ld_a_e(); break;
		case 0x7C : ISA::ld_a_h(); break;
		case 0x7D : ISA::ld_a_l(); break;
		case 0x7E : ISA::ld_a_hlp(); break;
		//TODO MISSING LD A A
		case 0x80 : ISA::add_a_b(); break;
		case 0x81 : ISA::add_a_c(); break;
		case 0x82 : ISA::add_a_d(); break;
		case 0x83 : ISA::add_a_e(); break;
		case 0x84 : ISA::add_a_h(); break;
		case 0x85 : ISA::add_a_l(); break;
		case 0x86 : ISA::add_a_hlp(); break;
		case 0x87 : ISA::add_a_a(); break;
		case 0x88 : ISA::adc_b(); break;
		case 0x89 : ISA::adc_c(); break;
		case 0x8A : ISA::adc_d(); break;
		case 0x8B : ISA::adc_e(); break;
		case 0x8C : ISA::adc_h(); break;
		case 0x8D : ISA::adc_l(); break;
		case 0x8E : ISA::adc_hlp(); break;
		case 0x8F : ISA::adc_a(); break;
		case 0x90 : ISA::sub_b(); break;
		case 0x91 : ISA::sub_c(); break;
		case 0x92 : ISA::sub_d(); break;
		case 0x93 : ISA::sub_e(); break;
		case 0x94 : ISA::sub_h(); break;
		case 0x95 : ISA::sub_l(); break;
		case 0x96 : ISA::sub_hlp(); break;
		case 0x97 : ISA::sub_a(); break;
		case 0x98 : ISA::sbc_b(); break;
		case 0x99 : ISA::sbc_c(); break;
		case 0x9A : ISA::sbc_d(); break;
		case 0x9B : ISA::sbc_e(); break;
		case 0x9C : ISA::sbc_h(); break;
		case 0x9D : ISA::sbc_l(); break;
		case 0x9E : ISA::sbc_hlp(); break;
		case 0x9F : ISA::sbc_a(); break;
		case 0xA0 : ISA::and_b(); break;
		case 0xA1 : ISA::and_c(); break;
		case 0xA2 : ISA::and_d(); break;
		case 0xA3 : ISA::and_e(); break;
		case 0xA4 : ISA::and_h(); break;
		case 0xA5 : ISA::and_l(); break;
		case 0xA6 : ISA::and_hlp(); break;
		case 0xA7 : ISA::and_a(); break;
		case 0xA8 : ISA::xor_b(); break;
		case 0xA9 : ISA::xor_c(); break;
		case 0xAA : ISA::xor_d(); break;
		case 0xAB : ISA::xor_e(); break;
		case 0xAC : ISA::xor_h(); break;
		case 0xAD : ISA::xor_l(); break;
		case 0xAE : ISA::xor_hlp(); break;
		case 0xAF : ISA::xor_a(); break;
		case 0xB0 : ISA::or_b(); break;
		case 0xB1 : ISA::or_c(); break;
		case 0xB2 : ISA::or_d(); break;
		case 0xB3 : ISA::or_e(); break;
		case 0xB4 : ISA::or_h(); break;
		case 0xB5 : ISA::or_l(); break;
		case 0xB6 : ISA::or_hlp(); break;
		case 0xB7 : ISA::or_a(); break;
		case 0xB8 : ISA::cp_b(); break;
		case 0xB9 : ISA::cp_c(); break;
		case 0xBA : ISA::cp_d(); break;
		case 0xBB : ISA::cp_e(); break;
		case 0xBC : ISA::cp_h(); break;
		case 0xBD : ISA::cp_l(); break;
		case 0xBE : ISA::cp_hlp(); break;
		case 0xBF : ISA::cp_a(); break;
		case 0xC0 : ISA::ret_nz(); break;
		case 0xC1 : ISA::pop_bc(); break;
		case 0xC2 : ISA::jp_nz_nn(arg16); break;
		case 0xC3 : ISA::jp_nn(arg16); break;
		case 0xC4 : ISA::call_nz_nn(arg16); break;
		case 0xC5 : ISA::push_bc(); break;
		case 0xC6 : ISA::add_a_n(arg1); break;
		case 0xC7 : ISA::rst_0(); break;
		case 0xC8 : ISA::ret_z(); break;
		case 0xC9 : ISA::ret(); break;
		case 0xCA : ISA::jp_z_nn(arg16); break;
		case 0xCB : CPU::extendedDecode(); break;
		case 0xCC : ISA::call_z_nn(arg16); break;
		case 0xCD : ISA::call_nn(arg16); break;
		case 0xCE : ISA::adc_n(arg1); break;
		case 0xCF : ISA::rst_08(); break;
		case 0xD0 : ISA::ret_nc(); break;
		case 0xD1 : ISA::pop_de(); break;
		case 0xD2 : ISA::jp_nc_nn(arg16); break;
		//space
		case 0xD4 : ISA::call_nc_nn(arg16); break;
		case 0xD5 : ISA::push_de(); break;
		case 0xD6 : ISA::sub_n(arg1); break;
		case 0xD7 : ISA::rst_10(); break;
		case 0xD8 : ISA::ret_c(); break;
		case 0xD9 : ISA::reti(); break;
		case 0xDA : ISA::jp_c_nn(arg16); break;
		//SPACE
		case 0xDC : ISA::call_c_nn(arg16); break;
		//SPACE
		case 0xDE : ISA::sbc_n(arg1); break;
		case 0xDF : ISA::rst_18(); break;
		case 0xE0 : ISA::ld_ff_n_ap(arg1); break;
		case 0xE1 : ISA::pop_hl(); break;
		case 0xE2 : ISA::ld_ff_c_a(); break;
		//SPACE
		//SPACE
		case 0xE5 : ISA::push_hl(); break;
		case 0xE6 : ISA::and_n(arg1); break;
		case 0xE7 : ISA::rst_20(); break;
		case 0xE8 : ISA::add_sp_n(arg1); break;
		case 0xE9 : ISA::jp_hl(); break;
		case 0xEA : ISA::ld_nnp_a(arg16); break; //CHANGED 1 to 2
		//SPACE
		//SPACE
		//SPACE
		case 0xEE : ISA::xor_n(arg1); break;
		case 0xEF : ISA::rst_28(); break;
		case 0xF0 : ISA::ld_ff_ap_n(arg1); break;
		case 0xF1 : ISA::pop_af(); break;
		case 0xF2 : ISA::ld_a_ff_c(); break;
		case 0xF3 : ISA::di_inst(); break;
		//SPACE
		case 0xF5 : ISA::push_af(); break;
		case 0xF6 : ISA::or_n(arg1); break;
		case 0xF7 : ISA::rst_30(); break;
		case 0xF8 : ISA::ld_hl_sp_n(arg1); break;
		case 0xF9 : ISA::ld_sp_hl(); break;
		case 0xFA : ISA::ld_a_nnp(arg16); break; //changed
		case 0xFB : ISA::ei(); break;
		//SPACE
		//SPACE
		case 0xFE : ISA::cp_n(arg1); break;
		case 0xFF : ISA::rst_38(); break;
	}
}

void CPU::extendedDecode(){
	uint8_t instruction = Memory::read(Register::read16($PC));
	incrPCby(0x01);
	switch (instruction){
		case 0x00 : ISA::rlc_b(); break;
		case 0x01 : ISA::rlc_c(); break;
		case 0x02 : ISA::rlc_d(); break;
		case 0x03 : ISA::rlc_e(); break;
		case 0x04 : ISA::rlc_h(); break;
		case 0x05 : ISA::rlc_l(); break;
		case 0x06 : ISA::rlc_hlp(); break;
		case 0x07 : ISA::rlc_a(); break;
		case 0x08 : ISA::rrc_b(); break;
		case 0x09 : ISA::rrc_c(); break;
		case 0x0A : ISA::rrc_d(); break;
		case 0x0B : ISA::rrc_e(); break;
		case 0x0C : ISA::rrc_h(); break;
		case 0x0D : ISA::rrc_l(); break;
		case 0x0E : ISA::rrc_hlp(); break;
		case 0x0F : ISA::rrc_a(); break;
		case 0x10 : ISA::rl_b(); break;
		case 0x11 : ISA::rl_c(); break;
		case 0x12 : ISA::rl_d(); break;
		case 0x13 : ISA::rl_e(); break;
		case 0x14 : ISA::rl_h(); break;
		case 0x15 : ISA::rl_l(); break;
		case 0x16 : ISA::rl_hlp(); break;
		case 0x17 : ISA::rl_a(); break;
		case 0x18 : ISA::rr_b(); break;
		case 0x19 : ISA::rr_c(); break;
		case 0x1A : ISA::rr_d(); break;
		case 0x1B : ISA::rr_e(); break;
		case 0x1C : ISA::rr_h(); break;
		case 0x1D : ISA::rr_l(); break;
		case 0x1E : ISA::rr_hlp(); break;
		case 0x1F : ISA::rr_a(); break;
		case 0x20 : ISA::sla_b(); break;
		case 0x21 : ISA::sla_c(); break;
		case 0x22 : ISA::sla_d(); break;
		case 0x23 : ISA::sla_e(); break;
		case 0x24 : ISA::sla_h(); break;
		case 0x25 : ISA::sla_l(); break;
		case 0x26 : ISA::sla_hlp(); break;
		case 0x27 : ISA::sla_a(); break;
		case 0x28 : ISA::sra_b(); break;
		case 0x29 : ISA::sra_c(); break;
		case 0x2A : ISA::sra_d(); break;
		case 0x2B : ISA::sra_e(); break;
		case 0x2C : ISA::sra_h(); break;
		case 0x2D : ISA::sra_l(); break;
		case 0x2E : ISA::sra_hlp(); break;
		case 0x2F : ISA::sra_a(); break;
		case 0x30 : ISA::swap_b(); break;
		case 0x31 : ISA::swap_c(); break;
		case 0x32 : ISA::swap_d(); break;
		case 0x33 : ISA::swap_e(); break;
		case 0x34 : ISA::swap_h(); break;
		case 0x35 : ISA::swap_l(); break;
		case 0x36 : ISA::swap_hlp(); break;
		case 0x37 : ISA::swap_a(); break;
		case 0x38 : ISA::srl_b(); break;
		case 0x39 : ISA::srl_c(); break;
		case 0x3A : ISA::srl_d(); break;
		case 0x3B : ISA::srl_e(); break;
		case 0x3C : ISA::srl_h(); break;
		case 0x3D : ISA::srl_l(); break;
		case 0x3E : ISA::srl_hlp(); break;
		case 0x3F : ISA::srl_a(); break;
		case 0x40 : ISA::bit_0_b(); break;
		case 0x41 : ISA::bit_0_c(); break;
		case 0x42 : ISA::bit_0_d(); break;
		case 0x43 : ISA::bit_0_e(); break;
		case 0x44 : ISA::bit_0_h(); break;
		case 0x45 : ISA::bit_0_l(); break;
		case 0x46 : ISA::bit_0_hlp(); break;
		case 0x47 : ISA::bit_0_a(); break;
		case 0x48 : ISA::bit_1_b(); break;
		case 0x49 : ISA::bit_1_c(); break;
		case 0x4A : ISA::bit_1_d(); break;
		case 0x4B : ISA::bit_1_e(); break;
		case 0x4C : ISA::bit_1_h(); break;
		case 0x4D : ISA::bit_1_l(); break;
		case 0x4E : ISA::bit_1_hlp(); break;
		case 0x4F : ISA::bit_1_a(); break;
		case 0x50 : ISA::bit_2_b(); break;
		case 0x51 : ISA::bit_2_c(); break;
		case 0x52 : ISA::bit_2_d(); break;
		case 0x53 : ISA::bit_2_e(); break;
		case 0x54 : ISA::bit_2_h(); break;
		case 0x55 : ISA::bit_2_l(); break;
		case 0x56 : ISA::bit_2_hlp(); break;
		case 0x57 : ISA::bit_2_a(); break;
		case 0x58 : ISA::bit_3_b(); break;
		case 0x59 : ISA::bit_3_c(); break;
		case 0x5A : ISA::bit_3_d(); break;
		case 0x5B : ISA::bit_3_e(); break;
		case 0x5C : ISA::bit_3_h(); break;
		case 0x5D : ISA::bit_3_l(); break;
		case 0x5E : ISA::bit_3_hlp(); break;
		case 0x5F : ISA::bit_3_a(); break;
		case 0x60 : ISA::bit_4_b(); break;
		case 0x61 : ISA::bit_4_c(); break;
		case 0x62 : ISA::bit_4_d(); break;
		case 0x63 : ISA::bit_4_e(); break;
		case 0x64 : ISA::bit_4_h(); break;
		case 0x65 : ISA::bit_4_l(); break;
		case 0x66 : ISA::bit_4_hlp(); break;
		case 0x67 : ISA::bit_4_a(); break;
		case 0x68 : ISA::bit_5_b(); break;
		case 0x69 : ISA::bit_5_c(); break;
		case 0x6A : ISA::bit_5_d(); break;
		case 0x6B : ISA::bit_5_e(); break;
		case 0x6C : ISA::bit_5_h(); break;
		case 0x6D : ISA::bit_5_l(); break;
		case 0x6E : ISA::bit_5_hlp(); break;
		case 0x6F : ISA::bit_5_a(); break;
		case 0x70 : ISA::bit_6_b(); break;
		case 0x71 : ISA::bit_6_c(); break;
		case 0x72 : ISA::bit_6_d(); break;
		case 0x73 : ISA::bit_6_e(); break;
		case 0x74 : ISA::bit_6_h(); break;
		case 0x75 : ISA::bit_6_l(); break;
		case 0x76 : ISA::bit_6_hlp(); break;
		case 0x77 : ISA::bit_6_a(); break;
		case 0x78 : ISA::bit_7_b(); break;
		case 0x79 : ISA::bit_7_c(); break;
		case 0x7A : ISA::bit_7_d(); break;
		case 0x7B : ISA::bit_7_e(); break;
		case 0x7C : ISA::bit_7_h(); break;
		case 0x7D : ISA::bit_7_l(); break;
		case 0x7E : ISA::bit_7_hlp(); break;
		case 0x7F : ISA::bit_7_a(); break;
		case 0x80 : ISA::res_0_b(); break;
		case 0x81 : ISA::res_0_c(); break;
		case 0x82 : ISA::res_0_d(); break;
		case 0x83 : ISA::res_0_e(); break;
		case 0x84 : ISA::res_0_h(); break;
		case 0x85 : ISA::res_0_l(); break;
		case 0x86 : ISA::res_0_hlp(); break;
		case 0x87 : ISA::res_0_a(); break;
		case 0x88 : ISA::res_1_b(); break;
		case 0x89 : ISA::res_1_c(); break;
		case 0x8A : ISA::res_1_d(); break;
		case 0x8B : ISA::res_1_e(); break;
		case 0x8C : ISA::res_1_h(); break;
		case 0x8D : ISA::res_1_l(); break;
		case 0x8E : ISA::res_1_hlp(); break;
		case 0x8F : ISA::res_1_a(); break;
		case 0x90 : ISA::res_2_b(); break;
		case 0x91 : ISA::res_2_c(); break;
		case 0x92 : ISA::res_2_d(); break;
		case 0x93 : ISA::res_2_e(); break;
		case 0x94 : ISA::res_2_h(); break;
		case 0x95 : ISA::res_2_l(); break;
		case 0x96 : ISA::res_2_hlp(); break;
		case 0x97 : ISA::res_2_a(); break;
		case 0x98 : ISA::res_3_b(); break;
		case 0x99 : ISA::res_3_c(); break;
		case 0x9A : ISA::res_3_d(); break;
		case 0x9B : ISA::res_3_e(); break;
		case 0x9C : ISA::res_3_h(); break;
		case 0x9D : ISA::res_3_l(); break;
		case 0x9E : ISA::res_3_hlp(); break;
		case 0x9F : ISA::res_3_a(); break;
		case 0xA0 : ISA::res_4_b(); break;
		case 0xA1 : ISA::res_4_c(); break;
		case 0xA2 : ISA::res_4_d(); break;
		case 0xA3 : ISA::res_4_e(); break;
		case 0xA4 : ISA::res_4_h(); break;
		case 0xA5 : ISA::res_4_l(); break;
		case 0xA6 : ISA::res_4_hlp(); break;
		case 0xA7 : ISA::res_4_a(); break;
		case 0xA8 : ISA::res_5_b(); break;
		case 0xA9 : ISA::res_5_c(); break;
		case 0xAA : ISA::res_5_d(); break;
		case 0xAB : ISA::res_5_e(); break;
		case 0xAC : ISA::res_5_h(); break;
		case 0xAD : ISA::res_5_l(); break;
		case 0xAE : ISA::res_5_hlp(); break;
		case 0xAF : ISA::res_5_a(); break;
		case 0xB0 : ISA::res_6_b(); break;
		case 0xB1 : ISA::res_6_c(); break;
		case 0xB2 : ISA::res_6_d(); break;
		case 0xB3 : ISA::res_6_e(); break;
		case 0xB4 : ISA::res_6_h(); break;
		case 0xB5 : ISA::res_6_l(); break;
		case 0xB6 : ISA::res_6_hlp(); break;
		case 0xB7 : ISA::res_6_a(); break;
		case 0xB8 : ISA::res_7_b(); break;
		case 0xB9 : ISA::res_7_c(); break;
		case 0xBA : ISA::res_7_d(); break;
		case 0xBB : ISA::res_7_e(); break;
		case 0xBC : ISA::res_7_h(); break;
		case 0xBD : ISA::res_7_l(); break;
		case 0xBE : ISA::res_7_hlp(); break;
		case 0xBF : ISA::res_7_a(); break;
		case 0xC0 : ISA::set_0_b(); break;
		case 0xC1 : ISA::set_0_c(); break;
		case 0xC2 : ISA::set_0_d(); break;
		case 0xC3 : ISA::set_0_e(); break;
		case 0xC4 : ISA::set_0_h(); break;
		case 0xC5 : ISA::set_0_l(); break;
		case 0xC6 : ISA::set_0_hlp(); break;
		case 0xC7 : ISA::set_0_a(); break;
		case 0xC8 : ISA::set_1_b(); break;
		case 0xC9 : ISA::set_1_c(); break;
		case 0xCA : ISA::set_1_d(); break;
		case 0xCB : ISA::set_1_e(); break;
		case 0xCC : ISA::set_1_h(); break;
		case 0xCD : ISA::set_1_l(); break;
		case 0xCE : ISA::set_1_hlp(); break;
		case 0xCF : ISA::set_1_a(); break;
		case 0xD0 : ISA::set_2_b(); break;
		case 0xD1 : ISA::set_2_c(); break;
		case 0xD2 : ISA::set_2_d(); break;
		case 0xD3 : ISA::set_2_e(); break;
		case 0xD4 : ISA::set_2_h(); break;
		case 0xD5 : ISA::set_2_l(); break;
		case 0xD6 : ISA::set_2_hlp(); break;
		case 0xD7 : ISA::set_2_a(); break;
		case 0xD8 : ISA::set_3_b(); break;
		case 0xD9 : ISA::set_3_c(); break;
		case 0xDA : ISA::set_3_d(); break;
		case 0xDB : ISA::set_3_e(); break;
		case 0xDC : ISA::set_3_h(); break;
		case 0xDD : ISA::set_3_l(); break;
		case 0xDE : ISA::set_3_hlp(); break;
		case 0xDF : ISA::set_3_a(); break;
		case 0xE0 : ISA::set_4_b(); break;
		case 0xE1 : ISA::set_4_c(); break;
		case 0xE2 : ISA::set_4_d(); break;
		case 0xE3 : ISA::set_4_e(); break;
		case 0xE4 : ISA::set_4_h(); break;
		case 0xE5 : ISA::set_4_l(); break;
		case 0xE6 : ISA::set_4_hlp(); break;
		case 0xE7 : ISA::set_4_a(); break;
		case 0xE8 : ISA::set_5_b(); break;
		case 0xE9 : ISA::set_5_c(); break;
		case 0xEA : ISA::set_5_d(); break;
		case 0xEB : ISA::set_5_e(); break;
		case 0xEC : ISA::set_5_h(); break;
		case 0xED : ISA::set_5_l(); break;
		case 0xEE : ISA::set_5_hlp(); break;
		case 0xEF : ISA::set_5_a(); break;
		case 0xF0 : ISA::set_6_b(); break;
		case 0xF1 : ISA::set_6_c(); break;
		case 0xF2 : ISA::set_6_d(); break;
		case 0xF3 : ISA::set_6_e(); break;
		case 0xF4 : ISA::set_6_h(); break;
		case 0xF5 : ISA::set_6_l(); break;
		case 0xF6 : ISA::set_6_hlp(); break;
		case 0xF7 : ISA::set_6_a(); break;
		case 0xF8 : ISA::set_7_b(); break;
		case 0xF9 : ISA::set_7_c(); break;
		case 0xFA : ISA::set_7_d(); break;
		case 0xFB : ISA::set_7_e(); break;
		case 0xFC : ISA::set_7_h(); break;
		case 0xFD : ISA::set_7_l(); break;
		case 0xFE : ISA::set_7_hlp(); break;
		case 0xFF : ISA::set_7_a(); break;

	}
}

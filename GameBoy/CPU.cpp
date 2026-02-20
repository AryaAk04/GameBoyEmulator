#include "CPU.h"
#include <iostream>
#include "Defs.h"
#include <fstream>


CPU::CPU(Memory* mem)
{
	this->Mem = mem;
}

void CPU::NoBootInit() {
	AF = 0x01B0;
	BC = 0x0013;
	DE = 0x00D8;
	HL = 0x014D;
	SP = 0xFFFE;
	PC = 0x0100;
	IME = true;
	Mem->BootEnable = false;
	Mem->WriteByte(0xFF05, 0x00);
	Mem->WriteByte(0xFF06, 0x00);
	Mem->WriteByte(0xFF07, 0x00);
	Mem->WriteByte(0xFF10, 0x80);
	Mem->WriteByte(0xFF40, 0x91);
	Mem->WriteByte(0xFF42, 0x00);
	Mem->WriteByte(0xFF43, 0x00);
	Mem->WriteByte(0xFF45, 0x00);
	Mem->WriteByte(0xFF47, 0xFC);
	Mem->WriteByte(0xFF48, 0xFF);
	Mem->WriteByte(0xFF49, 0xFF);
	Mem->WriteByte(0xFF4A, 0x00);
	Mem->WriteByte(0xFF4B, 0x00);
	Mem->WriteByte(0xFFFF, 0x00);
}

void CPU::SetFlag(u8 flag, bool state)
{
	if (state)
		this->F |= flag;
	else
		this->F &= ~flag;
}

bool CPU::GetFlag(u8 flag)
{
	return (F & flag) != 0;
}

void CPU::Execute(u8 opcode)
{
	switch (opcode)
	{
	case 0x00:
		break;
	case 0x01:
		LD_IMM(BC);
		break;
	case 0x02:
		LD(BC, A);
		break;
	case 0x03:
		INC(BC);
		break;
	case 0x04:
		INC(B);
		break;
	case 0x05:
		DEC(B);
		break;
	case 0x06:
		LD_IMM(B);
		break;
	case 0x07:
		RLCA();
		break;
	case 0x08:
		LD_a16_SP();
		break;
	case 0x09:
		ADD(HL, BC);
		break;
	case 0x0A:
		LD(A, BC);
		break;
	case 0x0B:
		DEC(BC);
		break;
	case 0x0C:
		INC(C);
		break;
	case 0x0D:
		DEC(C);
		break;
	case 0x0E:
		LD_IMM(C);
		break;
	case 0x0F:
		RRCA();
		break;


	case 0x10:
		STOP();
		break;
	case 0x11:
		LD_IMM(DE);
		break;
	case 0x12:
		LD(DE, A);
		break;
	case 0x13:
		INC(DE);
		break;
	case 0x14:
		INC(D);
		break;
	case 0x15:
		DEC(D);
		break;
	case 0x16:
		LD_IMM(D);
		break;
	case 0x17:
		RLA();
		break;
	case 0x18:
		JR();
		break;
	case 0x19:
		ADD(HL, DE);
		break;
	case 0x1A:
		LD(A, DE);
		break;
	case 0x1B:
		DEC(DE);
		break;
	case 0x1C:
		INC(E);
		break;
	case 0x1D:
		DEC(E);
		break;
	case 0x1E:
		LD_IMM(E);
		break;
	case 0x1F:
		RRA();
		break;


	case 0x20:
	{
		bool jump = !GetFlag(Z_FLAG);
		JR(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0x21:
		LD_IMM(HL);
		break;
	case 0x22:
		LDI_HL_A();
		break;
	case 0x23:
		INC(HL);
		break;
	case 0x24:
		INC(H);
		break;
	case 0x25:
		DEC(H);
		break;
	case 0x26:
		LD_IMM(H);
		break;
	case 0x27:
		DAA();
		break;
	case 0x28:
	{
		bool jump = GetFlag(Z_FLAG);
		JR(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0x29:
		ADD(HL, HL);
		break;
	case 0x2A:
		LDI_A_HL();
		break;
	case 0x2B:
		DEC(HL);
		break;
	case 0x2C:
		INC(L);
		break;
	case 0x2D:
		DEC(L);
		break;
	case 0x2E:
		LD_IMM(L);
		break;
	case 0x2F:
		CPL();
		break;


	case 0x30:
	{
		bool jump = !GetFlag(C_FLAG);
		JR(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0x31:
		LD_IMM(SP);
		break;
	case 0x32:
		LDD_HL_A();
		break;
	case 0x33:
		INC(SP);
		break;
	case 0x34:
		INC_HL();
		break;
	case 0x35:
		DEC_HL();
		break;
	case 0x36:
		//Mem->UpdateTimer(4);
		LD_IMM_HL();
		break;
	case 0x37:
		SCF();
		break;
	case 0x38:
	{
		bool jump = GetFlag(C_FLAG);
		JR(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0x39:
		ADD(HL, SP);
		break;
	case 0x3A:
		LDD_A_HL();
		break;
	case 0x3B:
		DEC(SP);
		break;
	case 0x3C:
		INC(A);
		break;
	case 0x3D:
		DEC(A);
		break;
	case 0x3E:
		LD_IMM(A);
		break;
	case 0x3F:
		CCF();
		break;


	case 0x40:
		LD(B, B);
		break;
	case 0x41:
		LD(B, C);
		break;
	case 0x42:
		LD(B, D);
		break;
	case 0x43:
		LD(B, E);
		break;
	case 0x44:
		LD(B, H);
		break;
	case 0x45:
		LD(B, L);
		break;
	case 0x46:
		LD(B, HL);
		break;
	case 0x47:
		LD(B, A);
		break;
	case 0x48:
		LD(C, B);
		break;
	case 0x49:
		LD(C, C);
		break;
	case 0x4A:
		LD(C, D);
		break;
	case 0x4B:
		LD(C, E);
		break;
	case 0x4C:
		LD(C, H);
		break;
	case 0x4D:
		LD(C, L);
		break;
	case 0x4E:
		LD(C, HL);
		break;
	case 0x4F:
		LD(C, A);
		break;


	case 0x50:
		LD(D, B);
		break;
	case 0x51:
		LD(D, C);
		break;
	case 0x52:
		LD(D, D);
		break;
	case 0x53:
		LD(D, E);
		break;
	case 0x54:
		LD(D, H);
		break;
	case 0x55:
		LD(D, L);
		break;
	case 0x56:
		LD(D, HL);
		break;
	case 0x57:
		LD(D, A);
		break;
	case 0x58:
		LD(E, B);
		break;
	case 0x59:
		LD(E, C);
		break;
	case 0x5A:
		LD(E, D);
		break;
	case 0x5B:
		LD(E, E);
		break;
	case 0x5C:
		LD(E, H);
		break;
	case 0x5D:
		LD(E, L);
		break;
	case 0x5E:
		LD(E, HL);
		break;
	case 0x5F:
		LD(E, A);
		break;


	case 0x60:
		LD(H, B);
		break;
	case 0x61:
		LD(H, C);
		break;
	case 0x62:
		LD(H, D);
		break;
	case 0x63:
		LD(H, E);
		break;
	case 0x64:
		LD(H, H);
		break;
	case 0x65:
		LD(H, L);
		break;
	case 0x66:
		LD(H, HL);
		break;
	case 0x67:
		LD(H, A);
		break;
	case 0x68:
		LD(L, B);
		break;
	case 0x69:
		LD(L, C);
		break;
	case 0x6A:
		LD(L, D);
		break;
	case 0x6B:
		LD(L, E);
		break;
	case 0x6C:
		LD(L, H);
		break;
	case 0x6D:
		LD(L, L);
		break;
	case 0x6E:
		LD(L, HL);
		break;
	case 0x6F:
		LD(L, A);
		break;


	case 0x70:
		LD(HL, B);
		break;
	case 0x71:
		LD(HL, C);
		break;
	case 0x72:
		LD(HL, D);
		break;
	case 0x73:
		LD(HL, E);
		break;
	case 0x74:
		LD(HL, H);
		break;
	case 0x75:
		LD(HL, L);
		break;
	case 0x76:
		HALT();
		break;
	case 0x77:
		LD(HL, A);
		break;
	case 0x78:
		LD(A, B);
		break;
	case 0x79:
		LD(A, C);
		break;
	case 0x7A:
		LD(A, D);
		break;
	case 0x7B:
		LD(A, E);
		break;
	case 0x7C:
		LD(A, H);
		break;
	case 0x7D:
		LD(A, L);
		break;
	case 0x7E:
		LD(A, HL);
		break;
	case 0x7F:
		LD(A, A);
		break;


	case 0x80:
		ADD(B);
		break;
	case 0x81:
		ADD(C);
		break;
	case 0x82:
		ADD(D);
		break;
	case 0x83:
		ADD(E);
		break;
	case 0x84:
		ADD(H);
		break;
	case 0x85:
		ADD(L);
		break;
	case 0x86:
		ADD(Mem->ReadByte(HL));
		break;
	case 0x87:
		ADD(A);
		break;
	case 0x88:
		ADC(B);
		break;
	case 0x89:
		ADC(C);
		break;
	case 0x8A:
		ADC(D);
		break;
	case 0x8B:
		ADC(E);
		break;
	case 0x8C:
		ADC(H);
		break;
	case 0x8D:
		ADC(L);
		break;
	case 0x8E:
		ADC(Mem->ReadByte(HL));
		break;
	case 0x8F:
		ADC(A);
		break;


	case 0x90:
		SUB(B);
		break;
	case 0x91:
		SUB(C);
		break;
	case 0x92:
		SUB(D);
		break;
	case 0x93:
		SUB(E);
		break;
	case 0x94:
		SUB(H);
		break;
	case 0x95:
		SUB(L);
		break;
	case 0x96:
		SUB(Mem->ReadByte(HL));
		break;
	case 0x97:
		SUB(A);
		break;
	case 0x98:
		SBC(B);
		break;
	case 0x99:
		SBC(C);
		break;
	case 0x9A:
		SBC(D);
		break;
	case 0x9B:
		SBC(E);
		break;
	case 0x9C:
		SBC(H);
		break;
	case 0x9D:
		SBC(L);
		break;
	case 0x9E:
		SBC(Mem->ReadByte(HL));
		break;
	case 0x9F:
		SBC(A);
		break;


	case 0xA0:
		AND(B);
		break;
	case 0xA1:
		AND(C);
		break;
	case 0xA2:
		AND(D);
		break;
	case 0xA3:
		AND(E);
		break;
	case 0xA4:
		AND(H);
		break;
	case 0xA5:
		AND(L);
		break;
	case 0xA6:
		AND(Mem->ReadByte(HL));
		break;
	case 0xA7:
		AND(A);
		break;
	case 0xA8:
		XOR(B);
		break;
	case 0xA9:
		XOR(C);
		break;
	case 0xAA:
		XOR(D);
		break;
	case 0xAB:
		XOR(E);
		break;
	case 0xAC:
		XOR(H);
		break;
	case 0xAD:
		XOR(L);
		break;
	case 0xAE:
		XOR(Mem->ReadByte(HL));
		break;
	case 0xAF:
		XOR(A);
		break;


	case 0xB0:
		OR(B);
		break;
	case 0xB1:
		OR(C);
		break;
	case 0xB2:
		OR(D);
		break;
	case 0xB3:
		OR(E);
		break;
	case 0xB4:
		OR(H);
		break;
	case 0xB5:
		OR(L);
		break;
	case 0xB6:
		OR(Mem->ReadByte(HL));
		break;
	case 0xB7:
		OR(A);
		break;
	case 0xB8:
		CP(B);
		break;
	case 0xB9:
		CP(C);
		break;
	case 0xBA:
		CP(D);
		break;
	case 0xBB:
		CP(E);
		break;
	case 0xBC:
		CP(H);
		break;
	case 0xBD:
		CP(L);
		break;
	case 0xBE:
		CP(Mem->ReadByte(HL));
		break;
	case 0xBF:
		CP(A);
		break;


	case 0xC0:
	{
		if (!GetFlag(Z_FLAG))
		{
			RET();
			tCycle += 12;
			Mem->UpdateTimer(12);
		}
		break;
	}
	case 0xC1:
		POP(BC);
		break;
	case 0xC2:
	{
		bool jump = !GetFlag(Z_FLAG);
		JP(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0xC3:
		JP();
		break;
	case 0xC4:
	{
		bool jump = !GetFlag(Z_FLAG);
		CALL(jump);
		if (jump)
		{
			tCycle += 12;
			Mem->UpdateTimer(12);
		}
		break;
	}
	case 0xC5:
		PUSH(BC);
		break;
	case 0xC6:
		ADD(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xC7:
		RST(0x00);
		break;
	case 0xC8:
	{
		if (GetFlag(Z_FLAG))
		{
			RET();
			tCycle += 12;
			Mem->UpdateTimer(12);
		}
		break;
	}
	case 0xC9:
		RET();
		break;
	case 0xCA:
	{
		bool jump = GetFlag(Z_FLAG);
		JP(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0xCB:
	{
		opcode = Mem->ReadByte(PC);
		PC++;
		ExecuteCB(opcode);
		return;
		break;
	}
	case 0xCC:
	{
		bool jump = GetFlag(Z_FLAG);
		CALL(jump);
		if (jump)
		{
			tCycle += 12;
			Mem->UpdateTimer(12);
		};
		break;
	}
	case 0xCD:
		CALL();
		break;
	case 0xCE:
		ADC(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xCF:
		RST(0x08);
		break;


	case 0xD0:
	{
		if (!GetFlag(C_FLAG))
		{
			RET();
			tCycle += 12;
			Mem->UpdateTimer(12);
		}
		break;
	}
	case 0xD1:
		POP(DE);
		break;
	case 0xD2:
	{
		bool jump = !GetFlag(C_FLAG);
		JP(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0xD4:
	{
		bool jump = !GetFlag(C_FLAG);
		CALL(jump);
		if (jump)
		{
			tCycle += 12;
			Mem->UpdateTimer(12);
		};
		break;
	}
	case 0xD5:
		PUSH(DE);
		break;
	case 0xD6:
		SUB(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xD7:
		RST(0x10);
		break;
	case 0xD8:
	{
		if (GetFlag(C_FLAG))
		{
			RET();
			tCycle += 12;
			Mem->UpdateTimer(12);
		}
		break;
	}
	case 0xD9:
		RET();
		IME = true;
		break;
	case 0xDA:
	{
		bool jump = GetFlag(C_FLAG);
		JP(jump);
		if (jump)
		{
			tCycle += 4;
			Mem->UpdateTimer(4);
		};
		break;
	}
	case 0xDC:
	{
		bool jump = GetFlag(C_FLAG);
		CALL(jump);
		if (jump)
		{
			tCycle += 12;
			Mem->UpdateTimer(12);
		};
		break;
	}
	case 0xDE:
		SBC(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xDF:
		RST(0x18);
		break;


	case 0xE0:
	{
		u8 offset = Mem->ReadByte(PC);
		PC++;
		u16 address = 0xFF00 + offset;
		Mem->UpdateTimer(4);
		tCycle += 4;
		Mem->WriteByte(address, A);
		break;
	}
	case 0xE1:
		POP(HL);
		break;
	case 0xE2:
		LD(0xFF00 + C, A);
		break;
	case 0xE5:
		PUSH(HL);
		break;
	case 0xE6:
		AND(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xE7:
		RST(0x20);
		break;
	case 0xE8:
		ADD_SP();
		break;
	case 0xE9:
		PC = HL;
		break;
	case 0xEA:
	{
		u16 address;
		address = Mem->ReadByte(PC);
		PC++;
		Mem->UpdateTimer(4);
		tCycle += 4;
		address |= (Mem->ReadByte(PC) << 8);
		PC++;
		Mem->UpdateTimer(4);
		tCycle += 4;
		LD(address, A);
		break;
	}
	case 0xEE:
		XOR(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xEF:
		RST(0x28);
		break;


	case 0xF0:
	{
		u8 offset = Mem->ReadByte(PC);
		Mem->UpdateTimer(4);
		PC++;
		u16 address = 0xFF00 + offset;
		LD(A, address);
		break;
	}
	case 0xF1:
		POP(AF);
		F &= 0xF0;
		break;
	case 0xF2:
	{
		u16 address = 0xFF00 + C;
		LD(A, address);
		break;
	}
	case 0xF3:
		DI();
		break;
	case 0xF5:
		PUSH(AF);
		break;
	case 0xF6:
		OR(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xF7:
		RST(0x30);
		break;
	case 0xF8:
		LD_HL_SP();
		break;
	case 0xF9:
		SP = HL;
		break;
	case 0xFA:
	{
		u16 address;
		address = Mem->ReadByte(PC);
		Mem->UpdateTimer(4);
		PC++;
		address |= (Mem->ReadByte(PC) << 8);
		Mem->UpdateTimer(4);
		PC++;
		LD(A, Mem->ReadByte(address));
		break;
	}
	case 0xFB:
		EI();
		break;
	case 0xFE:
		CP(Mem->ReadByte(PC));
		PC++;
		break;
	case 0xFF:
		RST(0x38);
		break;
	default:
		break;
	}

	tCycle += OpcodeCycle[opcode];
	Mem->UpdateTimer(OpcodeCycle[opcode]);
}

void CPU::ExecuteCB(u8 opcode)
{
	switch (opcode)
	{
	case 0x00:
		RLC(B);
		break;
	case 0x01:
		RLC(C);
		break;
	case 0x02:
		RLC(D);
		break;
	case 0x03:
		RLC(E);
		break;
	case 0x04:
		RLC(H);
		break;
	case 0x05:
		RLC(L);
		break;
	case 0x06:
		RLC(HL);
		break;
	case 0x07:
		RLC(A);
		break;
	case 0x08:
		RRC(B);
		break;
	case 0x09:
		RRC(C);
		break;
	case 0x0A:
		RRC(D);
		break;
	case 0x0B:
		RRC(E);
		break;
	case 0x0C:
		RRC(H);
		break;
	case 0x0D:
		RRC(L);
		break;
	case 0x0E:
		RRC(HL);
		break;
	case 0x0F:
		RRC(A);
		break;


	case 0x10:
		RL(B);
		break;
	case 0x11:
		RL(C);
		break;
	case 0x12:
		RL(D);
		break;
	case 0x13:
		RL(E);
		break;
	case 0x14:
		RL(H);
		break;
	case 0x15:
		RL(L);
		break;
	case 0x16:
		RL(HL);
		break;
	case 0x17:
		RL(A);
		break;
	case 0x18:
		RR(B);
		break;
	case 0x19:
		RR(C);
		break;
	case 0x1A:
		RR(D);
		break;
	case 0x1B:
		RR(E);
		break;
	case 0x1C:
		RR(H);
		break;
	case 0x1D:
		RR(L);
		break;
	case 0x1E:
		RR(HL);
		break;
	case 0x1F:
		RR(A);
		break;


	case 0x20:
		SLA(B);
		break;
	case 0x21:
		SLA(C);
		break;
	case 0x22:
		SLA(D);
		break;
	case 0x23:
		SLA(E);
		break;
	case 0x24:
		SLA(H);
		break;
	case 0x25:
		SLA(L);
		break;
	case 0x26:
		SLA(HL);
		break;
	case 0x27:
		SLA(A);
		break;
	case 0x28:
		SRA(B);
		break;
	case 0x29:
		SRA(C);
		break;
	case 0x2A:
		SRA(D);
		break;
	case 0x2B:
		SRA(E);
		break;
	case 0x2C:
		SRA(H);
		break;
	case 0x2D:
		SRA(L);
		break;
	case 0x2E:
		SRA(HL);
		break;
	case 0x2F:
		SRA(A);
		break;


	case 0x30:
		SWAP(B);
		break;
	case 0x31:
		SWAP(C);
		break;
	case 0x32:
		SWAP(D);
		break;
	case 0x33:
		SWAP(E);
		break;
	case 0x34:
		SWAP(H);
		break;
	case 0x35:
		SWAP(L);
		break;
	case 0x36:
		SWAP(HL);
		break;
	case 0x37:
		SWAP(A);
		break;
	case 0x38:
		SRL(B);
		break;
	case 0x39:
		SRL(C);
		break;
	case 0x3A:
		SRL(D);
		break;
	case 0x3B:
		SRL(E);
		break;
	case 0x3C:
		SRL(H);
		break;
	case 0x3D:
		SRL(L);
		break;
	case 0x3E:
		SRL(HL);
		break;
	case 0x3F:
		SRL(A);
		break;


	case 0x40: 
		BIT(0, B); 
		break;
	case 0x41: 
		BIT(0, C); 
		break;
	case 0x42: 
		BIT(0, D); 
		break;
	case 0x43: 
		BIT(0, E); 
		break;
	case 0x44: 
		BIT(0, H); 
		break;
	case 0x45: 
		BIT(0, L); 
		break;
	case 0x46: 
		Mem->UpdateTimer(4);
		BIT(0, Mem->ReadByte(HL)); 
		break;
	case 0x47:
		BIT(0, A); 
		break;
	case 0x48: 
		BIT(1, B); 
		break;
	case 0x49: 
		BIT(1, C); 
		break;
	case 0x4A: 
		BIT(1, D); 
		break;
	case 0x4B: 
		BIT(1, E); 
		break;
	case 0x4C: 
		BIT(1, H); 
		break;
	case 0x4D: 
		BIT(1, L); 
		break;
	case 0x4E: 
		Mem->UpdateTimer(4);
		BIT(1, Mem->ReadByte(HL)); 
		break;
	case 0x4F: 
		BIT(1, A); 
		break;


	case 0x50: 
		BIT(2, B); 
		break;
	case 0x51: 
		BIT(2, C); 
		break;
	case 0x52: 
		BIT(2, D); 
		break;
	case 0x53: 
		BIT(2, E); 
		break;
	case 0x54: 
		BIT(2, H); 
		break;
	case 0x55: 
		BIT(2, L); 
		break;
	case 0x56: 
		Mem->UpdateTimer(4);
		BIT(2, Mem->ReadByte(HL)); 
		break;
	case 0x57: 
		BIT(2, A); 
		break;
	case 0x58: 
		BIT(3, B); 
		break;
	case 0x59: 
		BIT(3, C); 
		break;
	case 0x5A: 
		BIT(3, D); 
		break;
	case 0x5B: 
		BIT(3, E); 
		break;
	case 0x5C:
		BIT(3, H);
		break;
	case 0x5D: 
		BIT(3, L);
		break;
	case 0x5E:
		Mem->UpdateTimer(4);
		BIT(3, Mem->ReadByte(HL));
		break;
	case 0x5F:
		BIT(3, A);
		break;


	case 0x60: 
		BIT(4, B); 
		break;
	case 0x61: 
		BIT(4, C); 
		break;
	case 0x62: 
		BIT(4, D); 
		break;
	case 0x63: 
		BIT(4, E); 
		break;
	case 0x64: 
		BIT(4, H); 
		break;
	case 0x65: 
		BIT(4, L); 
		break;
	case 0x66: 
		Mem->UpdateTimer(4);
		BIT(4, Mem->ReadByte(HL)); 
		break;
	case 0x67:
		BIT(4, A); 
		break;
	case 0x68:
		BIT(5, B);
		break;
	case 0x69:
		BIT(5, C);
		break;
	case 0x6A:
		BIT(5, D);
		break;
	case 0x6B: 
		BIT(5, E);
		break;
	case 0x6C: 
		BIT(5, H); 
		break;
	case 0x6D: 
		BIT(5, L);
		break;
	case 0x6E: 
		Mem->UpdateTimer(4);
		BIT(5, Mem->ReadByte(HL)); 
		break;
	case 0x6F: 
		BIT(5, A); 
		break;


	case 0x70: 
		BIT(6, B); 
		break;
	case 0x71: 
		BIT(6, C); 
		break;
	case 0x72: 
		BIT(6, D); 
		break;
	case 0x73: 
		BIT(6, E); 
		break;
	case 0x74: 
		BIT(6, H); 
		break;
	case 0x75: 
		BIT(6, L); 
		break;
	case 0x76: 
		Mem->UpdateTimer(4);
		BIT(6, Mem->ReadByte(HL)); 
		break;
	case 0x77: 
		BIT(6, A); 
		break;
	case 0x78: 
		BIT(7, B); 
		break;
	case 0x79: 
		BIT(7, C); 
		break;
	case 0x7A: 
		BIT(7, D); 
		break;
	case 0x7B: 
		BIT(7, E); 
		break;
	case 0x7C: 
		BIT(7, H);
		break;
	case 0x7D: 
		BIT(7, L); 
		break;
	case 0x7E: 
		Mem->UpdateTimer(4);
		BIT(7, Mem->ReadByte(HL));
		break;
	case 0x7F: 
		BIT(7, A); 
		break;


	case 0x80:
		RES(0, B); 
		break;
	case 0x81: 
		RES(0, C); 
		break;
	case 0x82: 
		RES(0, D); 
		break;
	case 0x83: 
		RES(0, E); 
		break;
	case 0x84: 
		RES(0, H); 
		break;
	case 0x85: 
		RES(0, L);
		break;
	case 0x86: 
		RES(0, HL); 
		break;
	case 0x87: 
		RES(0, A); 
		break;
	case 0x88: 
		RES(1, B); 
		break;
	case 0x89: 
		RES(1, C); 
		break;
	case 0x8A: 
		RES(1, D); 
		break;
	case 0x8B: 
		RES(1, E);
		break;
	case 0x8C:
		RES(1, H);
		break;
	case 0x8D: 
		RES(1, L);
		break;
	case 0x8E: 
		RES(1, HL); 
		break;
	case 0x8F: 
		RES(1, A); 
		break;


	case 0x90: 
		RES(2, B); 
		break;
	case 0x91: 
		RES(2, C); 
		break;
	case 0x92: 
		RES(2, D); 
		break;
	case 0x93: 
		RES(2, E);
		break;
	case 0x94: 
		RES(2, H);
		break;
	case 0x95:
		RES(2, L); 
		break;
	case 0x96:
		RES(2, HL);
		break;
	case 0x97: 
		RES(2, A); 
		break;
	case 0x98: 
		RES(3, B); 
		break;
	case 0x99: 
		RES(3, C); 
		break;
	case 0x9A: 
		RES(3, D); 
		break;
	case 0x9B: 
		RES(3, E); 
		break;
	case 0x9C: 
		RES(3, H); 
		break;
	case 0x9D: 
		RES(3, L); 
		break;
	case 0x9E: 
		RES(3, HL); 
		break;
	case 0x9F: 
		RES(3, A); 
		break;


	case 0xA0: 
		RES(4, B); 
		break;
	case 0xA1: 
		RES(4, C); 
		break;
	case 0xA2: 
		RES(4, D); 
		break;
	case 0xA3:
		RES(4, E); 
		break;
	case 0xA4: 
		RES(4, H); 
		break;
	case 0xA5: 
		RES(4, L); 
		break;
	case 0xA6: 
		RES(4, HL); 
		break;
	case 0xA7: 
		RES(4, A); 
		break;
	case 0xA8: 
		RES(5, B); 
		break;
	case 0xA9: 
		RES(5, C); 
		break;
	case 0xAA: 
		RES(5, D); 
		break;
	case 0xAB: 
		RES(5, E); 
		break;
	case 0xAC:
		RES(5, H); 
		break;
	case 0xAD: 
		RES(5, L);
		break;
	case 0xAE: 
		RES(5, HL); 
		break;
	case 0xAF: 
		RES(5, A); 
		break;


	case 0xB0: 
		RES(6, B); 
		break;
	case 0xB1: 
		RES(6, C); 
		break;
	case 0xB2: 
		RES(6, D); 
		break;
	case 0xB3: 
		RES(6, E);
		break;
	case 0xB4: 
		RES(6, H);
		break;
	case 0xB5: 
		RES(6, L); 
		break;
	case 0xB6:
		RES(6, HL);
		break;
	case 0xB7: 
		RES(6, A);
		break;
	case 0xB8:
		RES(7, B);
		break;
	case 0xB9:
		RES(7, C); 
		break;
	case 0xBA: 
		RES(7, D);
		break;
	case 0xBB:
		RES(7, E); 
		break;
	case 0xBC:
		RES(7, H);
		break;
	case 0xBD:
		RES(7, L);
		break;
	case 0xBE:
		RES(7, HL);
		break;
	case 0xBF:
		RES(7, A); 
		break;


		// SET b, r — Set bit b in register r
	case 0xC0: SET(0, B); break;
	case 0xC1: SET(0, C); break;
	case 0xC2: SET(0, D); break;
	case 0xC3: SET(0, E); break;
	case 0xC4: SET(0, H); break;
	case 0xC5: SET(0, L); break;
	case 0xC6: SET(0, HL); break;
	case 0xC7: SET(0, A); break;

	case 0xC8: SET(1, B); break;
	case 0xC9: SET(1, C); break;
	case 0xCA: SET(1, D); break;
	case 0xCB: SET(1, E); break;
	case 0xCC: SET(1, H); break;
	case 0xCD: SET(1, L); break;
	case 0xCE: SET(1, HL); break;
	case 0xCF: SET(1, A); break;

	case 0xD0: SET(2, B); break;
	case 0xD1: SET(2, C); break;
	case 0xD2: SET(2, D); break;
	case 0xD3: SET(2, E); break;
	case 0xD4: SET(2, H); break;
	case 0xD5: SET(2, L); break;
	case 0xD6: SET(2, HL); break;
	case 0xD7: SET(2, A); break;

	case 0xD8: SET(3, B); break;
	case 0xD9: SET(3, C); break;
	case 0xDA: SET(3, D); break;
	case 0xDB: SET(3, E); break;
	case 0xDC: SET(3, H); break;
	case 0xDD: SET(3, L); break;
	case 0xDE: SET(3, HL); break;
	case 0xDF: SET(3, A); break;

	case 0xE0: SET(4, B); break;
	case 0xE1: SET(4, C); break;
	case 0xE2: SET(4, D); break;
	case 0xE3: SET(4, E); break;
	case 0xE4: SET(4, H); break;
	case 0xE5: SET(4, L); break;
	case 0xE6: SET(4, HL); break;
	case 0xE7: SET(4, A); break;

	case 0xE8: SET(5, B); break;
	case 0xE9: SET(5, C); break;
	case 0xEA: SET(5, D); break;
	case 0xEB: SET(5, E); break;
	case 0xEC: SET(5, H); break;
	case 0xED: SET(5, L); break;
	case 0xEE: SET(5, HL); break;
	case 0xEF: SET(5, A); break;

	case 0xF0: SET(6, B); break;
	case 0xF1: SET(6, C); break;
	case 0xF2: SET(6, D); break;
	case 0xF3: SET(6, E); break;
	case 0xF4: SET(6, H); break;
	case 0xF5: SET(6, L); break;
	case 0xF6: SET(6, HL); break;
	case 0xF7: SET(6, A); break;

	case 0xF8: SET(7, B); break;
	case 0xF9: SET(7, C); break;
	case 0xFA: SET(7, D); break;
	case 0xFB: SET(7, E); break;
	case 0xFC: SET(7, H); break;
	case 0xFD: SET(7, L); break;
	case 0xFE: SET(7, HL); break;
	case 0xFF: SET(7, A); break;
	}
	tCycle += CBCycle[opcode];
	Mem->UpdateTimer(CBCycle[opcode]);
}

u8 CPU::Step()
{  
	tCycle = 0;
	if (isStop) {
		//if joypad -> !isStop
		if ((Mem->IE & Mem->IF) & 0x10)
			isStop = false;
		else
			return 0;
	}
	if (isHalt) {
		Mem->UpdateTimer(4);
		tCycle += 4;
		bool pending = (Mem->IE & Mem->IF) != 0;
		if (pending){
			isHalt = false;
			if (!IME)
				haltBug = true;
		}
		return tCycle;
	}

	u8 opcode = Mem->ReadByte(PC);
	if (haltBug)
		haltBug = false;
	else
		PC++;
	Execute(opcode);

	if (IMEpending)
	{
		IME = true;
		IMEpending = false;
	}
	Interrupt();
	return tCycle;

}

void CPU::Interrupt()
{
	u8 pending = Mem->IE & Mem->IF;
	if (pending & 0x1F)
		isHalt = false;

	if (!IME)
		return;

	if (pending & 0x01) {       // VBlank
		Mem->IF &= ~0x01;
		PUSH(PC);
		PC = 0x0040;
		IME = false;
		isHalt = false;
		tCycle += 20;
		Mem->UpdateTimer(20);
	}
	else if (pending & 0x02) {  // LCD STAT
		Mem->IF &= ~0x02;
		PUSH(PC);
		PC = 0x0048;
		IME = false;
		isHalt = false;
		tCycle += 20;
		Mem->UpdateTimer(20);
	}
	else if (pending & 0x04) {  // Timer
		Mem->IF &= ~0x04;
		PUSH(PC);
		PC = 0x0050;
		IME = false;
		isHalt = false;
		tCycle += 20;
		Mem->UpdateTimer(20);
	}
	else if (pending & 0x08) {  // Serial
		Mem->IF &= ~0x08;
		PUSH(PC);
		PC = 0x0058;
		IME = false;
		isHalt = false;
		tCycle += 20;
		Mem->UpdateTimer(20);
	}
	else if (pending & 0x10) {  // Joypad
		Mem->IF &= ~0x10;
		PUSH(PC);
		PC = 0x0060;
		IME = false;
		isHalt = false;
		tCycle += 20;
		Mem->UpdateTimer(20);
	}



}

void CPU::LD(u8& reg1, u8 reg)
{
	reg1 = reg;
}
void CPU::LD(u8& reg, u16 address)
{
	reg = Mem->ReadByte(address);
}
void CPU::LD(u16 address, u8 reg)
{
	Mem->WriteByte(address, reg);
}
void CPU::LD_IMM(u16& reg)
{
	u8 lo = Mem->ReadByte(PC++);
	u8 hi = Mem->ReadByte(PC++);
	reg = (hi << 8) | lo;
}
void CPU::LD_IMM(u8& reg)
{
	reg = Mem->ReadByte(PC);
	PC++;
}
void CPU::LD_IMM_HL()
{
	//
	u8 value = Mem->ReadByte(PC);
	//Mem->UpdateTimer(4);
	Mem->WriteByte(HL, value);
	PC++;

}
void CPU::LD_a16_SP()
{
	u16 address = 0;
	address = Mem->ReadByte(PC);
	PC++;
	address |= (Mem->ReadByte(PC) << 8);
	PC++;
	Mem->WriteByte(address, SP & 0xFF);
	Mem->WriteByte(address + 1, (SP >> 8) & 0xFF);
}
void CPU::LDI_HL_A()
{
	Mem->WriteByte(HL, A);
	HL++;
}
void CPU::LDD_HL_A()
{
	Mem->WriteByte(HL, A);
	HL--;
}
void CPU::LDI_A_HL()
{
	A = Mem->ReadByte(HL);
	HL++;
}
void CPU::LDD_A_HL()
{
	A = Mem->ReadByte(HL);
	HL--;
}
void CPU::LD_HL_SP()
{
	i8 value = Mem->ReadByte(PC);
	PC++;

	u16 oldSP = SP;
	u16 result = SP + value;
	u8 uvalue = static_cast<u8>(value);

	SetFlag(Z_FLAG, false);
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, ((SP ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10);
	SetFlag(C_FLAG, ((SP ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100);

	HL = result;
}

void CPU::INC(u16& reg)
{
	reg++;
}
void CPU::INC(u8& reg)
{
	u8 result = reg + 1;
	SetFlag(H_FLAG, ((reg&0x0F) + 1) > 0x0F);
	SetFlag(Z_FLAG, result == 0);
	SetFlag(N_FLAG, false);
	reg = result;
}
void CPU::INC_HL()
{
	u8 value = Mem->ReadByte(HL);
	Mem->UpdateTimer(4);
	tCycle += 4;
	INC(value);
	Mem->WriteByte(HL, value);
}
void CPU::DEC(u16& reg)
{
	reg--;
}
void CPU::DEC(u8& reg)
{
	SetFlag(H_FLAG, ((reg & 0x0F) -1) < 0x00);
	reg--;
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(N_FLAG, true);
}
void CPU::DEC_HL()
{
	u8 value = Mem->ReadByte(HL);
	Mem->UpdateTimer(4);
	tCycle += 4;
	DEC(value);
	Mem->WriteByte(HL, value);
}

void CPU::DAA()
{
	u8 a = A;
	u8 adjust = 0;

	if (GetFlag(N_FLAG)) {
		if (GetFlag(H_FLAG)) adjust |= 0x06;
		if (GetFlag(C_FLAG)) adjust |= 0x60;
		a -= adjust;
	}
	else {
		if (GetFlag(H_FLAG) || (a & 0x0F) > 0x09) adjust |= 0x06;
		if (GetFlag(C_FLAG) || a > 0x99) {
			adjust |= 0x60;
			SetFlag(C_FLAG, true);
		}
		else {
			SetFlag(C_FLAG, false);
		}
		a += adjust;
	}

	SetFlag(H_FLAG, false);
	SetFlag(Z_FLAG, a == 0);
	A = a;
}
void CPU::CPL()
{
	A = ~A;
	SetFlag(N_FLAG, true);
	SetFlag(H_FLAG, true);
}
void CPU::SCF()
{
	SetFlag(C_FLAG, true);
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, false);
}
void CPU::CCF()
{
	SetFlag(C_FLAG, !GetFlag(C_FLAG));
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, false);
}

void CPU::RLCA()
{
	u8 carry = 0;
	carry = (A >> 7) & 0x01;
	A <<= 1;
	A |= carry;
	SetFlag(Z_FLAG, 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, (bool)carry);
}
void CPU::RRCA()
{
	u8 carry = 0;
	carry = A & 0x01;
	A >>= 1;
	A |= (carry << 7);

	SetFlag(Z_FLAG, 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, (bool)carry);
}
void CPU::RLA()
{
	u8 carry = GetFlag(C_FLAG);
	u8 bit7 = A >> 7;
	A <<= 1;
	SetFlag(C_FLAG, (bool)bit7);
	A |= carry;
	SetFlag(Z_FLAG, false);
	SetFlag(H_FLAG, false);
	SetFlag(N_FLAG, false);
}
void CPU::RRA()
{
	u8 carry = GetFlag(C_FLAG);
	u8 bit0 = A & 0x01;
	A >>= 1;
	SetFlag(C_FLAG, (bool)bit0);
	A |= (carry << 7);
	SetFlag(Z_FLAG, false);
	SetFlag(H_FLAG, false);
	SetFlag(N_FLAG, false);
}

void CPU::RLC(u8& reg)
{
	u8 carry = 0;
	carry = (reg >> 7) & 0x01;
	reg <<= 1;
	reg |= carry;
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, (bool)carry);
}
void CPU::RLC(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	Mem->UpdateTimer(4);
	tCycle += 4;
	RLC(reg);
	Mem->WriteByte(address, reg);
}
void CPU::RRC(u8& reg)
{
	u8 carry = 0;
	carry = reg & 0x01;
	reg >>= 1;
	reg |= (carry << 7);

	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, (bool)carry);
}
void CPU::RRC(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	RRC(reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}
void CPU::RL(u8& reg)
{
	u8 carry = GetFlag(C_FLAG);
	u8 bit7 = reg >> 7;
	reg <<= 1;
	SetFlag(C_FLAG, (bool)bit7);
	reg |= carry;
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, false);
	SetFlag(N_FLAG, false);
}
void CPU::RL(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	RL(reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}
void CPU::RR(u8& reg)
{
	u8 carry = GetFlag(C_FLAG);
	u8 bit0 = reg & 0x01;
	reg >>= 1;
	SetFlag(C_FLAG, (bool)bit0);
	reg |= (carry << 7);
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, false);
	SetFlag(N_FLAG, false);
}
void CPU::RR(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	RR(reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}

void CPU::SLA(u8& reg)
{
	u8 carry = 0;
	carry = (reg >> 7) & 0x01;
	reg <<= 1;
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, (bool)carry);

}
void CPU::SLA(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	SLA(reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}
void CPU::SRA(u8& reg)
{
	u8 carry = reg & 0x01;
	u8 bit7 = reg & 0x80;
	reg >>= 1;
	reg |= bit7;
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, (bool)carry);
}
void CPU::SRA(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	SRA(reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}
void CPU::SWAP(u8& reg)
{
	u8 upper = reg & 0xF0;
	u8 lower = reg & 0x0F;
	lower <<= 4;
	upper >>= 4;
	reg = lower | upper;
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, 0);
}
void CPU::SWAP(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	SWAP(reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}
void CPU::SRL(u8& reg)
{
	u8 carry = reg & 0x01;
	reg >>= 1;
	SetFlag(Z_FLAG, reg == 0);
	SetFlag(H_FLAG, 0);
	SetFlag(N_FLAG, 0);
	SetFlag(C_FLAG, (bool)carry);
}
void CPU::SRL(u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	SRL(reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}

void CPU::BIT(u8 bit, u8 reg)
{
	u8 Bit = reg & (1 << bit);
	SetFlag(Z_FLAG, Bit == 0);
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, true);
}
void CPU::RES(u8 bit, u8& reg)
{
	u8 Bit = ~(1 << bit);
	reg &= Bit;
}
void CPU::RES(u8 bit, u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	RES(bit, reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}
void CPU::SET(u8 bit, u8& reg)
{
	u8 Bit = (1 << bit);
	reg |= Bit;
}
void CPU::SET(u8 bit, u16 address)
{
	Mem->UpdateTimer(4);
	tCycle += 4;
	u8 reg = Mem->ReadByte(address);
	SET(bit, reg);
	Mem->UpdateTimer(4);
	tCycle += 4;
	Mem->WriteByte(address, reg);
}

void CPU::JR()
{
	i8 address = Mem->ReadByte(PC);
	PC++;
	PC += address;
}
void CPU::JR(bool Condition)
{
	i8 address = Mem->ReadByte(PC);
	PC++;
	if (Condition)
		PC += address;
}
void CPU::JP()
{
	u8 low = Mem->ReadByte(PC);
	PC++;
	u8 high = Mem->ReadByte(PC);
	PC++;
	PC = (high << 8) | low;
}
void CPU::JP(bool condition)
{
	u8 low = Mem->ReadByte(PC);
	PC++;
	u8 high = Mem->ReadByte(PC);
	PC++;
	if(condition)
		PC = (high << 8) | low;
}

void CPU::ADD(u16& reg1, u16 reg2)
{
	bool HalfCarry = ((reg1 & 0x0FFF) + (reg2 & 0x0FFF) > 0x0FFF);
	bool Carry = (((u32)reg1 + (u32)reg2) > 0xFFFF);

	reg1 += reg2;
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, HalfCarry);
	SetFlag(C_FLAG, Carry);
}
void CPU::ADD(u8 value)
{
	SetFlag(H_FLAG, ((A & 0x0F) + (value & 0x0F)) > 0x0F);
	SetFlag(C_FLAG, ((u16)A + (u16)value) > 0xFF);
	A += value;
	SetFlag(Z_FLAG, A == 0);
	SetFlag(N_FLAG, false);
}
void CPU::ADD_SP()
{
	i8 value = Mem->ReadByte(PC);
	PC++;
	u16 oldSP = SP;
	u16 result = SP + value;
	u8 uvalue = (u8)value;

	SetFlag(Z_FLAG, false);
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, ((oldSP & 0x0F) + (uvalue & 0x0F)) > 0x0F);
	SetFlag(C_FLAG, ((oldSP & 0xFF) + uvalue) > 0xFF);

	SP = result;
}
void CPU::ADC(u8 value)
{
	u8 carry = GetFlag(C_FLAG) ? 1 : 0;

	SetFlag(H_FLAG, ((A & 0x0F) + (value & 0x0F) + carry) > 0x0F);
	SetFlag(C_FLAG, ((u16)A + (u16)value + carry) > 0xFF);

	A = A + value + carry;

	SetFlag(Z_FLAG, A == 0);
	SetFlag(N_FLAG, false);
}
void CPU::SUB(u8 value)
{
	SetFlag(C_FLAG, value > A);
	SetFlag(H_FLAG, (A & 0x0F) < (value & 0x0F));
	A -= value;
	SetFlag(Z_FLAG, A == 0);
	SetFlag(N_FLAG, true);
}
void CPU::SBC(u8 value)
{
	u8 carry = GetFlag(C_FLAG);
	SetFlag(C_FLAG, (value + carry > A));
	SetFlag(H_FLAG, (A & 0x0F) < ((value & 0x0F) + carry));
	A = A - value - carry;
	SetFlag(Z_FLAG, A == 0);
	SetFlag(N_FLAG, true);
}


void CPU::AND(u8 value)
{
	A &= value;
	SetFlag(Z_FLAG, A == 0);
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, true);
	SetFlag(C_FLAG, false);
}
void CPU::XOR(u8 value)
{
	A ^= value;
	SetFlag(Z_FLAG, A == 0);
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, false);
	SetFlag(C_FLAG, false);
}
void CPU::OR(u8 value)
{
	A |= value;
	SetFlag(Z_FLAG, A == 0);
	SetFlag(N_FLAG, false);
	SetFlag(H_FLAG, false);
	SetFlag(C_FLAG, false);
}
void CPU::CP(u8 value)
{
	SetFlag(C_FLAG, value > A);
	SetFlag(H_FLAG, (A & 0x0F) < (value & 0x0F));
	SetFlag(Z_FLAG, A == value);
	SetFlag(N_FLAG, true);
}

void CPU::POP(u16& reg)
{
	u8 low = Mem->ReadByte(SP);
	SP++;
	u8 high = Mem->ReadByte(SP);
	SP++;
	reg = (high << 8) | low;
}
void CPU::PUSH(u16 reg)
{
	SP--;
	u8 high = (reg >> 8) & 0xFF;
	Mem->WriteByte(SP, high);
	SP--;
	u8 low = reg & 0xFF;
	Mem->WriteByte(SP, low);
}
void CPU::CALL()
{
	u8 low = Mem->ReadByte(PC);
	PC++;
	u8 high = Mem->ReadByte(PC);
	PC++;
	u16 address = (high << 8) | low;
	PUSH(PC);
	PC = address;
}
void CPU::CALL(bool condition)
{
	u8 low = Mem->ReadByte(PC);
	PC++;
	u8 high = Mem->ReadByte(PC);
	PC++;
	u16 address = (high << 8) | low;
	if (condition)
	{
		PUSH(PC);
		PC = address;
	}
}
void CPU::RST(u16 value)
{
	PUSH(PC);
	PC = value;
}
void CPU::RET()
{
	POP(PC);
}

void CPU::DI()
{
	IME = false;
}
void CPU::EI()
{
	IMEpending = true;
}

void CPU::STOP()
{
	PC++;
	isStop = true;
}
void CPU::HALT()
{
	if (!IME && (Mem->IF & Mem->IE & 0x1F)) {
		haltBug = true;
		isHalt = false;
	}
	else
		isHalt = true;
}
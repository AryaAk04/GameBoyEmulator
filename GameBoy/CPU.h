#pragma once
#include <iostream>
#include <string>
#include "Defs.h"
#include "Memory.h"

class CPU
{
private:
	u8 OpcodeCycle[256]{
		4, 12, 8, 8,  4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4,
		0, 12, 8, 8,  4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
		8, 12, 8, 8,  4,  4,  8, 4, 8, 8, 8, 8, 4, 4, 8, 4,
		8, 12, 8, 8, 12, 12, 12, 4, 8, 8, 8, 8, 4, 4, 8, 4,
		4,  4, 4, 4,  4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4,  4, 4, 4,  4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4,  4, 4, 4,  4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		8,  8, 8, 8,  8, 8, 0, 8, 4, 4, 4, 4, 4, 4, 8, 4,
		4,  4, 4, 4,  4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4,  4, 4, 4,  4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4,  4, 4, 4,  4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4,  4, 4, 4,  4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		8, 12, 12, 16, 12, 16, 8, 16, 8, 16, 12, 4, 12, 24, 8, 16,
		8, 12, 12, 0,  12, 16, 8, 16, 8, 16, 12, 0, 12, 0, 8, 16,
        12, 12, 8, 0, 0, 16, 8, 16, 16, 4, 16, 0, 0, 0, 8, 16,
		12, 12, 8, 4, 0, 16, 8, 16, 12, 8, 16, 4, 0, 0, 8, 16
	};

    u8 CBCycle[256]{
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    };


public:
	CPU(Memory* mem);
	void NoBootInit();
	u8 Step();

private:
	union {
		struct
		{
			u8 F;
			u8 A;
		};
		u16 AF;
	};
	union {
		struct {
			u8 C;
			u8 B;
		};
		u16 BC;
	};
	union {
		struct {
			u8 E;
			u8 D;
		};
		u16 DE;
	};
	union {
		struct {
			u8 L;
			u8 H;
		};
		u16 HL;
	};

	u16 PC;
	u16 SP;
	u8 tCycle;

	bool IME;
	bool IMEpending;
	bool isStop;
	bool isHalt;
    bool haltBug;

	Memory* Mem;



	void SetFlag(u8 flag, bool state);
	bool GetFlag(u8 flag);

	void Execute(u8 opcode);
	void ExecuteCB(u8 opcode);

	void Interrupt();

	void LD(u8& reg1, u8 reg);
	void LD(u16 address, u8 reg);
	void LD(u8& reg, u16 address);
	void LD_IMM(u16& reg);
	void LD_IMM(u8& reg);
	void LD_IMM_HL();
	void LD_a16_SP();
	void LDI_HL_A();
	void LDD_HL_A();
	void LDI_A_HL();
	void LDD_A_HL();
	void LD_HL_SP();

	void INC(u16& reg);
	void INC(u8& reg);
	void INC_HL();

	void DEC(u16& reg);
	void DEC(u8& reg);
	void DEC_HL();

	void DAA();
	void CPL();
	void SCF();
	void CCF();

	void RLCA();
	void RRCA();
	void RLA();
	void RRA();

    void RLC(u8& reg);
    void RLC(u16 address);
    void RRC(u8& reg);
    void RRC(u16 address);
    void RL(u8& reg);
    void RL(u16 address);
    void RR(u8& reg);
    void RR(u16 address);

    void SLA(u8& reg);
    void SLA(u16 address);
    void SRA(u8& reg);
    void SRA(u16 address);
    void SWAP(u8& reg);
    void SWAP(u16 address);
    void SRL(u8& reg);
    void SRL(u16 address);

    void BIT(u8 bit, u8 reg);
    void RES(u8 bit, u8& reg);
    void RES(u8 bit, u16 address);
    void SET(u8 bit, u8& reg);
    void SET(u8 bit, u16 address);


	void JR();
	void JR(bool condition);
	void JP();
	void JP(bool condition);

	void ADD(u16& Reg1, u16 reg2);
	void ADD(u8 value);
	void ADD_SP();
	void ADC(u8 value);
	void SUB(u8 value);
	void SBC(u8 value);

	void AND(u8 value);
	void XOR(u8 value);
	void OR(u8 value);
	void CP(u8 value);

	void POP(u16& Reg);
	void PUSH(u16 reg);
	void CALL();
	void CALL(bool condition);
	void RST(u16 value);
	void RET();

	void DI();
	void EI();

	void STOP();
	void HALT();
};


#include <CLARA/pch.h>
#include <CLARA/Assembly.h>

using namespace CLARA;
using namespace CLARA::CLASM;

const auto keywords = array<string, Keyword::MAX>{
	/* Global  */ "global",
	/* Extern  */ "extern",
	/* Import  */ "import",
	/* Include */ "include",
};
const auto mnemonics = array<string, Mnemonic::MAX>{
	/* PUSH  */ "push",
	/* PUSHA */ "pusha",
	/* POP   */ "pop",
	/* DUP   */ "dup",
	/* JMP   */ "jmp",
	/* CALL  */ "call",
};
const auto mnemonicTable = array<vector<InstructionOverload>, static_cast<size_t>(Mnemonic::MAX)>{{
	/* PUSH */ {
		{Instruction::PUSHN,  {}},
		{Instruction::PUSHB,  {OperandType::IMM8}},
		{Instruction::PUSHW,  {OperandType::IMM16}},
		{Instruction::PUSHD,  {OperandType::IMM32}},
		{Instruction::PUSHQ,  {OperandType::IMM64}},
		{Instruction::PUSHF,  {OperandType::IMM32}},
		{Instruction::PUSHQF, {OperandType::IMM64}},
	},
	/* PUSHA */ {
		{Instruction::PUSHAB, {OperandType::IMM8}},
		{Instruction::PUSHAW, {OperandType::IMM16}},
		{Instruction::PUSHAD, {OperandType::IMM32}},
		{Instruction::PUSHAQ, {OperandType::IMM32}},
		{Instruction::PUSHAF, {OperandType::IMM32}},
	},
	/* POP */ {
		{Instruction::POP,    {OperandType::IMM8}},
		{Instruction::POPLN,  {OperandType::LV8}},
		{Instruction::POPL,   {OperandType::LV16}},
		{Instruction::POPLE,  {OperandType::LV32}},
		{Instruction::POPV,   {OperandType::LV16}},
		{Instruction::POPVE,  {OperandType::LV32}},
	},
	/* DUP */ {
		{Instruction::DUP,    {}},
		{Instruction::DUPE,   {OperandType::IMM8}},
	},
	/* JMP */ {
		{Instruction::JMP,    {}},
		{Instruction::JMPD,   {OperandType::REL32}},
	},
	/* CALL */ {
		{Instruction::CALL,   {}},
		{Instruction::CALLD,  {OperandType::REL32}},
	},
}};
const auto instructionMap = unordered_map<string, Instruction::Type>{
	{"nop",     Instruction::NOP},
	{"break",   Instruction::BREAK},
	{"throw",   Instruction::THROW},
	{"pushn",   Instruction::PUSHN},
	{"pushb",   Instruction::PUSHB},
	{"pushw",   Instruction::PUSHW},
	{"pushd",   Instruction::PUSHD},
	{"pushf",   Instruction::PUSHF},
	{"pushab",  Instruction::PUSHAB},
	{"pushaw",  Instruction::PUSHAW},
	{"pushaf",  Instruction::PUSHAF},
	{"pushs",   Instruction::PUSHS},
	{"pop",     Instruction::POP},
	{"popln",   Instruction::POPLN},
	{"popl",    Instruction::POPL},
	{"pople",   Instruction::POPLE},
	{"popv",    Instruction::POPV},
	{"popve",   Instruction::POPVE},
	{"swap",    Instruction::SWAP},
	{"dup",     Instruction::DUP},
	{"dupe",    Instruction::DUPE},
	{"local",   Instruction::LOCAL},
	{"global",  Instruction::GLOBAL},
	{"array",   Instruction::ARRAY},
	{"exf",     Instruction::EXF},
	{"inc",     Instruction::INC},
	{"dec",     Instruction::DEC},
	{"add",     Instruction::ADD},
	{"sub",     Instruction::SUB},
	{"mul",     Instruction::MUL},
	{"div",     Instruction::DIV},
	{"mod",     Instruction::MOD},
	{"not",     Instruction::NOT},
	{"and",     Instruction::AND},
	{"or",      Instruction::OR},
	{"xor",     Instruction::XOR},
	{"shl",     Instruction::SHL},
	{"shr",     Instruction::SHR},
	{"neg",     Instruction::NEG},
	{"toi",     Instruction::TOI},
	{"tof",     Instruction::TOF},
	{"cmpnn",   Instruction::CMPNN},
	{"cmpe",    Instruction::CMPE},
	{"cmpne",   Instruction::CMPNE},
	{"cmpge",   Instruction::CMPGE},
	{"cmple",   Instruction::CMPLE},
	{"cmpg",    Instruction::CMPG},
	{"cmpl",    Instruction::CMPL},
	{"if",      Instruction::IF},
	{"eval",    Instruction::EVAL},
	{"jt",      Instruction::JT},
	{"jnt",     Instruction::JNT},
	{"jmp",     Instruction::JMP},
	{"jmpd",    Instruction::JMPD},
	{"switch",  Instruction::SWITCH},
	{"rswitch", Instruction::RSWITCH},
	{"call",    Instruction::CALL},
	{"calld",   Instruction::CALLD},
	{"enter",   Instruction::ENTER},
	{"ret",     Instruction::RET},
	{"read",    Instruction::READ},
	{"write",   Instruction::WRITE},
	{"copy",    Instruction::COPY},
	{"fill",    Instruction::FILL},
	{"comp",    Instruction::COMP},
	{"native",  Instruction::NATIVE},
	{"cmd",     Instruction::CMD},
	{"cdecl",   Instruction::CDECL},
	{"stdc",    Instruction::STDC},
	{"thisc",   Instruction::THISC},
	{"fastc",   Instruction::FASTC},
};

auto DataType::fromName(const string& name)->DataType::Type
{
	if (name.size() == 2 && name[0] == 'D') {
		switch (name[1]) {
		case 'B': return DataType::DB;
		case 'W': return DataType::DW;
		case 'D': return DataType::DD;
		case 'Q': return DataType::DQ;
		case 'S': return DataType::DS;
		}
	}
	return DataType::MAX;
}

auto Keyword::fromName(const string& name)->Keyword::Type
{
	static_assert(keywords.size() == Keyword::MAX);
	auto idx = findIndexIf(keywords.begin(), keywords.end(), [&](const auto& val) {
		return val == name;
	});
	return static_cast<Keyword::Type>(idx);
}

auto Instruction::fromName(const string& name)->Instruction::Type
{
	auto it = instructionMap.find(name);
	return it != instructionMap.end() ? it->second : Instruction::MAX;
}

auto Mnemonic::fromName(const string& name)->Mnemonic::Type
{
	static_assert(mnemonics.size() == Mnemonic::MAX);
	auto idx = findIndexIf(mnemonics.begin(), mnemonics.end(), [&](const auto& val) {
		return val == name;
	});
	return static_cast<Mnemonic::Type>(idx);
}

auto Segment::fromName(string_view name)->Segment::Type
{
	if (name == "code") return Segment::Code;
	if (name == "data") return Segment::Data;
	return Segment::MAX;
}

const auto noOperands = vector<InstructionOperand>{};
const auto operandsImm8 = vector<InstructionOperand>{{{OperandType::IMM8}}};
const auto operandsImm16 = vector<InstructionOperand>{{{OperandType::IMM16}}};
const auto operandsImm32 = vector<InstructionOperand>{{{OperandType::IMM32}}};
const auto operandsImm64 = vector<InstructionOperand>{{{OperandType::IMM64}}};
const auto operandsS32 = vector<InstructionOperand>{{{OperandType::S32}}};
const auto operandsLv8 = vector<InstructionOperand>{{{OperandType::LV8}}};
const auto operandsLv16 = vector<InstructionOperand>{{{OperandType::LV16}}};
const auto operandsLv32 = vector<InstructionOperand>{{{OperandType::LV32}}};
const auto operandsV16 = vector<InstructionOperand>{{{OperandType::V16}}};
const auto operandsV32 = vector<InstructionOperand>{{{OperandType::V32}}};
const auto operandsRel32 = vector<InstructionOperand>{{{OperandType::REL32}}};
const auto switchOperands = vector<InstructionOperand>{{{OperandType::IMM16}}, {{OperandType::IMM32}}};

auto Instruction::getOperands(Type type)->const vector<InstructionOperand>&
{
	switch (type) {
	case THROW:
	case PUSHB:
	case POP:
	case DUPE:
	case ARRAY:
	case EVAL:
	case ENTER:
	case READ:
	case WRITE: return operandsImm8;
	case PUSHW: return operandsImm16;
	case PUSHD:
	case NATIVE:
	case CMD: return operandsImm32;
	case PUSHQ:
	case PUSHQF: return operandsImm64;
	case PUSHS: return operandsS32;
	case POPLN: return operandsLv8;
	case POPL: return operandsLv16;
	case POPLE: return operandsLv32;
	case POPV: return operandsV16;
	case POPVE: return operandsV32;
	case JT:
	case JNT:
	case JMPD:
	case CALLD: return operandsRel32;
	case SWITCH: break;
	default: break;
	}
	return noOperands;
}

auto Mnemonic::getOverloads(Mnemonic::Type type)->const vector<InstructionOverload>&
{
	return mnemonicTable[static_cast<std::underlying_type_t<Mnemonic::Type>>(type)];
}
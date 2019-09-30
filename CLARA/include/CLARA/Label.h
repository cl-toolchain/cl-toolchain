#pragma once
#include <CLARA/Token.h>

namespace CLARA::CLASM {

struct Label {
	string name;
	const Token& definition;
	Segment::Type segment;
	mutable uint64_t offset = 0;

	auto setOffset(uint64_t offset) const
	{
		this->offset = offset;
	}
};

}
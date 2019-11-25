#pragma once

#include <stdint.h>
#include <stddef.h>

class xString
{
public:
	enum class Type : uint8_t
	{
		RAM,
		Flash
	};
	xString() : data(nullptr), type(Type::RAM) {}
	xString(const char* inData, Type inType = Type::RAM) : data((char*) inData), type(inType) {  }

	size_t GetLineStartIndex(size_t lineNumber, size_t maxCharactersPerLine) const;
	int GetIndex(char search, size_t start = 0) const;
	size_t GetLineEndIndex(size_t start, size_t maxCharactersPerLine) const;
	size_t Length() const { return CalculateLength(); }
	size_t GetWordLength(size_t index) const;
	size_t NumLines(size_t maxCharactersPerLine) const;
	const char* GetData() const { return data; }
	char operator [] (size_t i) const;
	void Insert(char c, size_t index);
	void Remove(size_t index);

private:
	char Read(size_t i) const ;
	size_t CalculateLength() const;

	char* data;
	Type type;
};

#define FlashString(x) xString(PSTR(x), xString::Type::Flash)

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

	int GetLineStartIndex(int lineNumber, int maxCharactersPerLine) const;
	int GetIndex(char search, int start = 0) const;
	int GetLineEndIndex(int start, int maxCharactersPerLine) const;
	size_t Length() const { return CalculateLength(); }
	int GetWordLength(int index) const;
	int NumLines(int maxCharactersPerLine) const;
	const char* GetData() const { return data; }
	char operator [] (size_t i) const;
	void Insert(char c, int index);
	void Remove(int index);

private:
	char Read(size_t i) const ;
	size_t CalculateLength() const;

	char* data;
	Type type;
};

#define FlashString(x) xString(PSTR(x), xString::Type::Flash)

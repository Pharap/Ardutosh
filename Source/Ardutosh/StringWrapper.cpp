#include "StringWrapper.h"
#include "Platform.h"
#include "Defines.h"
#include <string.h>
#include <limits.h>

size_t xString::CalculateLength() const 
{
	if (!data)
		return 0;
	if (type == Type::Flash)
	{
		return strlen_P(data);
	}
	return strlen(data);
}

char xString::operator [] (size_t index) const
{
	return (index < Length()) ? Read(index) : '\0';
}

char xString::Read(size_t i) const
{
	return (type == Type::Flash) ? pgm_read_byte(&data[i]) : data[i];
}

int xString::GetIndex(char search, size_t start) const
{
	constexpr size_t intMax = static_cast<size_t>(INT_MAX);

	size_t length = Length();
	if (start < length)
	{
		for (size_t index = start; (index < length) && (index <= intMax); ++index)
		{
			if (Read(index) == search)
			{
				return index;
			}
		}
	}
	return -1;
}

size_t xString::GetWordLength(size_t index)  const
{
	size_t length = Length();
	size_t count = 0;

	while(index < length)
	{
		char c = Read(index);
		if (c == ' ' || c == '\n' || c == '\r')
		{
			break;
		}
		index++;
		count++;
	}
	return count;
}

size_t xString::NumLines(size_t maxCharactersPerLine) const
{
	size_t length = Length();
	size_t count = 0;
	size_t index = 0;

	while (index < length)
	{
		count++;
		index = GetLineEndIndex(index, maxCharactersPerLine);

		// If we end with a new line then count this as an extra line
		char last = Read(index - 1);
		if (index == length && (last == '\n' || last == '\r'))
		{
			count++;
		}
	}

	return count;
}

size_t xString::GetLineStartIndex(size_t lineNumber, size_t maxCharactersPerLine) const
{
	size_t length = Length();
	size_t index = 0;

	while (index < length)
	{
		if (lineNumber == 0)
		{
			break;
		}
		index = GetLineEndIndex(index, maxCharactersPerLine);
		lineNumber--;
	}

	return index; 
}

size_t xString::GetLineEndIndex(size_t start, size_t maxCharactersPerLine) const
{
	size_t length = Length();
	size_t index = start;

	while (index < length)
	{
		char c = Read(index);
		if (c == '\n' || c == '\r')
		{
			return index + 1;
		}

		if (index - start >= maxCharactersPerLine)
			return index;

		if (c != ' ')
		{
			size_t wordLength = GetWordLength(index);
			if (index + wordLength - start < maxCharactersPerLine)
			{
				index += wordLength;
			}
			else if (index == start)
			{
				return start + maxCharactersPerLine - 1;
			}
			else
			{
				return index;
			}
		}
		else
		{
			index++;
		}
	}

	return length;
}

void xString::Insert(char c, size_t index)
{
	size_t length = Length();

	for (size_t n = length; n > index; n--)
	{
		data[n] = data[n - 1];
	}
	data[index] = c;

	if (index == length)
	{
		data[index + 1] = '\0';
	}
}

void xString::Remove(size_t index)
{
	size_t length = Length();

	for (size_t n = index; n < length; n++)
	{
		data[n] = data[n + 1];
	}
}

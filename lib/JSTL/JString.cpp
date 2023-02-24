//
// Created by Jacob Lim on 2023/02/23.
//

#include "JSTL/JString.h"

namespace JSTL
{
	JString::JString(c_pointer char_type) : mString(), mLength(strLen(char_type))
	{
		mString = new char[mLength + 1];

		int i = 0;
		while (char_type[i] != '\0')
		{
			mString[i] = char_type[i];
			i++;
		}
		mString[i] = '\0';
	}
	JString::JString(const JSTL::JString &other) : JString(other.GetCString()) {}

	void JString::Append(const char *char_type)
	{
		const unsigned newLen = strLen(char_type) + mLength;
		char *newString = new char[newLen + 1];
		for (auto i = 0; i < mLength; ++i)
		{
			newString[i] = mString[i];
		}
		for (auto i = mLength; i < newLen; ++i)
		{
			newString[i] = char_type[i - mLength];
		}
		newString[newLen] = '\0';
		delete[] mString;
		mString = newString;
		mLength = newLen;
	}
	bool JString::Insert(unsigned index, c_pointer char_type)
	{
		if (index >= mLength)
			return false;
		const unsigned addLen = strLen(char_type);
		const unsigned newLen = mLength + addLen;
		char *newString = new char[newLen + 1];

		for (int i = 0; i < index; ++i)
		{
			newString[i] = mString[i];
		}
		for (int i = index; i < index + addLen; ++i)
		{
			newString[i] = char_type[i - index];
		}
		for (int i = index + addLen; i < newLen; ++i)
		{
			newString[i] = mString[i - addLen];
		}

		newString[newLen] = '\0';

		delete[] mString;
		mString = newString;
		mLength = newLen;

		return true;
	}
	int JString::IndexOf(c_pointer char_type)
	{
		for (int i = 0; i < mLength; ++i)
		{
			if (mString[i] == char_type[0])
			{
				const char *startIndexString = mString + i;
				if (*startIndexString == *char_type)
					return i;
			}
		}
		return -1;
	}
	int JString::LastIndexOf(c_pointer char_type)
	{
		for (int i = mLength - 1; i >= 0; i--)
		{
			if (mString[i] == char_type[0])
			{
				const char *startIndexString = mString + i;
				if (*startIndexString == *char_type)
					return i;
			}
		}
		return -1;
	}
	bool JString::RemoveAt(unsigned index)
	{
		if (index >= mLength)
			return false;
		mLength--;
		char *newString = new char[mLength];
		for (int i = 0; i < index; ++i)
		{
			newString[i] = mString[i];
		}
		for (unsigned i = index; i < mLength; ++i)
		{
			newString[i] = mString[i + 1];
		}
		delete[] mString;
		mString = newString;
		return true;
	}
	void JString::PadLeft(int totalLen)
	{
		if (totalLen <= mLength)
			return;
		const unsigned paddingLen = totalLen - mLength;
		const unsigned newLen = totalLen;
		char *newString = new char[totalLen + 1];

		for (int i = 0; i < newLen; ++i)
		{
			newString[i] = i < paddingLen ? ' ' : mString[i - paddingLen];
		}

		newString[newLen] = '\0';
		delete[] mString;
		mString = newString;
		mLength = newLen;
	}
	void JString::PadLeft(int totalLen, const char c)
	{
		if (totalLen <= mLength)
			return;
		const unsigned paddingLen = totalLen - mLength;
		const unsigned newLen = totalLen;
		char *newString = new char[totalLen + 1];

		for (int i = 0; i < newLen; ++i)
		{
			newString[i] = i < paddingLen ? c : mString[i - paddingLen];
		}

		newString[newLen] = '\0';
		delete[] mString;
		mString = newString;
		mLength = newLen;
	}
	void JString::PadRight(int totalLen)
	{
		if (totalLen <= mLength)
			return;
		const unsigned newLen = totalLen;
		char *newString = new char[totalLen + 1];

		for (int i = 0; i < newLen; ++i)
		{
			newString[i] = i < mLength ? mString[i] : ' ';
		}

		newString[newLen] = '\0';
		delete[] mString;
		mString = newString;
		mLength = newLen;
	}
	void JString::PadRight(int totalLen, const char c)
	{
		if (totalLen <= mLength)
			return;
		const unsigned newLen = totalLen;
		char *newString = new char[totalLen + 1];

		for (int i = 0; i < newLen; ++i)
		{
			newString[i] = i < mLength ? mString[i] : c;
		}

		newString[newLen] = '\0';
		delete[] mString;
		mString = newString;
		mLength = newLen;
	}
	void JString::Reverse()
	{
		if (mLength < 2)
			return;
		unsigned lh = 0, rh = mLength - 1;
		while (lh < rh)
		{
			const auto tempChar = mString[lh];
			mString[lh] = mString[rh];
			mString[rh] = tempChar;
			lh++;
			rh--;
		}
	}
	void JString::ToLower()
	{
		for (unsigned i = 0; i < mLength; ++i)
		{
			if (mString[i] >= 'A' && mString[i] <= 'Z')
				mString[i] += 32;
		}
	}
	void JString::ToUpper()
	{
		for (unsigned i = 0; i < mLength; ++i)
		{
			if (mString[i] >= 'a' && mString[i] <= 'z')
				mString[i] -= 32;
		}
	}

	JString JString::operator+(const JString &rhs) const
	{
		unsigned newLen = rhs.mLength + mLength;
		char *newString = new char[newLen + 1];
		for (unsigned i = 0; i < newLen; ++i)
		{
			newString[i] = i < mLength ? mString[i] : rhs.mString[i - mLength];
		}
		newString[newLen] = '\0';
		return {newString};
	}
	bool JString::operator==(const JString &rhs) const
	{
		if (&rhs == this)
			return true;
		if (mLength != rhs.mLength)
			return false;
		for (int i = 0; i < mLength; ++i)
		{
			if (mString[i] != rhs.mString[i])
			{
				return false;
			}
		}
		return true;
	}
	JString &JString::operator=(const JString &rhs)
	{
		if (&rhs == this)
			return *this;
		delete[] mString;

		mLength = rhs.mLength;
		mString = new char[mLength + 1];
		for (int i = 0; i < mLength; ++i)
		{
			mString[i] = rhs.mString[i];
		}
		mString[mLength] = '\0';

		return *this;
	}
	JString &JString::operator+=(const JString &rhs)
	{
		this->Append(rhs.mString);
		return *this;
	}

	std::ostream &operator<<(std::ostream &os, const JString &rhs)
	{
		os << rhs.mString;
		return os;
	}
	std::istream &operator>>(std::istream &is, JString &rhs)
	{
		is >> rhs.mString;
		return is;
	}

	unsigned JString::strLen(c_pointer char_type)
	{
		unsigned len = 0;
		while (char_type[len] != '\0')
		{
			len++;
		}
		return len;
	}
} // namespace JSTL

//
// Created by Jacob Lim on 2023/02/23.
//

#include "JString.h"

namespace JSTL
{
#pragma region CONSTRUCTOR
	JString::JString() : mString(), mLength(0), mCapacity(22) { Reserve(mCapacity); }
	JString::JString(c_pointer char_type) : mString(), mLength(strLen(char_type)), mCapacity(22)
	{
		if (mLength > mCapacity)
			mCapacity = mLength * 2;

		Reserve(mCapacity);

		for (int i = 0; i < mLength; ++i)
		{
			if (char_type[i] >= 0x20)
				mString[i] = char_type[i];
		}
		mString[mLength] = '\0';
	}
	JString::JString(const JSTL::JString &other) : JString(other.mString) {}
#pragma endregion CONSTRUCTOR

#pragma region INTERNALFUNCTION
	void JString::Reserve(size_t size)
	{
		if (mString == nullptr)
		{
			size_t optimized = size > mCapacity ? size : mCapacity;
			mString = (char *)malloc(optimized + 1);
			mCapacity = optimized;
			return;
		}
		if (mCapacity >= size)
			return;
		mCapacity = size;
		mString = (char *)realloc(mString, mCapacity + 1);
	}
	void JString::Append(const char *char_type)
	{
		const size_t addLen = strLen(char_type);
		const size_t newLen = addLen + mLength;

		Reserve(newLen > mCapacity ? newLen * 2 : newLen);

		for (auto i = mLength; i < newLen; ++i)
		{
			if (char_type[i - mLength] >= 0x20)
				mString[i] = char_type[i - mLength];
		}
		mLength = newLen;
		mString[mLength] = '\0';
	}
	bool JString::Insert(unsigned index, c_pointer char_type)
	{
		if (index >= mLength)
			return false;
		const unsigned addLen = strLen(char_type);
		const unsigned newLen = mLength + addLen;

		if (mCapacity >= newLen)
		{
			for (size_t i = mLength; i >= mLength - addLen; --i)
			{
				mString[i + addLen] = mString[i];
			}
			for (size_t i = index; i < mLength; ++i)
			{
				const char cachedChar = mString[i];
				mString[i] = char_type[i - index];
				mString[i + addLen] = cachedChar;
			}
		}
		else
		{
			mCapacity = newLen * 2;
			char *newString = (char *)malloc(mCapacity);
			for (size_t i = 0; i < index; ++i)
			{
				newString[i] = mString[i];
			}
			for (size_t i = index; i < index + addLen; ++i)
			{
				newString[i] = char_type[i - index];
			}
			for (size_t i = index + addLen; i < newLen; ++i)
			{
				newString[i] = mString[i - addLen];
			}
			delete[] mString;
			mString = newString;
		}

		mString[newLen] = '\0';
		mLength = newLen;

		return true;
	}
	size_t JString::IndexOf(c_pointer char_type)
	{
		for (int i = 0; i < mLength; ++i)
		{
			if (mString[i] == char_type[0])
			{
				const char *startIndexString = mString + i;
				if (JString{startIndexString} == JString{char_type})
					return i;
			}
		}
		return NPOS;
	}
	size_t JString::LastIndexOf(c_pointer char_type)
	{
		for (int i = mLength - 1; i >= 0; i--)
		{
			if (mString[i] == char_type[0])
			{
				const char *startIndexString = mString + i;
				if (JString{startIndexString} == JString{char_type})
					return i;
			}
		}
		return NPOS;
	}
	bool JString::RemoveAt(unsigned index)
	{
		if (index >= mLength)
			return false;

		mLength--;
		for (size_t i = index; i < mLength; ++i)
		{
			mString[i] = mString[i + 1];
		}
		return true;
	}
	void JString::PadLeft(size_t totalLen)
	{
		if (totalLen <= mLength)
			return;
		const unsigned paddingLen = totalLen - mLength;

		Reserve(totalLen > mCapacity ? totalLen * 2 : mCapacity);

		for (int i = mLength; i >= 0; --i)
		{
			mString[i + paddingLen] = mString[i];
		}

		for (int i = 0; i < paddingLen; ++i)
		{
			mString[i] = ' ';
		}
		mLength = totalLen;
	}
	void JString::PadLeft(size_t totalLen, char c)
	{
		if (totalLen <= mLength)
			return;
		const unsigned paddingLen = totalLen - mLength;

		Reserve(totalLen > mCapacity ? totalLen * 2 : mCapacity);

		for (int i = mLength; i >= 0; --i)
		{
			mString[i + paddingLen] = mString[i];
		}

		for (int i = 0; i < paddingLen; ++i)
		{
			mString[i] = c;
		}
		mLength = totalLen;
	}
	void JString::PadRight(size_t totalLen)
	{
		if (totalLen <= mLength)
			return;

		Reserve(totalLen > mCapacity ? totalLen * 2 : mCapacity);

		for (size_t i = mLength; i < totalLen; ++i)
		{
			mString[i] = ' ';
		}
		mString[totalLen] = '\0';

		mLength = totalLen;
	}
	void JString::PadRight(size_t totalLen, char c)
	{
		if (totalLen <= mLength)
			return;

		Reserve(totalLen > mCapacity ? totalLen * 2 : mCapacity);

		for (size_t i = mLength; i < totalLen; ++i)
		{
			mString[i] = c;
		}
		mString[totalLen] = '\0';

		mLength = totalLen;
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
#pragma endregion INTERNALFUNCTION

#pragma region OPERATOR
	JString JString::operator+(const JString &rhs) const
	{
		unsigned newLen = rhs.mLength + mLength;
		char *newString = (char *)malloc(newLen * 2);
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

		mCapacity = rhs.mCapacity;
		mLength = rhs.mLength;
		Reserve(mCapacity);
		for (int i = 0; i < mLength; ++i)
		{
			if (rhs.mString[i] >= 0x20)
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
		rhs.mLength = rhs.strLen(rhs.mString);
		rhs.mCapacity = rhs.mLength * 2;
		return is;
	}
#pragma endregion OPERATOR

	size_t JString::strLen(c_pointer char_type)
	{
		unsigned len = 0;
		while (char_type[len] != '\0')
		{
			len++;
		}
		return len;
	}
} // namespace JSTL

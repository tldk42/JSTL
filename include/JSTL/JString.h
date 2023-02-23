//
// Created by Jacob Lim on 2023/02/23.
//

#ifndef ENGINE_JSTRING_H
#define ENGINE_JSTRING_H
#include <iostream>

namespace JSTL
{
	typedef const char* c_pointer;

	class JString final
	{
	private:
		char* mString{};
		unsigned mLength{};

	public:
		JString() = default;
		JString(c_pointer char_type);
		JString(const JSTL::JString& other);

		~JString() { delete[] mString; }

		__inline unsigned GetLength() const { return mLength; }
		__inline c_pointer GetCString() const { return mString; }

		void Append(c_pointer char_type);
		bool Insert(unsigned index, c_pointer char_type);
		int IndexOf(c_pointer char_type);
		int LastIndexOf(c_pointer char_type);
		bool RemoveAt(unsigned index);
		void PadLeft(int totalLen);
		void PadLeft(int totalLen, const char c);
		void PadRight(int totalLen);
		void PadRight(int totalLen, const char c);
		void Reverse();
		void ToLower();
		void ToUpper();

		JString operator+(const JString& rhs) const;
		bool operator==(const JString& rhs) const;
		JString& operator+=(const JString& rhs);
		JString& operator=(const JString& rhs);
		friend std::istream& operator>>(std::istream& is, JString& rhs);
		friend std::ostream& operator<<(std::ostream& os, const JString& rhs);

	private:
		static unsigned strLen(c_pointer char_type);
	};

} // namespace JSTL
#endif // ENGINE_JSTRING_H

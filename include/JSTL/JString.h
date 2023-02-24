//
// Created by Jacob Lim on 2023/02/23.
//

#ifndef ENGINE_JSTRING_H
#define ENGINE_JSTRING_H

#include "CommonHeader.h"
#include <iostream>

namespace JSTL
{
	// TODO: custom allocator로 바꾸기
	/** V 0.1
	 * new delete 최소화 목표
	 * 초기 용량 22
	 * 다음 * 2
	 */
	class JString final
	{
	private:
		char* mString{};
		size_t mLength{};
		size_t mCapacity{};

	public:
		JString();
		JString(c_pointer char_type);
		JString(const JSTL::JString& other);

		~JString() { delete[] mString; }

		[[nodiscard]] FORCEINLINE size_t GetLength() const { return mLength; }
		[[nodiscard]] FORCEINLINE c_pointer GetCString() const { return mString; }
		[[nodiscard]] FORCEINLINE size_t GetCapacity() const {return mCapacity;}

		void Reserve(size_t size);
		void Append(c_pointer char_type);
		/**@todo 수정 필요 */
		[[deprecated("최적화 안됨")]]bool Insert(unsigned index, c_pointer char_type);
		size_t IndexOf(c_pointer char_type);
		size_t LastIndexOf(c_pointer char_type);
		bool RemoveAt(unsigned index);
		void PadLeft(size_t totalLen);
		void PadLeft(size_t totalLen, char c);
		void PadRight(size_t totalLen);
		void PadRight(size_t totalLen, char c);
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
		static size_t strLen(c_pointer char_type);
	};

} // namespace JSTL
#endif // ENGINE_JSTRING_H

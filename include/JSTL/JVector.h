//
// Created by Jacob Lim on 2023/02/23.
//

#ifndef ENGINE_JVECTOR_H
#define ENGINE_JVECTOR_H

#include "CommonHeader.h"

namespace JSTL
{

	template <typename T>
	class Vector
	{
	public:
#pragma region MEMBER TYPES
		using ValueType = T;
		using ReferenceType = T&;
		using ConstReferenceType = const T&;
		using Iterator = T*;
		using ConstIterator = const T*;
		using AllocatorType = std::allocator<T>;
		using SizeType = size_t;
		using ReverseIterator = std::reverse_iterator<Iterator>;
		using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
#pragma endregion MEMBER TYPES

#pragma region MEMBER FUNCTIONS
		Vector();
		explicit Vector(SizeType n, ConstReferenceType val = T{});
		Vector(std::initializer_list<ValueType> initializerList);
		Vector(const Vector& v);
		Vector& operator=(const Vector&);
		~Vector();
		void Assign(SizeType count, ConstReferenceType value);
		AllocatorType GetAllocator() const { return mAlloc; }
#pragma endregion MEMBER FUNCTIONS

#pragma region Element ACCESS
		ReferenceType at(SizeType index);
		ConstReferenceType at(SizeType index) const;
		FORCEINLINE ReferenceType operator[](SizeType index) { return mData[index]; }
		FORCEINLINE ConstReferenceType operator[](SizeType index) const { return mData[index]; }
		FORCEINLINE ReferenceType Front() { return *mData; }
		FORCEINLINE ConstReferenceType Front() const { return *mData; }
		FORCEINLINE ReferenceType Back() { return *(mDataEnd - 1); }
		FORCEINLINE ConstReferenceType Back() const { return *(mDataEnd - 1); }
#pragma endregion Element ACCESS

#pragma region ITERATORS
		FORCEINLINE Iterator Begin() noexcept { return mData; }
		FORCEINLINE ConstIterator Begin() const noexcept { return mData; }

		FORCEINLINE Iterator End() noexcept { return mDataEnd; }
		FORCEINLINE ConstIterator End() const noexcept { return mDataEnd; }

		FORCEINLINE ReverseIterator RBegin() noexcept { return ReverseIterator(mDataEnd); }
		FORCEINLINE ConstReverseIterator RBegin() const noexcept { return ReverseIterator(mDataEnd); }

		FORCEINLINE Iterator REnd() noexcept { return ReverseIterator(mData); }
		FORCEINLINE ConstIterator REnd() const noexcept { return ReverseIterator(mData); }
#pragma endregion ITERATORS

#pragma region CAPACITY
		[[nodiscard]] FORCEINLINE bool IsEmpty() const { return Begin() == End(); }
		[[nodiscard]] FORCEINLINE SizeType GetSize() const { return mDataEnd - mData; }
		[[nodiscard]] FORCEINLINE SizeType GetCapacity() const { return mReservedStart - mData; }

		void Reserve(SizeType newCapacity);
		void FitSize();
#pragma endregion CAPACITY

#pragma region MODIFIERS
		void Clear();

		Iterator Insert(ConstIterator pos, ConstReferenceType value);
		Iterator Insert(ConstIterator pos, int count, ReferenceType value);

		Iterator Erase(Iterator pos);
		Iterator Erase(Iterator start, Iterator end);

		void PushBack(ConstReferenceType val);
		void PopBack();

		void Resize(SizeType count);
		void Resize(SizeType count, ConstReferenceType value);

		void Swap(Vector<T>& other) noexcept;
#pragma endregion MODIFIERS

#pragma region OPERATORS
		bool operator==(const Vector<T>& other) const;
		bool operator!=(const Vector<T>& other) const;
		bool operator<(const Vector<T>& other) const;
		bool operator>(const Vector<T>& other) const;
		bool operator>=(const Vector<T>& other) const;
		bool operator<=(const Vector<T>& other) const;
#pragma endregion OPERATORS

	private:
		void Init_();
		void Init_(SizeType, ConstReferenceType);
		void Init_(ConstIterator, ConstIterator);
		void Destroy_();
		/** Reserve 5 if Default or current * 2 */
		void Reserve_();
		void UnCheckedAppend(ConstReferenceType);

	private:
		/** Vector's first element */
		Iterator mData;
		/** End of Vector's Data */
		Iterator mDataEnd;
		/** Reserved Space's first element */
		Iterator mReservedStart;

		AllocatorType mAlloc;
	};
	template <typename T>
	void Vector<T>::UnCheckedAppend(ConstReferenceType val)
	{
		mAlloc.construct(mDataEnd++, val);
	}
	template <typename T>
	void Vector<T>::FitSize()
	{
		if (mDataEnd != mReservedStart)
		{
			mAlloc.deallocate(mDataEnd, mReservedStart - mDataEnd);
			mReservedStart = mDataEnd;
		}
	}
	template <typename T>
	void Vector<T>::Reserve(Vector::SizeType newCapacity)
	{
		if (newCapacity > GetCapacity())
		{
			// allocate new memory space
			Iterator newData = mAlloc.allocate(newCapacity);
			// 기존 End 까지 Data Copy
			Iterator newDataEnd = std::uninitialized_copy(mData, mDataEnd, newData);
			// 기존 Data destroy
			Destroy_();
			// Set New Data
			mData = newData;
			mDataEnd = newDataEnd;
			mReservedStart = mData + newCapacity;
		}
	}

	template <typename T>
	void Vector<T>::Swap(Vector<T>& other) noexcept
	{
		std::swap(this->mData, other.mData);
		std::swap(this->mDataEnd, other.mDataEnd);
		std::swap(this->mReservedStart, other.mReservedStart);
	}
	template <typename T>
	void Vector<T>::Resize(Vector::SizeType count, ConstReferenceType value)
	{
		if (GetSize() > count)
			Resize(count);
		else
		{
			if (mReservedStart == mDataEnd)
				Reserve(count);
			std::uninitialized_fill(mDataEnd, mReservedStart, value);
			mDataEnd = mReservedStart;
		}
	}
	template <typename T>
	void Vector<T>::Resize(Vector::SizeType count)
	{
		if (count > GetSize())
			throw std::invalid_argument{"vector::resize"};
		while (Begin() + count != mDataEnd)
			mAlloc.destroy(--mDataEnd);
	}
	template <typename T>
	void Vector<T>::PopBack()
	{
		mAlloc.destroy(--mDataEnd);
	}
	template <typename T>
	void Vector<T>::PushBack(ConstReferenceType val)
	{
		if (mDataEnd == mReservedStart)
			Reserve_();
		UnCheckedAppend(val);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Erase(Vector::Iterator start, Vector::Iterator end)
	{
		if (start > end)
			throw std::invalid_argument{"vector::erase"};
		if (start < mData || mDataEnd < end)
			throw std::invalid_argument{"vector::erase"};
		for (Iterator i = start; i < mDataEnd - 1; ++i)
			*i = *(i + (end - start));
		mDataEnd -= (end - start);
		return end;
	}
	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Erase(Vector::Iterator pos)
	{
		if (pos < mData || pos > mDataEnd)
			throw std::invalid_argument{"vector::erase"};
		for (Iterator i = pos; i < mDataEnd - 1; ++i)
			*i = *(i + 1);
		mDataEnd--;
		return pos;
	}
	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Insert(Vector::ConstIterator pos, int count, ReferenceType value)
	{
		if (pos < mData || pos >= mDataEnd)
			throw std::out_of_range{"vector::insert(index 범위 벗어남)"};

		mDataEnd += count;

		int posInteger = 0;
		for (Iterator i = mData; i < pos; ++i)
		{
			posInteger++;
		}

		SizeType newSize = GetSize() + count;
		Iterator newData = mAlloc.allocate(newSize);
		Iterator newDataEnd = std::uninitialized_copy(mData, mDataEnd + 1, newData);

		for (int i = 0; i < posInteger; ++i)
			newData[i] = mData[i];

		for (int i = posInteger; i <= posInteger + count; ++i)
			newData[i] = value;

		int afterInserted = posInteger + count;
		int newLast = GetSize() + count;
		for (int i = afterInserted; i < newLast; ++i)
			newData[i] = mData[i - count];
		Destroy_();
		mData = newData;
		mDataEnd = newDataEnd;
		mReservedStart = mData + newSize;

		return mData + posInteger;
	}
	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Insert(Vector::ConstIterator pos, ConstReferenceType value)
	{
		if (pos < mData || pos >= mDataEnd)
			throw std::out_of_range{"vector::insert(index 범위 벗어남)"};
		// From Here need to resize current vector
		int posInteger = 0;
		for (Iterator i = mData; i < pos; ++i)
		{
			posInteger++;
		}
		SizeType newSize = GetSize() + 1;
		Iterator newData = mAlloc.allocate(newSize);
		Iterator newDataEnd = std::uninitialized_copy(mData, mDataEnd + 1, newData);

		newData[posInteger] = value;
		int afterPos = posInteger + 1;
		int newLast = GetSize() + 1;

		for (int i = afterPos; i < newLast; ++i)
		{
			newData[i] = mData[i - 1];
		}

		Destroy_();
		mData = newData;
		mDataEnd = newDataEnd;
		mReservedStart = mData + newSize;

		return mData + posInteger;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		Iterator it = mDataEnd;
		while (it != mData)
			mAlloc.destroy(--it);
		mDataEnd = mData;
	}
	template <typename T>
	const T& Vector<T>::at(Vector::SizeType index) const
	{
		if (index < GetSize())
			return mData[index];
		else
			throw std::out_of_range{"vector::at"};
	}
	template <typename T>
	T& Vector<T>::at(Vector::SizeType index)
	{
		if (index < GetSize())
			return mData[index];
		else
			throw std::out_of_range{"vector::at"};
	}
	template <typename T>
	void Vector<T>::Assign(Vector::SizeType count, ConstReferenceType value)
	{
		if (count < 1)
			throw std::invalid_argument{"Vector::assign"};

		if (count > GetCapacity())
		{
			Destroy_();
			Init_(count, value);
		}
		else
		{
			Iterator it = mDataEnd;
			while (it != mData)
				mAlloc.destroy(--it);
			mDataEnd = mData + count;
			std::uninitialized_fill(mData, mDataEnd, value);
		}
	}
	template <typename T>
	void Vector<T>::Reserve_()
	{
		SizeType newSize = std::max(ptrdiff_t(mReservedStart - mData) * 2, ptrdiff_t(5));
		Reserve(newSize);
	}
	template <typename T>
	void Vector<T>::Destroy_()
	{
		if (mData)
		{
			Iterator it = mDataEnd;
			// 데이터가 차있는 만큼 뒤에서 앞으로 destroy
			while (it != mData)
				mAlloc.destroy(--it); // == mDateEnd->~T();
			// 이제 할당된 전체 용량을 deallocate
			mAlloc.deallocate(mData, mReservedStart - mData);
		}
		mData = mDataEnd = mReservedStart = nullptr;
	}
	template <typename T>
	void Vector<T>::Init_(Vector::ConstIterator start, Vector::ConstIterator end)
	{
		mData = mAlloc.allocate(end - start);
		mReservedStart = mDataEnd = std::uninitialized_copy(start, end, mData);
	}
	template <typename T>
	void Vector<T>::Init_(Vector::SizeType size, ConstReferenceType val)
	{
		mData = mAlloc.allocate(size);
		mReservedStart = mDataEnd = mData + size;
		std::uninitialized_fill(mData, mReservedStart, val);
	}

	/** Do nothing Just Initialize */
	template <typename T>
	void Vector<T>::Init_()
	{
		mData = mDataEnd = mReservedStart = nullptr;
	}

	template <typename T>
	Vector<T>::Vector()
	{
		Init_();
	}

	template <typename T>
	Vector<T>::Vector(Vector::SizeType n, ConstReferenceType val)
	{
		Init_(n, val);
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<ValueType> initializerList)
	{
		mData = mAlloc.allocate(initializerList.size());
		mReservedStart = mDataEnd = std::uninitialized_copy(initializerList.begin(), initializerList.end(), mData);
	}

	template <typename T>
	Vector<T>::Vector(const Vector& v)
	{
		Init_(v.Begin(), v.End());
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (&rhs != this)
		{
			Destroy_();
			Init_(rhs.Begin(), rhs.End());
		}
		return *this;
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Destroy_();
	}


	template <typename T>
	bool Vector<T>::operator==(const Vector<T>& other) const
	{
		if (GetSize() == other.GetSize())
		{
			for (int i = 0; i < GetSize(); ++i)
			{
				if (at(i) != other.at(i))
					return false;
			}
			return true;
		}
		return false;
	}
	template <typename T>
	bool Vector<T>::operator!=(const Vector<T>& other) const
	{
		return *this != other;
	}
	template <typename T>
	bool Vector<T>::operator<(const Vector<T>& other) const
	{
		SizeType smallerSize = GetSize() < other.GetSize() ? GetSize() : other.GetSize();
		for (SizeType i = 0; i < smallerSize; ++i)
		{
			if (at(i) != other.at(i))
				return at(i) < other.at(i);
		}
		return GetSize() < other.GetSize();
	}
	template <typename T>
	bool Vector<T>::operator>(const Vector<T>& other) const
	{
		SizeType smallerSize = GetSize() < other.GetSize() ? GetSize() : other.GetSize();
		for (SizeType i = 0; i < smallerSize; ++i)
		{
			if (at(i) != other.at(i))
				return at(i) > other.at(i);
		}
		return GetSize() > other.GetSize();
	}
	template <typename T>
	bool Vector<T>::operator>=(const Vector<T>& other) const
	{
		return *this >= other;
	}
	template <typename T>
	bool Vector<T>::operator<=(const Vector<T>& other) const
	{
		return *this <= other;
	}


#if 0
	template <typename Vector>
	class JIterator
	{
	public:
		using ValueType = typename Vector::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

	public:
		explicit JIterator(PointerType p, size_t idx = 0) : mPtr(p), mPos(idx) {}
		~JIterator() = default;

		JIterator& operator++()
		{
			++mPos;
			return *this;
		};
		JIterator operator++(int)
		{
			JIterator iterator = *this;
			this->operator++();
			return iterator;
		}
		JIterator& operator--()
		{
			--mPos;
			return *this;
		};
		JIterator operator--(int)
		{
			JIterator iterator = *this;
			this->operator--();
			return iterator;
		}

		FORCEINLINE ReferenceType operator[](int index) const { return mPtr[index]; }
		FORCEINLINE ReferenceType operator*() const { return mPtr[mPos]; };
		FORCEINLINE PointerType operator->() const { return &mPtr[mPos]; };

		FORCEINLINE bool operator==(const JIterator& other) const noexcept
		{
			return &mPtr[mPos] == &other.mPtr[other.mPos];
		}
		FORCEINLINE bool operator!=(const JIterator& other) const noexcept
		{
			return &mPtr[mPos] != &other.mPtr[other.mPos];
		}

		JIterator operator+(size_t n) { return JIterator(&mPtr[mPos], mPos + n); };
		JIterator operator-(size_t n) { return JIterator(&mPtr[mPos], mPos - n); };

	private:
		friend Vector;
		PointerType mPtr;
		size_t mPos;
	};
	// TODO: custom allocator로 바꾸기
	template <typename T>
	class JVector final
	{
	public:
		using ValueType = T;
		using PointerType = T*;
		using ReferenceType = T&;
		using ConstReferenceType = const T&;
		using Iterator = JIterator<JVector<T>>;

	public:
		explicit JVector(size_t c = 1) : mPtr(), mSize(0), mCapacity(c) { Reserve(c); };
		JVector(const JVector<ValueType>& rhs) : mPtr(), mSize(rhs.mSize), mCapacity(rhs.mCapacity)
		{
			Reserve(mCapacity);
			std::copy(rhs.mPtr, rhs.mPtr + mSize, mPtr);
		};
		JVector(JVector<ValueType>&& rhs) noexcept
		    : mPtr(std::move(rhs.mPtr)), mSize(std::move(rhs.mSize)), mCapacity(std::move(rhs.mCapacity))
		{
			rhs.mPtr = 0;
			rhs.mSize = 0;
			rhs.mCapacity = 0;
		};
		JVector(const std::initializer_list<T>& initializerList)
		    : mPtr(), mSize(initializerList.size()), mCapacity(initializerList.size())
		{
			Reserve(mCapacity);
			std::copy(initializerList.begin(), initializerList.end(), mPtr);
		};
		~JVector() { delete[] mPtr; }

		JVector<ValueType>& operator=(const JVector& rhs)
		{
			if (this != &rhs)
			{
				auto temp = JVector(rhs);
				temp.Swap(*this);
			}
			return *this;
		};

		JVector<ValueType>& operator=(JVector&& rhs) noexcept
		{
			rhs.Swap(*this);
			return *this;
		}

		ReferenceType operator[](size_t pos) { return mPtr[pos]; }

		FORCEINLINE ReferenceType Front() const { return mPtr[0]; }
		FORCEINLINE ReferenceType Back() const { return mPtr[mSize - 1]; }

		FORCEINLINE Iterator Begin() noexcept { return Iterator(mPtr); }
		FORCEINLINE Iterator End() noexcept { return Iterator(mPtr, mSize); }
		FORCEINLINE Iterator Erase(const Iterator& it)
		{
			std::move(&mPtr[it.mPos + 1], &mPtr[mSize], &mPtr[it.mPos]);
			mSize--;
			return Iterator(mPtr);
		}
		FORCEINLINE Iterator Erase(const Iterator& start, const Iterator& end)
		{
			std::move(&mPtr[end.mPos], &mPtr[mSize], &mPtr[start.mPos]);
			mSize -= (end.mPos - start.mPos);
			return Iterator(mPtr);
		}
		FORCEINLINE void PushBack(ConstReferenceType val)
		{
			if (mSize >= mCapacity)
			{
				mCapacity *= 2;
				Reserve(mCapacity);
			}
			mPtr[mSize++] = val;
		}
		FORCEINLINE void PushBack(const T&& val)
		{
			if (mSize >= mCapacity)
			{
				Reserve(mCapacity * 2);
			}
			mPtr[mSize++] = std::move(val);
		}
		FORCEINLINE void PopBack()
		{
			if (mSize > 0)
			{
				mPtr[--mSize].~T();
			}
		}
		FORCEINLINE Iterator Insert(const Iterator& it, const T& val) {}

		FORCEINLINE void Resize(size_t n, ConstReferenceType val = T())
		{
			if (n == mSize)
				return;
			if (n > mSize)
			{
				Reserve(n);
				return;
			}
			mPtr = realloc(mPtr, n);
		}
		FORCEINLINE void Reserve(size_t n)
		{
			if (mPtr == nullptr)
			{
				mCapacity = mCapacity >= n ? mCapacity : n;
				mPtr = new ValueType[mCapacity];
			}

			if (n <= mCapacity)
				return;
			mCapacity = n;

			auto newBlock = new ValueType[mCapacity];
			std::copy(mPtr, mPtr + mSize, newBlock);
			delete[] mPtr;
			mPtr = newBlock;
		}
		FORCEINLINE void Swap(JVector<ValueType>& rhs) noexcept
		{
			std::swap(mPtr, rhs.mPtr);
			std::swap(mSize, rhs.mSize);
			std::swap(mCapacity, rhs.mCapacity);
		}

		[[nodiscard]] FORCEINLINE bool Empty() const { return mSize == 0; }
		FORCEINLINE void Clear() noexcept
		{
			for (size_t i = 0; i < mSize; ++i)
				mPtr[i].~T();
			mSize = 0;
		}
		[[nodiscard]] FORCEINLINE size_t GetSize() const { return mSize; }
		[[nodiscard]] FORCEINLINE size_t GetCapacity() const { return mCapacity; }

	private:
		PointerType mPtr;
		size_t mSize;
		size_t mCapacity;
	};

} // namespace JSTL
#endif


} // namespace JSTL
#endif // ENGINE_JVECTOR_H

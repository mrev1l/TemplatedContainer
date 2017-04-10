#pragma once
#include <stdlib.h>
#include <algorithm>

namespace structures
{
  template <typename TData>
  class CArray
  {
  public:
    CArray():
        mSize(0), mCapacity(0), mIterator(nullptr)
    {
    }
  
    CArray(
        const CArray & _array
      ):
        mSize(_array.mSize), mCapacity(_array.mCapacity)
    {
      mIterator = mAllocator.allocate(mCapacity);
      for (unsigned int i = 0; i < mSize;i++)
      {
        mAllocator.construct(mIterator + i, _array[i]);
      }
    }
  
    ~CArray()
    {
      mAllocator.deallocate(mIterator, mCapacity);
    }
  
    void push_back(
        const TData & _value
      )
    {
      if (mSize >= mCapacity)
        allocate(mCapacity + 1);

      mAllocator.construct(mIterator + mSize, _value);
      ++mSize;
    }
  
    void insert(
        unsigned int  _idx, 
        const TData & _value
      )
    {
      if (_idx < 0 ||
		  _idx > mSize)
        throw std::exception("Array index is of out range.");
  
      if (mSize + 1 > mCapacity)
      {
        allocate(mCapacity + 1);
        mAllocator.construct(mIterator + mSize, TData());
      }
  
      for (unsigned int i = mCapacity - 1; i > _idx; i--)
      {
        mAllocator.construct(mIterator + i, *(mIterator + i - 1));
      }
      mAllocator.construct(mIterator + _idx, _value);
      mSize++;
    }
  
    void erase(
        unsigned int _idx
      )
    {
      if (_idx < 0 ||
		  _idx >= mSize)
        throw std::exception("Array index is of out range.");

      mAllocator.destroy(mIterator + _idx);
      for (unsigned int i = _idx; i < mSize - 1; i++)
      {
        mAllocator.construct(mIterator + i, *(mIterator + i + 1));
      }
  
      --mSize;
    }

    void clear()
    {
      for (unsigned int i = 0; i < mSize; i++)
      {
        mAllocator.destroy(mIterator + i);
      }
      mSize = 0;
    }
  
    unsigned int size() const
    {
      return mSize;
    }
  
    unsigned int capacity() const
    {
      return mCapacity;
    }
  
    void pop_back()
    {
      TData * outdated = (mIterator + mSize - 1);
      mAllocator.destroy(outdated);
      --mSize;
    }
  
    TData & operator[](
        unsigned int _idx
      )
    {
      if (_idx < mSize)
        return *(mIterator + _idx);
      else
        throw std::exception("Array index is of out range.");
    }
  
    const TData & operator[](
        unsigned int _idx
      ) const
    {
      if (_idx < mSize)
        return *(mIterator + _idx);
      else
        throw std::exception("Array index is of out range.");
    }
  
    void resize(
        size_t _count
      )
    {
      if (_count < mSize)
        while (_count < mSize)
          this->pop_back();
  
      if (_count > mCapacity)
        this->reserve(_count);
  
      for (mSize; mSize < _count; mSize++)
        mAllocator.construct(mIterator + mSize, TData());
    }

    void reserve(
        unsigned int _count
      )
    {
      if (_count > mCapacity)
      {
        allocate(_count);
        mCapacity = _count;
      }
    }
  
    TData * begin()
    {
      return mIterator;
    }
  
    TData * end()
    {
      return mIterator + mSize;
    }
  
  protected:
  
  private:
    unsigned int          mSize;
	unsigned int          mCapacity;
    TData *               mIterator;
    std::allocator<TData> mAllocator;
  
    void allocate(
        unsigned int _count
      )
    {
      TData * newIterator = nullptr;
      newIterator = mAllocator.allocate(_count);
      errno_t res = memcpy_s(newIterator, (_count) * sizeof(TData), mIterator, mCapacity * sizeof(TData));
  
      if (res != 0) {
        mAllocator.deallocate(newIterator, _count);
        throw std::exception("Heap corruption. Failed to allocate memory.");
      }
  
      mAllocator.deallocate(mIterator, mCapacity);
      mIterator = std::move(newIterator);
      mCapacity = _count;
    }
  };
}
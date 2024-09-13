
///// "Unique pointer" /////

#pragma once

#include <cstdio>
#include <cstdlib>

template <typename T>
class uptr
{
	/* Fields */

   private:
	T* ptr;

	/* Constructors */

   public:

	uptr(const uptr&) = delete;
	uptr& operator=(const uptr&) = delete;

	uptr() :
		ptr(nullptr)
	{}

	uptr& operator=(std::nullptr_t)
	{
		this->freeOld();

		this->ptr = nullptr;

		return *this;
	}

	uptr(uptr&& that) :
		ptr(nullptr)
	{
		this->freeOld();

		this->ptr = that.ptr;

		that.ptr = nullptr;
	}

	uptr& operator=(uptr&& that)
	{
		if (this != &that)
		{
			this->freeOld();

			this->ptr = that.ptr;

			that.ptr = nullptr;
		}

		return *this;
	}

	~uptr()
	{
		this->freeOld();
	}

	static uptr make()
	{
		return uptr(new T());
	}

   private:

	uptr(T* t) :
		ptr(t)
	{}

	/* Methods and Operator Overloads */

   private:

	void freeOld()
	{
		if (this->ptr != nullptr)
		{
			delete this->ptr;
			this->ptr = nullptr;
		}
	}

   public:

	T* getRaw()
	{
		return this->ptr;
	}

	T& operator*()
	{
		if (this->ptr == nullptr)
		{
			std::fprintf(stderr, "Null pointer.\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			return *(this->ptr);
		}
	}
};

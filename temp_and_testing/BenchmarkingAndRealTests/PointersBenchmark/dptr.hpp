
///// "Dependency pointer" /////

#include <cstdio>
#include <cstdlib>
#include <vector>
#include "rptr.hpp"

template <typename T>
class dptr
{
	/* Structs */

   private:

	struct T_
	{
		T data;
		std::vector<rptr<T>> deps;
	};

	/* Fields */

   private:
	T_* ptr;

	/* Constructors */

   public:

	dptr(const dptr&) = delete;
	dptr& operator=(const dptr&) = delete;

	dptr() :
		ptr(nullptr)
	{}

	dptr& operator=(std::nullptr_t)
	{
		this->freeOld();

		this->ptr = nullptr;

		return *this;
	}

	dptr(dptr&& that) :
		ptr(nullptr)
	{
		this->freeOld();

		this->ptr = that.ptr;

		that.ptr = nullptr;
	}

	dptr& operator=(dptr&& that)
	{
		if (this != &that)
		{
			this->freeOld();

			this->ptr = that.ptr;

			that.ptr = nullptr;
		}

		return *this;
	}

	~dptr()
	{
		this->freeOld();
	}

	static dptr make(T* t)
	{
		return dptr(t);
	}

   private:

	dptr(T* t) :
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

	T* get()
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


///// "Dependency pointer" /////

#pragma once

#include <cstdio>
#include <cstdlib>
#include <vector>

template <typename T>
class dptr
{
	/* Structs */

   private:

	struct T_
	{
		T data;
		std::vector<T**> deps;
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

	static dptr make()
	{
		return dptr(new T_());
	}

   private:

	dptr(T_* t) :
		ptr(t)
	{}

	/* Methods and Operator Overloads */

   private:

	void freeOld()
	{
		if (this->ptr != nullptr)
		{
			for (T**& t : this->ptr->deps)
			{
				if (t != nullptr)
				{
					*t = nullptr;
				}
			}

			delete this->ptr;
			this->ptr = nullptr;
		}
	}

   public:

	T* getRaw()
	{
		return this->ptr;
	}

	// WARNING: this will completely break if the rptr ever changes location, which it very well may
	void getDependent(T** r)
	{
		if (this->ptr == nullptr)
		{
			std::fprintf(stderr, "Null pointer.\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			this->ptr->deps.push_back(r);
			*r = &(this->ptr->data);
		}
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
			return this->ptr->data;
		}
	}
};

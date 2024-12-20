
///// "Dependency pointer" /////

#pragma once

#include <cstdio>
#include <cstdlib>
#include <vector>

template <typename T> class dptr;
template <typename T> class wdptr;

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

	// void unlinkDependent(wdptr<T>* dependent)
	// {
	// 	if (this->ptr == nullptr)
	// 	{
	// 		std::fprintf(stderr, "Null pointer (while trying to do unlinkDependent()).\n");
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	else
	// 	{
	// 		// TODO
	// 		// for (auto& iter : this->ptr->deps)
	// 		// {
				
	// 		// }
	// 	}
	// }

   public:

	T* getRaw()
	{
		return this->ptr;
	}

	// TODO
	// void getDependent(T** r)
	// {
	// 	if (this->ptr == nullptr)
	// 	{
	// 		std::fprintf(stderr, "Null pointer (while trying to do getDependent()).\n");
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	else
	// 	{
	// 		this->ptr->deps.push_back(r);
	// 		*r = &(this->ptr->data);
	// 	}
	// }

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

	static std::size_t _SizeOfObj()
	{
		return sizeof(T_);
	}

	/* Friends */

	friend class wdptr;
};

template <typename T>
class wdptr
{
	typedef dptr<T>::T_ T_;

	/* Fields */

   private:
	T_* ptr;

	/* Constructors */

   public:

	wdptr(const wdptr&) = delete;
	wdptr& operator=(const wdptr&) = delete;

	wdptr() :
		ptr(nullptr)
	{}

	wdptr& operator=(std::nullptr_t)
	{
		this->unlinkOld();

		this->ptr = nullptr;

		return *this;
	}

	wdptr(wdptr&& that) :
		ptr(nullptr)
	{
		this->unlinkOld();

		this->ptr = that.ptr;

		that.ptr = nullptr;
	}

	wdptr& operator=(wdptr&& that)
	{
		if (this != &that)
		{
			this->freeOld();

			this->ptr = that.ptr;

			that.ptr = nullptr;
		}

		return *this;
	}

	~wdptr()
	{
		this->freeOld();
	}

	static wdptr make()
	{
		return wdptr(new T_());
	}

   private:

	wdptr(T_* t) :
		ptr(t)
	{}

	/* Methods and Operator Overloads */

   private:

	void unlinkOld()
	{
		if (this->ptr != nullptr)
		{
			// TODO
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
			std::fprintf(stderr, "Null pointer (while trying to do getDependent()).\n");
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

	static std::size_t _SizeOfObj()
	{
		return sizeof(T_);
	}
};

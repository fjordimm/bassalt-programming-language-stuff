
///// "Null-checked raw pointer" /////

#include <cstdio>
#include <cstdlib>

template <typename T>
class nptr
{
	/* Fields */

   private:
	T* ptr;

	/* Constructors */

   public:

	nptr() :
		ptr(nullptr)
	{}

	nptr(nptr const& that) :
		ptr(that.ptr)
	{}

	nptr& operator=(nptr const& that)
	{
		if (this != &that)
		{
			this->ptr = that.ptr;
		}

		return *this;
	}

	nptr& operator=(std::nullptr_t)
	{
		this->ptr = nullptr;

		return *this;
	}

	~nptr()
	{}

	static nptr make(T* t)
	{
		return nptr(t);
	}

   private:

	nptr(T* t) :
		ptr(t)
	{}

	/* Methods and Operator Overloads */

   public:

	T* get()
	{
		return this->ptr;
	}

	void free()
	{
		delete this->ptr;
		this->ptr = nullptr;
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

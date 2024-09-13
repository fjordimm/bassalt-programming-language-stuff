
///// "Raw pointer (but with container (and null-checked))" /////

#include <cstdio>
#include <cstdlib>

template <typename T>
class rptr
{
	/* Fields */

   private:
	T* ptr;

	/* Constructors */

   public:

	rptr() :
		ptr(nullptr)
	{}

	rptr(rptr const& that) :
		ptr(that.ptr)
	{}

	rptr& operator=(rptr const& that)
	{
		if (this != &that)
		{
			this->ptr = that.ptr;
		}

		return *this;
	}

	rptr& operator=(std::nullptr_t)
	{
		this->ptr = nullptr;

		return *this;
	}

	~rptr()
	{}

	static rptr make(T* t)
	{
		return rptr(t);
	}

   private:

	rptr(T* t) :
		ptr(t)
	{}

	/* Methods and Operator Overloads */

   public:

	T* getRaw()
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

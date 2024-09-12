
///// "Unique pointer" /////

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

	static uptr make(T* t)
	{
		return uptr(t);
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

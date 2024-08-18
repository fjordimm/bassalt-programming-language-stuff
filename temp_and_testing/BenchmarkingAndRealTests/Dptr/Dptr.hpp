
#include <cstdlib>
#include <memory>
#include <cstdio>

template <typename T>
class dptr;
template <typename T>
class wdptr;

template <typename T>
class dptr
{
	/* Structs */

   private:
	struct tstruct
	{
		T data;
		wdptr<T>* dep;
	};
	
	/* Fields */

   private:
	tstruct* ptr;

	/* Constructors */

   public:

	dptr(const dptr&) = delete;
	dptr& operator=(const dptr&) = delete;

	dptr() :
		ptr(nullptr)
	{}

	dptr& operator=(std::nullptr_t)
	{
		this->~dptr();

		this->ptr = nullptr;

		return *this;
	}

	dptr(dptr&& that) :
		ptr(nullptr)
	{
		this->~dptr();

		this->ptr = that.ptr;

		that.ptr = nullptr;
	}

	dptr& operator=(dptr&& that)
	{
		if (this != &that)
		{
			this->~dptr();

			this->ptr = that.ptr;

			that.ptr = nullptr;
		}

		return *this;
	}

	~dptr()
	{
		if (this->ptr != nullptr)
		{
			if (this->ptr->dep != nullptr)
			{
				*(this->ptr->dep) = nullptr;
			}

			delete this->ptr;
		}
	}

	static dptr make(T&& t)
	{
		return dptr(std::move(t));
	}

   private:

	dptr(T&& t) :
		ptr(nullptr)
	{
		this->~dptr();

		this->ptr = new tstruct();
		this->ptr->data = t;
		this->ptr->dep = nullptr;
	}

	/* Other Operator Overloads */

   public:

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

	/* Friends */

	friend wdptr<T>;
};

template <typename T>
class wdptr
{
	/* Fields */

   private:
	typename dptr<T>::tstruct* ptr;

	/* Constructors */

   public:

	wdptr(const wdptr&) = delete;
	wdptr& operator=(const wdptr&) = delete;

	wdptr() :
		ptr(nullptr)
	{}

	wdptr& operator=(std::nullptr_t)
	{
		this->~wdptr();

		this->ptr = nullptr;

		return *this;
	}

	wdptr(wdptr&& that) :
		ptr(nullptr)
	{
		this->~wdptr();

		this->ptr = that.ptr;
		this->ptr->dep = this;

		that.ptr = nullptr;
	}

	wdptr& operator=(wdptr&& that)
	{
		if (this != &that)
		{
			this->~wdptr();

			this->ptr = that.ptr;
			this->ptr->dep = this;

			that.ptr = nullptr;
		}

		return *this;
	}

	~wdptr()
	{
		if (this->ptr != nullptr)
		{
			if (this->ptr->dep != nullptr)
			{
				this->ptr->dep = nullptr;
			}
		}
	}

	static wdptr from(dptr<T>& d)
	{
		return wdptr(d.ptr);
	}

   private:

	wdptr(typename dptr<T>::tstruct* ptr) :
		ptr(nullptr)
	{
		this->~wdptr();

		this->ptr = ptr;
		this->ptr->dep = this;
	}

	/* Other Operator Overloads */

   public:

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

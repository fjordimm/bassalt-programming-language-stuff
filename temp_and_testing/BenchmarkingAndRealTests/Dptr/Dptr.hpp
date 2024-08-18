
template <typename T>
class dptr;
template <typename T>
class wdptr;

template <typename T>
class dptr
{
   private:
	struct _dptr
	{
		T data;
		wdptr* dep;
	};

   private:
	_dptr* ptr;

   public:

	dptr(const dptr&) = delete;
	dptr& operator=(const dptr&) = delete;

	dptr() :
		ptr(nullptr)
	{}

	dptr(dptr&& that) :
		ptr(that.ptr)
	{
		that.ptr = nullptr;
	}

	dptr& operator=(nullptr_t null)
	{
		this->~dptr();

		this->ptr = null;

		return *this;
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
			if (this->ptr->weakPtr != nullptr)
			{
				*(this->ptr->weakPtr) = nullptr;
			}

			delete this->ptr;
		}
	}

	static dptr make(T&& t)
	{
		return dptr();
	}

   private:

	dptr(T&& t) :
		ptr(nullptr)
	{
		this->ptr = new _dptr();
		this->ptr->data = t;
		this->ptr->dep = nullptr;
	}
};

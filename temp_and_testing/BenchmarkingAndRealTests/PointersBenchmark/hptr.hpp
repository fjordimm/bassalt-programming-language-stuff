
///// "Hashmap pointer" /////

#pragma once

#include <cstdio>
#include <cstdlib>
#include <unordered_map>

static std::size_t _ids = 0;
static std::unordered_map<std::size_t, void*> _map;
void InitializeHptrs()
{
	_map[0] = nullptr;
	_ids++;
}

template <typename T>
class hptr
{
	/* Fields */

   private:
	std::size_t id;

	/* Constructors */

   public:

	hptr(const hptr&) = delete;
	hptr& operator=(const hptr&) = delete;

	hptr() :
		id(0)
	{}

	hptr& operator=(std::nullptr_t)
	{
		this->freeOld();

		this->id = 0;

		return *this;
	}

	hptr(hptr&& that) :
		id(0)
	{
		this->freeOld();

		this->id = that.id;

		that.id = 0;
	}

	hptr& operator=(hptr&& that)
	{
		if (this != &that)
		{
			this->freeOld();

			this->id = that.id;

			that.id = 0;
		}

		return *this;
	}

	~hptr()
	{
		this->freeOld();
	}

	static hptr make()
	{
		_map[_ids] = new T();
		_ids++;
		return hptr(_ids - 1);
	}

   private:

	hptr(std::size_t id) :
		id(id)
	{}

	/* Methods and Operator Overloads */

   private:

	void freeOld()
	{
		if (this->id != 0)
		{
			delete static_cast<T*>(_map[this->id]);
			_map.erase(this->id);

			this->id = 0;
		}
	}

   public:

	T* getRaw()
	{
		return _map[this->id];
	}

	T& operator*()
	{
		if (this->id == 0)
		{
			std::fprintf(stderr, "Null pointer.\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			return *static_cast<T*>(_map[this->id]);
		}
	}
};

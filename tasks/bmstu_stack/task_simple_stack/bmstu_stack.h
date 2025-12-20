#pragma once

#include <exception>
#include <iostream>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() : data_(nullptr), size_(0u) {}

	bool empty() const noexcept { return size_ == 0u; }

	size_t size() const noexcept { return size_; }

	stack(const stack& other) : data_(nullptr), size_(0u)  // konstr kop
	{
		size_ = other.size_;

		data_ = (T*)(operator new(sizeof(T) * other.size_));
		for (size_t i = 0; i < size_; ++i)
		{
			new (data_ + i) T(other.data_[i]);
		}
	}
	stack(stack&& other) : data_(nullptr), size_(0u)  // konst perem
	{
		data_ = other.data_;
		size_ = other.size_;
		other.data_ = nullptr;
		other.size_ = 0;
	}
	stack& operator=(const stack& other)
	{
		if (this != &other)
		{
			clear();
			size_ = other.size_;

			data_ = (T*)(operator new(sizeof(T) * size_));
			for (size_t i = 0; i < size_; ++i)
			{
				new (data_ + i) T(other.data_[i]);
			}
			// perem kop
		}
		return *this;
	}

	stack& operator=(stack&& other)
	{
		if (this != &other)
		{
			clear();
			data_ = other.data_;
			size_ = other.size_;
			other.data_ = nullptr;
			other.size_ = 0;
		}
		return *this;
	}

	~stack() { clear(); }

	T* data() const { return data_; }
	template <typename... Args>
	void emplace(Args&&... args)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));
		for (size_t i = 0; i < size_; ++i)
		{
			new (&new_data[i]) T(std::move(data_[i]));
			data_[i].~T();
		}
		new (&new_data[size_]) T(std::forward<Args>(args)...);
		operator delete(data_);
		data_ = new_data;
		++size_;
	}

	void push(T&& value)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));

		for (size_t i = 0; i < size_; ++i)
		{
			new (&new_data[i]) T(std::move(data_[i]));
			data_[i].~T();
		}
		new (&new_data[size_]) T(std::move(value));
		operator delete(data_);
		data_ = new_data;
		++size_;
	}

	void clear() noexcept
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}
		operator delete(data_);
		data_ = nullptr;
		size_ = 0;
	}

	void push(const T& value)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));
		for (size_t i = 0; i < size_; ++i)
		{
			new (&new_data[i]) T(std::move(data_[i]));
			data_[i].~T();
		}
		new (&new_data[size_]) T(value);
		operator delete(data_);
		data_ = new_data;
		++size_;
	}

	void pop()
	{
		if (empty())
		{
			throw std::underflow_error("stack pustoi");
		}
		--size_;
		data_[size_].~T();
	}

	T& top()
	{
		if (empty())
		{
			throw std::underflow_error("stack pustoi");
		}
		return data_[size_ - 1];
	}

	const T& top() const
	{
		if (empty())
		{
			throw std::underflow_error("stack pustoi");
		}
		return data_[size_ - 1];
	}

   private:
	T* data_;
	size_t size_;
};
}  // namespace bmstu

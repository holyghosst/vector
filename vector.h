#ifndef VECTOR
#define VECTOR

#include <stdexcept>
#include <algorithm>

template<typename T>
class vector {
public:
  class iterator;
  class const_iterator;
  class reverse_iterator;
  using value_type = T;
  using pointer = value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
private:
  size_type sz;
  size_type max_size;
  pointer values;
public:
  vector() {
    sz = 0;
    max_size = 1;
    values = new value_type[max_size];
  }
  
  vector(std::initializer_list<T> ilist) {
    if(ilist.size() == 0) vector();
    else {
      sz = ilist.size();
      max_size = sz*2;
      values = new value_type[max_size];
      size_type i{0};
      for(const auto& el : ilist) {
        values[i] = el;
        ++i;
      }
    }
  }
  
  vector(const vector& other) {
    sz = other.sz;
    max_size = other.max_size;
    values = new value_type[max_size];
    for(size_type i{0}; i < sz; i++) {
      values[i] = other.values[i];
    }
  }
  
  vector& operator=(vector other) {
    std::swap(sz, other.sz);
    std::swap(max_size, other.max_size);
    std::swap(values, other.values);
    return *this;
  }
  
  vector& operator=(std::initializer_list<T> ilist) {
    sz = ilist.size();
    max_size = sz*2 + 1;
    delete[] values;
    values = new value_type[max_size];
    size_type i{0};
    for(const auto& el : ilist) {
      values[i] = el;
      ++i;
    }
    return *this;
  }
  
  ~vector() {
    delete[] values;
  }
  
  reference at(size_type pos) {
    if(pos >= size()) throw std::out_of_range("vector.at threw an exception: std::out_of_range");
    else return values[pos];
  
  }
  
  reference operator[](size_type pos) {
    return values[pos];
  }
  
  const_reference operator[](size_type pos) const {
    return values[pos];
  }
  
  reference front() {
    return values[0];
  }
  
  reference back() {
    return values[sz-1];
  }
  
  bool empty() const {
    return sz == 0;
  }
  
  size_type size() const {
    return sz;
  }
  
  size_type capacity() const {
    return max_size;
  }
  
  void reserve(size_type new_cap) {
    if(new_cap <= max_size) return;
    if(new_cap < sz) return;
    
    max_size = new_cap;
    pointer old_values = values;
    values = new value_type[max_size];
    for(size_type i{0}; i < sz; ++i) {
      values[i] = old_values[i];
    }
    delete[] old_values;
  }
  
  void shrink_to_fit() {
    max_size = sz;
    pointer old_values = values;
    values = new value_type[max_size];
    for(size_type i{0}; i <sz; ++i) {
      values[i] = old_values[i];
    }
    delete[] old_values;
  }
  
  void clear() {
    delete[] values;
    values = new value_type[max_size];
    sz = 0;
  }
  
  void push_back(value_type value) {
    if(sz < max_size) {
      values[sz] = value;
      sz++;
    }
    else {
      reserve(max_size*2);
      values[sz] = value;
      sz++;
    }
  }
  
  void pop_back() {
    pointer temporary = new value_type[max_size];
    for(size_type i{0}; i < sz - 1; i++) {
      temporary[i] = values[i];
    }
    
    delete[] values;
    values = temporary;
    --sz;
  }
  
  friend std::ostream& operator<<(std::ostream& o, const vector<T>& v) {
    o << "{";
    for(size_type i{0}; i < v.sz; i++) {
      o << v[i];
      if(i < v.sz - 1) {
        o << ", ";
      }
    }
    o << "} ";
    return o;
  }
  
  class iterator {
  public:
    using value_type = vector::value_type;
    using reference = vector::reference;
		using pointer = vector::pointer;
		using difference_type = vector::difference_type;
		using iterator_category = std::bidirectional_iterator_tag;
	private:
	  pointer ptr;
	public:
	  iterator() {
	    ptr = nullptr;
	  }
	  
	  iterator(pointer ptr) {
	    this->ptr = ptr;
	  }
	  
	  reference operator*() {
	    return *ptr;
	  }
	  
	  pointer operator->() {
	    return ptr;
	  }
	  
	  iterator& operator++() {
	    ++ptr;
	    return *this;
	  }
	  
	  iterator& operator++(int) {
	    return ptr++;
	  }
	  
	  iterator& operator--() {
	    --ptr;
	    return *this;
	  }
	  
	  iterator operator--(int) {
	    return ptr--;
	  }
	  
	  operator const_iterator() const {
      return const_iterator(ptr);
    }
    
    bool operator==(const const_iterator& r) const {
      return static_cast<const_iterator>(*this) == r;
    }
    
    bool operator !=(const const_iterator& r) const {
      return static_cast<const_iterator>(*this) != r;
    }
	  
  };
  
  class const_iterator {
  public:
    using value_type = vector::value_type;
    using reference = vector::reference;
		using pointer = vector::pointer;
		using difference_type = vector::difference_type;
		using iterator_category = std::bidirectional_iterator_tag;
  private:
    pointer ptr;
  public:
    const_iterator() {
      ptr = nullptr;
    }
    
    const_iterator(pointer ptr) {
      this->ptr = ptr;
    }
    
    reference operator*() const {
      return *ptr;
    }
    
    pointer operator->() const {
      return ptr;
    }
    
    const_iterator& operator++() {
      ++ptr;
      return *this;
    }
    
    const_iterator& operator++(int) {
      return ptr++;
    }
    
    const_iterator& operator--() {
      --ptr;
      return *this;
    }
    
    const_iterator operator--(int) {
      return ptr--;
    }
    
    bool operator==(const const_iterator& r) const {
      return ptr == r.ptr;
    }
    
    bool operator !=(const const_iterator& r) const {
      return ptr != r.ptr;
    }
    
  };
  
  
  class reverse_iterator {
    public:
    using value_type = vector::value_type;
    using reference = vector::reference;
		using pointer = vector::pointer;
		using difference_type = vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
	private:
	  pointer ptr;
	public:
	  reverse_iterator() {
	    ptr = nullptr;
	  }
	  
	  reverse_iterator(pointer ptr) {
	    this->ptr = ptr;
	  }
	  
	  reference operator*() {
	    return *ptr;
	  }
	  
	  pointer operator->() {
	    return ptr;
	  }
	  
	  reverse_iterator& operator++() {
	    --ptr;
	    return *this;
	  }
	  
	  reverse_iterator operator++(int) {
	    return ptr--;
	  }
	  
	  operator const_iterator() const {
      return const_iterator(ptr);
    }
    
    bool operator==(const const_iterator& r) const {
      return static_cast<const_iterator>(*this) == r;
    }
    
    bool operator !=(const const_iterator& r) const {
      return static_cast<const_iterator>(*this) != r;
    }
  
  };
   
  iterator begin() {
    return iterator(values);
  }
  
  const_iterator begin() const {
    return const_iterator(values);
  }
  
  iterator end() {
    return iterator(values + sz);
  }
  
  const_iterator end() const {
    return const_iterator(values + sz);
  }
  
  reverse_iterator rbegin() {
    return reverse_iterator(values + sz -1);
  }
  
  reverse_iterator rend() {
    return reverse_iterator(values - 1);
  }
  
};


#endif

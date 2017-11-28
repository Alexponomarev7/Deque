// https://github.com/Alexponomarev7/Deque

#include <iterator>
#include <stdlib.h>
#include <algorithm> 

namespace DequeNamespace {
    enum class ReallocationType {
        RT_INCREASE,
        RT_DECREASE,
        RT_NONE
    };
    
    enum class Errors {
        ER_OUT_OF_RANGE,
        ER_NO_ELEMENTS
    };
}

template<typename DType, typename category, typename value_type, typename difference_type, typename pointer, typename reference>
class DequeIterator : public std::iterator<category, value_type, difference_type, pointer, reference> {
public:
    typedef DequeIterator<DType, category, value_type, difference_type, pointer, reference> iterator;
    
    DequeIterator(size_t place, DType *d) : pointer_(static_cast<difference_type>(place)) {
        deque_ = d;
    }
    
    DequeIterator(const iterator &iter) : pointer_(iter.pointer_), deque_(iter.deque_) {}
    
    iterator &operator=(const iterator &d_iterator) {
        pointer_ = d_iterator.pointer_;
        deque_ = d_iterator.deque_;
        
        return *this;
    }
    
    iterator &operator+=(difference_type right) {
        pointer_ += right;
        return *this;
    }
    
    iterator &operator-=(difference_type right) {
        pointer_ -= right;
        return *this;
    }
    
    iterator operator+(difference_type right) const {
        iterator temp(*this);
        return temp += right;
    }
    
    iterator operator-(difference_type right) const {
        iterator temp(*this);
        return temp -= right;
    }
    
    difference_type operator-(const iterator &iter) const {
        difference_type answer = pointer_ - iter.pointer_;
        return answer;
    }
    
    iterator &operator++() {
        return operator+=(1);
    }
    
    iterator operator++(int) {
        iterator temp(*this);
        operator+=(1);
        return temp;
    }
    
    iterator &operator--() {
        return operator-=(1);
    }
    
    iterator operator--(int) {
        iterator temp(*this);
        operator-=(1);
        return temp;
    }
    
    bool operator==(iterator right) const {
        return pointer_ == right.pointer_ && deque_ == right.deque_;
    }
    
    bool operator!=(iterator right) const {
        return !operator==(right);
    }
    
    bool operator<(iterator right) const {
        return pointer_ < right.pointer_ && deque_ == right.deque_;
    }
    
    bool operator<=(iterator right) const {
        return !operator>(right);
    }
    
    bool operator>(iterator right) const {
        return right < *this;
    }
    
    bool operator>=(iterator right) const {
        return !operator<(right);
    }
    
    reference operator*() {
        return (*deque_)[pointer_];
    }
    
    pointer operator->() {
        return &(*deque_)[pointer_];
    }
    
    reference operator[](difference_type index) {
        return deque_->operator[](pointer_ + index);
    }
private:
    difference_type pointer_;
    
    DType *deque_;
};

template <typename T>
class Deque {
public:
    typedef DequeIterator<Deque<T>, std::random_access_iterator_tag, T, std::ptrdiff_t, T*, T&> iterator;
    typedef DequeIterator<const Deque<T>, std::random_access_iterator_tag, T, std::ptrdiff_t, const T*, const T&> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    Deque() : count_elements_(0), max_current_size_(MIN_SIZE_), l_pointer_(START_POSITION_L),
                r_pointer_(START_POSITION_R) {
        data = (T*)malloc(sizeof(T) * max_current_size_);
    }
    
    Deque(const Deque<T> &d) : count_elements_(d.count_elements_), max_current_size_(d.max_current_size_),
                                l_pointer_(d.l_pointer_), r_pointer_(d.r_pointer_) {
        data = (T*)malloc(sizeof(T) * max_current_size_);
        std::copy(d.data, d.data + max_current_size_, data);
    }
    
    ~Deque() {
        delete[] data;
    }
    
    void push_back(T element) {
        *(data + r_pointer_) = element;
        
        r_pointer_++;
        count_elements_++;
        
        r_pointer_ %= max_current_size_;
        reallocate(needReallocation());
    }
    
    void push_front(T element) {
        *(data + l_pointer_) = element;
        
        l_pointer_--;
        count_elements_++;
        
        l_pointer_ = (l_pointer_ + max_current_size_) % max_current_size_;
        reallocate(needReallocation());
    }
    
    void pop_back() {
        if (empty())
            throw DequeNamespace::Errors::ER_NO_ELEMENTS;
            
        r_pointer_--;
        count_elements_--;
        
        r_pointer_ = (r_pointer_ + max_current_size_) % max_current_size_;
        reallocate(needReallocation());
    }
    
    void pop_front() {
        if (empty())
            throw DequeNamespace::Errors::ER_NO_ELEMENTS;

        l_pointer_++;
        count_elements_--;
        
        l_pointer_ %= max_current_size_;
        reallocate(needReallocation());
    }
    
    T &operator[](size_t index) {
        if (index >= max_current_size_)
            throw DequeNamespace::Errors::ER_OUT_OF_RANGE;
        
        return data[(l_pointer_ + index + 1) % max_current_size_];
    }
    
    const T &operator[](size_t index) const {
        if (index >= max_current_size_)
            throw DequeNamespace::Errors::ER_OUT_OF_RANGE;
        
        return data[(l_pointer_ + index + 1) % max_current_size_];
    }
    
    T &front() {
        if (!size())
            throw DequeNamespace::Errors::ER_NO_ELEMENTS;
        
        return operator[](0);
    }
    
    T front() const {
        if (!size())
            throw DequeNamespace::Errors::ER_NO_ELEMENTS;
        
        return operator[](0);
    }
    
    T &back() {
        if (!size())
            throw DequeNamespace::Errors::ER_NO_ELEMENTS;
        
        return operator[](count_elements_ - 1);
    }
    
    T back() const {
        if (!size())
            throw DequeNamespace::Errors::ER_NO_ELEMENTS;
        
        return operator[](count_elements_ - 1);
    }
    
    size_t size() const {
        return count_elements_;
    }
    
    bool empty() const {
        return count_elements_ == 0;
    }
    
    iterator begin() {
        return iterator(0, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(0, this);
    }
    
    const_iterator begin() const {
        return cbegin();
    }
    
    iterator end() {
        return iterator(size(), this);
    }
    
    const_iterator cend() const {
        return const_iterator(size(), this);
    }
    
    const_iterator end() const {
        return cend();
    }
    
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }
    
    const_reverse_iterator rbegin() const {
        return crbegin();
    }
    
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    
    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }
    
    const_reverse_iterator rend() const {
        return crend();
    }
private:

    static const int RESIZE_ = 2;
    static const int MIN_SIZE_ = 8;
    static const int START_POSITION_L = 0;
    static const int START_POSITION_R = 1;

    size_t count_elements_;
    size_t max_current_size_;
    size_t l_pointer_;
    size_t r_pointer_;
    
    T *data;
    
    DequeNamespace::ReallocationType needReallocation() {
        if (2 * RESIZE_ * count_elements_ <= max_current_size_ && max_current_size_ != MIN_SIZE_) {
            return DequeNamespace::ReallocationType::RT_DECREASE;
        }
        
        if (count_elements_ == max_current_size_) {
            return DequeNamespace::ReallocationType::RT_INCREASE;
        }
        
        return DequeNamespace::ReallocationType::RT_NONE;
    }
    
    void reallocate(DequeNamespace::ReallocationType state) {
        if (state == DequeNamespace::ReallocationType::RT_NONE) {
            return;
        }
        
        size_t last_size = max_current_size_;
        
        if (state == DequeNamespace::ReallocationType::RT_INCREASE) {
            max_current_size_ *= RESIZE_;
        }

        if (state == DequeNamespace::ReallocationType::RT_DECREASE) {
            max_current_size_ /= RESIZE_;
        }
        
        T *buffer = (T*)malloc(sizeof(T) * max_current_size_);
        size_t current_pos = l_pointer_ + 1;
        for (size_t i = 1; i <= count_elements_; ++i) {
            current_pos %= last_size;
            buffer[i % max_current_size_] = data[current_pos];
            current_pos++;
        }
        
        l_pointer_ = 0;
        r_pointer_ = count_elements_ + 1;
        
        delete[] data;
        data = (T*)malloc(sizeof(T) * max_current_size_);
        std::copy(buffer, buffer + max_current_size_, data);
    }
};
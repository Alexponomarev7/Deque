#include <ctime>
#include <deque>
#include <gtest/gtest.h>

#include "deque.h"

namespace DequeTesting {
    const size_t COUNTS = 1e5;
    const double MAX_TIME = 10 * 1e-06;
    
    inline double getTime() {
        return static_cast<double>(clock()) / CLOCKS_PER_SEC;
    }
    
    void getNewTime(double &time, double &t_time) {
        time += getTime() - t_time;
        t_time = getTime();
    }
    
    template <typename DequeType>
    void fill(DequeType *d) {
        for (size_t i = 0; i < numberOfElements; ++i) {
            d->push_back(rand());
        }
    }
    
    class Check : public ::testing::Test {
    public:
        Deque::Deque <int> *d;
        
        double time = 0, t_time = 0;
    protected:
        void SetUp() {
            d = new Deque::Deque<int>();
            time = 0;
            t_time = getTime();
        }
        
        void TearDown() {
            delete d;
        }
    };
    
    class Compare : public ::testing::Test {
    public:
        Deque::Deque <int> *d;
        std::deque<int> *d_std;
        
        double time, t_time;
        
    protected:
        void SetUp() {
            d = new Deque::Deque<int>();
            d_std = new std::deque<int>();
            
            time = 0;
            t_time = getTime();
        }
        
        void TearDown() {
            delete d;
            delete d_std;
        }
    };
    
    TEST_F(Check, CheckPushBack) {
        for (size_t i = 0; i < COUNTS; ++i) {
            d->push_back(rand());
            getNewTime(time, t_time);
        }
        
        ASSERT_EQ(d->size(), COUNTS);
        ASSERT_LE(time / COUNTS, MAX_TIME);
    }
    
    TEST_F(Check, CheckPopBack) {
        fill(d);
        t_time = getTime();
        
        for (size_t i = 0; i < COUNTS; ++i) {
            ASSERT_FALSE(d->empty());
            
            d->pop_back();
            getNewTime(time, t_time);
        }
        
        ASSERT_FALSE(!d->empty());
        ASSERT_LE(time / COUNTS, MAX_TIME);
    }
    
    TEST_F(Check, ShuffledOperations) {
        d->push_back(rand());
        
        time = MAX_TIME;
        t_time = getTime();
        
        for (size_t i = 0; i < COUNTS; ++i) {
            int types = d->size() > 0 ? rand() % 4 : rand() % 2;
            switch (types) {
                case 0:
                    d->push_front(rand() % module);
                    break;
                case 1:
                    d->push_back(rand() % module);
                    break;
                case 3:
                    d->pop_front();
                    break;
                case 4:
                    d->pop_back();
                    break;
            }
            getNewTime(time, t_time);
        }
        
        ASSERT_LE(time / COUNTS, MAX_TIME);
    }
    
    TEST_F(Check, OperatorSquareBracesNoChange) {
        fill(d);
        
        t_time = getTime();
        
        int value;
        
        for (size_t i = 0; i < COUNTS; ++i) {
            value = (*d)[rand() % COUNTS];
            doNothing(value);
            getNewTime(time, t_time);
        }
        
        ASSERT_LE(time / COUNTS, MAX_TIME);
    }
    
    TEST_F(Check, OperatorSquareBraces) {
        fill(d);
        
        t_time = getTime();
        
        for (size_t i = 0; i < COUNTS; ++i) {
            (*d)[rand() % COUNTS] = rand();
            getNewTime(time, t_time);
        }
        
        ASSERT_LE(time / COUNTS, MAX_TIME);
    }
    
    TEST_F(Compare, StandardOperations) {
        for (size_t i = 0; i < COUNTS; ++i) {
            int n = rand();
            d->push_back(n);
            d_std->push_back(n);
            ASSERT_EQ(d->size(), d_std->size());
            ASSERT_EQ(d->front(), d_std->front());
            ASSERT_EQ(d->back(), d_std->back());
        }
        for (size_t i = 0; i < COUNTS; ++i) {
            int n = rand();
            d->push_front(n);
            d_std->push_front(n);
            ASSERT_EQ(d->size(), d_std->size());
            ASSERT_EQ(d->front(), d_std->front());
            ASSERT_EQ(d->back(), d_std->back());
        }
    }
    
    template <typename Iter1, typename Iter2>
    void testIters(Iter1 bd, Iter1 ed, Iter2 sbd, Iter2 sed) {
        ASSERT_EQ(bd, bd);
        ASSERT_EQ(*bd, *sbd);
        for (size_t i = 1; i < COUNTS / 2; ++i) {
            int k = rand() % (i - 1) + 1;
            if (k == 0)
                k = 1;
            ASSERT_EQ(*(bd + k), *(sbd + k));
            ASSERT_EQ(*(ed - k), *(sed - k));
            ASSERT_EQ(*(k + bd), *(sbd + k));
            ASSERT_EQ((ed - k) - bd, (sed - k) - sbd);
            ASSERT_LE(bd, ed - k);
        }
    }

    TEST_F(Compare, Iterators) {
        for (size_t i = 0; i < COUNTS; ++i) {
            int n = rand();
            
            d->push_back(n);
            d_std->push_back(n);
            ASSERT_EQ(d->size(), d_std->size());
            ASSERT_EQ(d->front(), d_std->front());
            ASSERT_EQ(d->back(), d_std->back());
        }
        for (size_t i = 0; i < COUNTS; ++i) {
            int n = rand();
            
            d->push_front(n);
            d_std->push_front(n);
            ASSERT_EQ(d->size(), d_std->size());
            ASSERT_EQ(d->front(), d_std->front());
            ASSERT_EQ(d->back(), d_std->back());
        }
        
        testIters(d->begin(), d->end(), d_std->begin(), d_std->end());
        testIters(d->cbegin(), d->cend(), d_std->cbegin(), d_std->cend());
        testIters(d->rbegin(), d->rend(), d_std->rbegin(), d_std->rend());
        testIters(d->crbegin(), d->crend(), d_std->crbegin(), d_std->crend());
    }
}

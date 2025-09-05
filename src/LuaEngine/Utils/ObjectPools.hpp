#ifndef ECLIPSE_OBJECT_POOLS_HPP
#define ECLIPSE_OBJECT_POOLS_HPP

#include <vector>
#include <deque>
#include <memory>
#include <shared_mutex>

namespace Eclipse
{
    template<typename T>
    class VectorPool
    {
    public:
        using VectorType = std::vector<T>;
        using VectorPtr = std::unique_ptr<VectorType>;

        static VectorPool& GetInstance()
        {
            static VectorPool instance;
            return instance;
        }

        // Get a vector from the pool (or create new)
        VectorPtr Acquire()
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            if (!pool.empty())
            {
                auto vec = std::move(pool.back());
                pool.pop_back();
                vec->clear();
                return vec;
            }

            // Create new vector with optimized initial capacity
            auto vec = std::make_unique<VectorType>();
            vec->reserve(DEFAULT_CAPACITY);
            return vec;
        }

        // Return vector to pool for reuse
        void Release(VectorPtr&& vec)
        {
            if (!vec) return;

            std::unique_lock<std::shared_mutex> lock(mutex);

            // Only keep vectors of reasonable size to avoid memory bloat
            if (vec->capacity() <= MAX_POOLED_CAPACITY)
            {
                vec->clear();
                pool.emplace_back(std::move(vec));

                // Prevent pool from growing too large
                if (pool.size() > MAX_POOL_SIZE)
                {
                    pool.pop_front();
                }
            }
        }

        // Get pool statistics
        size_t GetPoolSize() const 
        { 
            std::shared_lock<std::shared_mutex> lock(mutex);
            return pool.size(); 
        }

        void Clear()
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            pool.clear();
        }

    private:
        mutable std::shared_mutex mutex;
        std::deque<VectorPtr> pool;

        static constexpr size_t MAX_POOL_SIZE = 200;
        static constexpr size_t MAX_POOLED_CAPACITY = 200;
        static constexpr size_t DEFAULT_CAPACITY = 16;
    };

    // RAII wrapper for automatic pool management
    template<typename T>
    class PooledVector
    {
    public:
        using VectorType = std::vector<T>;
        using PoolType = VectorPool<T>;

        PooledVector() : pool(PoolType::GetInstance()), vector(pool.Acquire()) {}

        ~PooledVector() 
        { 
            if (vector)
            {
                pool.Release(std::move(vector)); 
            }
        }

        // Move-only semantics
        PooledVector(const PooledVector&) = delete;
        PooledVector& operator=(const PooledVector&) = delete;

        PooledVector(PooledVector&& other) noexcept
            : pool(other.pool), vector(std::move(other.vector)) {}

        PooledVector& operator=(PooledVector&& other) noexcept
        {
            if (this != &other)
            {
                if (vector) pool.Release(std::move(vector));
                vector = std::move(other.vector);
            }
            return *this;
        }

        // Vector-like interface
        VectorType& operator*() { return *vector; }
        const VectorType& operator*() const { return *vector; }

        VectorType* operator->() { return vector.get(); }
        const VectorType* operator->() const { return vector.get(); }

        VectorType* get() { return vector.get(); }
        const VectorType* get() const { return vector.get(); }

    private:
        PoolType& pool;
        typename PoolType::VectorPtr vector;
    };
}

#endif // ECLIPSE_OBJECT_POOLS_HPP
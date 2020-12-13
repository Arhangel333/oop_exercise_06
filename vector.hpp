#include <stdexcept>

using namespace std;

typedef double TP;

template <class T, size_t BLOCK_SIZE>
class Alloc;

template <class T>
class vector;

template <class T, class Alloc>
class TVector
{
public:
    const int TVECTOR_EXTRA_SIZE = 5;
    int size;
    int capacity;
    T *data;
    static const size_t memSize = 10000; //фиксированный размер аллокатора
                                         //friend class Alloc<T,memSize>;
    Alloc alloc;                         //экземпляр аллокатора

    TVector()
    {
        this->size = 0;
        this->capacity = 3;
        this->data = alloc.Alloc::allocate(3);
    }

    TVector(const TVector<T, Alloc> &v)
    {
        this->size = v.size;
        this->capacity = v.capacity;

        this->data = alloc.Alloc::allocate(v.capacity);
        for (int i = 0; i < this->size; i++)
        {
            this->data[i] = v.data[i];
        }
    }

    TVector(int x)
    {
        this->size = 0;
        this->capacity = x;

        this->data = alloc.Alloc::allocate(x);
    }

    void PushEnd(T x)
    {
        if (this->size >= this->capacity)
        {

            int oldcap = this->capacity;
            this->capacity += TVECTOR_EXTRA_SIZE;

            T *buf = alloc.Alloc::allocate(this->capacity);

            for (int i = 0; i < oldcap; i++)
            {
                buf[i] = this->data[i];
            }

            alloc.Alloc::deallocate(this->data, this->size);

            this->data = buf;

            buf = 0;
        }

        this->data[this->size] = x;

        this->size++;
    }

    void PopEnd()
    {
        if (this->size <= 0)
        {
            printf("TVector is empty.\n");
        }
        else
        {

            this->size--;
        }
    }

    int Erase(int i)
    {
        if (this->size <= 0)
        {
            return -1;
        }
        else
        {
            if ((i < this->size) && (i >= 0))
            {
                this->size--;
                for (i; i < this->size; i++)
                {
                    //if(i != )
                    this->data[i] = this->data[i + 1];
                }
            }
            else
            {
                return -1;
            }
        }
        return 0;
    }

    void insert(int i, T tr)
    {
        if ((i <= this->size) && (i >= 0))
        {
            this->PushEnd(tr);
            for (int j = this->size - 1; j > i; j--)
            {
                this->data[j].Print();
                this->data[j] = this->data[j - 1];
            }
            this->data[i] = tr;
        }
    }

    class iterator
    {
    public:
        int index = -1;
        TVector<T, Alloc> *vect;
        //std::iterator_traits<TVector<T, Alloc>::iterator> difference_type = int;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = T &;
        using pointer = T *;
        using iterator_category = std::forward_iterator_tag;
        std::shared_ptr<T> ptr;

        iterator()
        {
            index = 0;
        }

        iterator(const iterator &it)
        {
            this->vect = it.vect;
            this->ptr = it.ptr;
            this->index = it.index;
        }

        iterator(int ind, TVector<T, Alloc> *vect)
        {
            this->ptr = std::make_shared<T>((vect->data[ind]));
            this->index = ind;
            this->vect = vect;
        }

        iterator operator++()
        {
            //cout<<this<<" "<<this->index<<" "<<this->vect->size<<endl;
            if ((this->index + 1) > this->vect->size)
            {
                //printf("Iterator cannot be incremented past the end of range.%d %d\n", this->index, this->vect->size);
                throw std::out_of_range("Iterator cannot be incremented past the end of range.");
                return *this;
            }
            this->index++;
            this->ptr = std::make_shared<T>(this->vect->data[this->index]);

            return *this;
        }

        iterator &operator++(int)
        {
            if ((this->index + 1) > (this->vect->size))
            {
                //printf("Iterator cannot be incremented past the end of range.\n");
                throw std::out_of_range("Iterator cannot be incremented past the end of range.");
                return *this;
            }
            this->index++;
            this->ptr = std::make_shared<T>(this->vect->data[this->index]);
            return *this;
        }

        iterator operator=(iterator x)
        {
            //printf("=\n");
            this->ptr = x.ptr;
            return *this;
        }

        bool operator!=(const iterator &x)
        { //printf("!=\n");
            if (index != x.index)
                return true;
            //printf("false\n ");
            /* if(ptr != x.ptr) return true;
            printf("\t !=\n "); */
            return false;
        }

        bool operator>=(iterator x)
        {
            return this->ptr >= x.ptr;
        }

        int operator-(iterator x)
        {
            //printf("-\n");
            return this->index - x.index;
        }

        bool operator==(const iterator &x)
        {
            //printf("\t ==\n ");
            if (index == x.index)
                return true;
            if (ptr == x.ptr)
                return true;
            return false;
        }

        T operator*()
        {
            return *(this->ptr);
        }

        T *operator->()
        {
            return this->ptr;
        }

        ~iterator() {}
    };

    iterator begin()
    {
        TVector<T, Alloc>::iterator it(0, this);
        return it;
    }

    iterator end()
    {
        TVector<T, Alloc>::iterator it(this->size, this);
        return it;
    }

    void Printing()
    {
        printf(">>>>>>>>\n");
        for (int i = 0; i < this->size; i++)
        {
            cout << this->data[i] << " ";
        }
        cout << endl;
        printf(">>>>>>>>size %d | cap %d\n", this->size, this->capacity);
    }

    void Show()
    {
        printf(">>>>>>>>\n");
        for (int i = 0; i < this->capacity; i++)
        {
            cout << this->data[i] << " ";
        }
        cout << endl;
        printf(">>>>>>>>size %d | cap %d\n", this->size, this->capacity);
    }

    ~TVector()
    {
        this->size = 0;
        this->capacity = 0;
        alloc.Alloc::deallocate(this->data, this->size);
    }

    T operator[](int i)
    {
        return this->data[i];
    }

    T *operator++(int x)
    {
        return this->data[x + 1];
    }

    T *operator--(int x)
    {
        return this->data[x - 1];
    }

    void operator=(TVector<T, Alloc> vec)
    {
        for (int i = 0; i < this->size; i++)
        {
            this->data[i] = vec.data[i];
        }
    }
};

template <class T>
class vector
{
public:
    int size;
    int capacity;
    T *data;

    const int VECTOR_EXTRA_SIZE = 10;

    // friend class TVector<T, Alloc<T, memSize>>;

    vector()
    {
        this->size = 0;
        this->capacity = 3;
        this->data = new T[3];
    }

    vector(const vector<T> &v)
    {
        this->size = v.size;
        this->capacity = v.capacity;

        this->data = new T[v.capacity];
        for (int i = 0; i < this->size; i++)
        {
            this->data[i] = v.data[i];
        }
    }

    vector(int x)
    {
        this->size = 0;
        this->capacity = x;

        this->data = new T[x];
    }

    void PushEnd(T x)
    {
        if (this->size >= this->capacity)
        {

            int oldcap = this->capacity;
            this->capacity += VECTOR_EXTRA_SIZE;

            T *buf = new T[this->capacity];

            for (int i = 0; i < oldcap; i++)
            {
                buf[i] = this->data[i];
            }

            delete[] this->data;

            this->data = buf;

            buf = 0;
        }

        this->data[this->size] = x;

        this->size++;
    }

    bool Empty()
    {
        if (this->size == 0)
        {
            return true;
        }
        return false;
    }

    T PopEnd()
    {
        if (this->size <= 0)
        {
            printf("Vector is empty.\n");
            return -1;
        }
        else
        {

            this->size--;
        }
        return this->data[this->size];
    }

    void Erase(int i)
    {
        if (this->size <= 0)
        {
            printf("Vector is empty.\n");
        }
        else
        {
            if ((i < this->size) && (i >= 0))
            {
                this->size--;
                for (i; i < this->size; i++)
                {
                    //if(i != )
                    this->data[i] = this->data[i + 1];
                }
            }
            else
            {
                printf("В векторе нет элемента под индексом %d\n", i);
            }
        }
    }

    void insert(int i, T tr)
    {
        if ((i <= this->size) && (i >= 0))
        {
            this->PushEnd(tr);
            for (int j = this->size - 1; j > i; j--)
            {
                this->data[j].Print();
                this->data[j] = this->data[j - 1];
            }
            this->data[i] = tr;
        }
    }

    class iterator
    {
    public:
        int index = -1;
        vector<T> *vect;
        //std::iterator_traits<vector<T>::iterator> difference_type = int;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = T &;
        using pointer = T *;
        using iterator_category = std::forward_iterator_tag;
        std::shared_ptr<T> ptr;

        iterator()
        {
            index = 0;
        }

        iterator(const iterator &it)
        {
            this->vect = it.vect;
            this->ptr = it.ptr;
            this->index = it.index;
        }

        iterator(int ind, vector<T> *vect)
        {
            this->ptr = std::make_shared<T>((vect->data[ind]));
            this->index = ind;
            this->vect = vect;
        }

        iterator operator++()
        {
            //cout<<this<<" "<<this->index<<" "<<this->vect->size<<endl;
            if ((this->index + 1) > this->vect->size)
            {
                //printf("Iterator cannot be incremented past the end of range.%d %d\n", this->index, this->vect->size);
                throw std::out_of_range("Iterator cannot be incremented past the end of range.");
                return *this;
            }
            this->index++;
            this->ptr = std::make_shared<T>(this->vect->data[this->index]);

            return *this;
        }

        iterator &operator++(int)
        {
            if ((this->index + 1) > (this->vect->size))
            {
                //printf("Iterator cannot be incremented past the end of range.\n");
                throw std::out_of_range("Iterator cannot be incremented past the end of range.");
                return *this;
            }
            this->index++;
            this->ptr = std::make_shared<T>(this->vect->data[this->index]);
            return *this;
        }

        iterator operator=(iterator x)
        {
            //printf("=\n");
            this->ptr = x.ptr;
            return *this;
        }

        bool operator!=(const iterator &x)
        { //printf("!=\n");
            if (index != x.index)
                return true;
            //printf("false\n ");
            /* if(ptr != x.ptr) return true;
            printf("\t !=\n "); */
            return false;
        }

        bool operator>=(iterator x)
        {
            return this->ptr >= x.ptr;
        }

        int operator-(iterator x)
        {
            //printf("-\n");
            return this->index - x.index;
        }

        bool operator==(const iterator &x)
        {
            //printf("\t ==\n ");
            if (index == x.index)
                return true;
            if (ptr == x.ptr)
                return true;
            return false;
        }

        T operator*()
        {
            return *(this->ptr);
        }

        T *operator->()
        {
            return this->ptr;
        }

        ~iterator() {}
    };

    iterator begin()
    {
        vector<T>::iterator it(0, this);
        return it;
    }

    iterator end()
    {
        vector<T>::iterator it(this->size, this);
        return it;
    }

    void Printing()
    {
        printf(">>>>>>>>\n");
        for (int i = 0; i < this->size; i++)
        {
            cout << this->data[i] << " ";
        }
        cout << endl;
        printf(">>>>>>>>size %d | cap %d\n", this->size, this->capacity);
    }

    void Show()
    {
        printf(">>>>>>>>\n");
        for (int i = 0; i < this->capacity; i++)
        {
            cout << this->data[i] << " ";
        }
        cout << endl;
        printf(">>>>>>>>size %d | cap %d\n", this->size, this->capacity);
    }

    ~vector()
    {
        this->size = 0;
        this->capacity = 0;
        delete[] this->data;
    }

    T operator[](int i)
    {
        return this->data[i];
    }

    T *operator++(int x)
    {
        return this->data[x + 1];
    }

    T *operator--(int x)
    {
        return this->data[x - 1];
    }

    void operator=(vector<T> vec)
    {
        for (int i = 0; i < this->size; i++)
        {
            this->data[i] = vec.data[i];
        }
    }
};

template <class T, size_t BLOCK_SIZE>
class Alloc
{ //класс аллокатора
public:
    T *allocate(int n = 1) //конструктор аллокатора и выделение памяти
    {
        int idx;
        T *arr;
        if (last < BLOCK_SIZE)
        {
            idx = last;
            arr = &block[idx];
            last += n;
        }
        else
        {
            if (!free.Empty())
            {
                if (this->PlaceOK(n))
                {
                    arr = &block[idx];
                    for (int i = 0; i < n; i++)
                    {
                        idx = free.PopEnd();
                    }
                }
                else
                {
                    this->MoveData();
                }
            }
            else
            {
                throw std::runtime_error("Allocator has no more free blocks");
            }
        }
        return arr;
    }

    void deallocate(T *ptr, int n = 1) //деструктор аллокатора
    {
        for (int i = 0; i < n; i++)
        {
            int idx = ptr + i * sizeof(T) - &block[0];
            free.PushEnd(idx);
        }
    }

private:
    T block[BLOCK_SIZE]; //размер аллокатора
    vector<int> free;    //Освобождение памяти из стека
    int last = 0;        //наименьший из нераспределенных индексов

    bool PlaceOK(int n)
    {
        int ind = free.size - 1;
        for (int i = 0; i < n; i++)
        {
            if (free[ind] - 1 != free[ind - 1])
            {
                return false;
            }
        }
        return true;
    }

    void MoveData()
    {
        printf("Двигаем память аллокатором для удобного распределения\n");
    }
};

/* std::shared_ptr<Node> head;//головной узел
	std::weak_ptr<Node> tail;//хвостовой узел
	size_t size;//размер

	static const size_t memSize = 10000;//фиксированный размер аллокатора
	Alloc<Node,memSize> alloc;//конструктор аллокатора

	void deallocNode( Node *ptr ) {//деструктор аллокатора
		alloc.Alloc::deallocate( ptr );
	}

	std::shared_ptr<Node> allocNode(const T& val) {//Выделение памяти из аллокатора на узел очереди
		Node *res = alloc.Alloc::allocate();
		res->value = val;//
		return std::shared_ptr<Node>( res, std::bind( &Queue::deallocNode, this, std::placeholders::_1 ));
	}
 */

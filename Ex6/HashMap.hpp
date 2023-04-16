#include <vector>
#include <stdexcept>
#define CAP_BASE 16
#define LOW_LOAD_FAC (0.25)
#define UP_LOAD_FAC (0.75)

using std::vector;
using std::pair;



template<class KeyT, class ValueT>
class HashMap{
protected:
    typedef pair<KeyT,ValueT> k_v_pair;
    typedef vector<k_v_pair> hashmap_typ;
    void re_hashing(bool increase);
    long hash_func(const KeyT& k) const
    {
        return (std::hash<KeyT>{}(k) & (_capacity - 1));
    }
    int _capacity = CAP_BASE;
    size_t _size = 0;
    hashmap_typ* _map;
    int find_next_index(int cur_index) const {
        for (int i = cur_index; i < _capacity; ++i)
        {
            if (_map[i].size() > 0) { return i; }
        }
        return _capacity;
    }
public:
    HashMap(): _map(new hashmap_typ[CAP_BASE]){};
    HashMap(const vector<KeyT>& keys, const vector<ValueT>& vals);
    HashMap(const HashMap& hm);
    virtual ~HashMap() {delete[] _map;};
    size_t size() const {return _size;} ;
    int capacity() const {return _capacity;};

    bool empty() const;
    bool insert(const KeyT& key, const ValueT& val);
    bool contains_key(const KeyT& key) const;
    ValueT& at(const KeyT& key);
    ValueT at(const KeyT& key) const;

    virtual bool erase(const KeyT& key);
    double get_load_factor() const;
    int bucket_size(const KeyT& key) const;
    int bucket_index(const KeyT& key) const;
    void clear();

    HashMap<KeyT,ValueT>& operator=(const HashMap<KeyT,ValueT>& other);
    ValueT& operator[](const KeyT& key);
    const ValueT& operator[](const KeyT& key) const;
    bool operator==(const HashMap<KeyT,ValueT>& hmap) const;
    bool operator!=(const HashMap<KeyT,ValueT>& hmap) const;

    class ConstIterator{
        friend class HashMap;
    private:
        const HashMap& _hashmap;
        long _bucket_index;
        long _in_bucket_index;
    public:
        using value = const pair<KeyT, ValueT>;
        typedef value value_type;
        typedef value& reference;
        typedef value* pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;
        ConstIterator(const HashMap& hm,long bucket_index,long in_buc_index);
        ConstIterator& operator++()
        {
            if((++_in_bucket_index)  >= _hashmap._map[_bucket_index].size())
            {
                _bucket_index = _hashmap.find_next_index(++_bucket_index);
                _in_bucket_index = 0;
                if(_bucket_index == _hashmap._capacity){return *this;}
            }
            return *this;
        };
        ConstIterator operator++(int)
        {
            ConstIterator copy_it(*this);
            this->operator++();
            return copy_it;
        };

        bool operator==(const ConstIterator &lhs) const
        {
            bool x =((&this->_hashmap == &lhs._hashmap) &&
                    (_in_bucket_index == lhs._in_bucket_index) &&
                    (_bucket_index == lhs._bucket_index));
            return x;
        }
        bool operator!=(const ConstIterator &lhs) const
        {
            return !(this->operator==(lhs));
        }
        reference operator* () const
        {
           return _hashmap._map[_bucket_index][_in_bucket_index];
        }
        pointer operator->() const
        {
            return &(operator* ());
        }

    };
    using const_iterator = ConstIterator;
    const_iterator cbegin() const { return ConstIterator(*this,
                                                     find_next_index(0),0);};
    const_iterator cend() const { return ConstIterator(*this, _capacity, 0);};
    const_iterator begin() const { return cbegin();};
    const_iterator end() const { return cend();};
    const_iterator begin() { return cbegin();};
    const_iterator end() { return cend();};
};

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::ConstIterator::ConstIterator(const HashMap &hm,
long bucket_index,long in_buc_index): _hashmap(hm),_bucket_index(bucket_index),
_in_bucket_index(in_buc_index){};


template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(const vector<KeyT>& keys,
                                        const vector<ValueT>& vals): HashMap()
{
    if(keys.size() != vals.size()){throw std::length_error("");}
    for(size_t i = 0; i < keys.size(); ++i)
    {
        this->operator[](keys[i]) = vals[i];
    }
}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(const HashMap& hm): _capacity(hm.capacity()),
                    _size(hm.size()), _map(new hashmap_typ[hm.capacity()])
{
    for(int i = 0; i < _capacity; i++)
    {
        _map[i] = hm._map[i];
    }
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::empty() const
{
    for (int i = 0; i < _capacity; ++i)
    {
        if(_map[i].size() > 0) { return false; };
    }
    return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::insert(const KeyT& key, const ValueT& val)
{
    if(contains_key(key)) { return false; };
    _size++;
    _map[hash_func(key)].push_back(k_v_pair(key, val));
    if( get_load_factor() > UP_LOAD_FAC) { re_hashing(true); };
    return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::contains_key(const KeyT& key) const {
    if(hash_func(key) >= _capacity){ return false; }
    hashmap_typ bucket = _map[hash_func(key)];
    for(size_t i = 0; i < bucket.size(); ++i)
    {
        if(bucket[i].first == key){ return true; }
    }
    return false;
}


template<class KeyT, class ValueT>
ValueT& HashMap<KeyT, ValueT>::at(const KeyT& key)
{
    if(contains_key(key))
    {
        for (auto &it: _map[hash_func(key)])
        {
            if (it.first == key) { return it.second; };
        }
    }
    throw std::runtime_error("");
}

template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::at(const KeyT& key) const
{
    if(contains_key(key))
    {
        for (const auto &it: _map[hash_func(key)])
        {
            if (it.first == key) { return it.second; };
        }
    }
    throw std::runtime_error("");
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase(const KeyT& key)
{
    if(!contains_key(key)) { return false; }
    hashmap_typ& bucket = _map[hash_func(key)];
    auto k = bucket.begin();
    for (int i = 0; !(bucket[i].first == key); ++i){ k++; };
    bucket.erase(k);
    _size--;
    while(get_load_factor()<LOW_LOAD_FAC && _capacity > 1) {re_hashing(false);}
    return true;
}

template<class KeyT, class ValueT>
double HashMap<KeyT, ValueT>::get_load_factor() const
{
    return (((double) _size) / ((double) _capacity));
}

template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::re_hashing(bool increase)
{
    int old_capacity = _capacity;
    if (increase) { _capacity *= 2;}
    else { _capacity /= 2; };
    auto* re_hashed_map = new hashmap_typ[_capacity];
    for(int i = 0; i < old_capacity; ++i)
    {
        hashmap_typ bucket = _map[i];
        for( size_t j = 0; j < bucket.size(); ++j)
        {
            k_v_pair to_push(bucket[j].first, bucket[j].second);
            re_hashed_map[hash_func(bucket[j].first)].push_back(to_push);
        }
    }
    delete[] _map;
    _map = re_hashed_map;
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucket_size(const KeyT &key) const
{
    if(contains_key(key))
    {
        hashmap_typ& bucket = _map[hash_func(key)];
        return bucket.size();
    }
    throw std::runtime_error("");
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucket_index(const KeyT& key) const
{
    if(contains_key(key)) {
        hashmap_typ& bucket = _map[hash_func(key)];
        for(int i = 0; i < _capacity; ++i)
        {
            if(_map[i] == bucket){ return i; }
        }
    }
    throw std::runtime_error("");
}

template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::clear()
{
    delete[] _map;
    _map = new hashmap_typ[_capacity];
    _size = 0;
}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>& HashMap<KeyT, ValueT>::operator=(
                                        const HashMap<KeyT, ValueT>& other)
{
    if (this == &other) {return *this;};
    _capacity = other._capacity;
    _size = other._size;
    delete[] _map;
    _map = new hashmap_typ[other._capacity];
    for (int i = 0; i  < other._capacity; ++i)
    {
        _map[i] = other._map[i];
    }
    return *this;
}

template<class KeyT, class ValueT>
ValueT& HashMap<KeyT, ValueT>::operator[](const KeyT& key)
{
    if(!(contains_key(key))) { insert(key, ValueT()); }
    return at(key);
}

template<class KeyT, class ValueT>
const ValueT& HashMap<KeyT, ValueT>::operator[](const KeyT& key) const
{
    return at(key);
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap<KeyT,ValueT>& hmap) const
{
    if (_size != hmap._size) {return false;}
    for(auto i = 0; i < capacity(); i ++)
    {
        for (const auto& pair : _map[i])
        {
            if((!hmap.contains_key(pair.first)) ||
            (!(hmap.at(pair.first) == this->at(pair.first)))) { return false; }
        }
    }
    return true;
}


template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator!=(const HashMap<KeyT,ValueT>& hmap) const
{
    return !(*this == hmap);
}



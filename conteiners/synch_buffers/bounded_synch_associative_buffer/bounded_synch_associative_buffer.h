#include <algorithm>
#include <list>
#include <vector>

#include <condition_variable>
#include <shared_mutex>
#include <mutex>

/* Block bounded synchronous associative buffer implementation 
 * wait on pop method.
 **/


template <typename Key, typename Value, typename Hash=std::hash<Key> >
class bounded_synch_buffer {
private:
  bounded_synch_buffer(const bounded_synch_buffer&) = delete;
  bounded_synch_buffer operator=(const bounded_synch_buffer&) = delete;

  class bunch_buffer {
  private:
    using block = std::pair<Key, Value>;
    using chain = std::list<block>;
    using chain_iterator = typename chain::iterator;
    using const_chain_iterator = typename chain::const_iterator;

    chain _bunch;
    mutable std::shared_mutex _mutex;

  public:
    chain_iterator find_of(const Key& key) {
      return std::find_if(_bunch.begin(), _bunch.end(),
        [&key] (block const& current) -> bool 
        { return (current.first == key); });
    }
    
    const_chain_iterator const_find_of(const Key& key) const {
      return std::find_if(_bunch.cbegin(), _bunch.cend(),
        [&key] (block const& current) -> bool 
        { return (current.first == key); });
    }

    template <typename InnerKey, typename InnerValue>
    void add_or_update_item(InnerKey&& key, InnerValue&& value) {
      std::lock_guard guard(_mutex);
      auto find_iterator = find_of(key);
      if (find_iterator == _bunch.end()) {
        _bunch.emplace_back(std::forward<InnerKey>(key), 
                            std::forward<InnerValue>(value));
      }
      else {
        find_iterator->second = std::forward<InnerValue>(value);
      }
    }

    template <typename InnerValue>
    Value value_for(Key const& key, InnerValue&& default_value) const {
      std::shared_lock shared_guard(_mutex);
      auto find_iterator = const_find_of(key);
      if (find_iterator != _bunch.cend()) {
        return find_iterator->second;
      }
      // correct there if RVO or maybe std::optional
      return std::forward<InnerValue>(default_value);
    }

    void remove(Key const& key) {
      std::lock_guard guard(_mutex);
      auto find_iterator = const_find_of(key);
      if (find_iterator != _bunch.cend()) {
        _bunch.erase(find_iterator);
      }
    }
  };
  
public:
  using key_type = Key;
  using value_type = Value;

  template <typename InnerKey, typename InnerValue>
  void add_or_update_item(InnerKey&& key, InnerValue&& value) {
    _buffer[key_mapping(key)].add_or_update_item(
      std::forward<InnerKey>(key), std::forward<InnerValue>(value));
  }

  template <typename InnerValue>
  Value value_for(Key const& key, InnerValue&& default_value=Value()) const {
    return _buffer[key_mapping(key)].value_for(
      key, std::forward<InnerValue>(default_value));
  }

  void remove(Key const& key) {
    return _buffer[key_mapping(key)].remove(key);
  }

  size_t key_mapping(Key const& key) const {
    return _mapper(key) % _buffer_size;
  }

  // if buffer_size is prime number therefor less probability of hash collision
  bounded_synch_buffer(size_t buffer_size=17, Hash const& mapper=Hash())
  : _buffer(buffer_size), _buffer_size(buffer_size), _mapper(mapper) 
  {
  }

private:
  std::vector<bunch_buffer> _buffer;
  const size_t _buffer_size;
  Hash _mapper;
};


//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  ~HashTableTemplate();
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int sum = 0;
	int len = strlen(key);
	for(int i = 0; i < len; i++)
	{
		sum += (i+1)*key[i];
	}
  return sum % TableSize;
}

template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  // Add implementation here
  _buckets = new HashTableTemplateEntry<Data>*[TableSize];
  for(int i=0;i<TableSize;i++)
  {
    _buckets[i] = NULL;
  }
}
template <typename Data>
HashTableTemplate<Data>::~HashTableTemplate()
{
  for(int i=0;i<TableSize;i++)
  {
    if(_buckets[i] != NULL)
    {
      delete _buckets[i]->_key;
      delete _buckets[i];
    }
  }
  delete [] _buckets;
}

template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  // Add implementation here
  int h = hash(key);
  HashTableTemplateEntry<Data> *e = _buckets[h];
  while (e != NULL)
  {
    if(strcmp(key,e->_key)==0)
    {
      e->_data = data;
      return true;
    }
    e = e->_next;
  }
  e = new HashTableTemplateEntry<Data>();
  e->_key = strdup(key);
  e->_data = data;
	e->_next = _buckets[h];
  _buckets[h] = e;
  return false;
}

template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
  // Add implementation here
  int h = hash(key);
  HashTableTemplateEntry<Data> *e = _buckets[h];
  while(e != NULL)
  {
    if(strcmp(key,e->_key)==0)
    {
      *data = e->_data;
      return true;
    }
    e = e->_next;
  }
  return false;
}

template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
  int h = hash(key);
  HashTableTemplateEntry<Data> *e = _buckets[h];
  Data d;
  while(e != NULL)
  {
    if(strcmp(key,e->_key)==0)
    {
      d = e->_data;
      return d;
    }
    e = e->_next;
  }
  return d;
}

template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  // Add implementation here
  int h = hash(key);
  HashTableTemplateEntry<Data> *e = _buckets[h];
  while(e != NULL)
  {
    if(strcmp(key,e->_key)==0)
    {
      _buckets[h] = e->_next;
      delete e->_key;
      delete e;
      return true;
    }
    e = e->_next;
  }
  return false;
}

template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  // Add implementation here
  _hashTable = hashTable;
  _currentBucket = 0;
  _currentEntry = _hashTable->_buckets[_currentBucket];
}

template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
  // Add implementation here
  while(_currentEntry == NULL && _currentBucket <2039)
  {
    _currentEntry = _hashTable->_buckets[_currentBucket++];
  }
  if(_currentEntry == NULL)
  {
    return false;
  }
  key = _currentEntry->_key;
  data = _currentEntry->_data;
  _currentEntry = _currentEntry->_next;
  return true;
}

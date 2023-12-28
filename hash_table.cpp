#include "catalog.h"

// удаляет все значения
void HashTable::reset() { data.clear(); }

// количество значений
unsigned int HashTable::count() { return data.size(); }

// сохранить содержимое в файл по пути
void HashTable::save(std::string path) {
  std::ofstream file(path);
  if (file.is_open()) {
    unsigned int hash_id;
    std::string string;
    int number;
    for (auto i = data.begin(); i < data.end(); i++) {
      std::tie(hash_id, string, number) = *i;
      file << hash_id << " " << string << " " << number << "\n";
    }
    filepath = path;
    file.close();
  } else {
    throw std::invalid_argument("Unable to open file");
  }
}

// загрузить из файла по пути
void HashTable::load(std::string path) {
  std::ifstream file(path);
  if (file.is_open()) {
    this->reset();
    entry entry;
    unsigned int hash_id;
    char string[1024];
    int number;
    char s[1024];
    file.getline(s, 1024, '\n');
    while (s[0]) {
      sscanf(s, "%u %s %d", &hash_id, string, &number);
      entry = {hash_id, string, number};
      data.push_back(entry);
      file.getline(s, 1024, '\n');
    }
    filepath = path;
    file.close();
  } else {
    throw std::invalid_argument("Unable to open file");
  }
}

// добавить запись
void HashTable::add(entry &new_entry) {
  unsigned int index = hash_function(new_entry);
  data.insert(find_hash(index),
              {index, std::get<1>(new_entry), std::get<2>(new_entry)});
}

// удалить запись при ее наличии
void HashTable::remove(entry &new_entry) {
  unsigned int index = hash_function(new_entry);
  auto i = find_hash(index);
  while (std::get<0>(*i) == index &&
         !(std::get<1>(*i) == std::get<1>(new_entry) &&
           std::get<2>(*i) == std::get<2>(new_entry)))
    i++;
  if (std::get<1>(*i) == std::get<1>(new_entry) &&
      std::get<2>(*i) == std::get<2>(new_entry))
    data.erase(i);
}

// проверить наличие записи по ключу
bool HashTable::is_not_empty(unsigned int index) {
  auto i = find_hash(index);
  return std::get<0>(*i) == index;
}

// равенство
bool HashTable::operator==(const HashTable &other) {
  bool result = false;
  if (data.size() == other.data.size()) {
    result = true;
    auto i = data.begin();
    auto j = other.data.begin();
    while (i < data.end() && j < other.data.end()) {
      if (!(std::get<1>(*i) == std::get<1>(*j) &&
            std::get<0>(*i) == std::get<0>(*j) &&
            std::get<2>(*i) == std::get<2>(*j))) {
        result = false;
        break;
      }
      i++;
      j++;
    }
  }
  return result;
}

bool HashTable::operator!=(const HashTable &other) { return !(*this == other); }

// создает новую таблицу из пересечения двух
HashTable HashTable::operator&&(const HashTable &other) {
  HashTable result;
  auto i = data.begin();
  auto j = other.data.begin();
  while (i < data.end() && j < other.data.end()) {
    if (std::get<0>(*i) == std::get<0>(*j)) {
      auto k = j;
      while (std::get<0>(*i) == std::get<0>(*k)) {
        if (std::get<1>(*i) == std::get<1>(*k) &&
            std::get<2>(*i) == std::get<2>(*k)) {
          entry new_entry = *i;
          result.data.push_back(new_entry);
          break;
        }
        k++;
      }
      i++;
    } else {
      if (std::get<0>(*j) < std::get<0>(*i))
        j++;
      else
        i++;
    }
  }
  return result;
}

// хэш функция
unsigned int HashTable::hash_function(entry &new_entry) {
  long unsigned int sum = std::get<2>(new_entry) + std::get<1>(new_entry)[0];
  for (int i = 0; i < std::get<1>(new_entry).length(); i++) {
    sum *= std::get<1>(new_entry)[i];
  }
  return sum % MAX_HASH;
}

// найти элемент по хэшу
std::vector<entry>::iterator HashTable::find_hash(unsigned int hash) {
  std::vector<entry>::iterator pos;
  if (hash <= std::get<0>(*data.begin())) {
    pos = data.begin();
  } else if (hash > (std::get<0>(*(data.end() - 1)))) {
    pos = data.end();
  } else if (hash == (std::get<0>(*(data.end() - 1)))) {
    pos = data.end() - 1;
  } else {
    unsigned int delta = data.size() / 2;
    pos = data.begin() + delta;
    while (!(std::get<0>(*(pos - 1)) <= hash && std::get<0>(*pos) >= hash)) {
      if (delta > 1)
        delta /= 2;
      hash < std::get<0>(*pos) ? pos -= delta : pos += delta;
    }
    while (std::get<0>(*(pos - 1)) == hash)
      pos -= 1;
  }
  return pos;
}

std::string HashTable::get_path() { return filepath; }

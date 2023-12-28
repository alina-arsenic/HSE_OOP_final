#ifndef __CATALOG_H__
#define __CATALOG_H__

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#define MAX_HASH 10301

typedef std::tuple<unsigned int, std::string, int> entry;

class HashTable {
public:
  HashTable() : data() { data.reserve(10); };
  HashTable(const HashTable &other) : data(other.data){};
  ~HashTable(){};

  void reset();         // удаляет все значения
  unsigned int count(); // количество значений

  virtual void save(std::string path); // сохранить содержимое в файл по пути
  virtual void load(std::string path); // загрузить из файла по пути

  virtual void add(entry &new_entry); // добавить запись
  virtual void remove(entry &new_entry); // удалить запись при ее наличии
  virtual bool
  is_not_empty(unsigned int index); // проверить наличие записи по ключу

  bool operator==(const HashTable &other); // равенство
  bool operator!=(const HashTable &other); // неравенство
  HashTable operator&&(
      const HashTable &other); // создает новую таблицу из пересечения двух

  std::vector<entry>::iterator
  find_hash(unsigned int hash); // возвращает запись по хэшу
  std::string get_path();

private:
  std::vector<entry> data;
  std::string filepath;
  unsigned int hash_function(entry &new_entry);
};

typedef struct {
  unsigned int id;
  double x, y;
  std::string city, street;
  int house;
  HashTable organisation;
} address;

class Addresses : public HashTable {
public:
  Addresses() : data() { data.reserve(10); };
  Addresses(const Addresses &other) : data(other.data){};
  ~Addresses(){};

  void reset();         // удаляет все значения
  unsigned int count(); // количество значений
  virtual void save(std::string path); // сохранить содержимое в файл по пути
  virtual void load(std::string path); // загрузить из файла по пути

  virtual void add(address &new_address); // добавить запись
  virtual void remove(address &new_address); // удалить запись при ее наличии
  virtual bool
  is_not_empty(unsigned int index); // проверить наличие записи по ключу

  // добавить к записи хэш таблицу с организацией
  void add_organisation(address &addr, HashTable &org);

  std::vector<address>::iterator
  find_hash(unsigned int hash); // возвращает запись по хэшу
  int find_address(std::vector<address>::iterator &pos,
                   address &addr); // возвращает запись по адресу
  std::vector<address>::iterator
  nearest_address(double x, double y); // ближайшая точка на карте

private:
  std::vector<address> data;
  unsigned int hash_function(address &new_address);
};

#endif // __CATALOG_H__

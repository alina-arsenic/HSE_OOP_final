#include "catalog.h"

void Addresses::reset() { data.clear(); }

unsigned int Addresses::count() { return data.size(); }

void Addresses::save(std::string path) {
  std::ofstream file(path);
  if (file.is_open()) {
    for (auto i = data.begin(); i < data.end(); i++) {
      file << i->id << " " << i->x << " " << i->y << " " << i->city << " "
           << i->street << " " << i->house;
      if (i->organisation.count() != 0) {
        file << " " << i->organisation.get_path();
      } else {
        file << " -";
      }
      file << "\n";
    }
    file.close();
  } else {
    throw std::invalid_argument("Unable to open file");
  }
}

void Addresses::load(std::string path) {
  std::ifstream file(path);
  if (file.is_open()) {
    this->reset();
    address addr;
    unsigned int hash_id;
    double x, y;
    char city[1024], street[1024];
    int house;
    char s[1024], org[1024];
    file.getline(s, 1024, '\n');
    while (s[0]) {
      sscanf(s, "%u %lf %lf %s %s %d %s", &hash_id, &x, &y, city, street,
             &house, org);
      addr.x = x;
      addr.y = y;
      addr.id = hash_id;
      addr.city = city;
      addr.street = street;
      addr.house = house;
      if (strcmp(org, "-") != 0)
        addr.organisation.load(org);
      data.push_back(addr);
      file.getline(s, 1024, '\n');
    }
    file.close();
  } else {
    throw std::invalid_argument("Unable to open file");
  }
}

void Addresses::add(address &new_address) {
  unsigned int index = hash_function(new_address);
  new_address.id = index;
  data.insert(find_hash(index), new_address);
}

void Addresses::remove(address &new_address) {
  std::vector<address>::iterator pos;
  if (find_address(pos, new_address))
    data.erase(pos);
}

bool Addresses::is_not_empty(unsigned int index) {
  auto i = find_hash(index);
  return i->id == index;
}

std::vector<address>::iterator Addresses::find_hash(unsigned int hash) {
  std::vector<address>::iterator pos;
  if (hash <= data.begin()->id) {
    pos = data.begin();
  } else if (hash > (data.end() - 1)->id) {
    pos = data.end();
  } else if (hash == (data.end() - 1)->id) {
    pos = data.end() - 1;
  } else {
    unsigned int delta = data.size() / 2;
    pos = data.begin() + delta;
    while (!((pos - 1)->id <= hash && pos->id >= hash)) {
      if (delta > 1)
        delta /= 2;
      hash < pos->id ? pos -= delta : pos += delta;
    }
    while ((pos - 1)->id == hash)
      pos -= 1;
  }
  return pos;
}

unsigned int Addresses::hash_function(address &new_address) {
  long unsigned int sum = new_address.x + new_address.y;
  for (int i = 0; i < new_address.city.length(); i++) {
    sum *= new_address.city[i];
  }
  sum /= new_address.house;
  for (int i = 0; i < new_address.street.length(); i++) {
    sum *= new_address.street[i];
  }
  return sum % MAX_HASH;
}

int Addresses::find_address(std::vector<address>::iterator &pos,
                            address &addr) {
  unsigned int index = hash_function(addr);
  auto i = find_hash(index);
  while (i->id == index &&
         !(i->x == addr.x && i->y == addr.y && i->city == addr.city &&
           i->street == addr.street && i->house == addr.house))
    i++;
  if (i->x == addr.x && i->y == addr.y && i->city == addr.city &&
      i->street == addr.street && i->house == addr.house) {
    pos = i;
    return 1;
  }
  return 0;
}

void Addresses::add_organisation(address &addr, HashTable &org) {
  std::vector<address>::iterator pos;
  if (find_address(pos, addr)) {
    pos->organisation = org;
  }
}

std::vector<address>::iterator Addresses::nearest_address(double x, double y) {
  auto i = data.begin();
  auto min_addr = i;
  double min = std::pow(x - i->x, 2) + std::pow(y - i->y, 2);
  double test;
  for (auto i = data.begin() + 1; i < data.end(); i++) {
    test = std::pow(x - i->x, 2) + std::pow(y - i->y, 2);
    if (test < min) {
      min = test;
      min_addr = i;
    }
  }
  return min_addr;
}

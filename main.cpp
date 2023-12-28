#include "catalog.h"
#include <assert.h>
#include <ctime>

int main() {
  std::string products[] = {"chair", "sofa", "table", "lamp",   "closet",
                            "bed",   "desk", "door",  "fridge", "oven"};
  std::string cars[] = {"Ford", "Honda", "Hyundai", "Kia", "Toyota"};
  std::string cities[] = {
      "Moscow", "StPetersburg", "Kazan", "Yekaterinburg", "Tomsk",
      "Rostov", "Samara",       "Omsk",  "Krasnodar",     "Sochi"};
  std::string streets[] = {"KraskayaArmia", "Nagornaya", "Sadovaya",
                           "Solnechnaya", "Kalinina"};

  srand((unsigned)time(NULL));

  // создание таблиц с тремя разными организациями и одной пустой
  // проверка размера таблиц сразу после создания и по мере добавления элементов
  HashTable sup, con, log, empty;
  assert(sup.count() == 0);
  assert(con.count() == 0);
  assert(log.count() == 0);
  assert(empty.count() == 0);
  for (int i = 0; i < 25; i++) {
    entry entry1(0, products[rand() % 10], rand() % 50 + 1);
    sup.add(entry1);
    assert(sup.count() == i + 1);

    entry entry2(0, products[rand() % 10], rand() % 10 + 1);
    con.add(entry2);
    assert(con.count() == i + 1);

    entry entry3(0, cars[rand() % 5], rand() % 50 + 1);
    log.add(entry3);
    assert(log.count() == i + 1);
  }

  // сохоанение таблиц в файлы
  sup.save("txt/suppliers.txt");
  con.save("txt/consumers.txt");
  log.save("txt/logistics.txt");
  empty.save("txt/empty.txt");

  // загрузка таблицы из пустого файла, операция && на пустой таблице
  HashTable test;
  test.load("txt/empty.txt");
  assert(test.count() == 0);
  test = test && sup;
  assert(test.count() == 0);

  // загрузка таблицы из непустого файла, проверка равенств сохраненной и
  // загруженной таблиц
  test.load("txt/suppliers.txt");
  assert(test == sup);

  // операция any && any == any
  test = test && sup;
  assert(test == sup);

  // добавление новой записи, проверка нарушения равенства
  unsigned int count = test.count();
  entry entry_test(9148, "dishwaser", 29);
  test.add(entry_test);
  assert(test.count() == count + 1);
  assert(test != sup);

  // удаление несуществующей записи
  entry_test = {0, "dishwaser", 34};
  test.remove(entry_test);
  assert(test.count() == count + 1);

  // удаление существующей записи
  entry_test = {9148, "dishwaser", 29};
  test.remove(entry_test);
  assert(test.count() == count);

  // создание каталога с адресами, проверка размера по мере добавления элементов
  // сохранение в файл
  Addresses addr;
  assert(addr.count() == 0);
  for (int i = 0; i < 100; i++) {
    address a;
    a.x = (rand() % 100000) / (pow(10, rand() % 3));
    a.y = (rand() % 100000) / (pow(10, rand() % 3));
    a.city = cities[rand() % 10];
    a.street = streets[rand() % 5];
    a.house = rand() % 30 + 1;
    addr.add(a);
    assert(addr.count() == i + 1);
  }
  addr.save("txt/addresses.txt");

  // добавление таблицы с организацией к адресу
  // добавление записей к таблице и сверка количества записей
  count = addr.count();
  address a;
  a.x = 33.333;
  a.y = 5.443;
  a.city = "Samara";
  a.street = "Volskaya";
  a.house = 11;
  addr.add(a);
  addr.add_organisation(a, sup);
  assert(addr.count() == count + 1);

  a.x = 332.322;
  a.y = 5344.443;
  a.city = "Moscow";
  a.street = "Novoyasenevskaya";
  a.house = 2;
  addr.add(a);
  addr.add_organisation(a, con);
  assert(addr.count() == count + 2);

  a.x = 349.343;
  a.y = 223.13;
  a.city = "Surgut";
  a.street = "Oktyabrskaya";
  a.house = 21;
  addr.add(a);
  addr.add_organisation(a, log);
  assert(addr.count() == count + 3);

  // удаления существующией и несуществующей записи из таблицы и сверка
  // количества записей
  a.x = 3449.343;
  a.y = 2423.13;
  a.city = "NewYork";
  a.street = "1stAvenue";
  a.house = 21;
  addr.remove(a);
  assert(addr.count() == count + 3);
  addr.add(a);
  assert(addr.count() == count + 4);
  addr.remove(a);
  assert(addr.count() == count + 3);

  // нахождение ближайшей точки
  double x, y;
  x = 234.44;
  y = 235.3;
  auto result = addr.nearest_address(x, y);
  std::cout << "\nThe nearest addres to (" << x << "; " << y << ") is:\n"
            << result->id << " " << result->city << " " << result->house << " "
            << "(" << result->x << "; " << result->y << ")\n";

  x = 0;
  y = 0;
  result = addr.nearest_address(x, y);
  std::cout << "\nThe nearest addres to (" << x << "; " << y << ") is:\n"
            << result->id << " " << result->city << " " << result->house << " "
            << "(" << result->x << "; " << result->y << ")\n";

  x = 99999.6;
  y = 88888.345;
  result = addr.nearest_address(x, y);
  std::cout << "\nThe nearest address to (" << x << "; " << y << ") is:\n"
            << result->id << " " << result->city << " " << result->street << " "
            << result->house << " "
            << "(" << result->x << "; " << result->y << ")\n";

  addr.save("txt/addresses.txt");

  std::cout << "\nAll is fine.\n";
}

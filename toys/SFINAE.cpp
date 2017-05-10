#include <cstdio>
#include <type_traits>

struct ICounter {
  void Increase() { 
  }

};

namespace { //type failure

template <typename T>
void inc_counter(T &count_obj, typename std::enable_if<!std::is_integral<T>::value>::type* = nullptr) {
  std::printf("cnt increase1\n");
  count_obj.Increase();
}

template <typename T>
void inc_counter(T &fuck, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr) {
  std::printf("int increase1\n");
  ++fuck;
}

template <typename T>
void inc_counter2(T &count_obj) {
  std::printf("template counter2 increase\n");
}

//cant't bind non-const IConter
void inc_counter2(const ICounter &cnt) {
  std::printf("const IConter increase2\n");
}

}

namespace {//expression failure

template <typename T>
void inc_counter3(T &int_counter, typename std::decay<decltype(++int_counter)>::type* = nullptr) {
  std::printf("inc counter3\n");
  ++int_counter;
}

template <typename T>
void inc_counter3(T &obj_counter, std::decay_t<decltype(obj_counter.Increase())>* = nullptr) {
  std::printf("obj counter3\n");
  obj_counter.Increase();
}

}


namespace { //universe failure

template <typename T>
void foo(T &&val, 
         typename std::enable_if<
         std::is_same<std::decay_t<T>, float>::value
         >::type* = nullptr) {
  std::printf("universe reference on foo\n");
}


}

int main() {
  ICounter obj_cnt;
  int int_cnt;
  const ICounter const_obj_cnt;

  inc_counter(int_cnt);
  inc_counter(obj_cnt);

  inc_counter2(obj_cnt);
  inc_counter2(int_cnt);
  inc_counter2(const_obj_cnt);

  inc_counter3(int_cnt);
  inc_counter3(obj_cnt);

  float fuck;
  foo(fuck);
  foo(1.5f);

  return 0;
}

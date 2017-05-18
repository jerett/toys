
#include <type_traits>
#include <string>
#include <gtest/gtest.h>


//tet method
namespace {

template <typename T>
int Log(T &&data) {
  return Log_impl(std::forward<T>(data), std::is_integral<typename std::remove_reference<T>::type>());
}

template<typename T>
int Log_impl(T &&data, std::false_type) {
  return 1;
}

int Log_impl(int data, std::true_type) {
  return 2;
}

TEST(tag_dispatcher, Method) {
  ASSERT_EQ(Log(3), 2);
  int a = -1;
  ASSERT_EQ(Log(a), 2);
  std::string fuck("aaa");
  ASSERT_EQ(Log(fuck), 1);
}

}

//test constrcutor
namespace {

struct LogClass {
  template <typename T>
  LogClass(T &&data) : LogClass(std::forward<T>(data), std::is_integral<typename std::remove_reference<T>::type>()) {

  }

  template<typename T>
  LogClass(T &&data, std::false_type) {
    type_ = 1;
  }

  LogClass(int data, std::true_type) {
    type_ = 2;
  }

  LogClass(LogClass&) {
    type_ = 3;
  }

  LogClass(const LogClass&) {
    type_ = 4;
  }

  LogClass(LogClass&&) {
    type_ = 5;
  }

  int type_ = -1;
};


TEST(tag_dispatcher, Class) {
  ASSERT_EQ(LogClass(3).type_, 2);
  ASSERT_EQ(LogClass("aaa").type_, 1);

  int data = -1;
  ASSERT_EQ(LogClass(data).type_, 2);

  LogClass l1(data);
  ASSERT_EQ(LogClass(l1).type_, 3);
  
  const LogClass l2(data);
  ASSERT_EQ(LogClass(l2).type_, 4);

  ASSERT_EQ(LogClass(std::move(l1)).type_, 5);
}

}


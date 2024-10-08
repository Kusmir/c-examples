#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

namespace detail {
template <typename E>
constexpr decltype(auto) add_element(std::tuple<> &&tpl[[maybe_unused]], E &&element) {
  return std::tuple<E &&>(std::forward<E>(element));
}

template <typename E, typename... T>
constexpr decltype(auto) add_element(std::tuple<T...> &&tpl, E &&element) {
  return std::tuple_cat(std::forward<std::tuple<T...>>(tpl),
                        std::tuple<E &&>(std::forward<E>(element)));
}

template <typename T> struct is_int_seq : std::false_type {};

template <typename IntType, IntType... Ints>
struct is_int_seq<std::integer_sequence<IntType, Ints...>> : std::true_type {};

template <typename... T> struct contains_int_seq : std::false_type {};

template <typename T, typename... Args>
struct contains_int_seq<T, Args...>
    : std::integral_constant<bool, (is_int_seq<T>::value || ... ||
                                    is_int_seq<Args>::value)> {};


// invoke_intseq_ override to really invoke function and save result.
// It is chosen when all arguments have already been parsed
// into Pref tuple.
template <typename ResultType, typename Func, typename Pref>
constexpr decltype(auto) invoke_intseq_(std::vector<ResultType> &results,
                                        Func &&f, Pref &&pref) {
  if constexpr (std::is_same_v<
                    void, std::invoke_result_t<decltype(std::apply<Func, Pref>),
                                               Func &&, Pref &&>>) {
    std::apply(std::forward<Func>(f), std::forward<Pref>(pref));
    return;
  } else {
    decltype(auto) ret =
        std::apply(std::forward<Func>(f), std::forward<Pref>(pref));
    if constexpr (std::is_reference_v<decltype(ret)>)
      results.push_back(std::ref(ret));
    else
      results.push_back(ret);
  }
}

// Forward declarations.
template <typename ResultType, typename Func, typename Pref, typename... Rest,
          typename IntType, IntType... Ints>
constexpr decltype(auto)
invoke_intseq_(std::vector<ResultType> &results, Func &&f, Pref &&pref,
               std::integer_sequence<IntType, Ints...> &&, Rest &&...rest);

template <typename ResultType, typename Func, typename Pref, typename First,
          typename... Rest>
constexpr decltype(auto) invoke_intseq_(std::vector<ResultType> &results,
                                        Func &&f, Pref &&pref, First &&first,
                                        Rest &&...rest);


// Invoke method overriden to iterate over integer_sequence.
template <typename ResultType, typename Func, typename Pref, typename... Rest,
          typename IntType, IntType... Ints>
constexpr decltype(auto)
invoke_intseq_(std::vector<ResultType> &results, Func &&f, Pref &&pref,
               std::integer_sequence<IntType, Ints...> &&, Rest &&...rest) {

  (invoke_intseq_(results, std::forward<Func>(f), std::forward<Pref>(pref),
                  std::integral_constant<IntType, Ints>(),
                  std::forward<Rest>(rest)...),
   ...);
}

// Generic invoke method.
// Pref is a tuple, used to store left portion of arguments during iteration.
template <typename ResultType, typename Func, typename Pref, typename First,
          typename... Rest>
constexpr decltype(auto) invoke_intseq_(std::vector<ResultType> &results,
                                        Func &&f, Pref &&pref, First &&first,
                                        Rest &&...rest) {

  static_assert(!is_int_seq<First>::value);
  invoke_intseq_(
      results, std::forward<Func>(f),
      std::forward<decltype(add_element(std::forward<Pref>(pref),
                                        std::forward<First>(first)))>(
          add_element(std::forward<Pref>(pref), std::forward<First>(first))),
      std::forward<Rest>(rest)...);
}

// Real function calls will have std::integral_constant
// instead of std::integer_sequence. That's why we parse
// arguments to assure we detect correct type.
template <typename T> struct parse_arg {
  using type = T;
};
template <typename IntType, IntType... Ints>
struct parse_arg<std::integer_sequence<IntType, Ints...>> {
  using type = std::integral_constant<IntType, 0>;
};

} // namespace detail

template <typename Func, typename... Args>
constexpr decltype(auto) invoke_intseq(Func &&f, Args &&...args) {

  using ret_type =
      std::invoke_result_t<Func, typename detail::parse_arg<Args>::type...>;

  // Mock result to int if  f does not return anything.
  // In that case, result vector will be empty anyway.
  using result_type = typename std::conditional<
      std::is_same_v<ret_type, void>, int,
      typename std::conditional<
          std::is_reference_v<ret_type>,
          std::reference_wrapper<
              typename std::remove_reference<ret_type>::type>,
          ret_type>::type>::type;

  std::vector<result_type> result;
  detail::invoke_intseq_(result, std::forward<Func>(f),
                         std::forward<decltype(std::tuple<>())>(std::tuple<>()),
                         std::forward<Args>(args)...);

  if constexpr (std::is_same_v<ret_type, void>)
    return;
  else if constexpr (detail::contains_int_seq<Args...>::value) {
    return result;
  } else {
    auto ret = result[0];
    return static_cast<
      typename std::remove_reference<decltype(ret)>::type>(ret);
  }
}

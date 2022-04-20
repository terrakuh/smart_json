# Smart JSON

Header only automatic JSON encoding and decoding with [Boost.Describe]. Currently only [Boost.JSON](https://www.boost.org/doc/libs/1_78_0/libs/json/doc/html/index.html) is supported but other JSON libraries such as [nlohmann/json](https://github.com/nlohmann/json) could be integrated (in future). The following requirements need to be fulfilled:

- C++ 17
- CMake >= 3.14
- Boost >= 1.78

## Example

For the complete example look [here](./examples/showcase.cpp).

```cpp
BOOST_DEFINE_ENUM_CLASS(Log_level, off, debug, info, error);

struct Config
{
  std::string log_path;
  Log_level log_level;
  std::map<std::string, std::string> additional_headers;
  std::optional<std::string> save_path;

  struct Nested
  {
    int value = 0;
  };
  std::array<Nested, 3> fixed_array;
  std::vector<Nested> array;
};
BOOST_DESCRIBE_STRUCT(Config, (), (log_path, log_level, additional_headers, save_path, fixed_array, array));
BOOST_DESCRIBE_STRUCT(Config::Nested, (), (value));

int main()
{
  Config config{
    "v0.0.0",
    Log_level::info,
    { { "User-Agent", "smart_json" } },
    std::nullopt,
  };
  config.fixed_array[0].value = 1;
  config.fixed_array[1].value = 2;
  config.fixed_array[2].value = 3;
  config.array.push_back({ 61 });

  auto obj = smart_json::encode(config);
  std::cout << boost::json::serialize(obj) << "\n";

  config = smart_json::decode<Config>(obj);
}
```

And the output (formatted) is:

```json
{
  "logPath": "v0.0.0",
  "logLevel": "info",
  "additionalHeaders": {
    "User-Agent": "smart_json"
  },
  "savePath": null,
  "fixedArray": [
    {
      "value": 1
    },
    {
      "value": 2
    },
    {
      "value": 3
    }
  ],
  "array": [
    {
      "value": 61
    }
  ]
}
```

Note that the member names in C++ are in the typical snake-case format and will be automagically converted to the JSON typical camel-case. This is guaranteed to happen at compile time and inflicts no additional cost at runtime. If you want to implement a custom transformer, have a look at the [`smart_json::Camel_case_transformer`](./include/smart_json/camel_case_transformer.hpp) or the [`Upper_case_transformer`](./examples/playground.cpp) in the playgrounds example. You can also use `smart_json::No_transformer` if no conversion is desired.

## Supported types

- Any arithmetic type (`bool`, integrals, floating points)
- `std::string`
- `std::optional`
- Any `enum` / `struct` / `class` described with [Boost.Describe]
- Any container (eg. `std::vector`, `std::array`, etc.) that statisfies the following conditions will be encoded as an array:
  - return an [Iterator](https://en.cppreference.com/w/cpp/named_req/Iterator) for `begin()` and `end()`
  - return unsigned integral for `size()`
  - is not `std::string`
  - for decoding any of (order of precedence):
    - `push_back(value_type{})` (for dynamically sized array)
    - `push_front(value_type{})` (for dynamically sized array)
    - `operator[](value_type{})` (for fixed size array; bounds checking will be performed with `size()`)
- Any associative container (eg. `std::map`, etc.) that statisfies the following conditions will be encoded as an object:
  - fulfills all the constraints of the container above except for the decoding
  - has a type `key_type` which is equal to `std::string`
  - has a `mapped_type`
  - has a `value_type` which can be destructured into `[key, value]`
  - has an `insert(value_type{key, value})` method

## Building

There is no compiling required. However, there are some CMake options:

| Option                    | Default | Description                                                 |
| ------------------------- | ------- | ----------------------------------------------------------- |
| SMART_JSON_BUILD_EXAMPLES | `OFF`   | Builds the examples in `examples/`                          |
| SMART_JSON_FETCH_CONTENT  | `ON`    | Automagically fetches the missing and required dependencies |

[Boost.Describe]: https://www.boost.org/doc/libs/1_78_0/libs/describe/doc/html/describe.html

## Usage

```cmake
find_package(smart_json 0.3 REQUIRED)
target_link_libraries(myproject PRIVATE smart_json::smart_json)
```

And if you want to automagically fetch Boost 1.78 just add this before `find_package(smart_json)`:

```cmake
include(FetchContent)
FetchContent_Declare(
  Boost
  URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
  URL_HASH SHA256=94ced8b72956591c4775ae2207a9763d3600b30d9d7446562c552f0a14a63be7
)
FetchContent_MakeAvailable(Boost)
FetchContent_GetProperties(Boost SOURCE_DIR Boost_INCLUDE_DIR)
find_package(Boost 1.78 REQUIRED)
```

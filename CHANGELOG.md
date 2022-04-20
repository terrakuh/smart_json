# Change Log

## [Unreleased]

## [v0.3] - 2022-04-20
### Added
- Custom name transformer at compile time
- `Camel_case_transformer` to convert snake-case to JSON desired camel-case
- `No_transformer` for plain names

## [v0.2.1] - 2022-04-16
### Fixed
- Infinitie recursion for `std::optional` decoding
- Primitive decoding when type does not match exactly

## [v0.2] - 2022-04-16
### Changed
- Return values from `encode()` and `decode()` rather a reference

### Removed
- `const char*` no longer supported as string

[Unreleased]: https://github.com/terrakuh/smart_json/compare/v0.3..dev
[v0.3]: https://github.com/terrakuh/smart_json/compare/v0.2.1..v0.3
[v0.2.1]: https://github.com/terrakuh/smart_json/compare/v0.2..v0.2.1
[v0.2]: https://github.com/terrakuh/smart_json/compare/v0.1..v0.2

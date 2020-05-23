#include "JsonGetEx.h"
#include <optional>
#include <vector>
#include "../String/Base64.h"

template <typename Encoding, typename Allocator>
std::vector<uint8_t> JsonGetBlobEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    auto pStr = JsonGetStringEx(value, pName, true);
    return Base64Decode(pStr);
}

template <typename Encoding, typename Allocator>
std::optional<std::vector<uint8_t>> JsonGetOptionalBlobEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<std::vector<uint8_t>> data;
    auto pStr = JsonGetStringEx(value, pName);
    if (pStr)
    {
        data = Base64Decode(pStr);
    }
    return data;
}

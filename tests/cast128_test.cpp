#include "../CAST128_Example/cast128.h"

#include <cstdint>
#include <iostream>

namespace {

bool expectEqual(const char* name, std::uint32_t actual, std::uint32_t expected)
{
    if (actual == expected)
        return true;

    std::cerr << name << ": expected 0x" << std::hex << expected
              << ", got 0x" << actual << '\n';
    return false;
}

} // namespace

int main()
{
    CAST128 cipher;

    // RFC 2144, Appendix B.1: 128-bit key known-answer test.
    static const CAST128::Key key = {
        0x01234567, 0x12345678, 0x23456789, 0x3456789a
    };
    CAST128::Message block = { 0x01234567, 0x89abcdef };
    cipher.encrypt(key, block);

    bool ok = true;
    ok &= expectEqual("ciphertext left", block[0], 0x238b4fe5);
    ok &= expectEqual("ciphertext right", block[1], 0x847e44b2);

    cipher.decrypt(key, block);
    ok &= expectEqual("plaintext left", block[0], 0x01234567);
    ok &= expectEqual("plaintext right", block[1], 0x89abcdef);

    CAST128::Message zero = { 0, 0 };
    cipher.encrypt(key, zero);
    cipher.decrypt(key, zero);
    ok &= expectEqual("zero round-trip left", zero[0], 0);
    ok &= expectEqual("zero round-trip right", zero[1], 0);

    return ok ? 0 : 1;
}

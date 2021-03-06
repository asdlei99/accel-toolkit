#include "accelpp/hash/SHA256.hpp"

#define ConstAsciiString(s) s, sizeof(s) - 1

namespace accel::test {

	bool SHA256_CorrectnessTest() {

        // Generated by Python 3.6.3
        // SHA256("") = e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
        // SHA256("a") = ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb
        // SHA256("abc") = ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
        // SHA256("message digest") = f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650
        // SHA256("abcdefghijklmnopqrstuvwxyz") = 71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73
        // SHA256("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = db4bfcbd4da0cd85a60c3c37d3fbd8805c77f15fc6b1fdfe614ee0a7c8fdb4c0
        // SHA256("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = f371bc4a311f2b009eef952dd83ca80e2b60026c8e935592d0f9c308453c813e

        accelpp::hash::SHA256::HashValue sha256_correct_value[7] = {
            { 0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 },
            { 0xca, 0x97, 0x81, 0x12, 0xca, 0x1b, 0xbd, 0xca, 0xfa, 0xc2, 0x31, 0xb3, 0x9a, 0x23, 0xdc, 0x4d, 0xa7, 0x86, 0xef, 0xf8, 0x14, 0x7c, 0x4e, 0x72, 0xb9, 0x80, 0x77, 0x85, 0xaf, 0xee, 0x48, 0xbb },
            { 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad },
            { 0xf7, 0x84, 0x6f, 0x55, 0xcf, 0x23, 0xe1, 0x4e, 0xeb, 0xea, 0xb5, 0xb4, 0xe1, 0x55, 0x0c, 0xad, 0x5b, 0x50, 0x9e, 0x33, 0x48, 0xfb, 0xc4, 0xef, 0xa3, 0xa1, 0x41, 0x3d, 0x39, 0x3c, 0xb6, 0x50 },
            { 0x71, 0xc4, 0x80, 0xdf, 0x93, 0xd6, 0xae, 0x2f, 0x1e, 0xfa, 0xd1, 0x44, 0x7c, 0x66, 0xc9, 0x52, 0x5e, 0x31, 0x62, 0x18, 0xcf, 0x51, 0xfc, 0x8d, 0x9e, 0xd8, 0x32, 0xf2, 0xda, 0xf1, 0x8b, 0x73 },
            { 0xdb, 0x4b, 0xfc, 0xbd, 0x4d, 0xa0, 0xcd, 0x85, 0xa6, 0x0c, 0x3c, 0x37, 0xd3, 0xfb, 0xd8, 0x80, 0x5c, 0x77, 0xf1, 0x5f, 0xc6, 0xb1, 0xfd, 0xfe, 0x61, 0x4e, 0xe0, 0xa7, 0xc8, 0xfd, 0xb4, 0xc0 },
            { 0xf3, 0x71, 0xbc, 0x4a, 0x31, 0x1f, 0x2b, 0x00, 0x9e, 0xef, 0x95, 0x2d, 0xd8, 0x3c, 0xa8, 0x0e, 0x2b, 0x60, 0x02, 0x6c, 0x8e, 0x93, 0x55, 0x92, 0xd0, 0xf9, 0xc3, 0x08, 0x45, 0x3c, 0x81, 0x3e }
        };

        accelpp::hash::SHA256 sha256;
        if (sha256.Digest() != sha256_correct_value[0])
            return false;

        sha256.Update("a", 1);
        if (sha256.Digest() != sha256_correct_value[1])
            return false;

        sha256.Update("bc", 2);
        if (sha256.Digest() != sha256_correct_value[2])
            return false;

        sha256.Reset();
        sha256.Update(ConstAsciiString("message digest"));
        if (sha256.Digest() != sha256_correct_value[3])
            return false;

        sha256.Reset();
        sha256.Update(ConstAsciiString("abcdefghijklmnopqrstuvwxyz"));
        if (sha256.Digest() != sha256_correct_value[4])
            return false;

        sha256.Reset();
        sha256.Update(ConstAsciiString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
        if (sha256.Digest() != sha256_correct_value[5])
            return false;

        sha256.Reset();
        sha256.Update(ConstAsciiString("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
        if (sha256.Digest() != sha256_correct_value[6])
            return false;

        const char CustomString[] = "The quick brown fox jumps over the lazy dog";
        accelpp::hash::SHA256::HashValue sha256_CustomString = { 0xd7, 0xa8, 0xfb, 0xb3, 0x07, 0xd7, 0x80, 0x94, 0x69, 0xca, 0x9a, 0xbc, 0xb0, 0x08, 0x2e, 0x4f,
                                                               0x8d, 0x56, 0x51, 0xe4, 0x6d, 0x3c, 0xdb, 0x76, 0x2d, 0x02, 0xd0, 0xbf, 0x37, 0xc9, 0xe5, 0x92 };
        return accelpp::hash::SHA256(ConstAsciiString(CustomString)).Digest() == sha256_CustomString;
	}

}


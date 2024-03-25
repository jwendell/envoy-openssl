#include <gtest/gtest.h>
#include <openssl/evp.h>
#include <openssl/bytestring.h>


TEST(EVPTest, EVP1) {
  EVP_PKEY_free(nullptr);
  EVP_PKEY_free(EVP_PKEY_new());
}

TEST(EVPTest, EVP2) {
  bssl::UniquePtr<EVP_PKEY> k1(EVP_PKEY_new());
  EXPECT_EQ(EVP_PKEY_NONE, EVP_PKEY_id(k1.get()));
}

TEST(EVPTest, EVP3) {
  static const uint8_t keystr[] = {0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xcd, 0x00, 0x81, 0xea, 0x7b, 0x2a, 0xe1, 0xea, 0x06, 0xd5, 0x9f, 0x7c, 0x73, 0xd9, 0xff, 0xb9, 0x4a, 0x09, 0x61, 0x5c, 0x2e, 0x4b, 0xa7, 0xc6, 0x36, 0xce, 0xf0, 0x8d, 0xd3, 0x53, 0x3e, 0xc3, 0x18, 0x55, 0x25, 0xb0, 0x15, 0xc7, 0x69, 0xb9, 0x9a, 0x77, 0xd6, 0x72, 0x5b, 0xf9, 0xc3, 0x53, 0x2a, 0x9b, 0x6e, 0x5f, 0x66, 0x27, 0xd5, 0xfb, 0x85, 0x16, 0x07, 0x68, 0xd3, 0xdd, 0xa9, 0xcb, 0xd3, 0x59, 0x74, 0x51, 0x17, 0x17, 0xdc, 0x3d, 0x30, 0x9d, 0x2f, 0xc4, 0x7e, 0xe4, 0x1f, 0x97, 0xe3, 0x2a, 0xdb, 0x7f, 0x9d, 0xd8, 0x64, 0xa1, 0xc4, 0x76, 0x7a, 0x66, 0x6e, 0xcd, 0x71, 0xbc, 0x1a, 0xac, 0xf5, 0xe7, 0x51, 0x7f, 0x4b, 0x38, 0x59, 0x4f, 0xea, 0x9b, 0x05, 0xe4, 0x2d, 0x5a, 0xda, 0x99, 0x12, 0x00, 0x80, 0x13, 0xe4, 0x53, 0x16, 0xa4, 0xd9, 0xbb, 0x8e, 0xd0, 0x86, 0xb8, 0x8d, 0x28, 0x75, 0x8b, 0xac, 0xaf, 0x92, 0x2d, 0x46, 0xa8, 0x68, 0xb4, 0x85, 0xd2, 0x39, 0xc9, 0xba, 0xeb, 0x0e, 0x2b, 0x64, 0x59, 0x27, 0x10, 0xf4, 0x2b, 0x2d, 0x1e, 0xa0, 0xa4, 0xb4, 0x80, 0x2c, 0x0b, 0xec, 0xab, 0x32, 0x8f, 0x8a, 0x68, 0xb0, 0x07, 0x3b, 0xdb, 0x54, 0x6f, 0xee, 0xa9, 0x80, 0x9d, 0x28, 0x49, 0x91, 0x2b, 0x39, 0x0c, 0x15, 0x32, 0xbc, 0x7e, 0x29, 0xc7, 0x65, 0x8f, 0x81, 0x75, 0xfa, 0xe4, 0x6f, 0x34, 0x33, 0x2f, 0xf8, 0x7b, 0xca, 0xb3, 0xe4, 0x06, 0x49, 0xb9, 0x85, 0x77, 0x86, 0x9d, 0xa0, 0xea, 0x71, 0x83, 0x53, 0xf0, 0x72, 0x27, 0x54, 0x88, 0x69, 0x13, 0x64, 0x87, 0x60, 0xd1, 0x22, 0xbe, 0x67, 0x6e, 0x0f, 0xc4, 0x83, 0xdd, 0x20, 0xff, 0xc3, 0x1b, 0xda, 0x96, 0xa3, 0x19, 0x66, 0xc9, 0xaa, 0x2e, 0x75, 0xad, 0x03, 0xde, 0x47, 0xe1, 0xc4, 0x4f, 0x02, 0x03, 0x01, 0x00, 0x01};

  CBS cbs;
  CBS_init(&cbs, keystr, sizeof(keystr));
  bssl::UniquePtr<EVP_PKEY> k1(EVP_parse_public_key(&cbs));
  EXPECT_TRUE(k1.get());
  EXPECT_EQ(EVP_PKEY_RSA, EVP_PKEY_id(k1.get()));

  RSA *rsa1 = EVP_PKEY_get0_RSA(k1.get());
  EXPECT_TRUE(rsa1);

  RSA *rsa2 = EVP_PKEY_get1_RSA(k1.get());
  EXPECT_TRUE(rsa2);

  // We have ownership of rsa2 at this point. However, we pass ownership into
  // the EVP_PKEY_assign_RSA() call so that when the EVP_PKEY (k2) is freed,
  // the RSA (rsa2) is also freed.

  bssl::UniquePtr<EVP_PKEY> k2(EVP_PKEY_new());
  EXPECT_EQ(1, EVP_PKEY_assign_RSA(k2.get(), rsa2));
  EXPECT_EQ(EVP_PKEY_RSA, EVP_PKEY_id(k2.get()));
}

TEST(EVPTest, EVP4) {
  static const uint8_t keystr[] = {0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x2c, 0x15, 0x0f, 0x42, 0x9c, 0xe7, 0x0f, 0x21, 0x6c, 0x25, 0x2c, 0xf5, 0xe0, 0x62, 0xce, 0x1f, 0x63, 0x9c, 0xd5, 0xd1, 0x65, 0xc7, 0xf8, 0x94, 0x24, 0x07, 0x2c, 0x27, 0x19, 0x7d, 0x78, 0xb3, 0x3b, 0x92, 0x0e, 0x95, 0xcd, 0xb6, 0x64, 0xe9, 0x90, 0xdc, 0xf0, 0xcf, 0xea, 0x0d, 0x94, 0xe2, 0xa8, 0xe6, 0xaf, 0x9d, 0x0e, 0x58, 0x05, 0x6e, 0x65, 0x31, 0x04, 0x92, 0x5b, 0x9f, 0xe6, 0xc9};;

  CBS cbs;
  CBS_init(&cbs, keystr, sizeof(keystr));
  bssl::UniquePtr<EVP_PKEY> k1(EVP_parse_public_key(&cbs));
  EXPECT_TRUE(k1.get());
  EXPECT_EQ(EVP_PKEY_EC, EVP_PKEY_id(k1.get()));

  EC_KEY *ec1 = EVP_PKEY_get0_EC_KEY(k1.get());
  EXPECT_TRUE(ec1);

  EC_KEY *ec2 = EVP_PKEY_get1_EC_KEY(k1.get());
  EXPECT_TRUE(ec2);

  // We have ownership of ec2 at this point. However, we pass ownership into
  // the EVP_PKEY_assign_EC_KEY() call so that when the EVP_PKEY (k2) is freed,
  // the EC_KEY (ec2) is also freed.

  bssl::UniquePtr<EVP_PKEY> k2(EVP_PKEY_new());
  EXPECT_EQ(1, EVP_PKEY_assign_EC_KEY(k2.get(), ec2));
  EXPECT_EQ(EVP_PKEY_EC, EVP_PKEY_id(k2.get()));
}

TEST(EVPTest, test_EVP_PKEY_get_raw_public_key_RSA) {
  static const uint8_t keystr[] = {0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xcd, 0x00, 0x81, 0xea, 0x7b, 0x2a, 0xe1, 0xea, 0x06, 0xd5, 0x9f, 0x7c, 0x73, 0xd9, 0xff, 0xb9, 0x4a, 0x09, 0x61, 0x5c, 0x2e, 0x4b, 0xa7, 0xc6, 0x36, 0xce, 0xf0, 0x8d, 0xd3, 0x53, 0x3e, 0xc3, 0x18, 0x55, 0x25, 0xb0, 0x15, 0xc7, 0x69, 0xb9, 0x9a, 0x77, 0xd6, 0x72, 0x5b, 0xf9, 0xc3, 0x53, 0x2a, 0x9b, 0x6e, 0x5f, 0x66, 0x27, 0xd5, 0xfb, 0x85, 0x16, 0x07, 0x68, 0xd3, 0xdd, 0xa9, 0xcb, 0xd3, 0x59, 0x74, 0x51, 0x17, 0x17, 0xdc, 0x3d, 0x30, 0x9d, 0x2f, 0xc4, 0x7e, 0xe4, 0x1f, 0x97, 0xe3, 0x2a, 0xdb, 0x7f, 0x9d, 0xd8, 0x64, 0xa1, 0xc4, 0x76, 0x7a, 0x66, 0x6e, 0xcd, 0x71, 0xbc, 0x1a, 0xac, 0xf5, 0xe7, 0x51, 0x7f, 0x4b, 0x38, 0x59, 0x4f, 0xea, 0x9b, 0x05, 0xe4, 0x2d, 0x5a, 0xda, 0x99, 0x12, 0x00, 0x80, 0x13, 0xe4, 0x53, 0x16, 0xa4, 0xd9, 0xbb, 0x8e, 0xd0, 0x86, 0xb8, 0x8d, 0x28, 0x75, 0x8b, 0xac, 0xaf, 0x92, 0x2d, 0x46, 0xa8, 0x68, 0xb4, 0x85, 0xd2, 0x39, 0xc9, 0xba, 0xeb, 0x0e, 0x2b, 0x64, 0x59, 0x27, 0x10, 0xf4, 0x2b, 0x2d, 0x1e, 0xa0, 0xa4, 0xb4, 0x80, 0x2c, 0x0b, 0xec, 0xab, 0x32, 0x8f, 0x8a, 0x68, 0xb0, 0x07, 0x3b, 0xdb, 0x54, 0x6f, 0xee, 0xa9, 0x80, 0x9d, 0x28, 0x49, 0x91, 0x2b, 0x39, 0x0c, 0x15, 0x32, 0xbc, 0x7e, 0x29, 0xc7, 0x65, 0x8f, 0x81, 0x75, 0xfa, 0xe4, 0x6f, 0x34, 0x33, 0x2f, 0xf8, 0x7b, 0xca, 0xb3, 0xe4, 0x06, 0x49, 0xb9, 0x85, 0x77, 0x86, 0x9d, 0xa0, 0xea, 0x71, 0x83, 0x53, 0xf0, 0x72, 0x27, 0x54, 0x88, 0x69, 0x13, 0x64, 0x87, 0x60, 0xd1, 0x22, 0xbe, 0x67, 0x6e, 0x0f, 0xc4, 0x83, 0xdd, 0x20, 0xff, 0xc3, 0x1b, 0xda, 0x96, 0xa3, 0x19, 0x66, 0xc9, 0xaa, 0x2e, 0x75, 0xad, 0x03, 0xde, 0x47, 0xe1, 0xc4, 0x4f, 0x02, 0x03, 0x01, 0x00, 0x01};

  CBS cbs;
  CBS_init(&cbs, keystr, sizeof(keystr));
  bssl::UniquePtr<EVP_PKEY> pkey {EVP_parse_public_key(&cbs)};
  EXPECT_TRUE(pkey.get());
  EXPECT_EQ(EVP_PKEY_RSA, EVP_PKEY_id(pkey.get()));

  size_t len = 999;
  EXPECT_FALSE(EVP_PKEY_get_raw_public_key(pkey.get(), nullptr, &len));
  ASSERT_EQ(999, len);
}

TEST(EVPTest, test_EVP_PKEY_get_raw_public_key_EC) {
  static const uint8_t keystr[] = {0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x2c, 0x15, 0x0f, 0x42, 0x9c, 0xe7, 0x0f, 0x21, 0x6c, 0x25, 0x2c, 0xf5, 0xe0, 0x62, 0xce, 0x1f, 0x63, 0x9c, 0xd5, 0xd1, 0x65, 0xc7, 0xf8, 0x94, 0x24, 0x07, 0x2c, 0x27, 0x19, 0x7d, 0x78, 0xb3, 0x3b, 0x92, 0x0e, 0x95, 0xcd, 0xb6, 0x64, 0xe9, 0x90, 0xdc, 0xf0, 0xcf, 0xea, 0x0d, 0x94, 0xe2, 0xa8, 0xe6, 0xaf, 0x9d, 0x0e, 0x58, 0x05, 0x6e, 0x65, 0x31, 0x04, 0x92, 0x5b, 0x9f, 0xe6, 0xc9};

  CBS cbs;
  CBS_init(&cbs, keystr, sizeof(keystr));
  bssl::UniquePtr<EVP_PKEY> pkey {EVP_parse_public_key(&cbs)};
  EXPECT_TRUE(pkey.get());
  EXPECT_EQ(EVP_PKEY_EC, EVP_PKEY_id(pkey.get()));

  size_t len = 999;
  EXPECT_FALSE(EVP_PKEY_get_raw_public_key(pkey.get(), nullptr, &len));
  ASSERT_EQ(999, len);
}

TEST(EVPTest, test_EVP_PKEY_get_raw_public_key_X25519) {
  static const uint8_t keystr[] = {0x30, 0x2a, 0x30, 0x05, 0x06, 0x03, 0x2b, 0x65, 0x6e, 0x03, 0x21, 0x00, 0xe6, 0xdb, 0x68, 0x67, 0x58, 0x30, 0x30, 0xdb, 0x35, 0x94, 0xc1, 0xa4, 0x24, 0xb1, 0x5f, 0x7c, 0x72, 0x66, 0x24, 0xec, 0x26, 0xb3, 0x35, 0x3b, 0x10, 0xa9, 0x03, 0xa6, 0xd0, 0xab, 0x1c, 0x4c};

  CBS cbs;
  CBS_init(&cbs, keystr, sizeof(keystr));
  bssl::UniquePtr<EVP_PKEY> pkey {EVP_parse_public_key(&cbs)};
  EXPECT_TRUE(pkey.get());
  EXPECT_EQ(EVP_PKEY_X25519, EVP_PKEY_id(pkey.get()));

  size_t len = 999;
  ASSERT_TRUE(EVP_PKEY_get_raw_public_key(pkey.get(), nullptr, &len));
  ASSERT_EQ(32, len);

  unsigned char raw[32];
  len = sizeof(raw);
  ASSERT_TRUE(EVP_PKEY_get_raw_public_key(pkey.get(), raw, &len));

  static const uint8_t expected_raw[] { 0xE6, 0xDB, 0x68, 0x67, 0x58, 0x30, 0x30, 0xDB, 0x35, 0x94, 0xC1, 0xA4, 0x24, 0xB1, 0x5F, 0x7C, 0x72, 0x66, 0x24, 0xEC, 0x26, 0xB3, 0x35, 0x3B, 0x10, 0xA9, 0x03, 0xA6, 0xD0, 0xAB, 0x1C, 0x4C };

  ASSERT_EQ(sizeof(expected_raw), len);

  for (int i = 0; i < len; i++) {
    ASSERT_EQ(raw[i], expected_raw[i]);
  }
}

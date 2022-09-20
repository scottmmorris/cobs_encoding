#include <stdio.h>
#include "cobs.h"
#include <string.h>

void printByteArray(uint8_t *data, size_t length)
{
  printf("ARRAY: ");
	for (const uint8_t *byte = (const uint8_t *)data; length--; ++byte)
  {
		printf("%d ", *byte);
  }
  printf("\n");
}

int compareByteArrays(char *test_name, uint8_t *data1, size_t length1, uint8_t *data2, size_t length2)
{
  if (length1 != length2)
  {
    printf("[%s] FAILED: Byte arrays are of different lengths: %ld and %ld\n", test_name, length1, length2);
    printByteArray(data1, length1);
    printByteArray(data2, length2);
    return 0;
  }


  for (const uint8_t *byte1 = (const uint8_t *)data1, *byte2 = (const uint8_t *)data2; length1--; ++byte1, ++byte2)
  {
    if (*byte1 != *byte2)
    {
		  printf("[%s] FAILED: Byte mismatch at position %ld: %d and %d\n", test_name, length2 - length1, *byte1, *byte2);
      printByteArray(data1, length2);
      printByteArray(data2, length2);
      return 0;
    }
  }
  printf("[%s] SUCCESS\n", test_name);
  return 1;
}

int main()
{
  size_t n, m;
  cobs_encode_result encode_res;
  cobs_decode_result decode_res;
  uint8_t encode_buffer[1000], decode_buffer[1000];

  // -- TEST 1 --
  memset(decode_buffer, 'A', sizeof(decode_buffer));
  memset(encode_buffer, 'A', sizeof(encode_buffer));
  uint8_t raw_data_1[] = { 0x01, 0x02, 0x03 };
  uint8_t encoded_data_1[] = { 0x04, 0x01, 0x02, 0x03 };

  encode_res = cobs_encode(encode_buffer, sizeof(encode_buffer), raw_data_1, sizeof(raw_data_1));
  compareByteArrays("NoZeroInData - Encode", encoded_data_1, sizeof(encoded_data_1), encode_buffer, encode_res.out_len);
  decode_res = cobs_decode(decode_buffer, sizeof(decode_buffer), encode_buffer, encode_res.out_len);
  compareByteArrays("NoZeroInData - Decode", raw_data_1, sizeof(raw_data_1), decode_buffer, decode_res.out_len);

  // -- TEST 2 --
  memset(decode_buffer, 'A', sizeof(decode_buffer));
  memset(encode_buffer, 'A', sizeof(encode_buffer));
  uint8_t raw_data_2[] = { 0xAA, 0x00, 0xAB };
  uint8_t encoded_data_2[] = { 0x02, 0xAA, 0x02, 0xAB };

  encode_res = cobs_encode(encode_buffer, sizeof(encode_buffer), raw_data_2, sizeof(raw_data_2));
  compareByteArrays("ZeroInData - Encode", encoded_data_2, sizeof(encoded_data_2), encode_buffer, encode_res.out_len);
  decode_res = cobs_decode(decode_buffer, sizeof(decode_buffer), encode_buffer, encode_res.out_len);
  compareByteArrays("ZeroInData - Decode", raw_data_2, sizeof(raw_data_2), decode_buffer, decode_res.out_len);

  // -- TEST 3 --
  memset(decode_buffer, 'A', sizeof(decode_buffer));
  memset(encode_buffer, 'A', sizeof(encode_buffer));
  uint8_t raw_data_3[] = { 0x00, 0x00, 0x00 };
  uint8_t encoded_data_3[] = { 0x01, 0x01, 0x01, 0x01 };

  encode_res = cobs_encode(encode_buffer, sizeof(encode_buffer), raw_data_3, sizeof(raw_data_3));
  compareByteArrays("AllZeros - Encode", encoded_data_3, sizeof(encoded_data_3), encode_buffer, encode_res.out_len);
  decode_res = cobs_decode(decode_buffer, sizeof(decode_buffer), encode_buffer, encode_res.out_len);
  compareByteArrays("AllZeros - Decode", raw_data_3, sizeof(raw_data_3), decode_buffer, decode_res.out_len);

  // -- TEST 4 --
  memset(decode_buffer, 'A', sizeof(decode_buffer));
  memset(encode_buffer, 'A', sizeof(encode_buffer));
  uint8_t raw_data_4[] = { 0x00, 0xAA, 0xAB, 0xAC, 0x00, 0x00, 0xAD };
  uint8_t encoded_data_4[] = { 0x01, 0x04, 0xAA, 0xAB, 0xAC, 0x01, 0x02, 0xAD };

  encode_res = cobs_encode(encode_buffer, sizeof(encode_buffer), raw_data_4, sizeof(raw_data_4));
  compareByteArrays("ManyBytes - Encode", encoded_data_4, sizeof(encoded_data_4), encode_buffer, encode_res.out_len);
  decode_res = cobs_decode(decode_buffer, sizeof(decode_buffer), encode_buffer, encode_res.out_len);
  compareByteArrays("ManyBytes - Decode", raw_data_4, sizeof(raw_data_4), decode_buffer, decode_res.out_len);

  // -- TEST 5 --
  memset(decode_buffer, 'A', sizeof(decode_buffer));
  memset(encode_buffer, 'A', sizeof(encode_buffer));
  uint8_t raw_data_5[254];
  uint8_t encoded_data_5[255];
  for(int i = 0; i < 255; i++) {
    raw_data_5[i] = i + 1;
    if (i == 0) {
      encoded_data_5[i] = 255;
    } else {
      encoded_data_5[i] = i;
    }
  }

  encode_res = cobs_encode(encode_buffer, sizeof(encode_buffer), raw_data_5, sizeof(raw_data_5));
  compareByteArrays("Excatly254Bytes - Encode", encoded_data_5, sizeof(encoded_data_5), encode_buffer, encode_res.out_len);
  decode_res = cobs_decode(decode_buffer, sizeof(decode_buffer), encode_buffer, encode_res.out_len);
  compareByteArrays("Excatly254Bytes - Decode", raw_data_5, sizeof(raw_data_5), decode_buffer, decode_res.out_len);

  // -- TEST 6 --
  memset(decode_buffer, 'A', sizeof(decode_buffer));
  memset(encode_buffer, 'A', sizeof(encode_buffer));
  uint8_t raw_data_6[257];
  uint8_t encoded_data_6[259];
  for(int i = 0; i < 254; i++) {
    raw_data_6[i] = i + 1;
    if (i == 0) {
      encoded_data_6[i] = 255;
    } else {
      encoded_data_6[i] = i;
    }
  }
  raw_data_6[254] = 1;
  raw_data_6[255] = 2;
  raw_data_6[256] = 0;
  encoded_data_6[254] = 254;
  encoded_data_6[255] = 3;
  encoded_data_6[256] = 1;
  encoded_data_6[257] = 2;
  encoded_data_6[258] = 1;

  encode_res = cobs_encode(encode_buffer, sizeof(encode_buffer), raw_data_6, sizeof(raw_data_6));
  compareByteArrays("MoreThan254Bytes - Encode", encoded_data_6, sizeof(encoded_data_6), encode_buffer, encode_res.out_len);
  decode_res = cobs_decode(decode_buffer, sizeof(decode_buffer), encode_buffer, encode_res.out_len);
  compareByteArrays("MoreThan254Bytes - Decode", raw_data_6, sizeof(raw_data_6), decode_buffer, decode_res.out_len);

  // -- TEST 7 --
  memset(decode_buffer, 'A', sizeof(decode_buffer));
  uint8_t encoded_data_7[] = { 0x02, 0x00, 0x00 };

  decode_res = cobs_decode(decode_buffer, sizeof(decode_buffer), encoded_data_7, sizeof(encoded_data_7));
  if (decode_res.status == COBS_DECODE_ZERO_BYTE_IN_INPUT) {
    printf("[ZeroByteInInput - Decode] SUCCESS\n");
  } else {
    printf("[ZeroByteInInput - Decode] FAILED: Decoding returned status 0x%02X, when correct status should be 0x%02X\n", decode_res.status, COBS_DECODE_ZERO_BYTE_IN_INPUT);
  }
}

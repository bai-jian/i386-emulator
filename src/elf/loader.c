unsigned char loader[] = {
  0x55, 0x89, 0xe5, 0x83, 0xe4, 0xf0, 0x83, 0xec, 0x20, 0xc7, 0x44, 0x24,
  0x1c, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x3c, 0x8b, 0x44, 0x24, 0x1c, 0x89,
  0x04, 0x24, 0xe8, 0x45, 0x00, 0x00, 0x00, 0x8b, 0x54, 0x24, 0x1c, 0x89,
  0x04, 0x95, 0x40, 0x01, 0x10, 0x00, 0x8b, 0x44, 0x24, 0x1c, 0x8b, 0x14,
  0x85, 0x40, 0x01, 0x10, 0x00, 0x8b, 0x44, 0x24, 0x1c, 0x8b, 0x04, 0x85,
  0x00, 0x01, 0x10, 0x00, 0x39, 0xc2, 0x74, 0x06, 0xb8, 0x01, 0x00, 0x00,
  0x00, 0x82, 0x83, 0x44, 0x24, 0x1c, 0x01, 0x83, 0x7c, 0x24, 0x1c, 0x0c,
  0x7e, 0xbd, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x82, 0xb8, 0x00, 0x00, 0x00,
  0x00, 0xc9, 0xc3, 0x00, 0x55, 0x89, 0xe5, 0x83, 0xec, 0x18, 0x83, 0x7d,
  0x08, 0x00, 0x74, 0x06, 0x83, 0x7d, 0x08, 0x01, 0x75, 0x07, 0xb8, 0x01,
  0x00, 0x00, 0x00, 0xeb, 0x12, 0x8b, 0x45, 0x08, 0x83, 0xe8, 0x01, 0x89,
  0x04, 0x24, 0xe8, 0xd9, 0xff, 0xff, 0xff, 0x0f, 0xaf, 0x45, 0x08, 0xc9,
  0xc3, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x7a, 0x52, 0x00, 0x01, 0x7c, 0x08, 0x01, 0x1b, 0x0c, 0x04, 0x04,
  0x88, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0xb0, 0xff, 0xff, 0xff, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08,
  0x85, 0x02, 0x42, 0x0d, 0x05, 0x69, 0xc5, 0x0c, 0x04, 0x04, 0x00, 0x00,
  0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x2c, 0xff, 0xff, 0xff,
  0x63, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08, 0x85, 0x02, 0x42, 0x0d,
  0x05, 0x02, 0x5f, 0xc5, 0x0c, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x78, 0x00, 0x00, 0x00, 0xd0, 0x02, 0x00, 0x00, 0xb0, 0x13, 0x00, 0x00,
  0x80, 0x9d, 0x00, 0x00, 0x80, 0x89, 0x05, 0x00, 0x00, 0x5f, 0x37, 0x00,
  0x00, 0x15, 0x61, 0x02, 0x00, 0xfc, 0x8c, 0x1c
};
unsigned int loader_len = 308;

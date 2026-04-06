#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

typedef struct {
	uint8_t reserved;
	uint8_t version_byte;
	uint16_t length_be;
	uint8_t flags_raw;
} proto_header_raw_t;

typedef struct {
	unsigned ver_major;
	unsigned ver_minor;
	unsigned length;
	unsigned flags;
} proto_header_bits_t;

#pragma pack(pop)

static uint16_t be16_to_cpu(uint16_t be)
{
	return (uint16_t)(((be & 0xFFu) << 8) | ((be >> 8) & 0xFFu));
}

int main(void)
{
	const uint8_t stream[5] = {0x00, 0x03, 0x00, 0x20, 0x00};

	proto_header_raw_t raw = {0};
	memcpy(&raw, stream, sizeof(raw));

	uint16_t length = be16_to_cpu(raw.length_be);
	unsigned flags = (unsigned)(raw.flags_raw & 0x1Fu);

	proto_header_bits_t view = {0};
	view.ver_major = (unsigned)((raw.version_byte >> 4) & 0xFu);
	view.ver_minor = (unsigned)(raw.version_byte & 0xFu);
	view.length = (unsigned)length;
	view.flags = flags;

	printf("version:%u.%u, length:%u, flags:0x%02X\n", view.ver_major, view.ver_minor, view.length,
	       view.flags & 0xFFu);

	return 0;
}

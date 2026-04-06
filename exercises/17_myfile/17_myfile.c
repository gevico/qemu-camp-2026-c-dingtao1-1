#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type)
{
	const char *type_str;

	switch (e_type) {
	case ET_NONE:
		type_str = "Unknown";
		break;
	case ET_REL:
		type_str = "Relocatable";
		break;
	case ET_EXEC:
		type_str = "Executable";
		break;
	case ET_DYN:
		type_str = "Shared Object/PIE";
		break;
	default:
		type_str = "Unknown";
		break;
	}
	printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[])
{
	char filepath[2][256] = {
		"./17_myfile.o",
		"./17_myfile",
	};

	(void)argc;
	(void)argv;

	for (int i = 0; i < 2; i++) {
		int fd = open(filepath[i], O_RDONLY);
		if (fd < 0) {
			perror(filepath[i]);
			continue;
		}

		Elf64_Ehdr ehdr;
		ssize_t n = read(fd, &ehdr, sizeof(ehdr));
		close(fd);

		if (n != (ssize_t)sizeof(ehdr)) {
			fprintf(stderr, "%s: read error\n", filepath[i]);
			continue;
		}

		if (!(ehdr.e_ident[EI_MAG0] == ELFMAG0 && ehdr.e_ident[EI_MAG1] == ELFMAG1 &&
		      ehdr.e_ident[EI_MAG2] == ELFMAG2 && ehdr.e_ident[EI_MAG3] == ELFMAG3)) {
			fprintf(stderr, "%s: not an ELF file\n", filepath[i]);
			continue;
		}

		print_elf_type(ehdr.e_type);
	}

	return 0;
}

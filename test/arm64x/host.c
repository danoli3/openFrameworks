// loads oftest.dll and calls it; built as ARM64, ARM64EC and x64
#include <windows.h>
#include <stdio.h>
typedef const char * (*arch_fn)(void);
typedef unsigned long (*crc_fn)(const char *);
typedef unsigned long (*ver_fn)(void);
typedef int (*xml_fn)(const char *);
int main(int argc, char ** argv) {
#if defined(_M_ARM64EC)
	const char * self = "ARM64EC";
#elif defined(_M_ARM64)
	const char * self = "ARM64";
#elif defined(_M_X64)
	const char * self = "x64";
#else
	const char * self = "unknown";
#endif
	const char * dll = argc > 1 ? argv[1] : "oftest.dll";
	HMODULE h = LoadLibraryA(dll);
	if (!h) { printf("%-8s host: LoadLibrary(%s) failed, error %lu\n", self, dll, GetLastError()); return 1; }
	arch_fn arch = (arch_fn)GetProcAddress(h, "oftest_arch");
	crc_fn crc = (crc_fn)GetProcAddress(h, "oftest_crc32");
	ver_fn ver = (ver_fn)GetProcAddress(h, "oftest_png_version");
	xml_fn xml = (xml_fn)GetProcAddress(h, "oftest_xml_children");
	if (!arch || !crc || !ver || !xml) { printf("%-8s host: missing export\n", self); return 1; }
	printf("%-8s host -> DLL code %-7s | zlib crc32(\"openFrameworks\") = %08lx | libpng %lu | pugixml children = %d\n",
		self, arch(), crc("openFrameworks"), ver(), xml("<of><a/><b/><c/></of>"));
	return 0;
}

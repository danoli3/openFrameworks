// ARM64X test DLL: statically links openFrameworks' zlib, libpng and pugixml
#include <cstring>
#include <iterator>
#include <zlib.h>
#include <png.h>
#include <pugixml.hpp>

extern "C" {
__declspec(dllexport) const char * oftest_arch() {
#if defined(_M_ARM64EC)
	return "ARM64EC";
#elif defined(_M_ARM64)
	return "ARM64";
#elif defined(_M_X64)
	return "x64";
#else
	return "unknown";
#endif
}
__declspec(dllexport) unsigned long oftest_crc32(const char * s) {
	return crc32(0L, reinterpret_cast<const Bytef *>(s), static_cast<uInt>(std::strlen(s)));
}
__declspec(dllexport) unsigned long oftest_png_version() {
	return png_access_version_number();
}
__declspec(dllexport) int oftest_xml_children(const char * xml) {
	pugi::xml_document doc;
	if (!doc.load_string(xml)) return -1;
	auto children = doc.first_child().children();
	return static_cast<int>(std::distance(children.begin(), children.end()));
}
}

#pragma once

#ifdef MSFL_EXPORTS
#define MSFL_EXP __declspec(dllexport)
#else
#define MSFL_EXP
#endif

extern "C" {
	namespace msfl {
		//byte pointer with length
		class RawBytes {
		public:
			unsigned char* bytes = nullptr;
			int len = 0;
			MSFL_EXP void CopyToBuffer(RawBytes* buffer);
			MSFL_EXP void deleteMemory();
			MSFL_EXP RawBytes() {};
		};
	}
}
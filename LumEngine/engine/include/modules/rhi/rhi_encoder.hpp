#pragma once
#include "rhi/rhi_common.hpp"
#include "rhi/backend/opengl/gl_device.hpp"
namespace lum::rhi {

	template<typename T>
	concept Device = T::rhiDevice == true;

	using InvokeFn = void(*)(vptr userParams, cvptr device);
	using Storage = std::aligned_storage_t<256, alignof(std::max_align_t)>;

	struct Command {

		InvokeFn invoke;
		Storage data;

	};

	class Encoder {
	public:

		void begin_record() {
			bRecording = true;
		}
		void end_record() {
			bRecording = false;
		}

		template<Device D>
		void flush(const D& device);

	private:

		std::vector<Command> cmd;
		bool bRecording = false;

	};

}
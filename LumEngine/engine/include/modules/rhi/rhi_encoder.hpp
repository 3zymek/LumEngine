//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Records and plays RHI functions
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi {

	using InvokeFn = void(*)(vptr userParams, cvptr device);
	using Storage = std::aligned_storage_t<256, alignof(std::max_align_t)>;

	struct Command {

		InvokeFn invoke;
		Storage data;

	};

	class Encoder {
	public:


		void Record() {

		}

		void Flush() {

		}

	private:

		std::vector<Command> cmd;
		bool bRecording = false;

	};

}
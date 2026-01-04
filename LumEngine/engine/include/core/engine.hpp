#pragma once

#include "audio/audio_manager.hpp"
#include "audio/systems/sys_audio.hpp"
#include "event/ev_bus.hpp"
#include "entity/ecs_manager.hpp"
#include "render/core/renderer.hpp"
#include "core/scene_manager.hpp"
#include "render/core/mesh_manager.hpp"
#include "input/input_manager.hpp"
namespace lum::core {

	struct EngineConfiguration {

		render::RenderConfig	render_config;

	};

	class Engine {
	public:

		Engine(const EngineConfiguration& cfg) { Init(cfg); }

		void BeginFrame() {

			renderer.BeginFrame();

			event_bus.PollEvents();

		}


		void EndFrame() {

			audio_system.Update();

			renderer.EndFrame();

		}

		inline bool Running() { return running; }

		[[nodiscard]] inline ev::		EventBus&		GetEventBus		( ) noexcept { return event_bus;	}
		[[nodiscard]] inline render::	Renderer&		GetRenderer		( ) noexcept { return renderer;		}
		[[nodiscard]] inline render::	MeshManager&	GetMeshManager	( ) noexcept { return mesh_mgr;		}
		[[nodiscard]] inline render::	ShaderManager&	GetShaderManager( ) noexcept { return shader_mgr;	}
		[[nodiscard]] inline ecs::		EntityManager&	GetECSManager	( ) noexcept { return ecs_mgr;		}
		[[nodiscard]] inline audio::	AudioManager&	GetAudioManager	( ) noexcept { return audio_mgr;	}
		[[nodiscard]] inline core::		SceneManager&	GetSceneManager	( ) noexcept { return scene_mgr;	}

	private:

		void Init(const EngineConfiguration& cfg) {

			renderer.Init(cfg.render_config);
			audio_mgr.Init();

			input::SetActiveWindow(renderer.GetWindow());

		}

		inline void Terminate() {

		}

		ev::	EventBus		event_bus;
		render::Renderer		renderer;
		render::MeshManager		mesh_mgr;
		render::ShaderManager	shader_mgr;
		ecs::	EntityManager	ecs_mgr		 { event_bus			};
		audio::	AudioManager	audio_mgr	 { ecs_mgr, event_bus	};
		audio::	AudioSystem		audio_system { audio_mgr			};
		core::	SceneManager	scene_mgr;

		bool running = true;

	};
}
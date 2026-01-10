#pragma once

#include "audio/audio_manager.hpp"
#include "audio/audio_system.hpp"
#include "event/event_bus.hpp"
#include "entity/ecs_manager.hpp"
#include "core/scene_manager.hpp"
#include "window_context/input_common.hpp"
#include "entity/systems/transform_system.hpp"
#include "entity/entity.hpp"
/*
namespace lum::core {

	struct EngineConfiguration {

	};

	class Engine {
	public:

		Engine(const EngineConfiguration& cfg) { Init(cfg); }

		void BeginFrame() {

			renderer.BeginFrame();

			event_bus.PollEvents();

		}


		void EndFrame() {

			transform_sys.Update();
			audio_sys.Update();

			renderer.EndFrame();

		}

		inline bool Running() { return running; }

		[[nodiscard]] inline ev::		EventBus&		GetEventBus		( ) noexcept { return event_bus;	}
		[[nodiscard]] inline render::	RenderDevice&	GetRenderer		( ) noexcept { return renderer;		}
		[[nodiscard]] inline render::	MeshStorage&	GetMeshManager	( ) noexcept { return mesh_mgr;		}
		[[nodiscard]] inline render::	ShaderManager&	GetShaderManager( ) noexcept { return shader_mgr;	}
		[[nodiscard]] inline ecs::		EntityManager&	GetECSManager	( ) noexcept { return ecs_mgr;		}
		[[nodiscard]] inline audio::	AudioManager&	GetAudioManager	( ) noexcept { return audio_mgr;	}
		[[nodiscard]] inline core::		SceneManager&	GetSceneManager	( ) noexcept { return scene_mgr;	}
		[[nodiscard]] inline systems::	TransformSystem&	GetTransformSystem	( ) noexcept { return transform_sys;	}
		[[nodiscard]] inline systems::	AudioSystem&		GetAudioSystem		( ) noexcept { return audio_sys;		}
		
	private:

		void Init(const EngineConfiguration& cfg) {

			renderer.Init(cfg.render_config);
			audio_mgr.Init();
			shader_mgr.Init();
			
			input::SetActiveWindow(renderer.GetWindow());

		}

		inline void Terminate() {

		}

		ev::		EventBus		event_bus;
		render::	RenderDevice	renderer;
		render::	MeshStorage		mesh_mgr;
		render::	ShaderManager	shader_mgr;
		systems::	TransformSystem transform_sys{ ecs_mgr				};
		ecs::		EntityManager	ecs_mgr		 { event_bus			};
		audio::		AudioManager	audio_mgr	 { ecs_mgr, event_bus	};
		systems::	AudioSystem		audio_sys	 { audio_mgr			};
		core::		SceneManager	scene_mgr;

		bool running = true;

	};
}*/
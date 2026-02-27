#pragma once

#include "render/render_system.hpp"

#include "render/renderer.hpp"
#include "entity/ecs_manager.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/material.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/render.hpp"
#include "entity/components/name.hpp"
#include "entity/components/camera.hpp"
#include "entity/components/light.hpp"

#include "platform/input_common.hpp"
#include "platform/key_codes.hpp"

#include "platform/window.hpp"

#include "imgui.h"

namespace lum {

	namespace render {

		void RenderSystem::Initialize(render::Renderer* renderer) {
		
			mRenderer = renderer;
		
		}

		void RenderSystem::Update(ecs::MEntityManager* entityMgr, Window* window) {

			float32 aspect = (float32)window->GetWidth() / (float32)window->GetHeight();

			entityMgr->Each<CCamera, CTransform>(
				[&](CCamera& camera, CTransform& transform) {
					FCameraData data;
					data.mPosition = transform.mPosition;
					data.mProjection = glm::perspective(glm::radians(camera.mFov), aspect, camera.mNear, camera.mFar);
					data.mView = glm::lookAt(transform.mPosition, camera.mTarget, camera.mUp);

					glm::vec3 forward = glm::normalize(camera.mTarget - transform.mPosition);
					glm::vec3 right = glm::normalize(glm::cross(forward, camera.mUp));

					float32 speed = 5.0f * 0.1f;
					
					static bool sCameraLocked = false;

					if (input::KeyPressedOnce(input::Key::LEFT_CONTROL)) {
						sCameraLocked = !sCameraLocked;
					}
					static glm::vec2 sLastMousePos;
					if (!sCameraLocked) {
						float32 speed = 5.0f * 0.1f;

						if (input::KeyPressed(input::Key::W)) transform.mPosition += forward * speed;
						if (input::KeyPressed(input::Key::S)) transform.mPosition -= forward * speed;
						if (input::KeyPressed(input::Key::A)) transform.mPosition -= right * speed;
						if (input::KeyPressed(input::Key::D)) transform.mPosition += right * speed;

						static glm::vec2 sLastMousePos = input::GetMousePos();
						glm::vec2 currentPos = input::GetMousePos();
						glm::vec2 delta = currentPos - sLastMousePos;
						sLastMousePos = currentPos;

						camera.mYaw += delta.x * 0.1f;
						camera.mPitch -= delta.y * 0.1f;
						camera.mPitch = glm::clamp(camera.mPitch, -89.0f, 89.0f);
					}
					else {
						sLastMousePos = input::GetMousePos();
					}
					glm::vec2 currentPos = input::GetMousePos();
					glm::vec2 delta = currentPos - sLastMousePos;
					sLastMousePos = currentPos;

					camera.mYaw += delta.x * 0.1f;
					camera.mPitch -= delta.y * 0.1f;
					camera.mPitch = glm::clamp(camera.mPitch, -89.0f, 89.0f);
					data.mPitch = camera.mPitch;
					data.mYaw = camera.mYaw;

					glm::vec3 direction;
					direction.x = cos(glm::radians(camera.mYaw)) * cos(glm::radians(camera.mPitch));
					direction.y = sin(glm::radians(camera.mPitch));
					direction.z = sin(glm::radians(camera.mYaw)) * cos(glm::radians(camera.mPitch));

					camera.mTarget = transform.mPosition + glm::normalize(direction);

					mRenderer->UpdateCamera(data);
				}
			);

			ImGui::Begin("Materials");
			entityMgr->Each<CRender, CTransform, CMaterial, CStaticMesh>(
				[&](CRender& render, CTransform& transform, CMaterial& material, CStaticMesh& mesh)
				{
					if (!render.bVisible) return;
					render::Object obj;
					
					ImGui::DragFloat("Roughness", &material.mMat.mRoughnessValue, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Metallic", &material.mMat.mMetallicValue, 0.01f, 0.0f, 1.0f);
					ImGui::ColorEdit3("BaseColor", glm::value_ptr(material.mMat.mBaseColor), 0.01f);

					obj.mMaterial = material.mMat;
					obj.mStaticMesh = mesh.mMesh;
					obj.mPosition = transform.mPosition;
					obj.mRotation = transform.mRotation;
					obj.mScale = transform.mScale;
					mRenderer->Draw(obj);
				});
			ImGui::End();

			uint32 pointLights = 0;
			entityMgr->Each<CTransform, CPointLight>(
				[&](CTransform& transform, CPointLight& light) {
					PointLight pointL;
					pointL.mColor = light.mColor;
					pointL.mIntensity = light.mIntensity;
					pointL.mPosition = transform.mPosition;
					pointL.mRadius = light.mRadius;
					mRenderer->mPointLights[pointLights] = pointL;
					pointLights++;
				}
			);
			mRenderer->mPointLightsCount = pointLights;

			if (ImGui::CollapsingHeader("Point Lights")) {
				uint32_t i = 0;
				entityMgr->Each<CTransform, CPointLight>(
					[&](CTransform& transform, CPointLight& light) {
						ImGui::PushID(i);
						std::string label = "Light " + std::to_string(i);
						if (ImGui::TreeNode(label.c_str())) {
							ImGui::DragFloat3("Position", &transform.mPosition.x, 0.1f);
							ImGui::ColorEdit3("Color", &light.mColor.x);
							ImGui::DragFloat("Intensity", &light.mIntensity, 0.01f, 0.0f, 100.0f);
							ImGui::DragFloat("Radius", &light.mRadius, 0.1f, 0.0f, 1000.0f);
							ImGui::TreePop();
						}
						ImGui::PopID();
						i++;
					}
				);
			}

		}

	} // namespace lum::render

} // namespace lum
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

            float32 width = (float32)window->GetWidth();
            float32 height = (float32)window->GetHeight();
            
            if (width == 0 || height == 0)
                return;

            float32 aspect = width / height;

            entityMgr->Each<CCamera, CTransform>(
                [&](CCamera& camera, CTransform& transform) {

                    FRenderCamera data;

                    data.mPosition = transform.mPosition;
                    data.mProjection = glm::perspective(glm::radians(camera.mFov), aspect, camera.mNear, camera.mFar);
                    data.mView = glm::lookAt(transform.mPosition, camera.mTarget, camera.mUp);

                    glm::vec3 forward = glm::normalize(camera.mTarget - transform.mPosition);
                    glm::vec3 right = glm::normalize(glm::cross(forward, camera.mUp));

                    static bool     sCameraLocked = false;
                    static glm::vec2 sLastMousePos = input::GetMousePos();

                    if (input::KeyPressedOnce(input::Key::LEFT_CONTROL))
                        sCameraLocked = !sCameraLocked;


                    ImGuiIO& io = ImGui::GetIO();

                    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
                    ImGui::Begin("##overlay", nullptr,
                        ImGuiWindowFlags_NoDecoration |
                        ImGuiWindowFlags_NoBackground |
                        ImGuiWindowFlags_NoInputs |
                        ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoMove
                    );

                    ImGui::Text("FPS: %.1f", io.Framerate);
                    ImGui::Separator();
                    ImGui::Text("Position: %.2f  %.2f  %.2f", transform.mPosition.x, transform.mPosition.y, transform.mPosition.z);
                    ImGui::Text("Yaw:   %.1f", camera.mYaw);
                    ImGui::Text("Pitch: %.1f", camera.mPitch);

                    ImGui::End();

                    if (!sCameraLocked) {
                        float32 speed = 5.0f * 0.1f;
                        if (input::KeyPressed(input::Key::W)) transform.mPosition += forward * speed;
                        if (input::KeyPressed(input::Key::S)) transform.mPosition -= forward * speed;
                        if (input::KeyPressed(input::Key::A)) transform.mPosition -= right * speed;
                        if (input::KeyPressed(input::Key::D)) transform.mPosition += right * speed;

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

                    


                    glm::vec3 direction;
                    direction.x = cos(glm::radians(camera.mYaw)) * cos(glm::radians(camera.mPitch));
                    direction.y = sin(glm::radians(camera.mPitch));
                    direction.z = sin(glm::radians(camera.mYaw)) * cos(glm::radians(camera.mPitch));
                    camera.mTarget = transform.mPosition + glm::normalize(direction);

                    mRenderer->UpdateCamera(data);
                }
            );

            entityMgr->Each<CRender, CTransform, CMaterial, CStaticMesh>(
                [&](CRender& render, CTransform& transform, CMaterial& material, CStaticMesh& mesh) {

                    if (!render.bVisible) return;

                    render::FRenderInstance inst;
                    inst.mMaterial = &material;
                    inst.mStaticMesh = &mesh;
                    inst.mTransform = &transform;

                    mRenderer->Submit(inst);

                }
            );

            entityMgr->Each<CTransform, CPointLight>(
                [&](CTransform& transform, CPointLight& light) {

                    FPointLight pointL;

                    pointL.mPosition = transform.mPosition;
                    pointL.mIntensity = light.mIntensity;
                    pointL.mColor = light.mColor;
                    pointL.mRadius = light.mRadius;

                    mRenderer->AddPointLight(pointL);

                }
            );

            entityMgr->Each<CTransform, CSpotLight>(
                [&](CTransform& transform, CSpotLight& light) {

                    FSpotLight spotL;

                    spotL.mPosition = transform.mPosition;
                    spotL.mIntensity = light.mIntensity;
                    spotL.mColor = light.mColor;
                    spotL.mRadius = light.mRadius;
                    spotL.mDirection = glm::normalize(light.mDirection);
                    spotL.mInnerCone = glm::cos(glm::radians(light.mInnerConeAngle));
                    spotL.mOuterCone = glm::cos(glm::radians(light.mOuterConeAngle));

                    mRenderer->AddSpotLight(spotL);

                }
            );


            static int sSelectedEntity = -1;

            ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_Always);
            ImGui::Begin("Scene");

            ImGui::BeginChild("EntityList", ImVec2(180, 0), true);
            ImGui::SeparatorText("Entities");

            entityMgr->EachWithID<CName>(
                [&](ecs::EntityID id, CName& name) {
                    bool selected = (sSelectedEntity == (int32)id);
                    if (ImGui::Selectable(name.mName.Data(), selected))
                        sSelectedEntity = (int32)id;
                }
            );

            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("Inspector", ImVec2(0, 0), true);

            if (sSelectedEntity >= 0) {
                ImGui::SeparatorText("Inspector");

                entityMgr->EachWithID<CName, CTransform>(
                    [&](ecs::EntityID id, CName& name, CTransform& transform) {
                        if ((int32)id != sSelectedEntity) return;

                        ImGui::SeparatorText("Transform");
                        ImGui::DragFloat3("Position", glm::value_ptr(transform.mPosition), 0.1f);
                        ImGui::DragFloat3("Rotation", glm::value_ptr(transform.mRotation), 0.1f);
                        ImGui::DragFloat3("Scale", glm::value_ptr(transform.mScale), 0.01f);
                    }
                );

                entityMgr->EachWithID<CName, CMaterial>(
                    [&](ecs::EntityID id, CName& name, CMaterial& material) {
                        if ((int32)id != sSelectedEntity) return;

                        ImGui::SeparatorText("Material");
                        ImGui::DragFloat("Roughness", &material.mMat.mRoughnessValue, 0.01f, 0.0f, 1.0f);
                        ImGui::DragFloat("Metallic", &material.mMat.mMetallicValue, 0.01f, 0.0f, 1.0f);
                        ImGui::ColorEdit3("Base Color", glm::value_ptr(material.mMat.mBaseColor));
                    }
                );

                entityMgr->EachWithID<CName, CPointLight>(
                    [&](ecs::EntityID id, CName& name, CPointLight& light) {
                        if ((int32)id != sSelectedEntity) return;

                        ImGui::SeparatorText("Point Light");
                        ImGui::ColorEdit3("Color", glm::value_ptr(light.mColor));
                        ImGui::DragFloat("Intensity", &light.mIntensity, 0.01f, 0.0f, 100.0f);
                        ImGui::DragFloat("Radius", &light.mRadius, 0.1f, 0.0f, 1000.0f);
                    }
                );

                entityMgr->EachWithID<CName, CSpotLight>(
                    [&](ecs::EntityID id, CName& name, CSpotLight& light) {
                        if ((int32)id != sSelectedEntity) return;
                        ImGui::SeparatorText("Spot Light");
                        ImGui::ColorEdit3("Color", glm::value_ptr(light.mColor));
                        ImGui::DragFloat("Intensity", &light.mIntensity, 0.01f, 0.0f, 100.0f);
                        ImGui::DragFloat("Radius", &light.mRadius, 0.1f, 0.0f, 1000.0f);
                        ImGui::DragFloat3("Direction", glm::value_ptr(light.mDirection), 0.01f, -1.0f, 1.0f);
                        ImGui::DragFloat("Inner Cone", &light.mInnerConeAngle, 0.5f, 0.0f, light.mOuterConeAngle);
                        ImGui::DragFloat("Outer Cone", &light.mOuterConeAngle, 0.5f, light.mInnerConeAngle, 90.0f);
                    }
                );

            }
            else {
                ImGui::TextDisabled("Select an entity...");
            }

            ImGui::EndChild();
            ImGui::End();
        }

	} // namespace lum::render

} // namespace lum
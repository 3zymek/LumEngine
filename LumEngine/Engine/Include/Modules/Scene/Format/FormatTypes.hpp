#pragma once

#include <unordered_map>
#include <vector>

#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Utils/Logger.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Core/Utils/StringBuilder.hpp"
#include "Entity/EcsTypes.hpp"
#include "Platform/FileSystem/FileSystem.hpp"

namespace lum {

	class AudioManager;
	struct Scene;
	namespace render { class Renderer; }
	namespace ev { class EventBus; }

	struct SceneManagerContext {
		SafePtr<ev::EventBus> m_EventBus = nullptr;
	};

	namespace fmt {

		enum class TokenType : uint8 {
			Identifier, LBracket, RBracket, LSquareBracket, RSquareBracket,
			Parameter, Component, Colon, Dollar, String, Number, EndOfLine
		};

		enum class FileFormat : uint8 { Scene, Material };

		struct Token {
			uint32 m_Line{};
			TokenType m_Type{};
			String m_Value{};
			Path m_FilePath{};
		};

		struct DeserializeContext {
			Scene& m_Scene;
			std::unordered_map<uint64, EntityID> m_PersistentToEntity{};
			std::unordered_map<EntityID, std::vector<EntityID>> m_PersistentChildren{};
			EntityID m_CurrentEntity{};
			SceneManagerContext m_Ctx{};
		};

		using DeserializeFn = void(*)(std::vector<Token>&, int32&, DeserializeContext&);
		using SerializeFn = void(*)(StringBuilder&, ComponentBase*);

		struct SceneComponentInfo {
			String m_SerializationName{};
			uint64 m_TypeId{};
			DeserializeFn m_DeserializeFn = nullptr;
			SerializeFn m_SerializeFn = nullptr;
		};

		void RegisterSceneComponents(std::vector<SceneComponentInfo>& infos);

		inline bool IsValidFormat(const Path& file, FileFormat format) {
			const String ext = file.Extension();
			return (ext == ".lsc" && format == FileFormat::Scene) ||
				(ext == ".lmt" && format == FileFormat::Material);
		}

		namespace detail {

			inline void LogTokenError(const std::vector<Token>& tokens, int32 index, StringView message) {
				if (index >= 0 && static_cast<usize>(index) < tokens.size()) {
					const auto& token = tokens[static_cast<usize>(index)];
					LUM_LOG_ERROR("{} at line {} in {}", message, token.m_Line, token.m_FilePath.ToString());
				}
				else {
					LUM_LOG_ERROR("{} at end of token stream", message);
				}
			}

			inline bool InBlock(const std::vector<Token>& tokens, int32 i) {
				return i >= 0 && static_cast<usize>(i) < tokens.size() && tokens[static_cast<usize>(i)].m_Type != TokenType::RBracket;
			}
			inline bool InSquareBlock(const std::vector<Token>& tokens, int32 i) {
				return i >= 0 && static_cast<usize>(i) < tokens.size() && tokens[static_cast<usize>(i)].m_Type != TokenType::RSquareBracket;
			}
			inline bool IsString(const std::vector<Token>& tokens, int32 i, StringView value) {
				return i >= 0 && static_cast<usize>(i) < tokens.size() && tokens[static_cast<usize>(i)].m_Value == ToLower(value);
			}
			inline bool IsToken(const std::vector<Token>& tokens, int32 i, TokenType type) {
				return i >= 0 && static_cast<usize>(i) < tokens.size() && tokens[static_cast<usize>(i)].m_Type == type;
			}

			inline bool Expect(std::vector<Token>& tokens, int32& i, TokenType type, StringView message) {
				if (IsToken(tokens, i, type)) return true;
				LogTokenError(tokens, i, message);
				i = static_cast<int32>(tokens.size());
				return false;
			}
			inline void ExpectOpeningBracketNext(std::vector<Token>& tokens, int32& i) {
				++i;
				if (Expect(tokens, i, TokenType::LBracket, "Opening bracket expected")) ++i;
			}
			inline void ExpectOpeningSquareBracketNext(std::vector<Token>& tokens, int32& i) {
				++i;
				if (Expect(tokens, i, TokenType::LSquareBracket, "Opening square bracket expected")) ++i;
			}
			inline void ExceptOpeningSquareBracketInPlace(std::vector<Token>& tokens, int32& i) {
				Expect(tokens, i, TokenType::LSquareBracket, "Opening square bracket expected");
			}
			inline void ExceptColonNext(std::vector<Token>& tokens, int32& i) {
				++i;
				if (Expect(tokens, i, TokenType::Colon, "Colon expected")) ++i;
			}

			inline String ReadString(std::vector<Token>& tokens, int32& i) {
				if (!Expect(tokens, i, TokenType::String, "String expected")) return {};
				return tokens[static_cast<usize>(i)].m_Value;
			}
			inline String ReadStringParameter(std::vector<Token>& tokens, int32& i) {
				ExceptColonNext(tokens, i);
				return ReadString(tokens, i);
			}
			inline float32 ReadFloat(std::vector<Token>& tokens, int32& i) {
				if (!Expect(tokens, i, TokenType::Number, "Float expected")) return 0.0f;
				try { return std::stof(tokens[static_cast<usize>(i)].m_Value); }
				catch (const std::exception&) { LogTokenError(tokens, i, "Invalid float"); i = static_cast<int32>(tokens.size()); return 0.0f; }
			}
			inline float32 ReadFloatParameter(std::vector<Token>& tokens, int32& i) {
				ExceptColonNext(tokens, i);
				return ReadFloat(tokens, i);
			}
			inline bool ReadBool(std::vector<Token>& tokens, int32& i) { return ReadFloat(tokens, i) > 0.0f; }
			inline bool ReadBoolParameter(std::vector<Token>& tokens, int32& i) {
				ExceptColonNext(tokens, i);
				return ReadBool(tokens, i);
			}
			inline int64 ReadInt(std::vector<Token>& tokens, int32& i) {
				if (!Expect(tokens, i, TokenType::Number, "Integer expected")) return 0;
				try { return std::stoll(tokens[static_cast<usize>(i)].m_Value); }
				catch (const std::exception&) { LogTokenError(tokens, i, "Invalid integer"); i = static_cast<int32>(tokens.size()); return 0; }
			}
			inline int64 ReadIntParameter(std::vector<Token>& tokens, int32& i) {
				ExceptColonNext(tokens, i);
				return ReadInt(tokens, i);
			}
			inline Vector3 ReadVec3(std::vector<Token>& tokens, int32& i) {
				float32 values[3]{};
				for (int32 n = 0; n < 3; ++n) {
					int32 index = i + n;
					if (!IsToken(tokens, index, TokenType::Number)) { LogTokenError(tokens, index, "Vector3 component expected"); i = static_cast<int32>(tokens.size()); return {}; }
					try { values[n] = std::stof(tokens[static_cast<usize>(index)].m_Value); }
					catch (const std::exception&) { LogTokenError(tokens, index, "Invalid Vector3 component"); i = static_cast<int32>(tokens.size()); return {}; }
				}
				return { values[0], values[1], values[2] };
			}
			inline Vector3 ReadVec3Parameter(std::vector<Token>& tokens, int32& i) {
				ExceptColonNext(tokens, i);
				return ReadVec3(tokens, i);
			}
			inline Vector2 ReadVec2(std::vector<Token>& tokens, int32& i) {
				float32 values[2]{};
				for (int32 n = 0; n < 2; ++n) {
					int32 index = i + n;
					if (!IsToken(tokens, index, TokenType::Number)) { LogTokenError(tokens, index, "Vector2 component expected"); i = static_cast<int32>(tokens.size()); return {}; }
					try { values[n] = std::stof(tokens[static_cast<usize>(index)].m_Value); }
					catch (const std::exception&) { LogTokenError(tokens, index, "Invalid Vector2 component"); i = static_cast<int32>(tokens.size()); return {}; }
				}
				return { values[0], values[1] };
			}
			inline Vector2 ReadVec2Parameter(std::vector<Token>& tokens, int32& i) {
				ExceptColonNext(tokens, i);
				return ReadVec2(tokens, i);
			}

			inline void WriteStringParameter(const String& value, StringBuilder& sb) {
				sb.Append("\t\t\t\""); sb.Append(value); sb.AppendLine("\"");
			}
			template<usize tN> inline void WriteStringParameter(const FixedString<tN>& value, StringBuilder& sb) {
				sb.Append("\t\t\t\""); sb.Append(value.Data()); sb.AppendLine("\"");
			}
			inline void WriteBoolParameter(bool value, StringBuilder& sb) { sb.Append("\t\t\t"); sb.AppendLine(value ? "1" : "0"); }
			inline void WriteFloatParameter(float32 value, StringBuilder& sb) { sb.Append("\t\t\t"); sb.AppendLine(value); }
			inline void WriteIntParameter(int64 value, StringBuilder& sb) { sb.Append("\t\t\t"); sb.AppendLine(value); }
			inline void WriteVec3Parameter(const Vector3& value, StringBuilder& sb) {
				sb.Append("\t\t\t"); sb.Append(value.m_X); sb.Append(" "); sb.Append(value.m_Y); sb.Append(" "); sb.AppendLine(value.m_Z);
			}
			inline void WriteVec2Parameter(const Vector2& value, StringBuilder& sb) {
				sb.Append("\t\t\t"); sb.Append(value.m_X); sb.Append(" "); sb.AppendLine(value.m_Y);
			}
		}
	}
}

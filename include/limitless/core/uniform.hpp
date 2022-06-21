#pragma once

#include <memory>
#include <string>
#include <chrono>
#include <limitless/core/texture_visitor.hpp>
#include <limitless/core/context_debug.hpp>

namespace Limitless {
    class Texture;
    class ShaderProgram;

    enum class UniformType {
        Value,
        Sampler,
        Time
    };

    enum class UniformValueType {
        Float,
        Int,
        Uint,
        Vec2,
        Vec3,
        Vec4,
        Mat3,
        Mat4
    };

    class Uniform {
    protected:
        std::string name;
        UniformType type;
        UniformValueType value_type;
        bool changed;

        friend class UniformSerializer;

        // compares values equality
        friend bool operator==(const Uniform& lhs, const Uniform& rhs) noexcept;
        friend bool operator<(const Uniform& lhs, const Uniform& rhs) noexcept;
        friend bool operator!=(const Uniform& lhs, const Uniform& rhs) noexcept;
    public:
        Uniform(std::string name, UniformType type, UniformValueType value_type) noexcept;
        virtual ~Uniform() = default;

        [[nodiscard]] auto getType() const noexcept { return type; }
        [[nodiscard]] auto getValueType() const noexcept { return value_type; }
        [[nodiscard]] const auto& getName() const noexcept { return name; }
        [[nodiscard]] virtual bool& getChanged() noexcept { return changed; }

        //TODO:: fix? is it legal
        void setName(std::string _name) { name = std::move(_name); }

        [[nodiscard]] virtual Uniform* clone() noexcept = 0;
        virtual void set(const ShaderProgram& shader) = 0;
    };

    template<typename T>
    class UniformValue : public Uniform {
    protected:
        T value;

        constexpr UniformValueType getUniformValueType();
        UniformValue(const std::string& name, UniformType type, const T& value);
        friend class UniformSerializer;

    public:
        UniformValue(const std::string& name, const T& value) noexcept;
        ~UniformValue() override = default;

        [[nodiscard]] const auto& getValue() const noexcept { return value; }
        void setValue(const T& val) noexcept;

        [[nodiscard]] UniformValue* clone() noexcept override;
        void set(const ShaderProgram& shader) override;
    };

    class UniformSampler : public UniformValue<int> {
    private:
        std::shared_ptr<Texture> sampler;
        GLuint sampler_id{};
        friend class UniformSerializer;
    public:
        UniformSampler(const std::string& name, std::shared_ptr<Texture> sampler) noexcept;
        ~UniformSampler() override = default;

        [[nodiscard]] const auto& getSampler() const noexcept { return sampler; }
        void setSampler(const std::shared_ptr<Texture>& texture) noexcept;

        [[nodiscard]] UniformSampler* clone() noexcept override;
        void set(const ShaderProgram& shader) override;
    };

    class UniformTime : public UniformValue<float> {
    private:
        std::chrono::time_point<std::chrono::steady_clock> start;
        friend class UniformSerializer;
    public:
        explicit UniformTime(const std::string& name) noexcept;
        ~UniformTime() override = default;

        void update() noexcept;

        [[nodiscard]] bool& getChanged() noexcept override { changed = true; return changed; }

        [[nodiscard]] UniformTime* clone() noexcept override;
        void set(const ShaderProgram& shader) override;
    };

    std::string getUniformDeclaration(const Uniform& uniform) noexcept;
    size_t getUniformSize(const Uniform& uniform);
    size_t getUniformAlignment(const Uniform& uniform);

    bool operator==(const Uniform& lhs, const Uniform& rhs) noexcept;
    bool operator<(const Uniform& lhs, const Uniform& rhs) noexcept;
    bool operator!=(const Uniform& lhs, const Uniform& rhs) noexcept;
}

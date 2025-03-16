#pragma once

#include <vector>
#include <variant>
#include <iostream>
#include <type_traits>
#include "util.hpp"
#include "../lib/glad/glad.h"
#include "../lib/GLFW/glfw3.h"

#ifndef GL_UNSIGNED_BYTE
    #define GL_UNSIGNED_BYTE 0
#endif

#ifndef GL_UNSIGNED_SHORT
    #define GL_UNSIGNED_SHORT 1
#endif

#ifndef GL_UNSIGNED_INT
    #define GL_UNSIGNED_INT 2
#endif

namespace Math {
    template<typename T>
    concept validIndexType = (
        (std::is_same_v<std::decay_t<T>, uint8_t >) ||
        (std::is_same_v<std::decay_t<T>, uint16_t>) ||
        (std::is_same_v<std::decay_t<T>, uint32_t>)
    );

    enum class IndexType {
        ui8bit  = GL_UNSIGNED_BYTE ,
        ui16bit = GL_UNSIGNED_SHORT,
        ui32bit = GL_UNSIGNED_INT
    };

    template<std::integral T>
    constexpr auto getClosestValidIndexType() {
        if constexpr (sizeof(T) <= 1) return static_cast<uint8_t>(0);
        else if constexpr (sizeof(T) <= 2) return static_cast<uint16_t>(0);
        else return static_cast<uint32_t>(0);
    }

    template<std::integral T>
    using closestValidIndexType = std::remove_const_t<std::remove_reference_t<decltype(getClosestValidIndexType<T>())>>;

    class IndexList {
        private:
            static constexpr const std::array<IndexType, 3> possibleEnumTypes = {
                IndexType::ui8bit, IndexType::ui16bit, IndexType::ui32bit
            };

            std::variant<std::vector<uint8_t>, std::vector<uint16_t>, std::vector<uint32_t>> data;

        public:
            // Statics/free functions:

            friend std::ostream &operator<<(std::ostream &out, const IndexList &indexList)
            {
                out << "IndexList" << (indexList.getTypeByteDepth() * 8) << '{';
                for (size_t i = 0; i < indexList.getCount(); i++)
                {
                    if (i > 0) out << ',' << ' ';
                    out << indexList.getAt(i);
                }
                out << '}';

                return out;
            }

            template<validIndexType T, std::integral ...Args>
            static IndexList create(Args ...args) {
                return IndexList{static_cast<T>(args)...};
            }


            // Constructors:

            IndexList(): data(std::vector<uint8_t>()) {}

            template<std::integral T>
            IndexList(std::initializer_list<T> values) {
                std::vector<closestValidIndexType<T>> newData = {};
                newData.reserve(values.size());
                for (auto i: values) newData.emplace_back(i);
                this->data = newData;
            }

            template<std::integral T, size_t spanCount>
            IndexList(std::span<const T, spanCount> data) {
                this->setData(data);
            }

            template<std::integral T>
            IndexList(const std::vector<T> &vectorData) {
                this->setData(std::span(vectorData));
            }

            template<validIndexType T>
            IndexList(std::vector<T> &&vectorData) {
                this->data = std::move(vectorData);
            }

            IndexList(const IndexList &other): data(other.data) {}

            IndexList(IndexList &&other): data(std::move(other.data)) {}


            // Indices getters setters:

            template<std::integral T, size_t spanCount>
            void setData(std::span<const T, spanCount> data) {
                this->data = std::vector<closestValidIndexType<T>>();
                this->ensureCapacity(data.size());

                for (size_t i = 0; i < data.size(); ++i) {
                    this->add(data[i]);
                }
            }

            uint32_t getAt(size_t index) const
            {
                return std::visit(
                    ([=](const auto &v) -> uint32_t {return v.at(index);}),
                    data);
            }

            void setAt(size_t index, uint32_t newValue)
            {
                return std::visit(
                    ([=](auto &v) -> void {v[index] = newValue;}),
                    data);
            }

            void ensureCapacity(size_t minimumCapacity)
            {
                if (minimumCapacity <= (this->getCapacity())) return;
                else return std::visit(
                    ([=](auto &v) -> void {v.reserve(minimumCapacity);}),
                    data);
            }

            void add(uint32_t newValue)
            {
                return std::visit(
                    ([=](auto &v) -> void {v.emplace_back(newValue);}),
                    data);
            }

            void removeLast()
            {
                return std::visit(
                    ([=](auto &v) -> void {v.pop_back();}),
                    data);
            }

            void insert(size_t index, uint32_t newValue)
            {
                return std::visit(
                    ([=](auto &v) -> void {v.insert((v.begin() + index), newValue);}),
                    data);
            }

            void insert(size_t index, size_t count, uint32_t newValue) {
                std::visit(
                    ([=](auto &v) -> void {v.insert((v.begin() + index), count, newValue);}),
                    data);
            }

            void removeAt(size_t index)
            {
                return std::visit(
                    ([=](auto &v) -> void {v.erase(v.begin() + index);}),
                    data);
            }

            void removeAt(size_t begin, size_t count)
            {
                return std::visit(
                    ([=](auto &v) -> void {
                        v.erase((v.begin() + begin), (v.begin() + begin + count));}),
                    data);
            }

            void clear() {
                std::visit(([](auto &v) -> void {v.clear();}), data);
            }


            // Read general object data:

            void *getDataPtr()
            {
                return std::visit(([=](auto &v) -> void * {return v.data();}), data);
            }

            const void *getDataPtr() const
            {
                return std::visit(([=](const auto &v) -> const void * {return v.data();}), data);
            }

            size_t getCount() const
            {
                return std::visit(
                    ([=](const auto &v) -> size_t {return v.size();}),
                    data);
            }

            size_t getCapacity() const
            {
                return std::visit(
                    ([=](const auto &v) -> size_t {return v.capacity();}),
                    data);
            }

            size_t getTypeByteDepth() const
            {
                static constexpr size_t possibleResults[3] = {sizeof(uint8_t),sizeof(uint16_t),sizeof(uint32_t)};
                return (possibleResults[data.index()]);
            }

            size_t getMemorySize() const {return ((getCount()) * (getTypeByteDepth()));}

            IndexType getTypeEnum() const {return (possibleEnumTypes[data.index()]);}


            // Change number type:

            void setTypeEnum(size_t newTypeEnum)
            {
                switch (newTypeEnum)
                {
                    case GL_UNSIGNED_BYTE:  return (void)setTypeEnumTypeSpecific<uint8_t>();
                    case GL_UNSIGNED_SHORT: return (void)setTypeEnumTypeSpecific<uint16_t>();
                    case GL_UNSIGNED_INT:   return (void)setTypeEnumTypeSpecific<uint32_t>();
                    default: std::cerr << "Invalid index type enum \'" << newTypeEnum << "\'\n";
                }
            }

            template<validIndexType T>
            void setTypeEnumTypeSpecific()
            {
                std::vector<T> newVector = {};
                size_t count = this->getCount();

                newVector.reserve(std::max(count, this->getCapacity()));
                for (size_t i = 0; i < count; ++i) newVector.emplace_back(static_cast<T>(this->getAt(i)));

                this->data = std::move(newVector);
            }


            // Operators:

            IndexList &operator=(const IndexList &other)
            {
                this->data = other.data;
                return (*this);
            }

            IndexList &operator=(IndexList &&other)
            {
                if ((this) != (&other))
                {
                    this->data = std::move(other.data);
                }

                return (*this);
            }

            template<std::integral T, size_t spanCount>
            IndexList &operator=(std::span<const T, spanCount> data) {
                this->setData(data);
                return (*this);
            }

            template<std::integral T>
            IndexList &operator=(const std::vector<T> &vectorData) {
                this->setData(std::span(vectorData));
                return (*this);
            }

            template<validIndexType T>
            IndexList &operator=(std::vector<T> &&vectorData) {
                this->data = std::move(vectorData);
                return (*this);
            }
    };
}
#pragma once

#include "json.hpp"

#include <vector>
#include <cassert>

using nlohmann::json;

template <typename T>
class Grid2D
{
public:
    Grid2D() = default;
    Grid2D(size_t width, size_t height, T fill);
    Grid2D(size_t width, size_t height, const std::vector<T>& fill);

    void set(size_t x, size_t y, T value);
    T& get(size_t x, size_t y);
    const T& get(size_t x, size_t y) const;

    size_t getWidth() const;
    size_t getHeight() const;

private:
    size_t width {};
    size_t height {};

    size_t toIndex(size_t x, size_t y) const;
    std::vector<T> m_data;

    template <typename T_>
    friend void to_json(json& j, const Grid2D<T_>& g);

    template <typename T_>
    friend void from_json(const json& j, Grid2D<T_>& g);
};

template <typename T>
inline Grid2D<T>::Grid2D(size_t width, size_t height, T fill)
    : width(width), height(height), m_data(width * height, fill)
{

}

template <typename T>
inline Grid2D<T>::Grid2D(size_t width, size_t height, const std::vector<T>& fill)
    : width(width), height(height), m_data(fill)
{
    assert(m_data.size() == width * height);
}

template <typename T>
inline void Grid2D<T>::set(size_t x, size_t y, T value)
{
    m_data.at(toIndex(x, y)) = value;
}

template <typename T>
inline T& Grid2D<T>::get(size_t x, size_t y)
{
    return m_data.at(toIndex(x, y));
}

template <typename T>
inline const T& Grid2D<T>::get(size_t x, size_t y) const
{
    return m_data.at(toIndex(x, y));
}

template <typename T>
inline size_t Grid2D<T>::getWidth() const
{
    return width;
}

template <typename T>
inline size_t Grid2D<T>::getHeight() const
{
    return height;
}

template <typename T>
inline size_t Grid2D<T>::toIndex(size_t x, size_t y) const
{
    assert(x < width && y < height);
    return y * width + x;
}

template <typename T_>
inline void to_json(json& j, const Grid2D<T_>& g)
{
    j = json{
        {"width", g.width},
        {"height", g.height},
        {"data", g.m_data}
    };
}

template <typename T_>
inline void from_json(const json& j, Grid2D<T_>& g)
{
    j.at("width").get_to(g.width);
    j.at("height").get_to(g.height);
    j.at("data").get_to(g.m_data);
}

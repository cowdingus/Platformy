#pragma once

#include "json.hpp"

#include <stdexcept>
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
    size_t m_width {};
    size_t m_height {};

    size_t toIndex(size_t x, size_t y) const;
    std::vector<T> m_data;

    template <typename T_>
    friend void to_json(json& j, const Grid2D<T_>& g);

    template <typename T_>
    friend void from_json(const json& j, Grid2D<T_>& g);
};

template <typename T>
inline Grid2D<T>::Grid2D(size_t width, size_t height, T fill)
    : m_width(width), m_height(height), m_data(width * height, fill)
{

}

template <typename T>
inline Grid2D<T>::Grid2D(size_t width, size_t height, const std::vector<T>& fill)
    : m_width(width), m_height(height), m_data(fill)
{
    assert(m_data.size() == width * height);
}

template <typename T>
inline void Grid2D<T>::set(size_t x, size_t y, T value)
{
    if (x > m_width || y > m_height) throw std::out_of_range("Given coordinates exceed bounds");
    m_data.at(toIndex(x, y)) = value;
}

template <typename T>
inline T& Grid2D<T>::get(size_t x, size_t y)
{
    if (x > m_width || y > m_height) throw std::out_of_range("Given coordinates exceed bounds");
    return m_data.at(toIndex(x, y));
}

template <typename T>
inline const T& Grid2D<T>::get(size_t x, size_t y) const
{
    if (x > m_width || y > m_height) throw std::out_of_range("Given coordinates exceed bounds");
    return m_data.at(toIndex(x, y));
}

template <typename T>
inline size_t Grid2D<T>::getWidth() const
{
    return m_width;
}

template <typename T>
inline size_t Grid2D<T>::getHeight() const
{
    return m_height;
}

template <typename T>
inline size_t Grid2D<T>::toIndex(size_t x, size_t y) const
{
    assert(x < m_width && y < m_height);
    return y * m_width + x;
}

template <typename T_>
inline void to_json(json& j, const Grid2D<T_>& g)
{
    j = json{
        {"width", g.m_width},
        {"height", g.m_height},
        {"data", g.m_data}
    };
}

template <typename T_>
inline void from_json(const json& j, Grid2D<T_>& g)
{
    j.at("width").get_to(g.m_width);
    j.at("height").get_to(g.m_height);
    j.at("data").get_to(g.m_data);
}

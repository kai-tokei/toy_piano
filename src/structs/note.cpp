#pragma once
#include <Siv3D.hpp> // Siv3D v0.6.15

struct Note
{
    String key;
    size_t oct;

    bool operator==(const Note &note) const
    {
        return (note.key == key) and (note.oct == oct);
    }

    bool operator!=(const Note &note) const
    {
        return (note.key != key) and (note.oct != oct);
    }
};

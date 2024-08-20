#pragma once
#include <Siv3D.hpp> // Siv3D v0.6.15
#include "scenes/title.cpp"
#include "scenes/game.cpp"
#include "./structs/note.cpp"
#include "./objects/toy_piano.cpp"

using App = SceneManager<String>;

void Main()
{
	ToyPiano toyPiano;

	while (System::Update())
	{
		Note note = toyPiano.analize();

		ClearPrint();
		if (toyPiano.isDown())
			Print << U"{} {}"_fmt(note.key, note.oct);
	}
}

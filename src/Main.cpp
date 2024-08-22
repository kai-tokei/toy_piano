#pragma once
#include <Siv3D.hpp> // Siv3D v0.6.15
#include "scenes/title.cpp"
#include "scenes/game.cpp"
#include "./structs/note.cpp"
#include "./objects/toy_piano.cpp"

using App = SceneManager<String>;

void Main()
{
	const Font font{50};
	ToyPiano toyPiano;
	Array<Note> buffer{Note{U"A", 0}};
	Array<Note> notes;
	double input_timer = 0;

	while (System::Update())
	{
		input_timer += Scene::DeltaTime();

		buffer = notes;
		notes = toyPiano.analyze();
		for (Note note : notes)
		{
			if (note == Note{U"A", 0})
			{
				if (input_timer > 0.5)
				{
					ClearPrint();
					input_timer = 0;
				}
			}
			else if (not buffer.contains(note))
			{
				Print << U"{} {} {}"_fmt(note.key, note.oct, note.size);
				input_timer = 0;
			}
		}
		// font(U"{} {} {}"_fmt(note.key, note.oct, note.size)).draw(20, 20);
	}
}

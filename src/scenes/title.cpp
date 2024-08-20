# include <Siv3D.hpp> // Siv3D v0.6.13

using App = SceneManager<String>;

class Title : public App::Scene
{
	public:
		Title (const InitData& init)
			: IScene { init }
		{

		}

		~Title () {}

	private:
};

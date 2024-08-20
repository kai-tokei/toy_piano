# include <Siv3D.hpp>

using App = SceneManager<String>;

class Game : public App::Scene
{
	public:
		Game (const InitData& init)
			: IScene { init }
		{

		}

		~Game () {}

	private:
};

class X1Intro : public amy::Display
{
	public:
		X1Intro();
		~X1Intro();

		void on_enter( sf::RenderWindow& );
		void updates ( sf::RenderWindow& );
		void on_exit ( sf::RenderWindow& );
};
class X3Seahorse : public amy::Display
{
	public:
		X3Seahorse();
		~X3Seahorse();

		void on_enter( sf::RenderWindow& );
		void updates ( sf::RenderWindow& );
		void on_exit ( sf::RenderWindow& );
};
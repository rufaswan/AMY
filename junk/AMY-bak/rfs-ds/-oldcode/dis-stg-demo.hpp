class STGDemo : public amy::Display
{
	public:
		STGDemo();
		~STGDemo();

		void updates( sf::RenderWindow& );
	private:
		amy::BGFollow layer1;
		amy::BGStatic layer2;
};

#ifndef AMY_KEY
#define AMY_KEY

#include ""

namespace amy
{
	class Key
	{
		public:
			Key();
			~Key();

			bool is_press;
			bool is_hold;
			void tick();
			sf::Keyboard::Key  kb;
		protected:
		private:
	};
}

#endif

namespace amy
{
	class iVect
	{
		public:
			iVect();
			virtual ~iVect();
			unsigned int size;
			bool locked;
			std::vector <int> ivec;

			void assign(int[], unsigned int size, bool lock = false);
			int at(unsigned int);
			int first();
			int last();
			void clear();
			bool empty();
		protected:
		private:
	};
}

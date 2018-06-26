namespace amy
{
	class FileLoad
	{
		public:
			FileLoad();
			~FileLoad();

			void load  ( const std::string&, const std::string& );
			void unload( const std::string&, const std::string& );

			std::map < std::string, amy::FileData > data;
	};
}


#ifndef RFS_FILE
#define RFS_FILE

#include "defs.hpp"

namespace rfs
{
	class File
	{
		public:
			File();
			File( const ascii &file );
			~File();

			bool  fnew  ( const ascii &file );
			bool  fopen ( const ascii &file );
			uint  fsize ();
			uint  fget_uint ( uint pos, uint byte, bool bigend=false );
			int   fget_int  ( uint pos, uint byte, bool bigend=false );
			ascii fget_ascii( uint pos, uint byte=0 );
			void  fput_uint ( uint n, uint pos, uint byte, bool bigend=false );
			void  fput_int  (  int n, uint pos, uint byte, bool bigend=false );
			void  fput_ascii( const ascii &str, uint pos,  uint byte=0 );
			void  fseek ( uint pos );
			void  fclose();

		protected:
		private:

			FILE* fp;
			uint  fs;
	};
}

#endif

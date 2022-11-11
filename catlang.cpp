//--------------------------------------------
// CAT PROGRAMMING LANGUAGE
// author: 	Huan Xu
// credit to: BigZaphod sean@fifthace.com
//--------------------------------------------
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

typedef std::vector<int> mem_t;
mem_t program;
mem_t memory;
mem_t::iterator mem_pos;
mem_t::iterator prog_pos;

int register_val;
bool has_register_val = false;

void quit( bool error )
{
    if( error )
    {
        printf( "\nERROR!\n" );
        exit(1);
    }
}

bool exec( int instruction )
{

    switch( instruction )
    {
    // meow
    case 0:
        {
            if( prog_pos == program.begin() )
                quit( true );

            prog_pos--;	// skip previous command.
            int level = 1;
            while( level > 0 )
            {
                if( prog_pos == program.begin() )
                    break;

                prog_pos--;
            
                if( (*prog_pos) == 0 )
                    level++;
                else
                if( (*prog_pos) == 7 )  // look for mEOW
                    level--;
            }

            if( level != 0 )
                quit(true);

            return exec( *prog_pos );
        }
    
    // meOw
    case 1:
        if( mem_pos == memory.begin() )
            quit( true );
        else
            mem_pos--;
        break;

    // meoW
    case 2:
        mem_pos++;
        if( mem_pos == memory.end() )
        {
            memory.push_back(0);
            mem_pos = memory.end();
            mem_pos--;
        }
        break;
    
    // meOW
    case 3:
        if( (*mem_pos) == 3 )
            quit( false );
        return exec(*mem_pos);
    
    // mEow
    case 4:
        if( (*mem_pos) != 0 )
            printf( "%c", *mem_pos );
        else
        {
            (*mem_pos) = getchar();
            while( getchar() != '\n' );
        }
        break;
    
    // mEOw
    case 5:
        (*mem_pos)--;
        break;
    
    // mEoW
    case 6:
        (*mem_pos)++;
        break;

    // mEOW
    case 7:
        if( (*mem_pos) == 0 )
        {
            int level = 1;
            int prev = 0;
            prog_pos++;	  // have to skip past next command when looking for next meow.
            if( prog_pos == program.end() )
                break;
            while( level > 0 )
            {
                prev = *prog_pos;
                prog_pos++;
                
                if( prog_pos == program.end() )
                    break;
                
                if( (*prog_pos) == 7 )
                    level++;
                else
                if( (*prog_pos) == 0 )	// look for meow command.
                {
                    level--;
                    if( prev == 7 )
                        level--;
                }
            }
            if( level != 0 )
                quit( true );
        }
        break;
    
    // Meow
    case 8:
        (*mem_pos) = 0;
        break;

    // MEow
    case 9:
        if( has_register_val )
            (*mem_pos) = register_val;
        else
            register_val = (*mem_pos);
        has_register_val = !has_register_val;
        break;

    // MeOw
    case 10:
        printf( "%d\n", *mem_pos );
        break;
    
    // MeoW
    case 11:
        {
            char buf[100];
            int c = 0;
            while( c < sizeof(buf)-1 )
            {
                buf[c] = getchar();
                c++;
                buf[c] = 0;
                
                if( buf[c-1] == '\n' )
                    break;
            }
            // swallow, just in case.
            if( c == sizeof(buf) )
                while( getchar() != '\n' );
            
            (*mem_pos) = atoi( buf );

            break;
        }

    // bad stuff
    default:
        quit( false );
    };

    prog_pos++;

    return true;
}


int main( int argc, char** argv )
{
	if( argc < 2 )
	{
		printf( "Usage: %s program.cat\n\n", argv[0] );
		exit( 1 );
	}

	FILE* f = fopen( argv[1], "rb" );

	if( f == NULL )
	{
		printf( "Cannot open source file [%s].\n", argv[1] );
        exit( 1 );
	}

    char buf[4];
    memset( buf, 0, 4 );
    int pos = 0;

    while( !feof(f) )
    {
        int found = 0;
        buf[3] = fgetc( f );

        if( found = !strncmp( "meow", buf, 4 ) )
            program.push_back( 0 );
        else if( found = !strncmp( "meOw", buf, 4 ) )
            program.push_back( 1 );
        else if( found = !strncmp( "meoW", buf, 4 ) )
            program.push_back( 2 );
        else if( found = !strncmp( "meOW", buf, 4 ) )
            program.push_back( 3 );
        else if( found = !strncmp( "mEow", buf, 4 ) )
            program.push_back( 4 );
        else if( found = !strncmp( "mEOw", buf, 4 ) )
            program.push_back( 5 );
        else if( found = !strncmp( "mEoW", buf, 4 ) )
            program.push_back( 6 );
        else if( found = !strncmp( "mEOW", buf, 4 ) )
            program.push_back( 7 );
        else if( found = !strncmp( "Meow", buf, 4 ) )
            program.push_back( 8 );
        else if( found = !strncmp( "MEow", buf, 4 ) )
            program.push_back( 9 );
        else if( found = !strncmp( "MeOw", buf, 4 ) )
            program.push_back( 10 );
        else if( found = !strncmp( "MeoW", buf, 4 ) )
            program.push_back( 11 );
            
        if( found )
        {
            memset( buf, 0, 4 );
        }
        else
        {
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = buf[3];
            buf[3] = 0;
        }
    }

	fclose( f );


    // init main memory.
    memory.push_back( 0 );
    mem_pos = memory.begin();

    prog_pos = program.begin();
    while( prog_pos != program.end() )
        if( !exec( *prog_pos ) )
            break;

    quit( false );

	return 0;
}



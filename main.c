#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*          VM       */
typedef struct Var
{
    union data
    {
        double d;
        int src_index;
        char c;      
    }data;
    enum {Int,Double,Char}data_type;
}Var;

struct Frame
{   
    Var *local_field;
    Var return_var;
};

/*			 Lexer	*/
typedef struct Token
{
	char *value;

	enum TokenType 
    {
        VAR,
        ADD,
        SUB,
        MUL,
        DIV,
        EQL,
        CMP,
        IF,
        ELSE,
        ELIF,
        FOR
    }token_type;
}Token;


Token* lexer(const char* source)
{
    Token *token_stream = calloc(256,sizeof(Token));
    static unsigned int src_index    = 0;
		   unsigned int stream_index = 0;
			int line = 1;

        while(source[src_index]!='\0')
		{
          	// Skip whitespaces between tokens
            if(!isspace(source[src_index]))
            {
				switch(source[src_index])
				{
					case '=':
					{
						token_stream[stream_index].token_type = EQL;
						stream_index++;
						puts("EQL");
						break;
					}
					case '+':
					{					
						token_stream[stream_index].token_type = ADD;
						stream_index++;
						puts("ADD");
						break;
					}
					default:
					{
						char tbuffer[50] = "";
						unsigned int t_index = 0;
						while(isalnum(source[src_index]))
						{
							tbuffer[t_index] = source[src_index];
							t_index++;
							src_index++;
						}
						src_index--;
						if(strcmp(tbuffer,"var"))
						{
							token_stream[stream_index].token_type = VAR;
						}
						else if(token_stream[stream_index].token_type == VAR)
						{	// Assuming last token is variable, assign its value

							token_stream[stream_index].value = tbuffer;
							stream_index++;
						}						
						else
						{
							// Otherwise, call any exceptions and return 1;
							printf("possible error line %d",line);
							return 1;
						}
					printf("(%s)\n",tbuffer);							
					}	
				}
            }
            src_index++;  
			if(source[src_index]=='\n')
			{
				line++;
			}     
        }
      return token_stream;
}
/*		Parser		*/

void parser(Token *stream);

/***************************/
char* loadfile(const char* filename);


int main()
{
    const char *source = loadfile("example.txt");
    lexer(source);
	
	
    return 0;
}

char* loadfile(const char* filename)
{
    char *source;
    FILE *f = fopen(filename, "r");
    
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);
    source = malloc(fsize + 1);
    fread(source, fsize, 1, f);
    fclose(f);
    source[fsize] = 0;
    return source;
}


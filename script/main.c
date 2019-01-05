#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define VAR_SIZE 4

typedef struct Var
{
    union data
    {
        double d;
        int src_index;
        char c;      
    }data;
    enum {Int,Double,Char}type_name;
}Var;
// VM
struct Frame
{   
    Var *local_field;
    Var return_var;
};

// Lexer
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

typedef enum Token 
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
    }Token;

    

Token* lexer(const char* source)
{
    Token *token_stream = calloc(256,sizeof(Token));

    static unsigned int tpos_begin,tpos_end,src_index,stream_index = 0;
    
        while(source[src_index]!='\0')
        {

            if(source[src_index] == '=')
            {
                token_stream[stream_index] = EQL;
                stream_index++;
            }
            else if(source[src_index] == '+')
            {
                token_stream[stream_index] = ADD;
                stream_index++;
            }
            else if(!isspace(source[src_index]))
            {
                tpos_begin = src_index;
                char buffer[256] = "";
                unsigned int buffer_index = 0;
                while(!isspace(source[src_index]))
                {
                    buffer[buffer_index] = source[src_index];
                    buffer_index++;
                    src_index++;
                }
                printf("token = (%s)\n",buffer);


            }
            src_index++;       
        }
    
      return token_stream;
}

int main(int argc, char **argv)
{
    const char *source = loadfile("example.txt");
    lexer(source);
    
    return 0;
}

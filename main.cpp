#include <iostream>
#include <fstream>
#include <sstream>


enum class TokenType{ /// create a user defined datatype using enum class containing the names of the token as type
    _return,/// return type
    int_lit,/// integer literal
    semi ///semi colon
};

struct Token{
    TokenType type;
    std::optional<std::string> value {};
};

std::string token_asm(std::vector<Token> &tokens){
    std::stringstream assembly;
    assembly<<"global\n_start\n";

    for(int i= 0;i<tokens.size();i++){
        const Token &token = tokens.at(i);
        if (token.type==TokenType::_return){
            if((i+1)<tokens.size()&& tokens.at(i+1).type== TokenType::int_lit){
                if((i+2)<tokens.size()&& tokens.at(i+2).type== TokenType::semi){
                    assembly<<"    mov 60\n";
                    assembly<<"    mov rdi, "<< tokens.at(i+1).value.value()<<"\n";
                    assembly<<"    syscall";
                }
            }
        }
    }
    return assembly.str();
}

std::vector<Token> tokenize(const std::string &str){
    std::vector<Token> tokens;/// create a vector array of type Token
    std::string buf ;/// initializing string buf to contain the each character of the code we write in our language
    for(int i = 0 ;i<str.length();i++) { /// loop to go through each character of the code
        char c = str.at(i);

        if (std::isalpha(c)) {/// check if its alphabet cuz first letter of the identifier must be alphabetic
            buf.push_back(c);/// push the character into the buffer
            i++;
            while (std::isalnum(str.at(i))) {/// if first letter is alphabetic then the rest can be alphabet and number sooo
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            if (buf =="return") { /// if buffer contain the string return then
                tokens.push_back({TokenType::_return});/// push the token return into the token vector denoting that a
                /// token named return is identified
                buf.clear();/// you are using the buffer to check for valid identifier or token so each time you put something in the
                continue;///buffer you need to clear it after use so that you can load next character for checking
            } else {
                std::cout << "you fucked up buddy1" << std::endl;
                exit(EXIT_FAILURE);
            }

        } else if(std::isdigit(c)) {/// if the continueous string is disrupted by an unknown character  like white space then
            buf.push_back(c);/// its is checked with if its a digit then only its is checked with white space
            i++;
            while (std::isdigit(str.at(i))) {/// contineously taking in the digits if there is more in the code

                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({ TokenType::int_lit,.value=buf});/// push the token int_lit to the token vector to denote that it has identified
            buf.clear();
        }
        else if (c == ';'){
            tokens.push_back({TokenType::semi});/// same as for semicolon
            buf.clear();
        }


        else if (std::isspace(c)) {
            continue;/// and finally for white space note: this is done after checking

        }
//        else{
//        std::cout << "you fucked up buddy2" << std::endl;
//        exit(EXIT_FAILURE);
//    }


    }
    return tokens;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "incorrect man" << std::endl;
        std::cerr << "type in two arguments " << std::endl;
        return EXIT_FAILURE;
    }
    std::string content;
    {
        std::fstream input(argv[1], std::ios::in);  /// getting the file containing the code of the custom language that
        std::stringstream contentS;/// we created using fstream
        contentS << input.rdbuf();/// put the contents of the file into the read buffer
        content = contentS.str();/// convert the contents of the file into strings
    }
//    std::cout<< content<<std::endl;
   std::vector<Token> things = tokenize(content);///call the function tokenize and pass the string in content variable
   std::cout<<token_asm(things)<<std::endl;
    {
        std::fstream file("asm.out",std::ios::out);
        file<<token_asm(things);
    }


    return EXIT_SUCCESS;
}

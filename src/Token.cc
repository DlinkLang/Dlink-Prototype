/**
 * @file Token.cc
 * @date 2017.08.02
 * @author dev_kr
 * @brief Dlink의 렉서가 반환하는 토큰 타입을 정의합니다.
 */

#include "Token.hh"


namespace Dlink
{
    #define MAP_TOKEN(token) std::pair<TokenType, std::string>(TokenType::token, #token)

    const std::map<TokenType, std::string> token_map =
    {
        MAP_TOKEN(none),               
        MAP_TOKEN(eof),               
        MAP_TOKEN(identifier),         
        MAP_TOKEN(bin_integer),        
        MAP_TOKEN(oct_integer),        
        MAP_TOKEN(dec_integer),        
        MAP_TOKEN(hex_integer),        
        MAP_TOKEN(floating),
        MAP_TOKEN(character),          
        MAP_TOKEN(string),             
        MAP_TOKEN(plus),               
        MAP_TOKEN(increment),          
        MAP_TOKEN(plus_assign),        
        MAP_TOKEN(minus),              
        MAP_TOKEN(decrement),          
        MAP_TOKEN(minus_assign),       
        MAP_TOKEN(multiply),           
        MAP_TOKEN(divide),             
        MAP_TOKEN(divide_assign),      
        MAP_TOKEN(modulo),             
        MAP_TOKEN(modulo_assign),      
        MAP_TOKEN(assign),             
        MAP_TOKEN(equal),              
        MAP_TOKEN(noteq),              
        MAP_TOKEN(greater),            
        MAP_TOKEN(eqgreater),          
        MAP_TOKEN(less),               
        MAP_TOKEN(logic_and),          
        MAP_TOKEN(logic_or),           
        MAP_TOKEN(bit_not),            
        MAP_TOKEN(bit_and),            
        MAP_TOKEN(bit_and_assign),     
        MAP_TOKEN(bit_or),             
        MAP_TOKEN(bit_or_assign),      
        MAP_TOKEN(bit_xor),            
        MAP_TOKEN(bit_xor_assign),     
        MAP_TOKEN(bit_lshift),         
        MAP_TOKEN(bit_lshift_assign),  
        MAP_TOKEN(bit_rshift),         
        MAP_TOKEN(bit_rshift_assign),  
        MAP_TOKEN(lbrace),             
        MAP_TOKEN(rbrace),             
        MAP_TOKEN(lparen),             
        MAP_TOKEN(rparen),             
        MAP_TOKEN(lbparen),            
        MAP_TOKEN(rbparen),            
        MAP_TOKEN(dot),                
        MAP_TOKEN(comma),              
        MAP_TOKEN(apostrophe),         
        MAP_TOKEN(semicolon),          
        MAP_TOKEN(colon),              
        MAP_TOKEN(exclamation),        
        MAP_TOKEN(question),           
        MAP_TOKEN(_if),                
        MAP_TOKEN(_else),              
        MAP_TOKEN(_switch),            
        MAP_TOKEN(_case),              
        MAP_TOKEN(_for),               
        MAP_TOKEN(_while),             
        MAP_TOKEN(_do),                
        MAP_TOKEN(_break),             
        MAP_TOKEN(_continue),          
        MAP_TOKEN(_return),            
        MAP_TOKEN(_null),              
        MAP_TOKEN(_const),             
    };

    #undef MAP_TOKEN
}

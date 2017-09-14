#include <iostream>
#include <map>
#include <sstream>
#include <string>

typedef int Texp;
typedef int Tkoef;
    
typedef std::map<Texp,Tkoef> Tpoly;
typedef std::string::iterator Tsi; 

bool IsNumeric (char c){
    return c>='0' && c<='9';
}

bool IsAlpha (char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool stoi (Tsi istart, Tsi iend,  int& result){
    //skipt space
    while (istart != iend && *istart==' '  ) ++istart;
        
    if (istart == iend) return  false;
    
    //parse first number
    if  (!IsNumeric(*istart)) return  false; 
    result*= (*istart - '0');
    ++istart;
    
    //parse next number
    for (;istart!=iend;++istart) {
        // '999  ' skipt space in end 
       if  ((*istart)==' ')  break; 
       
       if  (!IsNumeric(*istart)) return  false; 
       result*=10;
       result+= (*istart - '0');
    }; 
    
    // '999  ' skipt space in end 
    for (;istart!=iend;++istart) {
        // '999  xxx' skipt spase in end 
       if  ( (*istart)!=' ') return  false; 
    };    
    return  true;   
}



void put(Tkoef koef, Texp  exp, Tpoly& result){ 
        
    if (result.count(exp) >0){
        result[exp]+=koef;        
    }
    else {
        result[exp]=koef;
    }
   
}
    
bool get(Tsi istart, Tsi iend, Tkoef& koef, Texp&  exp ){
    koef = 1;
    exp = 0;
    bool exist_var = false;
    for (Tsi i = istart; i != iend ; ++i) { 
        if ((*i == '-')  ){
           istart = i+1;  
           koef = -1;           
        } 
        else if ((*i == '+')  ){
           istart = i+1;           
        }         
        else if  ((*i == '*')  ){
            //get before i
           if (!stoi(istart,i,koef)) return  false;                      
        }        
        else if (IsAlpha(*i)) {
            exp = 1; // 
            exist_var =true;
        }                           
        else if ((*i == '^')  ){
            if (!exist_var) return  false; // because 9* ^2 it is a error
             //get after i
           if (!stoi(i+1,iend,exp)) return  false;   
           break; 
        }   
    }
        
    return true;
}

std::string derivative(std::string polynomial) {
    //std::cout << "derivative";    
    polynomial.append("\n");
    Tsi istart=polynomial.begin();
    
    Tpoly result;
    
    for (Tsi i = polynomial.begin(); i != polynomial.end() ; ++i) {
        if ((*i == '+') || (*i == '-') || (*i == '\n')  ){
            
           Tkoef koef;
           Texp  exp ;
           if (!get(istart, i, koef, exp )) return std::string();
           put(koef, exp, result );
           istart=i;
        }
        
    } 
    std::stringstream ss;
    bool is_first_element=true;
    for (Tpoly::reverse_iterator i = result.rbegin(); i != result.rend() ; ++i) {
        
        if ((*i).first == 0) continue; //for case:  dx (const)
        
        Texp new_exp = (*i).first - 1;
        Tkoef new_koef = (*i).first * (*i).second;
        
        if (new_koef >=0) {
            //because not need append first sign in start string: +10*x^5 +x^4
            if (!is_first_element) ss<< "+";         
        }
        //else sign negative '-' append automatic: -10*x^5 -x^4
        
        is_first_element =false;
        
        bool putKoeff= (new_koef !=1 && new_koef !=-1 ); // usually '1' was ommit
        bool putX = ( new_exp >=1 ); 
        // x not need if x^0 
                
        if (putKoeff || !putX)  ss<< new_koef; //if  !putX then koef need alwas  # dx(-x)=-1 or dx(x)=1
            
        
        if (putKoeff &&  putX) ss<< "*";
        //else  #  dx(2*x) = 2 therefore there "*" not need
        
        
        if (putX) ss<< "x";
        //else  #  dx(2*x) = 2 therefore there "x" not need
        
        if (new_exp >1) ss<< "^" << new_exp; 
        //else  #  dx(2*x^2) = 2*x therefore there "^" not need        
    }    
    return ss.str();
    
}

int main(){
    //2*x^100+100*x^2
    //std::string input;//="2*x^100+100*x^2";
    //std::cout << "enter polynomial: ";
    //std::cin >> input;
    //std::getline(std::cin, input);
    
    //std::cout << derivative (input);
    std::cout << std::boolalpha;
    /*std::cout << (derivative ("x^2+x")== "2*x+1") << '\n'; // 
    std::cout << (derivative ("2*x^100+100*x^2") == "200*x^99+200*x")<< '\n';
    std::cout << (derivative ("x^10000+x+1")== "10000*x^9999+1")<< '\n'; // 
    std::cout << (derivative ("-x^2-x^3") == "-3*x^2-2*x")<< '\n';
    std::cout << (derivative ("x+x+x+x+x+x+x+x+x+x") == "10")<< '\n';*/
    std::cout << (derivative ("+-9*-^2 "))<< '\n'; // == "-2*x+1"*/
    return 0;
}






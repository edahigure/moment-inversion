#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stack> 
#include <queue> 
#include <string.h>


using namespace std; 

char * newstr(int N);
int * newint(int N);
bool isSpace(char c);
bool isDelim(char c);
bool isLetter(char c);
bool isDigit(char c);

class  parser
{
   public:
        parser();
        ~parser(){};    
   void evaluate(char* expstr);    
   void print(void);  
   void handleErr(int);
   void show_output(void) ;
   char* get_rpn(void);
	int * get_type(void); 

   private:
  	int NONE;
	int DELIMITER;
	int FUNCTION;
	int NUMBER;
	int OPERATOR;
	int CONSTANT;
//sintax error types
	int SYNTAX;
	int UNBALPARENS;
	int NOEXP;
	int DIVBYZERO;
	int NOVAR;
   const char* EOE;
//this token indicate end of the expression
//	QString 	exp_result;
	char *exp;
	int expIdx;	
	char* token;	
	int *var;	
	char **var_names;
	int var_num;	
	int tokType;	
	void getToken(void); 

   int N_tokens;      

   queue <char*> queue_input;
   queue <int>   queue_input_type;
   queue <int>   queue_input_precedence;
   queue <char>  queue_input_associativity;

   stack <char*> stack_operator;
   stack <int>   stack_operator_type;
   stack <int>   stack_operator_precedence;
   stack <char>  stack_operator_associativity;

   queue <char*> queue_output;
   queue <int>   queue_output_type;
   queue <int>   queue_output_precedence;
   queue <char>  queue_output_associativity;



   int precedence(char *token,int tokType);
   char associativity(char *token,int tokType);
   bool questions(bool stack_op_empty,int operator_type, int operator_precedence, int input_precedence, char operator_associativity, char* operator_top);



};	

typedef class  parser parser;

void show_q(queue <char*> gq) ;
void show_q(queue <int> gq) ;
void show_q(queue <char> gq);


void show_s(stack <char*> gq) ;
void show_s(stack <int> gq) ;
void show_s(stack <char> gq);
void show_s(stack <double> gq);
#endif


#include <stdio.h>
#include <string.h>


char * newstr(int N)
{
  return (char*) malloc((N)*sizeof(char) );
}

int * new_array_i(int N)
{
  return (int*) malloc((N)*sizeof(int) );
}


char ** newstr_array(int N)
{
  return (char**) malloc((N)*sizeof(char[500]) );
}


bool isSpace(char c)
{
    bool ans;
    if(c==' ') {
    ans=true;
    }
    else {
    ans=false;
    }

    return ans;

}

bool isDelim(char c)
{
   char *delim;
   bool ans;
   ans =false;
   delim=newstr(9);
   strcpy(delim,"+-/*%^=()");
   for(int k=0;k<(int)strlen(delim);k++){
      if(c==delim[k]){
         ans=true;
      }
   }
   return ans;
}


bool isLetter(char c)
{
   char *letter;
   bool ans;
   ans =false;
   letter=newstr(54);
   strcpy(letter,"qwertyuiopasdfghjklñzxcvbnmQWERTYUIOPASDFGHJKLÑZXCVBNM");
   for(int k=0;k<(int)strlen(letter);k++){
      if(c==letter[k]){
         ans=true;
      }
   }
   return ans;
}


bool isDigit(char c)
{
   char *digit;
   bool ans;
   ans =false;
   digit=newstr(10);
   strcpy(digit,"1234567890");
   for(int k=0;k<(int)strlen(digit);k++){
      if(c==digit[k]){
         ans=true;
      }
   }
   return ans;
}
 

parser::parser()
{
	NONE=0;
	DELIMITER=1;   
	FUNCTION=2;
	NUMBER=3;
   OPERATOR=4;
   CONSTANT=5;


	SYNTAX=0;
	UNBALPARENS=1;
	NOEXP=2;
	DIVBYZERO=3;
	NOVAR=4;

	EOE="\0";
	var_num=0;
	var=new int[26];
}

void parser::print()
{
	NONE=0;
   printf("NONE: %d\nDELIMITER: %d\nFUNCTION: %d\nNUMBER:%d\n",NONE,DELIMITER,FUNCTION,NUMBER);
}



void parser::getToken(void)
{

   char str[500];

	tokType=NONE;
   token=newstr(500);
	strcpy(token,"");


   if(expIdx==(int)strlen(exp))
	{
		strcpy(token,EOE);
		return;
	}


	//omit white space.
   while( expIdx<(int)strlen(exp) &&    isSpace(exp[expIdx])==true ){
      ++expIdx;
   }

	// white space end of the expression
	if(expIdx == (int)strlen(exp))
	{
		strcpy(token,EOE);
		return;
	}



	if(  isDelim(exp[expIdx])==true  ) //is an operator
	{

      if(exp[expIdx]=='(' || exp[expIdx]==')' ) 
		tokType=DELIMITER;
      else
		tokType=OPERATOR;

      sprintf(token,"%s%c",token,exp[expIdx]);
		expIdx++;
	}
	else if( isLetter( exp[expIdx]  )==true )
	{
	   while(  isDelim( exp[expIdx] )==false )
	   {
         sprintf(token,"%s%c",token,exp[expIdx]);
	   	expIdx++;
 			if(expIdx>=(int)strlen(exp)) break;
 		}
 		tokType=FUNCTION; 	
      
      if(strcmp(token,"pi")==0)
 		tokType=CONSTANT; 	

 	}
 	else if( isDigit( exp[expIdx] )== true )
 	{
 		while( isDelim(exp[expIdx]) ==false )
 		{
         sprintf(token,"%s%c",token,exp[expIdx]);
 			expIdx++;
 			if(expIdx>=(int)strlen(exp))  break;
 		}
 		tokType=NUMBER;
 	}
 	else
 	{
      strcpy(token,EOE);
 		return;
 	}

}


void parser::handleErr(int error)
{
	const char *err[]={"Syntax Error",
						"Unbalanced Parentheses",
						"No expression Present",
						"Division by Zero","No FUNCTIONs"
						};
	printf("%s\n",err[error]);						
}


int parser::precedence(char *token,int tokType)
{
    int prec=0;
    if(tokType==OPERATOR)
    {
        if(strcmp(token,"^")==0)
        prec=4;                      
        if(strcmp(token,"*")==0)
        prec=3;                      
        if(strcmp(token,"/")==0)
        prec=3;                      
        if(strcmp(token,"%")==0)
        prec=3;                      

        if(strcmp(token,"+")==0)
        prec=2;                 
        if(strcmp(token,"-")==0)
        prec=2;                           
    }
    return prec;

}


char parser::associativity(char *token,int tokType)
{
    char assoc= 'N';

    if(tokType==OPERATOR)
    {
        if(strcmp(token,"^")==0)
        assoc='R';                      
        if(strcmp(token,"*")==0)
        assoc='L';                      
        if(strcmp(token,"/")==0)
        assoc='L';                      
        if(strcmp(token,"+")==0)
        assoc='L';                 
        if(strcmp(token,"-")==0)
        assoc='L';                           
    }
    return assoc;

}


bool parser::questions(bool stack_op_empty,int operator_type, int operator_precedence, int input_precedence, char operator_associativity, char* operator_top)
{

   bool bool_0=false,bool_1=false,bool_2=false;
   bool bool_5=false,bool_3=false,bool_4=false;

   bool_1=false; 
   if(stack_op_empty==false)
   bool_1=operator_type==FUNCTION;                          


   bool_2=false; 
   if(stack_op_empty==false)
   bool_2=(operator_precedence >input_precedence);         

   bool_3=false; 
   if(stack_op_empty==false)
   bool_3=(operator_precedence == input_precedence)&& operator_associativity=='L';

   bool_4=false;
   if(stack_op_empty==false){
     bool_4=strcmp(operator_top,")")!=0;        
   }

   bool_5=(bool_1 || bool_2 || bool_3)&&bool_4;


   return bool_5;

}


void parser::evaluate(char* expstr)
{
   queue <char*> * result;
   char *str;
   char* Token;
   int  Type;
   bool bool_5=false,bool_0=false;

   result=new queue <char*>();

   exp=newstr(1000);
   str=newstr(1000);



   if(expstr[0]=='(')  
   sprintf(exp,"%s%s","1*",expstr);
   else
   sprintf(exp,"%s",expstr);



	expIdx=0;
	getToken();

   

	 while(strcmp(token,EOE)!=0){	
       queue_input.push((char*)token);
       queue_input_type.push((int)tokType);
       queue_input_precedence.push(precedence(token,tokType));
       queue_input_associativity.push(associativity(token,tokType));
   	 getToken();
    }


    Token=newstr(300);
    strcpy(Token," ");
    Type=0;

    while (queue_input.empty()==false) 
    {         

        strcpy(Token,queue_input.front());
        Type=queue_input_type.front();
  
        if( queue_input_type.front()==NUMBER){

            queue_output.push((char*)queue_input.front());
            queue_output_type.push(queue_input_type.front());
            queue_output_precedence.push(queue_input_precedence.front());
            queue_output_associativity.push(queue_input_associativity.front());

              
        }

        if( queue_input_type.front()==CONSTANT){

            queue_output.push((char*)queue_input.front());
            queue_output_type.push(queue_input_type.front());
            queue_output_precedence.push(queue_input_precedence.front());
            queue_output_associativity.push(queue_input_associativity.front());

              
        }



        if( queue_input_type.front()==FUNCTION){
           stack_operator.push((char*)queue_input.front());
           stack_operator_type.push(FUNCTION);
           stack_operator_precedence.push(0);
           stack_operator_associativity.push('N');  
        }


        if( queue_input_type.front()==OPERATOR){

		     if(stack_operator.empty()==false){
		     bool_5=questions(stack_operator.empty(),stack_operator_type.top(),stack_operator_precedence.top(),
		              queue_input_precedence.front(),stack_operator_associativity.top(), stack_operator.top());
		     }
		     else bool_5=false;


           while(bool_5==true)
           {


               if(stack_operator.empty()==false)
               {
                    

                    queue_output.push(stack_operator.top());
                    queue_output_type.push(stack_operator_type.top());
                    queue_output_precedence.push(stack_operator_precedence.top());
                    queue_output_associativity.push(stack_operator_associativity.top());


                    stack_operator.pop(); 
                    stack_operator_type.pop(); 
                    stack_operator_precedence.pop(); 
                    stack_operator_associativity.pop();
                  
           
               }


		         if(stack_operator.empty()==false){
               bool_5=questions(stack_operator.empty(),stack_operator_type.top(),stack_operator_precedence.top(),
               queue_input_precedence.front(),stack_operator_associativity.top(), stack_operator.top());
					}
					else bool_5=false;

               
           }

           stack_operator.push(queue_input.front());
           stack_operator_type.push(queue_input_type.front());
           stack_operator_precedence.push(queue_input_precedence.front());
           stack_operator_associativity.push(queue_input_associativity.front());


           
        }

        if( strcmp(queue_input.front(),"(")==0){
            stack_operator.push((char*)queue_input.front());
            stack_operator_type.push(queue_input_type.front());
            stack_operator_precedence.push(queue_input_precedence.front());
            stack_operator_associativity.push(queue_input_associativity.front());

              
        }

        if( strcmp(queue_input.front(),")")==0){
             while(strcmp(stack_operator.top(),"(")!=0) {
               queue_output.push(stack_operator.top());
               queue_output_type.push(stack_operator_type.top());
               queue_output_precedence.push(stack_operator_precedence.top());
               queue_output_associativity.push(stack_operator_associativity.top());


				   stack_operator.pop(); 
				   stack_operator_type.pop(); 
				   stack_operator_precedence.pop(); 
				   stack_operator_associativity.pop();
               if( stack_operator.empty()==true )
               printf("error: mismathched parenthesis\n");
            }
            while(strcmp(stack_operator.top(),"(")==0) {
				   stack_operator.pop(); 
				   stack_operator_type.pop(); 
				   stack_operator_precedence.pop(); 
				   stack_operator_associativity.pop();
            }

        }
      


 	     queue_input.pop(); 
	     queue_input_type.pop(); 
	     queue_input_precedence.pop(); 
	     queue_input_associativity.pop();

    } 

    while(stack_operator.empty()==false) {

 
      queue_output.push(stack_operator.top());
      queue_output_type.push(stack_operator_type.top());
      queue_output_precedence.push(stack_operator_precedence.top());
      queue_output_associativity.push(stack_operator_associativity.top());


      if(strcmp(stack_operator.top(),")")==0 ||strcmp(stack_operator.top(),"(")==0)
      printf("mismatched parentesis\n");

	   stack_operator.pop(); 
	   stack_operator_type.pop(); 
	   stack_operator_precedence.pop(); 
	   stack_operator_associativity.pop();

   }


}
  

char* parser::get_rpn(void)
{
     int n;
     char* out;
     n=(int)queue_output.size();
     out=newstr(500);
     strcpy(out," ");
     while(queue_output.empty()==false)
     {
          sprintf(out,"%s %s",out,queue_output.front());
          queue_output.pop();
 
     }
     return out;     
}


int* parser::get_type(void)
{
     int n;
     int* out;
     n=(int)queue_output.size();
     out=new_array_i(n);

     for(int k=0;k<n;k++)
     {
         out[k]=queue_output_type.front();
         queue_output_type.pop();
 
     }
     return out;
     
}


void parser::show_output(void)
{
     show_q(queue_output);
}

void show_s(stack <char*> gq) 
{ 
    stack <char*> g = gq; 
    while (!g.empty()) 
    { 
        printf("%s ",(char*)g.top());
        g.pop(); 
    } 
    printf("\n");
} 


void show_s(stack <int> gq) 
{ 
    stack <int> g = gq; 
    while (!g.empty()) 
    { 
        printf("%d ",(int)g.top());
        g.pop(); 
    } 
    printf("\n");
} 

void show_s(stack <double> gq) 
{ 
    stack <double> g = gq; 
    while (!g.empty()) 
    { 
        printf("%f ",(double)g.top());
        g.pop(); 
    } 
    printf("\n");
} 



void show_s(stack <char> gq) 
{ 
    stack <char> g = gq; 
    while (!g.empty()) 
    { 
        printf("%c ",(char)g.top());
        g.pop(); 
    } 
    printf("\n");
} 


void show_q(queue <char*> gq) 
{ 
    queue <char*> g = gq; 
    while (!g.empty()) 
    { 
        printf("%s ",(char*)g.front());
        g.pop(); 
    } 
    printf("\n");
} 


void show_q(queue <int> gq) 
{ 
    queue <int> g = gq; 
    while (!g.empty()) 
    { 
        printf("%d ",(int)g.front());
        g.pop(); 
    } 
    printf("\n");
} 


void show_q(queue <char> gq) 
{ 
    queue <char> g = gq; 
    while (!g.empty()) 
    { 
        printf("%c ",(char)g.front());
        g.pop(); 
    } 
    printf("\n");
} 


double num_eval(char *s)
{
	double a, b;
	int i;
	char *e, *w ;

   w=newstr(10);
   strcpy(w," \t\n\r\f");
   
   queue <char*> queue_input; 
   queue <int> queue_num_eval; 

   stack <char*> stack_token;
   stack <double> stack_number;


	for (s = strtok(s, w); s; s = strtok(0, w)) {
		a = strtod(s, &e);
		if (e > s)	{	
         queue_input.push(s);
         queue_num_eval.push(0);
      }
		else if (strcmp(s,"pi")==0){
         queue_input.push(s);
         queue_num_eval.push(0);
      }

		else if (strcmp(s,"cos")==0){
         queue_input.push(s);
         queue_num_eval.push(1);
      }
		else if (strcmp(s,"sin")==0){
         queue_input.push(s);
         queue_num_eval.push(1);
      }

		else if (strcmp(s,"log")==0){
         queue_input.push(s);
         queue_num_eval.push(1);
      }



		else if (strcmp(s,"tan")==0){
         queue_input.push(s);
         queue_num_eval.push(1);
      }

		else if (strcmp(s,"exp")==0){
         queue_input.push(s);
         queue_num_eval.push(1);
      }


		else if (strcmp(s,"sqrt")==0){
         queue_input.push(s);
         queue_num_eval.push(1);
      }


		else if (*s == '+'){
         queue_input.push(s);
         queue_num_eval.push(2);
      }
		else if (*s == '-'){
         queue_input.push(s);
         queue_num_eval.push(1);
      }	
		else if (*s == '*'){
         queue_input.push(s);
         queue_num_eval.push(2);
		}	
		else if (*s == '/'){
         queue_input.push(s);
         queue_num_eval.push(2);
      }	
		else if (*s == '%'){
         queue_input.push(s);
         queue_num_eval.push(2);
      }	

		else if (*s == '^'){
         queue_input.push(s);
         queue_num_eval.push(2);
      }
		else {
			fprintf(stderr, "'%s': ", s);
			fprintf(stderr,"unknown operator!!!\n");abort();
		}
	}

   char *token;
   int type;
   int n_arg;

   double pi;
   pi=4.0*atan(1.0);

      
   token=newstr(500);

   while(queue_input.empty()==false){

      strcpy(token,queue_input.front());
      type=queue_num_eval.front();
      n_arg=type;

      if(type==0) 
      {
          if(strcmp(token,"pi")==0){
               stack_number.push(pi);              
          }
          else
          {
		       a=atof(token);
		       stack_number.push(a);        
          }
      }
      else
      {

         if(stack_number.size()<n_arg) {
         fprintf(stderr,"not enough arguments\n");
         abort();
         }
         else
         {


             if (strcmp(token,"sin")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			         stack_number.push(sin(b));
             }

             if (strcmp(token,"cos")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			         stack_number.push(cos(b));
             }


             if (strcmp(token,"tan")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			         stack_number.push(tan(b));
             }


             if (strcmp(token,"log")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			         stack_number.push(log(b));
             }


             if (strcmp(token,"exp")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			         stack_number.push(exp(b));
             }


             if (strcmp(token,"sqrt")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			         stack_number.push(sqrt(b));
             }


             if (strcmp(token,"+")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			       a=stack_number.top();
			         stack_number.pop();
			         stack_number.push(a+b);
             }

             if (strcmp(token,"-")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();

//			         stack_number.push(-b);

                if(stack_number.empty()==false){
			       a=stack_number.top();
			         stack_number.pop();
                }else a=0.0;
			         stack_number.push(a-b);
             }

             if (strcmp(token,"*")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			       a=stack_number.top();
			         stack_number.pop();
			         stack_number.push(a*b);
             }

             if (strcmp(token,"/")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			       a=stack_number.top();
			         stack_number.pop();
			         stack_number.push(a/b);
             }

             if (strcmp(token,"%")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			       a=stack_number.top();
			         stack_number.pop();
			         stack_number.push((int)a%(int)b);
             }


             if (strcmp(token,"^")==0){
               
   			    b=stack_number.top();
			         stack_number.pop();
			       a=stack_number.top();
			         stack_number.pop();
			         stack_number.push(pow(a, b));
             }

         }

      }

      queue_input.pop();           
      queue_num_eval.pop();  

   }
    return  stack_number.top();
}



int main( int argc, char *argv[] )
{ 

   char *str;
   char *exp;
   parser par;
   char* exp_rpn;

   stack <char*> q_str;
   double result;
   

   exp=newstr(300);

   strcpy(exp,argv[1]);

   par.evaluate(exp);
   exp_rpn=par.get_rpn();
   result= num_eval(exp_rpn);
   if(fabs(result)-fabs((int)result)==0){
   printf("%d\n",(int)result); }
   else{
    if(fabs(result)>100000000 ||  fabs(result)<.1){
//      printf("%e\n",result);
      printf("%lf\n",result);
}
    else{
      printf("%lf\n",result);}
  }

}

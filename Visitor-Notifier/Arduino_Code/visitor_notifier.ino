#include <Key.h>
#include <Keypad.h>
#include<LiquidCrystal.h>


LiquidCrystal lcd(11,10,14,15,16,17);

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Definitions

#define checker 12
#define ir 2
#define visit 18
#define disp 19
#define MAX_PHONE_NUMBERS 10 // Change the value to change the number users

// Code from here


// Global Declarations
byte colPins[COLS] = {5, 4, 3}; // Pins used for the columns of the keypad
byte rowPins[ROWS] = {9, 8, 7, 6}; // Pins used for the rows of the keypad
char num[MAX_PHONE_NUMBERS][10]; // Phone number storage
int vc = 0; //  Number of visitors
int n=0; // n
char customKey;
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
bool istaking = false; // boolean to check if the visitor is entering his phone number.
int check;
int irsensor;
int final;
int vp;
bool memory_error = false; // boolean to check if the memory has exceeded.
int i = -1;

// setup function
/*
Setting up arduino 
*/
void setup()
{
  Serial.begin(9600);
  
  lcd.print("Welcome");
}
// Notify
/*
Notfies the user that the owner is not in house and asks his/her phone number.
*/
void notify()
{
   lcd.clear();
   lcd.print("Ownernotinhouse!");
   delay(1000);
   lcd.clear();
   lcd.print("Entermobilenumber");
   delay(1000);
   lcd.clear();
}
// Do function
/*
Doo function takes user mobile number(10 digits)
Doo function is active till the user enters his phone number completely.
*/
void doo()
{ 
      
      customKey = customKeypad.getKey();
      if (customKey=='#' && i!=-1)
      {
        delonec(i);
        i--;
        return; 
      }
      else if (customKey=='*' && i!=-1)
      {
        lcd.clear();
        delfull();
        i = -1;
        return;
      } 
      else if(customKey)
      {
        i++;
        lcd.print(customKey);
        num[vc][i]=customKey;
      }
      istaking = true;
      if(i==9)
      {
        istaking=false;
        lcd.clear();     
        lcd.print("Thanks");
        
        if (vc==(MAX_PHONE_NUMBERS-1))
        {
            
            lcd.clear();
            lcd.print("Thanks");
            delay(1000);
            lcd.clear();
            
            lcd.print("Memory Exceeded!");
            delay(5000);
            memory_error = !memory_error;
            lcd.clear();
            lcd.print("Welcome");
        }
        vc++;
        i = -1;
        delay(500);
        lcd.clear();
        lcd.print("Welcome");
      }
}
// dofinalwork funciton
/*
Do final work function
This function prints the phone numbers stored one by one.
This function is called when the owner comes to house and presses the arduino switch
*/
void dofinalwork()
{
  int n = vc;
  bool va = false;
  int k;
  while (n>0)
  {
    if (!va)
    {
      lcd.clear();
      lcd.print("Visitor ");
      lcd.print(vc-n+1);
      delay(1000);
      lcd.clear();
      va = !va;
    }
    
    for (k = 0; k<10; k++)
    {
      lcd.print(num[vc-n][k]);     
    }
    delay(2000);
    lcd.clear();
    n--;
    va = !va;
    
  }
}
// delfull function
/*
delfull function delets the entire phone number on the lcd.
This function is called when the '*' button is pressed.
*/
void delfull()
{
  int looper;
  for (looper=0; looper<=9; looper++)
  {
    num[vc][looper] = '\0';  
  }
 
}
// delonec function
/*
This function deletes one character from the lcd screen.
This function is called when '#' button is pressed on the keypad.
*/
void delonec(int pos)
{
  int joker;
  num[vc][pos] = '\0';
  lcd.clear();
  for (joker = 0; joker < pos; joker++)
  {
    lcd.print(num[vc][joker]);
  }  
}
// Loop function
/*
This funciton is predefined and called periodically by the arduino.
In this function we check for the signals of the switches.
*/
void loop()
{
    check=digitalRead(checker);
    irsensor=digitalRead(ir);
    vp=digitalRead(visit);
    final=digitalRead(disp);
    if (final!=1)
    {
        if (istaking)
      {
        doo();
      }
      else 
      {
        
        if(irsensor==1)
        {
          if(memory_error)
          {
          lcd.clear();
          lcd.print("Limit reached !");
          delay(2000);
          lcd.clear();
          return;
          }
          lcd.clear();
          notify();
          doo();
         }
       else
         {
          delay(100);
         }
         }
    }
    else
    {
    dofinalwork();  
    }
  
}
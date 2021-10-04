/*************************************************************************
vercao 21/08/21
Dispositivo de dominacao para Airsoft

- inclui o registrador de jogo para marcar as invercoes 

AULTORo Alexsander noal silva

manual.mecatronico@gmail.com

https://www.youtube.com/c/ManualMecatr%C3%B4nicoPorAlexNoal?sub_confirmation=1
https://www.instagram.com/manual_mecatronico/

*************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <MicroLCD.h>
#include <EEPROM.h>

#define espacoEEPROM 1000

//LCD_SH1106 lcd; /* for SH1106 OLED module */
LCD_SSD1306 lcd; /* for SSD1306 OLED module */


void EEPROMWriteInt(int address, int value); //2 Bytes
int  EEPROMReadInt(int address);

void menu();
void fimdejogo();    //verifica se o jogo terminou e mostra o placar
void numeroTela (); 
void initTela () ;
void invertDominacao ();  //conta o tempo para inverter o contator
void d7seg ();            // logica q atualiza os displays de  7 segmentos


int tres=0, flagres=1;         // estabelece o tempo que a sirene vai levar para tocar indicanto retorno dos mortos uuuhhh
int vvermelho=0, vverde=0;     // conta o numero de vitorias de cada time
int flaginit=0,tempConvert=5, sirene=0;
int dominacao=0;          // =0 sem dominante =1 vermelhor dominando =2 verde dominando
int tverde=1,tvermelho=1,tempjogo=EEPROMReadInt (0) ;  // marcadores de tempo do jogo
unsigned long temp=0;      // marcador de tempo associado a funcao milles marca um seg
unsigned long porcvermelho=0, porcverde=0;    // regitra os valores para fim de jogo em porcentagem
bool fim=true, acabando=false;     // marcador de fim de jogo 
bool menu1=false, testeresete=true;
int resetejogo=0,flegresetejogo=0;   // contator de pulsos para rezetar o jogo, aperte o botao varias vezes para reiniciar o jogo
int espacoeepron =  EEPROMReadInt (10), testeleeepron=0;
void setup()
{
  
  lcd.begin();
  pinMode (13,INPUT_PULLUP);
  pinMode (2, OUTPUT);
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (8, OUTPUT);
  pinMode (10, OUTPUT);
  pinMode (11, OUTPUT);
  pinMode (12, OUTPUT);
  pinMode (15, OUTPUT);
  pinMode (16, OUTPUT);
  pinMode (17, OUTPUT);
  pinMode (9, OUTPUT);
  pinMode (14, OUTPUT);
  
  digitalWrite (2 , LOW); 
  digitalWrite (3 , LOW);
  digitalWrite (4 , LOW);
  digitalWrite (5 , LOW);
  digitalWrite (6 , LOW);
  digitalWrite (7 , LOW);
  digitalWrite (8 , LOW);
  digitalWrite (10 , LOW);
  digitalWrite (11, LOW);
  digitalWrite (14 , LOW);
  digitalWrite (15, LOW);
  digitalWrite (16, LOW);
  digitalWrite (17 , LOW);
  digitalWrite (9 , LOW);
  digitalWrite (12 , LOW);
	    
 
}

void loop()
{ 
menu();  
fimdejogo();

if (millis()- temp <= 1000)
  {  
  invertDominacao ();      
  initTela();   
  d7seg();
  if (tres==flagres) {sirene=3;flagres=1;}
  if (resetejogo > 12) {fim=true;resetejogo=0;flegresetejogo=0;digitalWrite(12, HIGH); delay (1500); digitalWrite(12, LOW);}     // funsao q reseta o jogo a clicar varias vezes
 
  }
else
  {
  if (resetejogo==flegresetejogo){resetejogo=0;flegresetejogo=0;} else {flegresetejogo=resetejogo;}   // funsao q reseta o jogo a clicar varias vezes
  if (dominacao==1) {tvermelho--;}
  if (dominacao==2) {tverde--;}  
  flaginit++;
  if (tres!=0) {flagres++;}
  if (sirene > 0) {sirene=sirene-1; digitalWrite(12, HIGH);}
  else            {digitalWrite(12 , LOW);}
  temp=millis(); 
  
  if (flaginit==6)
    {
    flaginit=0; 
    lcd.clear();  
    }
  invertDominacao ();  
 }
  
  

}

void initTela ()                              // ****************************************estado de inicio de jogo fica alternando entre o verde e o vermelho
  {
  if (flaginit<=2)
    { 
  
        lcd.setCursor (0,0);    
        lcd.println(" Aperte para");
        lcd.println(" Ativa a contagem");
        lcd.println(" do time VERDE");
        lcd.setCursor (25,4); 
        lcd.println(flaginit);
        
    digitalWrite (2 , HIGH);
    digitalWrite (3 , HIGH);
    digitalWrite (4 , HIGH);
    digitalWrite (5 , HIGH);
    digitalWrite (6 , HIGH);
    digitalWrite (7 , HIGH);
    digitalWrite (8 , HIGH); 
    digitalWrite (9 , LOW);
    digitalWrite (10 , LOW);
    digitalWrite (11, LOW);
    digitalWrite (14 , LOW);
    digitalWrite (15, LOW);
    digitalWrite (16, LOW);
    digitalWrite (17 , LOW);    
    if (digitalRead(13)==true)
        {
        flaginit=10; 
        resetejogo++;
        }
    }
  else
    {
    if (flaginit<=5) 
      {
    
          lcd.setCursor (0,0);
          lcd.println(" Aperte para");
          lcd.println(" Ativa a contagem");
          lcd.println(" do time VERMELHO");
          lcd.setCursor (25,4); 
          lcd.println(flaginit);
          
      digitalWrite (9 , HIGH);
      digitalWrite (10 , HIGH);
      digitalWrite (11, HIGH);
      digitalWrite (14 , HIGH);
      digitalWrite (15, HIGH);
      digitalWrite (16, HIGH);
      digitalWrite (17 , HIGH);
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      if (digitalRead(13)==true)
        {
        flaginit=90; 
        resetejogo++;
        }
      }
    } 

  if  (flaginit>=10 & flaginit<10+tempConvert )
    {
    lcd.setCursor (0,0);
    lcd.println(" Aperte para");
    lcd.println(" Ativa a contagem");
    lcd.println(" do time VERDE");
    lcd.setCursor (25,4); 
    lcd.println(flaginit-10);
    lcd.setCursor (((flaginit-10)*(100/tempConvert)),5);
    lcd.println("====");
    if (digitalRead(13)==false) 
      {
      flaginit=0; 
      lcd.clear();   
      }
    }
  if  (flaginit>=90 & flaginit<90+tempConvert)
    {
    lcd.setCursor (0,0);
    lcd.println(" Aperte para");
    lcd.println(" Ativa a contagem");
    lcd.println(" do time VERMELHO");
    lcd.setCursor (25,4); 
    lcd.println(flaginit-90);
    lcd.setCursor (((flaginit-90)*(100/tempConvert)),5);
    lcd.println("====");
    if (digitalRead(13)==false)  
      {
      flaginit=3; 
      lcd.clear();    
      }
    }  
    
  }
  

void invertDominacao ()                        // **************************funcao que alterna a dominacao do jogo**************************
  {
  if (flaginit==10+tempConvert or dominacao==2  )
    {
    if (flaginit==10+tempConvert)
      {
      espacoeepron =  EEPROMReadInt (10)+2;
      EEPROMWriteInt(espacoeepron, tverde+10000);
      espacoeepron = espacoeepron+2;
      EEPROMWriteInt(espacoeepron, tvermelho);
      EEPROMWriteInt(10, espacoeepron);
      
      lcd.clear();
      flaginit=202;                      //flaginte e o ponteiro que indica o estado do programa
      sirene=4;
      }  
     
    dominacao=2;                 // DOMINACAO VERDE 
 
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.setCursor (0,0);
    lcd.println("    VERDE "); 
    lcd.println("  DOMINANDO");
    lcd.setFontSize(FONT_SIZE_SMALL);
    
    porcverde=tverde*100.0;                              // tira a porcentagem do tempo de jogo
    porcverde=porcverde/tempjogo;
    
    lcd.setCursor (15,4);
    lcd.print(porcverde);
    lcd.print("%   -   ");
    lcd.print(porcvermelho);
    lcd.println("%  ");
    lcd.println(" Verde  -  Vermelho ");
    lcd.print("  ");
    lcd.print(tverde);
    lcd.print("    -    ");
    lcd.print(tvermelho);
    lcd.print(" ");
    }
   if (flaginit==90+tempConvert or dominacao==1 )  
    {
    if (flaginit==90+tempConvert)
      {

      espacoeepron =  EEPROMReadInt (10)+2;
      EEPROMWriteInt(espacoeepron, tverde);
      espacoeepron = espacoeepron+2;
      EEPROMWriteInt(espacoeepron, tvermelho+10000);
      EEPROMWriteInt(10, espacoeepron);
        
      lcd.clear();
      flaginit=201;
      sirene=4;
      }
    dominacao=1;                 // DOMINACAO VERMELHA 
    
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.setCursor (0,0);
    lcd.println("  VERMELHO");
    lcd.println("  DOMINANDO");
    lcd.setFontSize(FONT_SIZE_SMALL);

    porcvermelho=tvermelho*100.0;                              // tira a porcentagem do tempo de jogo
    porcvermelho=porcvermelho/tempjogo;
    
    lcd.setCursor (15,4);
    lcd.print(porcverde);
    lcd.print("%   -   ");
    lcd.print(porcvermelho);
    lcd.println("%  ");
    
    lcd.println(" Verde  -  Vermelho ");
    lcd.print("  ");
    lcd.print(tverde);
    lcd.print("    -    ");
    lcd.print(tvermelho);
    lcd.print("   ");
    }
  if (digitalRead(13)==true )
    {
    if (dominacao==1) {flaginit=10; dominacao=3;lcd.clear();}    
    if (dominacao==2) {flaginit=90; dominacao=4;lcd.clear();} 
    if (testeresete==true) {resetejogo++; testeresete=false;}  
    }
  else  
    {
    testeresete=true;  
    if (dominacao==3) {flaginit=201;dominacao=1;lcd.clear(); }    
    if (dominacao==4) {flaginit=202; dominacao=2;lcd.clear();}        
    }
  
  }

void numeroTela () 
  {
    
  }
  
void fimdejogo()                                              //*********************************fim do jogo *************************************
  {
  if (tverde==0)  
    {
    fim=true;
    vverde++;
    
    lcd.clear();
    digitalWrite(12, HIGH);
    delay (3000);
    digitalWrite(12, LOW);
    delay (3000);
    digitalWrite(12, HIGH);
    delay (3000);
    }
  if (tvermelho==0)
    {
    fim=true;
    vvermelho++;
    
    lcd.clear();
    digitalWrite(12, HIGH);
    delay (3000);
    digitalWrite(12, LOW);
    delay (3000);
    digitalWrite(12, HIGH);
    delay (3000);
    } 
  while (fim==true)
    {
    dominacao=0;
    flaginit=0;  
    
    acabando=false;
    digitalWrite(12, LOW);
    tvermelho=tempjogo;
    tverde=tempjogo;
    lcd.setCursor (5,0);
    lcd.println(" FIM DE JOGO");     
    lcd.println(" Aperte para reiniciar"); 
    lcd.println(" =*=PLACAR=*=");  
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.print(" Vermelho = ");
    //lcd.setCursor (24,2); 
    lcd.println(vvermelho);     
    lcd.print(" Verde = ");
    lcd.print(vverde); 
    lcd.setFontSize(FONT_SIZE_SMALL);
    if (digitalRead(13)==true ) {fim=false;lcd.clear();delay (200);menu1=true;}
    
    } 

  }  
  
void menu()                                         //***************************************menu do jogo*********************************
  {
while (menu1==true)
  {
   while (flaginit<=100)                             // ativa as configuracoes ao reiniciar o arduino
    {
    lcd.setCursor (0,0);      
    lcd.println(" Config. tempos "); 
     
    if (flaginit<=20) {lcd.setCursor (3,1); lcd.print(" ->");} 
    else {
         lcd.setCursor (3,1);
         lcd.print("   ");
         if (flaginit<=40) {lcd.setCursor (3,2); lcd.print(" ->");} 
         else 
            {
            lcd.setCursor (3,2);
            lcd.print("   ");
            if (flaginit<=60) {lcd.setCursor (3,3); lcd.print(" ->");}
            else 
               {
               lcd.setCursor (3,3);
               lcd.print("   ");
               if (flaginit<=80) {lcd.setCursor (3,4); lcd.print(" ->");}
               else 
                 {
                 lcd.setCursor (3,4);
                 lcd.print("   ");
                 if (flaginit<=100) {lcd.setCursor (3,5); lcd.print(" ->");}
                 else 
                   {
                   lcd.setCursor (3,5);
                   lcd.print("   ");
                   }
                 }
               }
            }
         }
    lcd.setCursor (24,1);
    lcd.print(" Convercao  ");     
    lcd.print(tempConvert); 
    lcd.print(" Seg");
    lcd.setCursor (24,2); 
    lcd.print(" Jogo   ");     
    lcd.print(tempjogo);
    lcd.print(" Seg"); 
    lcd.setCursor (24,3); 
    lcd.print(" Sire Res  ");     
    lcd.print(tres);
    lcd.print(" Seg"); 
    lcd.setCursor (21,4);
    lcd.print("LimpResg "); 
    lcd.print(EEPROMReadInt (10));
    lcd.setCursor (21,5);
    lcd.print("Ler Resgistros");
    
    if (millis()-temp <= 100)
      {             
      }
    else
      {
      temp=millis();
      flaginit++;
      if (digitalRead(13)==true)
        {
        if (flaginit<=20)
          {
          if (tempConvert<=40)
            {
            tempConvert++;  
            }
          else
            {
            lcd.clear();
            tempConvert=1;   
            }
          flaginit=10;  
          }
        else 
          {   
          if (flaginit<=40)
            {
            if (tempjogo<=6600)
              {
              tempjogo=tempjogo+10;  
              }
            else
              {
              lcd.clear();
              tempjogo=30;   
              }
            flaginit=30;  
            }
           else 
            {
            if (flaginit<=60)
              {
              if (tres<=300)
               {
               if (tres==0)
                 {
                 tres=30;  
                 }
               else
                 {
                 tres=tres+1;    
                 }               
               }
             else
               {
               lcd.clear();
               tres=0;   
               }
              flaginit=50;  
              }
            else
              {
              if (flaginit<=80)
                {
                EEPROMWriteInt(10, 10);      
                }
              else
                {
                if (flaginit<=100)
                  {
                   //testeleeepron = EEPROMWriteInt(10, 12) ;
                   flaginit=12;
                   while(1)
                    {
                    
                    lcd.clear ();
                    lcd.setCursor (5,1);
                    lcd.print("Tela ");
                    lcd.print((flaginit-12)/8);
                    lcd.setCursor (5,2);
                    lcd.print("Verd ");
                    lcd.print(EEPROMReadInt (flaginit));
                    lcd.setCursor (5,3);
                    lcd.print("Red ");
                    lcd.print(EEPROMReadInt (flaginit+2));
                    lcd.setCursor (5,4);
                    lcd.print("Verd ");
                    lcd.print(EEPROMReadInt (flaginit+4));
                    lcd.setCursor (5,5);
                    lcd.print("Red ");
                    lcd.print(EEPROMReadInt (flaginit+6));
                    flaginit=flaginit+8;
                    delay (2500);
                    if (digitalRead(13)==true or flaginit>EEPROMReadInt (10)) {flaginit=20;lcd.clear ();break;}   
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  lcd.clear();  
  flaginit=0; 
  menu1=false; 
  if ( tempjogo != EEPROMReadInt (0)) {EEPROMWriteInt(0, tempjogo);}   
  }
  }

void d7seg ()
  {
  switch(porcverde) 
    {
    case 100:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (2 , HIGH);
      digitalWrite (8 , HIGH);
      digitalWrite (4 , HIGH);
      digitalWrite (3 , HIGH);
      digitalWrite (7 , HIGH);
      digitalWrite (6 , HIGH);
      break;
      }
    case 90:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);
       
      digitalWrite (8 , HIGH);
      digitalWrite (2 , HIGH);
      break;
      }
    case 80:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (3 , HIGH);
      digitalWrite (2 , HIGH);
      digitalWrite (5 , HIGH);
      digitalWrite (6 , HIGH);
      digitalWrite (7 , HIGH);
      break;
      }   
    case 70:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (8 , HIGH);
      digitalWrite (2 , HIGH);
      digitalWrite (5 , HIGH);
      digitalWrite (3 , HIGH);
      digitalWrite (7 , HIGH);
      break;
      }
    case 60:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (2 , HIGH);
      digitalWrite (5 , HIGH);
      digitalWrite (4 , HIGH);
      digitalWrite (8 , HIGH);
      break;
      }
    case 50:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (3 , HIGH);
      digitalWrite (4 , HIGH);
      digitalWrite (5 , HIGH);
      digitalWrite (8 , HIGH);
      digitalWrite (7 , HIGH);
      break;
      }
    case 40:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (4 , HIGH);
      digitalWrite (3 , HIGH);
      digitalWrite (5 , HIGH);
      digitalWrite (6 , HIGH);
      digitalWrite (7 , HIGH);
      digitalWrite (8 , HIGH);
      break;
      }  
    case 30:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (3 , HIGH);
      digitalWrite (2 , HIGH);
      digitalWrite (8 , HIGH);
      break;
      }
    case 20:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (2 , HIGH);
      digitalWrite (3 , HIGH);
      digitalWrite (4 , HIGH);
      digitalWrite (5 , HIGH);
      digitalWrite (6 , HIGH);
      digitalWrite (7 , HIGH);
      digitalWrite (8 , HIGH);
      break;
      } 
    case 10:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      digitalWrite (2 , HIGH);
      digitalWrite (3 , HIGH);
      digitalWrite (4 , HIGH);
      digitalWrite (5 , HIGH);
      digitalWrite (8 , HIGH);
      digitalWrite (7 , HIGH);
      break;
      }
    case 6:
      {
      if (acabando==false) {sirene=7;acabando=true;}   
      break; 
      }
    case 1:
      {
      digitalWrite (2 , LOW); 
      digitalWrite (3 , LOW);
      digitalWrite (4 , LOW);
      digitalWrite (5 , LOW);
      digitalWrite (6 , LOW);
      digitalWrite (7 , LOW);
      digitalWrite (8 , LOW);

      
      digitalWrite (5 , HIGH);
      break;
      }   
    }

  switch(porcvermelho) 
    {
    case 100:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (10 , HIGH);
      digitalWrite (11 , HIGH);
      digitalWrite (17 , HIGH);
      digitalWrite (15 , HIGH);
      digitalWrite (16 , HIGH);
      break;
      }
    case 90:
      {
      sirene=2;
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (17 , HIGH);
      break;
      }
    case 80:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (10 , HIGH);
      digitalWrite (11 , HIGH);
      digitalWrite (14 , HIGH);
      digitalWrite (16 , HIGH);
      digitalWrite (17, HIGH);
      break;
      }   
    case 70:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (10 , HIGH);
      digitalWrite (17 , HIGH);
      digitalWrite (16 , HIGH);
      digitalWrite (14 , HIGH);
      break;
      }
    case 60:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (15 , HIGH);
      digitalWrite (14 , HIGH);
      digitalWrite (9 , HIGH);
      digitalWrite (17 , HIGH);
      break;
      }
    case 50:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (10 , HIGH);
      digitalWrite (14 , HIGH);
      digitalWrite (15 , HIGH);
      digitalWrite (16 , HIGH);
      break;
      }
    case 40:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (10 , HIGH);
     digitalWrite (11 , HIGH);
      digitalWrite (14 , HIGH);
      digitalWrite (15 , HIGH);
      digitalWrite (16 , HIGH);
      break;
      }  
    case 30:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (17 , HIGH);
      digitalWrite (16 , HIGH);
      break;
      }
    case 20:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (10 , HIGH);
      digitalWrite (11 , HIGH);
     digitalWrite (14 , HIGH);
      digitalWrite (15 , HIGH);
      digitalWrite (16 , HIGH);
      digitalWrite (17 , HIGH);
      break;
      } 
    case 10:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);

      digitalWrite (9 , HIGH);
      digitalWrite (10 , HIGH);
      digitalWrite (14 , HIGH);
      digitalWrite (15 , HIGH);
      digitalWrite (16 , HIGH);
      digitalWrite (17 , HIGH);
      break;
      
      }
    case 6:
      {
      if (acabando==false) {sirene=7;acabando=true;}   
      break;
      }
    case 1:
      {
      digitalWrite (9 , LOW);
      digitalWrite (10 , LOW);
      digitalWrite (11, LOW);
      digitalWrite (14 , LOW);
      digitalWrite (15, LOW);
      digitalWrite (16, LOW);
      digitalWrite (17 , LOW);
      
      digitalWrite (14 , HIGH);
      break;
      }   
    }  
        
  }


void EEPROMWriteInt(int address, int value) {
   byte hiByte = highByte(value);
   byte loByte = lowByte(value);

   EEPROM.write(address, hiByte);
   EEPROM.write(address + 1, loByte);   
}

int EEPROMReadInt(int address) {
   byte hiByte = EEPROM.read(address);
   byte loByte = EEPROM.read(address + 1);
   
   return word(hiByte, loByte); 
}  

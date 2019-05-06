#define serial      // if serial not defined ,no serial will be printed 

//for right sensor
#define trig1 2
#define echo1 3

//for front sensor
#define trig2 4
#define echo2 5

//for left sensor
#define trig3 6
#define echo3 7

#define front_dir 0
#define left_dir 1
#define back_dir 2
#define right_dir 3

//left motor
#define motor1a 8
#define motor1b 9

//right motor
#define motor2a 12
#define motor2b 13

#define enable1 10
#define enable2 11

unsigned long duration;
unsigned int front_distance, right_distance, left_distance;
unsigned char ways[] = {4,3,2,1,8,15,16,23,22,29,30,37,36,43,   
                        11,10,9,16,23,22,29,30,37,36,43,
                        20,19,18,17,16,23,22,29,30,37,36,43,
                        7,6,5,12,13,14,21,28,27,26,25,18,17,16,23,22,29,30,37,36,43,
                        35,34,41,40,33,32,31,24,23,22,29,30,37,36,43,
                        63,56,55,54,53,52,59,58,51,44,43,
                        42,49,48,47,46,45,52,59,58,51,44,43,
                        39,38,45,52,59,58,51,44,43,
                        70,69,62,61,60,59,58,51,44,43,
                        68,67,66,65,58,51,44,43,
                        90,89,88,87,86,79,72,73,66,65,58,51,44,43,
                        91,84,83,82,81,80,79,72,73,66,65,58,51,44,43,
                        77,76,75,74,81,80,79,72,73,66,65,58,51,44,43};


unsigned char *ptr, first_place, new_direction, old_direction = front_dir;

void get_first_place() //get now manually from ways picture 
{
 first_place = 7; 
}

void get_distances() 
{
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duration = pulseIn(echo1, HIGH);
  right_distance= duration*0.034/2;
  

  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration = pulseIn(echo2, HIGH);
  front_distance= duration*0.034/2;
  

  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  duration = pulseIn(echo3, HIGH);
  left_distance= duration*0.034/2;

#ifdef serial
    Serial.print(left_distance);
    Serial.print("  ");
    Serial.print(front_distance);
    Serial.print("  ");
    Serial.println(right_distance);
#endif
    
}


void goto_front()
{
  //left motor forward
  digitalWrite(motor1a,HIGH); 
  digitalWrite(motor1b,LOW);
  analogWrite(enable1,(90));

  //right motor forward
  digitalWrite(motor2a,HIGH);
  digitalWrite(motor2b,LOW);
  analogWrite(enable2,(90));
}

void turn_left() 
{

  //left motor backward
  digitalWrite(motor1a,LOW);
  digitalWrite(motor1b,HIGH);
  analogWrite(enable1,(90));

  //right motor forward
  digitalWrite(motor2a,HIGH);
  digitalWrite(motor2b,LOW);
  analogWrite(enable2,(90));
  
  delay(500); 
  digitalWrite(motor1b,LOW);
  digitalWrite(enable1,LOW);
  digitalWrite(motor2a,LOW);
  digitalWrite(enable2,LOW);

  
}

void turn_right() 
{
  
  //left motor forward
  digitalWrite(motor1a,HIGH);
  digitalWrite(motor1b,LOW);
  analogWrite(enable1,(90));

  //right motor backward
  digitalWrite(motor2a,LOW);
  digitalWrite(motor2b,HIGH);
  analogWrite(enable2,(90));    
  delay(500); 

  digitalWrite(motor1a,LOW);
  digitalWrite(enable1,LOW);
  digitalWrite(motor2b,LOW);
  digitalWrite(enable2,LOW);
}

void turn_180()
{
  turn_left();
  turn_left();
}



void robot_move()
{
  #ifdef serial
  Serial.println(first_place,DEC); 
  #endif

  for(int i=0 ; i<176 ; i++)
    if(ways[i] == first_place) {ptr = &ways[i]; break;}
  
  for(int j=1 ; ptr[j-1] != 43 ; j++)
  {
          #ifdef serial
          Serial.print(ptr[j],DEC); 
          #endif
          if( ptr[j-1]-ptr[j] == 1) 
              new_direction = front_dir;
            
          else if(ptr[j]-ptr[j-1] == 7) 
              new_direction = left_dir;
          
          else if(ptr[j-1]-ptr[j] == 7)
              new_direction = right_dir;
           
          else if(ptr[j]-ptr[j-1] == 1)
              new_direction = back_dir;

            if(new_direction == old_direction)
             {
               goto_front();
               #ifdef serial
               Serial.println("  goto_front"); 
               #endif
             }
            else if(new_direction - old_direction == 1 || new_direction - old_direction == -3)
              {
                turn_left();
                #ifdef serial
                Serial.println("  turn_left"); 
                #endif
              }
            else if(new_direction - old_direction == -1 || new_direction - old_direction == 3)
              {
                turn_right();
                #ifdef serial
                Serial.println("  turn_right"); 
                #endif
              }
             else if(new_direction - old_direction == -2 || new_direction - old_direction == 2)
              {
                turn_180();
                #ifdef serial
                Serial.println("  turn_180"); 
                #endif
              }

        old_direction = new_direction;
  }
}

void configurations()
{
   pinMode(trig1, OUTPUT); // Sets the trigPin as an Output
   pinMode(echo1, INPUT); // Sets the echoPin as an Input
   pinMode(trig2, OUTPUT); // Sets the trigPin as an Output
   pinMode(echo2, INPUT); // Sets the echoPin as an Input
   pinMode(trig3, OUTPUT); // Sets the trigPin as an Output
   pinMode(echo3, INPUT); // Sets the echoPin as an Input

  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  
  pinMode(motor1a, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(motor2b, OUTPUT);
  
   Serial.begin(9600);  
}
void setup() {
 configurations();
 get_first_place();
 robot_move();
}

void loop() {

//get_distances(); //when to test the sensors;
  
}

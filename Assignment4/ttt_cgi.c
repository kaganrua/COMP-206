#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
int main(void){
  
  char *data = getenv("QUERY_STRING");
  char *address;
  char *port;
  char *username;
  char *password;
  char *gamename;
  char *square;
  char *command;

  char *token;

  
  address = strtok(data,"&");
  port = strtok(NULL,"&");
  username = strtok(NULL,"&");
  password = strtok(NULL,"&");
  gamename = strtok(NULL,"&");
  square = strtok(NULL,"&");
  command = strtok(NULL,"&");

  token = strtok(address,"=");
  address = strtok(NULL,"=");
  if(address == NULL){
    address = "";
  }

  token = strtok(port,"=");
  port = strtok(NULL,"=");
  if(port == NULL){
    port = "";
  }

  token = strtok(username,"=");
  username = strtok(NULL,"=");
  if(username == NULL){
    username = "";
  }

  token = strtok(password,"=");
  password = strtok(NULL,"=");
  if(password == NULL){
    password = "";
  }

  token = strtok(gamename,"=");
  gamename = strtok(NULL,"=");
  if(gamename == NULL){
    gamename = "";
  }

  token = strtok(square,"=");
  square = strtok(NULL,"=");
  if(square == NULL){
    square = "";
  }

  token = strtok(command,"=");
  command = strtok(NULL,"=");

	printf("Content-Type: text/html;\n\n");
  FILE *fp;
  fp = fopen("../ttt.html","r");
  char line[2000];
  int counter = 1;
  fgets(line,1999,fp);
  while(!feof(fp)){
		if(counter == 4){
			printf("<form action=\"ttt.cgi\" method=\"get\">");
		}
		else if(counter == 5){
	     printf("<b>Server Address:</b> <input type=\"text\" name=\"address\" size=\"20\" value=\"%s\"><br />",address );
    }else if(counter == 6){
	     printf("<b>Server Port:</b> <input type=\"text\" name=\"port\" size=\"20\" value=\"%s\">  <br />",port);
    }else if(counter == 7){
	     printf("<b>Username:</b> <input type=\"text\" name=\"username\" size=\"20\" value=\"%s\">  <br />",username);
    }else if(counter == 8){
	     printf("<b>Password:</b> <input type=\"text\" name=\"password\" size=\"20\" value=\"%s\">  <br />",password);
    }else if(counter == 9){
	     printf("<b>Gamename:</b> <input type=\"text\" name=\"gamename\" size=\"20\" value=\"%s\"> <br />",gamename);
    }else if(counter ==10){
	     printf("<b>Square:</b> <input type=\"text\" name=\"square\" size=\"20\" value=\"%s\">  <br />",square);
    }else if(counter == 18){
			char *argv[]={"CLIENT",address,port,command,username,password,gamename,square};
      int argc = 8;
      int sock;
      struct sockaddr_in server;
      char message[200], prev_msg[200];
      char addr[20] = "127.0.0.1";
      unsigned short int port = 8888;
      if( argc>1 ){
        strcpy( addr, argv[1] );
      }

      if( argc>2 ){
        port = (unsigned short int)atoi( argv[2] );
      }
      memset(prev_msg, '\0', 200 );

      server.sin_addr.s_addr = inet_addr( argv[1] );
      server.sin_family = AF_INET;
      server.sin_port = htons( port );

      //keep communicating with server

      //Construct the messaqge properly COMMAND,USERNAME,PASSWORD,GAMENAME,SQUARE
      char *x = ",";
      char *y = "website";
      for(int i = 3; i < 8; i++){
        strcat(message,argv[i]);
        strcat(message,x);
      }
      strcat(message,y);
      message[strlen(message)]='\0';

      //Create socket
      sock = socket(AF_INET , SOCK_STREAM , 0);
      if (sock == -1)
      {
        printf("Could not create socket");
      }

      else {
        //Connect to remote server
        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
          perror("connect failed. Error");
        }
        else {

          strcpy( prev_msg, message );
          //Send some data
          if( send(sock , message , 200, 0) < 0)
          {
            puts("Send failed");
          }
          else {

            size_t read_size;
            int bytes_read = 0;
            char incoming_msg[2000];
            char tmp[2000];
            int flag=0;
            while( bytes_read < 2000 ){
              read_size = recv(sock , tmp, 2000, 0);
              if(read_size <= 0){
                  puts("Server disconnected");
                  fflush(stdout);
                  close(sock);
                  flag=1;
                  break;
              }
              memcpy( incoming_msg+bytes_read, tmp, read_size );
              bytes_read += read_size;
            }
              if(flag!=1){
                printf( "Server response: %s\n", incoming_msg );
                close(sock);
              }
            }
          }
        }
    }

    else{
      printf("%s",line);
    }

    fgets(line,1999,fp);
	  counter++;
  }
  fclose(fp);
	printf("</body>");
  printf("</html>");
}

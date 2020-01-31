/*********************************************************
* FILE: ttt_server.c
* 
* A starter code for your answer to A4 Q1. Most of the
* networking is already done for you, and we even gave
* suggested data types (structs and linked lists of these)
* to represent users and tic-tac-toe games. You must just
* figure out how to fill in the various functions required
* to make the games happen.
*
* Good luck, and rember to ask questions quickly if you get 
* stuck. My Courses Dicussions is the first place to try, 
* then office hours.
*
* AUTHOR: YOU!
* DATE: Before Dec 3rd
***********************************************************/

#include <stdio.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>	
#include <stdlib.h>
#include <time.h>

struct USER{
	char username[100];
	char password[100];
	struct USER *next;
};

enum GAME_STATE{ 
	CREATOR_WON=-2,
	IN_PROGRESS_CREATOR_NEXT=-1,
	DRAW=0,
	IN_PROGRESS_CHALLENGER_NEXT=1,
	CHALLENGER_WON=2
};

struct GAME{
	char gamename[100];
	struct USER *creator;
	struct USER *challenger;
	enum GAME_STATE state;
	char ttt[3][3];
	struct GAME *next;
};

struct USER *user_list_head = NULL;
struct GAME *game_list_head = NULL;

int check(char ttt[3][3], int turn){
	for(int i=0 ; i<3 ; i++)
		if(ttt[i][0]!=' ' && ttt[i][0] == ttt[i][1] && ttt[i][1] == ttt[i][2]){
			if(ttt[i][0] == 'x')
				return 2;
			else return -2;
		}
	for(int i=0 ; i<3 ; i++)
		if(ttt[0][i]!=' ' && ttt[0][i] == ttt[1][i] && ttt[1][i] == ttt[2][i]){
			if(ttt[0][i] == 'x')
				return 2;
			else return -2;
		}
	if(ttt[0][0]!=' ' && ttt[0][0] == ttt[1][1] && ttt[1][1] == ttt[2][2])
		return (ttt[0][0]=='x' ? 2 : -2);
	if(ttt[2][0]!=' ' && ttt[2][0] == ttt[1][1] && ttt[1][1] == ttt[0][2])
		return (ttt[2][0]=='x' ? 2 : -2);
	int isEmpty = 0;
	for(int i=0 ; i<3 ; i++)
		for(int j=0 ; j<3 ; j++)
			if(ttt[i][j] == ' ')
				isEmpty = 1;
	if(isEmpty == 0)
		return 0;
	return turn;
}

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[2000];

	unsigned short int port = 8888;

	if( argc > 1 )
		port = (unsigned short int)atoi(argv[1]);
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );
	
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind failed. Error");
		return 1;
	}

	listen(socket_desc , 3);

	printf( "Game server ready on port %d.\n", port );

	while( 1 ){
		c = sizeof(struct sockaddr_in);

		//accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
			perror("accept failed");
			return 1;
		}

		char temp[200];
		memset(client_message, '\0', 200);
		int bytes_read = 0;
		while( bytes_read < 200 ){
			read_size = recv(client_sock , temp , 200, 0);
			if(read_size <= 0){
				puts("Client disconnected");
				fflush(stdout);
				close(client_sock);
				close(socket_desc);
				return 0;
			}
			memcpy( client_message+bytes_read, temp, read_size );
			bytes_read += read_size;
		}

	  	char response[2000];
	  	response[0] = '\0';
	  	char* command = strtok( client_message, "," );
	  	char *username = strtok( NULL, "," );
	  	char *password = strtok( NULL, ",");

		if( command == NULL || username == NULL || password == NULL ){
			sprintf( response, "MUST ENTER A VALID COMMAND WITH ARGUMENTS FROM THE LIST:\n" );
			sprintf( response+strlen(response), "LOGIN,USER,PASS\n" );
			sprintf( response+strlen(response), "CREATE,USER,PASS,GAMENAME\n" );
			sprintf( response+strlen(response), "JOIN,USER,PASS,GAMENAME,SQUARE\n" );
			sprintf( response+strlen(response), "MOVE,USER,PASS,GAMENAME,SQUARE\n" );
			sprintf( response+strlen(response), "LIST,USER,PASS\n" );
			sprintf( response+strlen(response), "SHOW,USER,PASS,GAMENAME\n" );
			write(client_sock , response , 2000);  
		  	close(client_sock);
			continue;
		}

		if( strcmp( command, "LOGIN" ) == 0 ){			
			if(user_list_head == NULL){ 
				 
				
				 
				struct USER * user1 = (struct USER *) malloc (sizeof(struct USER));
			
				strcpy(user1->username, username);
				strcpy(user1->password, password);			
				user1->next=NULL;

	
				
				user_list_head=user1;
				strcpy(response, "First user created, added"); 
				
			}
			 
			else{		
				
				struct USER * curr = user_list_head;
				int is_created = 0;
				//cc
				while(curr!= NULL){			
					if(strcmp(username, curr->username)==0 && strcmp(password, curr->password)==0){
						strcpy(response, "User logged in");
						is_created = 1; 
						break;
						
					}
					//else if the username correct BUT PASSWORD IS WRONG! 
					else if(strcmp(username, curr->username)==0 && strcmp(password, curr->password)!=0){
						strcpy(response, "Wrong password!");
						is_created = 1;
						break;
					}
					curr=curr->next;
					
					

				}
				if(is_created == 0){
					struct USER * freespot = user_list_head;
					while(freespot->next!=NULL){
						freespot=freespot->next;
					}
					//malloc space for curr and then point it to user_ptr	
					//struct USER * curr = (struct USER *) malloc (sizeof(struct USER));				
					struct USER * user2 = (struct USER *) malloc (sizeof(struct USER));
					strcpy(user2->username, username);
					strcpy(user2->password, password);			
					user2->next=NULL;
					freespot->next = user2;
					strcpy(response, "New user created, added");
				}
			}

			
	  	}
	  	else if( strcmp( command, "CREATE" ) == 0 ){ 
	  		char *game_name = strtok( NULL, ",");


			if( game_name == NULL ){
				sprintf( response, "CREATE COMMAND MUST BE CALLED AS FOLLOWS:\n" );
				sprintf( response+strlen(response), "CREATE,USER,PASS,GAMENAME\n" );
				write(client_sock , response , 2000);  
		  		close(client_sock);
		  		continue;
			}

			struct GAME *game = (struct GAME*)malloc( sizeof(struct GAME) );

			strcpy( game->gamename, game_name );

			struct USER *curr = user_list_head;
			int userExists = 0, wrongPass = 0;

			while(curr != NULL){
				if(strcmp(username, curr->username) == 0 && strcmp(password, curr->password) == 0){
					game->creator = curr;
					userExists = 1;
					break;
				}
				else if(strcmp(username, curr->username) == 0){
					wrongPass = 1;
					userExists = 1;
					break;
				}
				curr = curr->next;
			}

			if(userExists == 0){
				strcpy(response, "User not found!");
			}
			else if(wrongPass == 1){
				strcpy(response, "Wrong password!");
			}

			else {

				for( int row=0; row<3; row++ )
					for( int col=0; col<3; col++ )
						game->ttt[row][col] = ' ';

				game->next = NULL;
				game->challenger = NULL;
				game->state = IN_PROGRESS_CHALLENGER_NEXT;

				int gameExists = 0;


				if(game_list_head == NULL){
					game_list_head = game;

				}
				else {

					struct GAME * freespot = game_list_head;

					while(freespot!=NULL){
						if(strcmp(freespot->gamename,game_name)==0){
							gameExists = 1;
							strcpy(response,"Game already exists");
							break;
						}
						if(freespot->next==NULL)
							break;
						freespot=freespot->next;
					}
					if(gameExists == 0)
						freespot->next =game;
				}

				if(gameExists == 0){
					sprintf( response, "GAME %s CREATED. WAITING FOR OPPONENT TO JOIN.\r\n", game->gamename);
					sprintf( response, "%sa  %c | %c | %c \r\n",response,  game->ttt[0][0],  game->ttt[0][1],  game->ttt[0][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sb  %c | %c | %c \r\n", response, game->ttt[1][0],  game->ttt[1][1],  game->ttt[1][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sc  %c | %c | %c \r\n", response, game->ttt[2][0],  game->ttt[2][1],  game->ttt[2][2]);
					sprintf( response, "%s\r\n", response );
					sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
				}

				
			}
		}
		else if( strcmp( command, "JOIN" ) == 0 ){ 
	  		
	  		char *game_name = strtok( NULL, ",");
			char *move_name = strtok( NULL, ",");

			if(move_name[0]<'a' || move_name[0]>'c')
				sprintf( response, "INVALID MOVE %s. ROW MUST BE a-c", move_name);
			else if(move_name[1]<'1' || move_name[1]>'3')
				sprintf( response, "INVALID MOVE %s. COL MUST BE 1-3", move_name);
			else {
				struct GAME *curr = game_list_head;
				int gameExists = 0;

				while(curr!=NULL){
					if(strcmp(curr->gamename,game_name)==0){
						gameExists = 1;
						break;
					}
					curr = curr->next;
				}
				if(gameExists == 0){
					strcpy(response,"Game does not exist!");
				}
				else if(curr->challenger != NULL){
					strcpy(response, "Game is full!");
				}
				else {
					struct USER *curr2 = user_list_head;
					int userExists = 0, wrongPass = 0;

					while(curr2 != NULL){
						if(strcmp(username, curr2->username) == 0 && strcmp(password, curr2->password) == 0){
							curr->challenger = curr2;
							userExists = 1;
							break;
						}
						else if(strcmp(username, curr2->username) == 0){
							wrongPass = 1;
							userExists = 1;
							break;
						}
						curr2 = curr2->next;
					}

					if(userExists == 0)
						strcpy(response, "User not found!");
					else if(wrongPass == 1)
						strcpy(response, "Wrong password!");
					else {
						curr->ttt[move_name[0]-'a'][move_name[1]-'1'] = 'x';
						curr->state = IN_PROGRESS_CREATOR_NEXT;
						sprintf( response, "GAME %s BETWEEN %s AND %s\r\nIN PROGRESS: %s TO MOVE NEXT AS o\r\n", curr->gamename, curr->creator->username, curr->challenger->username, curr->creator->username);
						sprintf( response, "%sa  %c | %c | %c \r\n",response,  curr->ttt[0][0],  curr->ttt[0][1],  curr->ttt[0][2]);
						sprintf( response, "%s  ---|---|---\r\n", response );
						sprintf( response, "%sb  %c | %c | %c \r\n", response, curr->ttt[1][0],  curr->ttt[1][1],  curr->ttt[1][2]);
						sprintf( response, "%s  ---|---|---\r\n", response );
						sprintf( response, "%sc  %c | %c | %c \r\n", response, curr->ttt[2][0],  curr->ttt[2][1],  curr->ttt[2][2]);
						sprintf( response, "%s\r\n", response );
						sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
					}
				}
			}
		}
		else if( strcmp( command, "MOVE" ) == 0 ){ 
			char *game_name = strtok( NULL, ",");
			char *move_name = strtok( NULL, ",");
			int row,col;
			row=move_name[0]-'a';
			col=move_name[1]-'1';

			if(move_name[0]<'a' || move_name[0]>'c')
				sprintf( response, "INVALID MOVE %s. ROW MUST BE a-c", move_name);
			else if(move_name[1]<'1' || move_name[1]>'3')
				sprintf( response, "INVALID MOVE %s. COL MUST BE 1-3", move_name);
			else {

				struct GAME *curr = game_list_head;
				int gameExists = 0;

				while(curr!=NULL){
					if(strcmp(curr->gamename,game_name)==0){
						gameExists = 1;
						break;
					}
					curr = curr->next;
				}
				if(gameExists == 0){
					strcpy(response,"Game does not exist!");
				}
				else {
					struct USER *curr2 = user_list_head;
					int userExists = 0, wrongPass = 0;

					while(curr2 != NULL){
						if(strcmp(username, curr2->username) == 0 && strcmp(password, curr2->password) == 0){
							userExists = 1;
							break;
						}
						else if(strcmp(username, curr2->username) == 0){
							wrongPass = 1;
							userExists = 1;
							break;
						}
						curr2 = curr2->next;
					}

					if(userExists == 0)
						strcpy(response, "User not found!");
					else if(wrongPass == 1)
						strcpy(response, "Wrong password!");
					else {
						if(strcmp(curr->creator->username, username) == 0 || strcmp(curr->challenger->username, username) == 0){
							if(curr->state == DRAW || curr->state == CREATOR_WON || curr->state == CHALLENGER_WON){
								sprintf( response, "The game is finished. You cannot make any more moves.");
							}
							else {
								if(curr->state == IN_PROGRESS_CREATOR_NEXT){
									if(strcmp(curr->challenger->username,username) == 0)
										sprintf(response, "It is not your turn!");
									else {
										if(curr->ttt[row][col] != ' ')
											sprintf(response, "The square %s is full!",move_name);
										else {
											curr->ttt[row][col] = 'o'; 
											curr->state = check(curr->ttt, 1);
											sprintf( response, "%sGAME %s: CREATED BY %s, CHALLENGED BY: %s. ", response, curr->gamename, curr->creator->username, curr->challenger->username);
											if(curr->state == CREATOR_WON)
												strcat( response, "CREATOR WON");
											else if(curr->state == CHALLENGER_WON)
												strcat( response, "CHALLENGER WON");
											else if(curr->state == DRAW)
												strcat( response, "DRAW");
											else if(curr->state == IN_PROGRESS_CREATOR_NEXT)
												sprintf( response, "%sIN PROGRESS: %s to move next as o", response, curr->creator->username);
											else
												sprintf( response, "%sIN PROGRESS: %s to move next as x", response, curr->challenger->username);
											strcat( response, "\r\n");
											sprintf( response, "%sa  %c | %c | %c \r\n",response,  curr->ttt[0][0],  curr->ttt[0][1],  curr->ttt[0][2]);
											sprintf( response, "%s  ---|---|---\r\n", response );
											sprintf( response, "%sb  %c | %c | %c \r\n", response, curr->ttt[1][0],  curr->ttt[1][1],  curr->ttt[1][2]);
											sprintf( response, "%s  ---|---|---\r\n", response );
											sprintf( response, "%sc  %c | %c | %c \r\n", response, curr->ttt[2][0],  curr->ttt[2][1],  curr->ttt[2][2]);
											sprintf( response, "%s\r\n", response );
											sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
										}

									}
								}
								else {
									if(strcmp(curr->creator->username,username) == 0)
										sprintf(response, "It is not your turn!");
									else {
										if(curr->ttt[row][col] != ' ')
											sprintf(response, "The square %s is full!",move_name);
										else {
											curr->ttt[row][col] = 'x';
											curr->state = check(curr->ttt, -1); 
											sprintf( response, "%sGAME %s: CREATED BY %s, CHALLENGED BY: %s. ", response, curr->gamename, curr->creator->username, curr->challenger->username);
											if(curr->state == CREATOR_WON)
												strcat( response, "CREATOR WON");
											else if(curr->state == CHALLENGER_WON)
												strcat( response, "CHALLENGER WON");
											else if(curr->state == DRAW)
												strcat( response, "DRAW");
											else if(curr->state == IN_PROGRESS_CREATOR_NEXT)
												sprintf( response, "%sIN PROGRESS: %s to move next as o", response, curr->creator->username);
											else
												sprintf( response, "%sIN PROGRESS: %s to move next as x", response, curr->challenger->username);
											strcat( response, "\r\n");
											sprintf( response, "%sa  %c | %c | %c \r\n",response,  curr->ttt[0][0],  curr->ttt[0][1],  curr->ttt[0][2]);
											sprintf( response, "%s  ---|---|---\r\n", response );
											sprintf( response, "%sb  %c | %c | %c \r\n", response, curr->ttt[1][0],  curr->ttt[1][1],  curr->ttt[1][2]);
											sprintf( response, "%s  ---|---|---\r\n", response );
											sprintf( response, "%sc  %c | %c | %c \r\n", response, curr->ttt[2][0],  curr->ttt[2][1],  curr->ttt[2][2]);
											sprintf( response, "%s\r\n", response );
											sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
										}
									}
								}
							}
						}
						else sprintf(response, "You are not in this game!");
					}
				}
			}
		}
		else if( strcmp( command, "LIST" ) == 0 ){ 
			struct USER *curr2 = user_list_head;
			int userExists = 0, wrongPass = 0;

			while(curr2 != NULL){
				if(strcmp(username, curr2->username) == 0 && strcmp(password, curr2->password) == 0){
					userExists = 1;
					break;
				}
				else if(strcmp(username, curr2->username) == 0){
					wrongPass = 1;
					userExists = 1;
					break;
				}
				curr2 = curr2->next;
			}

			if(userExists == 0)
				strcpy(response, "User not found!");
			else if(wrongPass == 1)
				strcpy(response, "Wrong password!");
			else {
				struct GAME *curr = game_list_head;
				int gameExists = 0;

				while(curr!=NULL){
					sprintf( response, "%sGAME %s: CREATED BY %s, CHALLENGED BY: %s. ", response, curr->gamename, curr->creator->username, curr->challenger->username);
					if(curr->state == CREATOR_WON)
						strcat( response, "CREATOR WON");
					else if(curr->state == CHALLENGER_WON)
						strcat( response, "CHALLENGER WON");
					else if(curr->state == DRAW)
						strcat( response, "DRAW");
					else if(curr->state == IN_PROGRESS_CREATOR_NEXT)
						sprintf( response, "%sIN PROGRESS: %s to move next as o", response, curr->creator->username);
					else
						sprintf( response, "%sIN PROGRESS: %s to move next as x", response, curr->challenger->username);
					strcat( response, "\r\n");
					sprintf( response, "%sa  %c | %c | %c \r\n",response,  curr->ttt[0][0],  curr->ttt[0][1],  curr->ttt[0][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sb  %c | %c | %c \r\n", response, curr->ttt[1][0],  curr->ttt[1][1],  curr->ttt[1][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sc  %c | %c | %c \r\n", response, curr->ttt[2][0],  curr->ttt[2][1],  curr->ttt[2][2]);
					sprintf( response, "%s\r\n", response );
					sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
					curr = curr->next;
				}

			}
		}
		else if( strcmp( command, "SHOW" ) == 0 ){
			char *game_name = strtok( NULL, ",");

			struct USER *curr2 = user_list_head;
			int userExists = 0, wrongPass = 0;

			while(curr2 != NULL){
				if(strcmp(username, curr2->username) == 0 && strcmp(password, curr2->password) == 0){
					userExists = 1;
					break;
				}
				else if(strcmp(username, curr2->username) == 0){
					wrongPass = 1;
					userExists = 1;
					break;
				}
				curr2 = curr2->next;
			}

			if(userExists == 0)
				strcpy(response, "User not found!");
			else if(wrongPass == 1)
				strcpy(response, "Wrong password!");
			else {
				struct GAME *curr = game_list_head;
				int gameExists = 0;

				while(curr!=NULL){
					if(strcmp(curr->gamename,game_name)==0){
						gameExists = 1;
						break;
					}
					curr = curr->next;
				}
				if(gameExists == 0){
					strcpy(response,"Game does not exist!");
				}
				else {
					sprintf( response, "GAME %s BETWEEN %s AND %s.\r\n", curr->gamename, curr->creator->username, curr->challenger->username);
					if(curr->state == CREATOR_WON)
						strcat( response, "CREATOR WON");
					else if(curr->state == CHALLENGER_WON)
						strcat( response, "CHALLENGER WON");
					else if(curr->state == DRAW)
						strcat( response, "DRAW");
					else if(curr->state == IN_PROGRESS_CREATOR_NEXT)
						sprintf( response, "%sIN PROGRESS: %s to move next as o", response, curr->creator->username);
					else
						sprintf( response, "%sIN PROGRESS: %s to move next as x", response, curr->challenger->username);
					strcat( response, "\r\n");
					sprintf( response, "%sa  %c | %c | %c \r\n",response,  curr->ttt[0][0],  curr->ttt[0][1],  curr->ttt[0][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sb  %c | %c | %c \r\n", response, curr->ttt[1][0],  curr->ttt[1][1],  curr->ttt[1][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sc  %c | %c | %c \r\n", response, curr->ttt[2][0],  curr->ttt[2][1],  curr->ttt[2][2]);
					sprintf( response, "%s\r\n", response );
					sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
				}
			}
		}
		else{
	  		sprintf( response, "COMMAND %s NOT IMPLEMENTED", command );
		}

		write(client_sock , response , 2000);  
		close(client_sock);
	}

	close(socket_desc);	
	
	return 0;
}


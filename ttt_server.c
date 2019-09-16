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

int checkWin(char ttt[3][3]) {
	if ((ttt[0][0]=='X' && ttt[1][0]=='X' && ttt[2][0]=='X') || (ttt[0][1]=='X' && ttt[1][1]=='X' && ttt[2][1]=='X') || (ttt[0][2]=='X' && ttt[1][2]=='X' &&
	ttt[2][2]=='X') || (ttt[0][0]=='X' && ttt[0][1]=='X' && ttt[0][2]=='X') || (ttt[1][0]=='X' && ttt[1][1]=='X' && ttt[1][2]=='X') || (ttt[2][0]=='X' &&
	ttt[2][1]=='X' && ttt[2][2]=='X') || (ttt[0][0]=='X' && ttt[1][1]=='X' && ttt[2][2]=='X') || (ttt[0][2]=='X' && ttt[1][1]=='X' && ttt[2][0]=='X')) {
		return 1;
	}
	if ((ttt[0][0]=='O' && ttt[1][0]=='O' && ttt[2][0]=='O') || (ttt[0][1]=='O' && ttt[1][1]=='O' && ttt[2][1]=='O') || (ttt[0][2]=='O' && ttt[1][2]=='O' &&
	ttt[2][2]=='O') || (ttt[0][0]=='O' && ttt[0][1]=='O' && ttt[0][2]=='O') || (ttt[1][0]=='O' && ttt[1][1]=='O' && ttt[1][2]=='O') || (ttt[2][0]=='O' &&
	ttt[2][1]=='O' && ttt[2][2]=='O') || (ttt[0][0]=='O' && ttt[1][1]=='O' && ttt[2][2]=='O') || (ttt[0][2]=='O' && ttt[1][1]=='O' && ttt[2][0]=='O')) {
		return 2;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (ttt[i][j] == ' ') {
				return 0;
			}
		}
	}
	return 3;
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
			struct USER *newUser;
			struct USER *temp;
			int isNewUser = 1;

			if (user_list_head == NULL) {
				newUser = (struct USER *)malloc(sizeof(struct USER));
				strcpy(newUser->username, username);
				strcpy(newUser->password, password);
				newUser->next = NULL;
				user_list_head = newUser;
				strcpy(response, "NEW USER CREATED");
			} else {
				temp = user_list_head;

				if (strcmp(temp->username, username) == 0) { // check the head
					if (strcmp(temp->password, password) == 0) {
						sprintf(response, "WELCOME BACK %s", username);
					} else {
						strcpy(response, "ERROR INVALID PASSWORD");
					}
					isNewUser = 0;
				}

				while(temp->next != NULL) {
					if (strcmp(temp->username, username) == 0) {
						if (strcmp(temp->password, password) == 0) {
							sprintf(response, "WELCOME BACK %s", username);
						} else {
							strcpy(response, "ERROR INVALID PASSWORD");
						}
						isNewUser = 0;
						break;
					}
					temp = temp->next;
				}

				if (strcmp(temp->username, username) == 0) { // check the last USER
					if (strcmp(temp->password, password) == 0) {
						sprintf(response, "WELCOME BACK %s", username);
					} else {
						strcpy(response, "ERROR INVALID PASSWORD");
					}
					isNewUser = 0;
				}

				if (isNewUser == 1) {
					newUser = (struct USER *)malloc(sizeof(struct USER));
					strcpy(newUser->username, username);
					strcpy(newUser->password, password);
					newUser->next = NULL;
					temp->next = newUser;
					strcpy(response, "NEW USER CREATED");
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

			struct USER *tempUser = user_list_head;
			int validLogIn = 0;

			while (tempUser != NULL) {
				if (strcmp(tempUser->username, username)==0 && strcmp(tempUser->password, password)==0) {
					validLogIn = 1;
				}
				tempUser = tempUser->next;
			}

			if (validLogIn == 1) {
				struct GAME *newGame;
				struct GAME *tempGame;

				if (game_list_head == NULL) {
					newGame = (struct GAME*)malloc(sizeof(struct GAME));
					strcpy(newGame->gamename, game_name);
					struct USER *creator = (struct USER*)malloc(sizeof(struct USER));
					strcpy(creator->username, username);
					strcpy(creator->password, password);
					newGame->creator = creator;
					for( int row=0; row<3; row++ )
						for( int col=0; col<3; col++ )
							newGame->ttt[row][col] = ' ';
					newGame->next = NULL;

					game_list_head = newGame;

					sprintf( response, "GAME %s CREATED WAITING FOR OPPONENT TO JOIN\r\n", newGame->gamename);
					sprintf( response, "%sa  %c | %c | %c \r\n",response,  newGame->ttt[0][0],  newGame->ttt[0][1],  newGame->ttt[0][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sb  %c | %c | %c \r\n", response, newGame->ttt[1][0],  newGame->ttt[1][1],  newGame->ttt[1][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sc  %c | %c | %c \r\n", response, newGame->ttt[2][0],  newGame->ttt[2][1],  newGame->ttt[2][2]);
					sprintf( response, "%s\r\n", response );
					sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
				} else {
					int isNewGame = 1;
					tempGame = game_list_head;

					if (strcmp(tempGame->gamename, game_name) == 0) {
						isNewGame = 0;
					}
					while(tempGame->next != NULL) {
						if (strcmp(tempGame->gamename, game_name) == 0) {
							isNewGame = 0;
						}
						tempGame = tempGame->next;
					}
					if (strcmp(tempGame->gamename, game_name) == 0) {
						isNewGame = 0;
					}

					if (isNewGame == 1) {
						newGame = (struct GAME*)malloc(sizeof(struct GAME));
						strcpy(newGame->gamename, game_name);
						struct USER *creator = (struct USER*)malloc(sizeof(struct USER));
						strcpy(creator->username, username);
						strcpy(creator->password, password);
						newGame->creator = creator;
						for( int row=0; row<3; row++ )
							for( int col=0; col<3; col++ )
								newGame->ttt[row][col] = ' ';
						newGame->next = NULL;
						tempGame->next = newGame;

						sprintf( response, "GAME %s CREATED WAITING FOR OPPONENT TO JOIN\n", newGame->gamename);
						sprintf( response, "%sa  %c | %c | %c \n",response,  newGame->ttt[0][0],  newGame->ttt[0][1],  newGame->ttt[0][2]);
						sprintf( response, "%s  ---|---|---\n", response );
						sprintf( response, "%sb  %c | %c | %c \n", response, newGame->ttt[1][0],  newGame->ttt[1][1],  newGame->ttt[1][2]);
						sprintf( response, "%s  ---|---|---\n", response );
						sprintf( response, "%sc  %c | %c | %c \n", response, newGame->ttt[2][0],  newGame->ttt[2][1],  newGame->ttt[2][2]);
						sprintf( response, "%s\n", response );
						sprintf( response, "%s   %c   %c   %c\n", response, '1', '2', '3' );
					} else {
						strcpy(response, "ERROR GAME ALREADY EXISTS");
					}
				}
			} else {
				strcpy(response, "ERROR INVALID USER");
			}
		}

		else if( strcmp( command, "JOIN" ) == 0 ){
			char *game_name = strtok( NULL, ",");
			char *position = strtok( NULL, ",");

			if( position == NULL || game_name == NULL){
				sprintf( response, "JOIN COMMAND MUST BE CALLED AS FOLLOWS:\n" );
				sprintf( response+strlen(response), "JOIN,USER,PASS,GAMENAME,POSITION\n" );
				write(client_sock , response , 2000);
				close(client_sock);
				continue;
			} else if (strcmp(position, "a1")!=0 && strcmp(position, "a2")!=0 && strcmp(position, "a3")!=0 && strcmp(position, "b1")!=0 && strcmp(position, "b2")!=0 && strcmp(position, "b3")!=0 && strcmp(position, "b3") && strcmp(position, "c1")!=0 && strcmp(position, "c2")!=0 && strcmp(position, "c3")!=0) {
				strcpy(response, "ERROR INVALID POSITION");
				write(client_sock, response, 2000);
				close(client_sock);
				continue;
			}

			struct USER *tempUser = user_list_head;
			int validLogIn = 0;

			while (tempUser != NULL) {
				if (strcmp(tempUser->username, username)==0 && strcmp(tempUser->password, password)==0) {
					validLogIn = 1;
				}
				tempUser = tempUser->next;
			}

			if (validLogIn == 1) {
				struct GAME *tempGame = game_list_head;
				int canJoin = 0;

				while(tempGame != NULL) {
					if (strcmp(tempGame->gamename, game_name)==0 && tempGame->challenger == NULL) {
							struct USER *challenger = (struct USER*)malloc(sizeof(struct USER));
							strcpy(challenger->username, username);
							strcpy(challenger->password, password);
							tempGame->challenger = challenger;

							if (position[0] == 'a') {
								tempGame->ttt[0][(position[1]-'0')-1] = 'X';
							} else if(position[0] == 'b') {
								tempGame->ttt[1][(position[1]-'0')-1] = 'X';
							} else if (position[0] == 'c') {
								tempGame->ttt[2][(position[1]-'0')-1] = 'X';
							}
							tempGame->state = IN_PROGRESS_CREATOR_NEXT;

							canJoin = 1;
							break;
					}
					tempGame = tempGame->next;
				}

				if (canJoin == 1) {
					sprintf( response, "GAME %s BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);
					sprintf(response, "%sIN PROGRRESS: %s TO MOVE NEXT AS O\r\n", response, tempGame->creator->username);
					sprintf( response, "%sa  %c | %c | %c \r\n",response,  tempGame->ttt[0][0],  tempGame->ttt[0][1],  tempGame->ttt[0][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sb  %c | %c | %c \r\n", response, tempGame->ttt[1][0],  tempGame->ttt[1][1],  tempGame->ttt[1][2]);
					sprintf( response, "%s  ---|---|---\r\n", response );
					sprintf( response, "%sc  %c | %c | %c \r\n", response, tempGame->ttt[2][0],  tempGame->ttt[2][1],  tempGame->ttt[2][2]);
					sprintf( response, "%s\r\n", response );
					sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
				} else {
					strcpy(response, "ERROR CANNOT JOIN GAME");
				}
			} else {
				strcpy(response, "ERROR INVALID USER");
			}
		}
		else if( strcmp( command, "MOVE" ) == 0 ){
			char *game_name = strtok( NULL, ",");
			char *position = strtok( NULL, ",");

			if( position == NULL || game_name == NULL){
				sprintf( response, "MOVE COMMAND MUST BE CALLED AS FOLLOWS:\n" );
				sprintf( response+strlen(response), "MOVE,USER,PASS,GAMENAME,POSITION\n" );
				write(client_sock , response , 2000);
				close(client_sock);
				continue;
			}  else if (strcmp(position, "a1")!=0 && strcmp(position, "a2")!=0 && strcmp(position, "a3")!=0 && strcmp(position, "b1")!=0 && strcmp(position, "b2")!=0 && strcmp(position, "b3")!=0 && strcmp(position, "b3") && strcmp(position, "c1")!=0 && strcmp(position, "c2")!=0 && strcmp(position, "c3")!=0) {
				strcpy(response, "ERROR INVALID POSITION");
				write(client_sock, response, 2000);
				close(client_sock);
				continue;
			}

			struct GAME* tempGame = game_list_head;
			int gameExists = 0;

			while(tempGame != NULL) {
				if (strcmp(tempGame->gamename, game_name)==0) {
					gameExists = 1;
					break;
				}
				tempGame = tempGame->next;
			}

			if (gameExists == 1) {
				int positionTaken = 0;

				if (tempGame->state == IN_PROGRESS_CREATOR_NEXT) {
					 if (strcmp(tempGame->creator->username, username)==0 && strcmp(tempGame->creator->password, password)==0) {
						 if (position[0] == 'a') {
							 if (tempGame->ttt[0][(position[1]-'0')-1] == ' ') {
								 tempGame->ttt[0][(position[1]-'0')-1] = 'O';
							 } else {
								 positionTaken = 1;
							 }
						 } else if(position[0] == 'b') {
							 if (tempGame->ttt[1][(position[1]-'0')-1] == ' ') {
								 tempGame->ttt[1][(position[1]-'0')-1] = 'O';
							 } else {
								 positionTaken = 1;
							 }
						 } else if (position[0] == 'c') {
							 if (tempGame->ttt[2][(position[1]-'0')-1] == ' ') {
								 tempGame->ttt[2][(position[1]-'0')-1] = 'O';
							 } else {
								 positionTaken = 1;
							 }
						 }

						 if (positionTaken == 0) {
							 if (checkWin(tempGame->ttt) == 0) {
								 tempGame->state = IN_PROGRESS_CHALLENGER_NEXT;

								 sprintf( response, "GAME %s BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);
								 sprintf(response, "%sIN PROGRRESS: %s TO MOVE NEXT AS X\r\n", response, tempGame->challenger->username);
							 } else if (checkWin(tempGame->ttt) == 2) {
								 tempGame->state = CREATOR_WON;

								 sprintf( response, "GAME %s BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);
								 sprintf(response, "%sGAME OVER: %s WINS\r\n", response, tempGame->creator->username);
							 } else if (checkWin(tempGame->ttt) == 3) {
								 tempGame->state = DRAW;

								 sprintf( response, "GAME %s BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);
								 sprintf(response, "%sGAME OVER: DRAW\r\n", response);
							 }
							 sprintf( response, "%sa  %c | %c | %c \r\n",response,  tempGame->ttt[0][0],  tempGame->ttt[0][1],  tempGame->ttt[0][2]);
							 sprintf( response, "%s  ---|---|---\r\n", response );
							 sprintf( response, "%sb  %c | %c | %c \r\n", response, tempGame->ttt[1][0],  tempGame->ttt[1][1],  tempGame->ttt[1][2]);
							 sprintf( response, "%s  ---|---|---\r\n", response );
							 sprintf( response, "%sc  %c | %c | %c \r\n", response, tempGame->ttt[2][0],  tempGame->ttt[2][1],  tempGame->ttt[2][2]);
							 sprintf( response, "%s\r\n", response );
							 sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
						 } else {
							 strcpy(response, "ERROR INVALID MOVE");
						 }
					 } else {
						 if (strcmp(tempGame->challenger->username, username)==0 && strcmp(tempGame->challenger->password, password)==0) {
							 strcpy(response, "ERROR IT IS NOT YOUR TURN");
						 } else {
							 strcpy(response, "ERROR INVALID USER");
						 }
					 }
				} else if (tempGame->state == IN_PROGRESS_CHALLENGER_NEXT) {
					if (strcmp(tempGame->challenger->username, username)==0 && strcmp(tempGame->challenger->password, password)==0) {
						if (position[0] == 'a') {
							if (tempGame->ttt[0][(position[1]-'0')-1] == ' ') {
								tempGame->ttt[0][(position[1]-'0')-1] = 'X';
							} else {
								positionTaken = 1;
							}
						} else if(position[0] == 'b') {
							if (tempGame->ttt[1][(position[1]-'0')-1] == ' ') {
								tempGame->ttt[1][(position[1]-'0')-1] = 'X';
							} else {
								positionTaken = 1;
							}
						} else if (position[0] == 'c') {
							if (tempGame->ttt[2][(position[1]-'0')-1] == ' ') {
								tempGame->ttt[2][(position[1]-'0')-1] = 'X';
							} else {
								positionTaken = 1;
							}
						}

						if (positionTaken == 0) {
							if (checkWin(tempGame->ttt) == 0) {
								tempGame->state = IN_PROGRESS_CREATOR_NEXT;

								sprintf( response, "GAME %s BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);
								sprintf(response, "%sIN PROGRRESS: %s TO MOVE NEXT AS O\r\n", response, tempGame->creator->username);
							} else if (checkWin(tempGame->ttt) == 1) {
								tempGame->state = CHALLENGER_WON;

								sprintf( response, "GAME %s BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);
								sprintf(response, "%sGAME OVER: %s WINS\r\n", response, tempGame->challenger->username);
							} else if (checkWin(tempGame->ttt) == 3) {
								tempGame->state = DRAW;

								sprintf( response, "GAME %s BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);
								sprintf(response, "%sGAME OVER: DRAW\r\n", response);
							}
							sprintf( response, "%sa  %c | %c | %c \r\n",response,  tempGame->ttt[0][0],  tempGame->ttt[0][1],  tempGame->ttt[0][2]);
							sprintf( response, "%s  ----|----|----\r\n", response );
							sprintf( response, "%sb  %c | %c | %c \r\n", response, tempGame->ttt[1][0],  tempGame->ttt[1][1],  tempGame->ttt[1][2]);
							sprintf( response, "%s  ----|----|----\r\n", response );
							sprintf( response, "%sc  %c | %c | %c \r\n", response, tempGame->ttt[2][0],  tempGame->ttt[2][1],  tempGame->ttt[2][2]);
							sprintf( response, "%s\r\n", response );
							sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
						} else {
							strcpy(response, "ERROR INVALID MOVE");
						}
					} else {
						if (strcmp(tempGame->creator->username, username)==0 && strcmp(tempGame->creator->password, password)==0) {
							strcpy(response, "ERROR IT IS NOT YOUR TURN");
						} else {
							strcpy(response, "ERROR INVALID USER");
						}
				}
				} else {
					strcpy(response, "ERROR GAME ENDED");
				}
			} else {
				strcpy(response, "ERROR GAME DOES NOT EXIST");
			}
		}
		else if( strcmp( command, "LIST" ) == 0 ){
			struct USER *tempUser = user_list_head;
			int validLogIn = 0;

			while (tempUser != NULL) {
				if (strcmp(tempUser->username, username)==0 && strcmp(tempUser->password, password)==0) {
					validLogIn = 1;
				}
					tempUser = tempUser->next;
			}

			if (validLogIn == 1) {
				struct GAME *tempGame = game_list_head;

				if (tempGame == NULL) {
					strcpy(response, "ERROR NO GAMES ONLINE");
				}

				while(tempGame != NULL) {
					if (tempGame->challenger == NULL) {
						sprintf(response, "%sGAME %s: CREATED BY %s WAITING FOR OPPONENT\r\n",response ,tempGame->gamename, tempGame->creator->username);
					} else {
						sprintf(response, "%sGAME %s: BETWEEN %s AND %s\r\n", response, tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);

						if (tempGame->state == CREATOR_WON) {
							sprintf(response, "%sGAME STATUS: %s WON\r\n", response, tempGame->creator->username);
						} else if (tempGame->state == IN_PROGRESS_CREATOR_NEXT) {
							sprintf(response, "%sGAME STATUS: IN PROGRESS %s TO MOVE NEXT AS O\r\n", response, tempGame->creator->username);
						} else if (tempGame->state == DRAW) {
							sprintf(response, "%sGAME STATUS: DRAW\r\n", response);
						} else if (tempGame->state == IN_PROGRESS_CHALLENGER_NEXT) {
							sprintf(response, "%sGAME STATUS: IN PROGRESS %s TO MOVE NEXT AS X\r\n", response, tempGame->challenger->username);
						} else {
							sprintf(response, "%sGAME STATUS: %s WON\r\n", response, tempGame->challenger->username);
						}
						sprintf( response, "%sa %c | %c | %c \r\n",response,  tempGame->ttt[0][0],  tempGame->ttt[0][1],  tempGame->ttt[0][2]);
						sprintf( response, "%s  ---|---|---\r\n", response );
						sprintf( response, "%sb %c | %c | %c \r\n", response, tempGame->ttt[1][0],  tempGame->ttt[1][1],  tempGame->ttt[1][2]);
						sprintf( response, "%s  ---|---|---\r\n", response );
						sprintf( response, "%sc %c | %c | %c \r\n", response, tempGame->ttt[2][0],  tempGame->ttt[2][1],  tempGame->ttt[2][2]);
						sprintf( response, "%s\r\n", response );
						sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
					}
					tempGame = tempGame->next;
				}
			} else{
					strcpy(response, "ERROR INVALID USER");
			}
		}
		else if( strcmp( command, "SHOW" ) == 0 ){
			char *game_name = strtok( NULL, ",");

			if(game_name == NULL){
				sprintf( response, "MOVE COMMAND MUST BE CALLED AS FOLLOWS:\n" );
				sprintf( response+strlen(response), "MOVE,USER,PASS,GAMENAME,POSITION\n" );
				write(client_sock , response , 2000);
				close(client_sock);
				continue;
			}

			struct USER *tempUser = user_list_head;
			int validLogIn = 0;

			while (tempUser != NULL) {
				if (strcmp(tempUser->username, username)==0 && strcmp(tempUser->password, password)==0) {
					validLogIn = 1;
				}
					tempUser = tempUser->next;
			}

			if (validLogIn == 1) {
				struct GAME *tempGame = game_list_head;
				int validGame = 0;

				while(tempGame != NULL) {
					if (strcmp(tempGame->gamename, game_name)==0) {
						if (tempGame->challenger == NULL) {
							sprintf(response, "GAME %s: CREATED BY %s WAITING FOR OPPONENT", tempGame->gamename, tempGame->creator->username);
						} else {
							sprintf(response, "GAME %s: BETWEEN %s AND %s\r\n", tempGame->gamename, tempGame->creator->username, tempGame->challenger->username);

							if (tempGame->state == CREATOR_WON) {
								sprintf(response, "%sGAME STATUS: %s WON\r\n", response, tempGame->creator->username);
							} else if (tempGame->state == IN_PROGRESS_CREATOR_NEXT) {
								sprintf(response, "%sGAME STATUS: IN PROGRESS %s TO MOVE NEXT AS O\r\n", response, tempGame->creator->username);
							} else if (tempGame->state == DRAW) {
								sprintf(response, "%sGAME STATUS: DRAW\r\n", response);
							} else if (tempGame->state == IN_PROGRESS_CHALLENGER_NEXT) {
								sprintf(response, "%sGAME STATUS: IN PROGRESS %s TO MOVE NEXT AS X\r\n", response, tempGame->challenger->username);
							} else {
								sprintf(response, "%sGAME STATUS: %s WON\r\n", response, tempGame->challenger->username);
							}
							sprintf( response, "%sa  %c | %c | %c \r\n",response,  tempGame->ttt[0][0],  tempGame->ttt[0][1],  tempGame->ttt[0][2]);
							sprintf( response, "%s  ---|---|---\r\n", response );
							sprintf( response, "%sb  %c | %c | %c \r\n", response, tempGame->ttt[1][0],  tempGame->ttt[1][1],  tempGame->ttt[1][2]);
							sprintf( response, "%s  ---|---|---\r\n", response );
							sprintf( response, "%sc  %c | %c | %c \r\n", response, tempGame->ttt[2][0],  tempGame->ttt[2][1],  tempGame->ttt[2][2]);
							sprintf( response, "%s\r\n", response );
							sprintf( response, "%s   %c   %c   %c\r\n", response, '1', '2', '3' );
						}
						validGame = 1;
						break;
					}
					tempGame = tempGame->next;
				}
				if (validGame == 0) {
					sprintf(response, "ERROR GAME %s NOT FOUND", game_name);
				}
			} else {
				strcpy(response, "ERROR INVALID USER");
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

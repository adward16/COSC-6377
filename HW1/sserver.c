#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> //used for reading options from command line

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int mysrv_sock, my_portno, n, written, host_sock, client, h, local_port, i, numWrite, p, d = 0, final_pos = 0;
    struct sockaddr_in websrv_addr, serv_addr, cli_addr;
    struct hostent *websrv;
    socklen_t clilen;
    char buffer[1000000]; char url_req[1000], h_path[500], host[500], h_buffer[1000], response[100], m_hash[200], m_buff[200];
    FILE *fp,*md5sum;

    if(argc < 3) printf("usage %s port hostname url",argv[0]);

    for(int count = 1; count < 3; ++count){
      if (strcmp(argv[count], "-p")==0)
      {
          local_port = atoi(argv[count+1]); //original code
          //printf("%d\n",mysrv_portno);
      }
    }


    host_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (host_sock < 0)
        error("ERROR opening socket");


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(local_port);


    if (bind(host_sock, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");

    while(1){
      printf("Waiting for clients\n");
      listen(host_sock,5);
      clilen = sizeof(cli_addr);

      client = accept(host_sock,(struct sockaddr *) &cli_addr,&clilen);
      if (client < 0){
        printf("ERROR occured accepting client connection\n");
        continue;
      }

      printf("A client has connected\n");

      if((h = read(client,h_buffer,sizeof(h_buffer))) < 0){
        printf("ERROR occured in reading from buffer \n");
        close(client);
        continue;
      }

      //printf("Checkpoint 1 Passed\n");

  /****** configutation for socket connection to web server ******/

      my_portno = 80; //original code
      mysrv_sock = socket(AF_INET, SOCK_STREAM, 0);
      if (mysrv_sock< 0){
        printf("ERROR occured creating web server socket\n");
        close(mysrv_sock);
        continue;
      }

      printf("Recieved URL: %s\n",h_buffer);

      for(i = 0; i < strlen(h_buffer); i++){
        if(h_buffer[i] == '/'){
          strncpy(host,h_buffer,i);
          host[i] = '\0';
          break;
        }
      }

      //printf("The host is %s \n", host);

      for(i; i<strlen(h_buffer); i++){
        strcat(h_path,&h_buffer[i]);
        break;
      }

      //printf("The path is %s\n",h_path);

      //printf("Checkpoint 2 Passed\n");

      websrv = gethostbyname(host);
         if (websrv == NULL) {
           printf("ERROR occured retrieiving host name from client\n");
           close(client);
           continue;
      }

      // sprintf(m_hash,"echo -n %s | MD5\n",h_buffer);
      //
      // md5sum = popen(m_hash,"r");
      //
      // if(md5sum != NULL){
      //
      //   fgets(m_buff,strlen(m_buff),md5sum);
      //
      // }
      // pclose(md5sum);

      bzero((char *) &websrv_addr, sizeof(websrv_addr));
      websrv_addr.sin_family = AF_INET;
      bcopy((char *)websrv->h_addr,
           (char *)&websrv_addr.sin_addr.s_addr,
           websrv->h_length);
      websrv_addr.sin_port = htons(my_portno);
      if(connect(mysrv_sock,(struct sockaddr *) &websrv_addr,sizeof(websrv_addr)) < 0){
        printf("ERROR occured connecting to web server\n");
        close(mysrv_sock);
        continue;
      }

      //n = write(mysrv_sock,"GET / HTTP/1.1 \r\n",strlen("GET / HTTP/1.1 \r\n"));

      //may want to us snprintf per Milad
      sprintf(url_req,"GET %s HTTP/1.0\r\nHost:%s\r\n\r\n",h_path,host);

      n = write(mysrv_sock,url_req,strlen(url_req));

      if(n < 0){
        printf("ERROR occured writing to client\n");
        close(client);
        continue;
      }

      bzero(buffer,sizeof(buffer));

      //printf("Checkpoint 3 Passed\n");

      fp = fopen("testing","wb+");

      if (fp!= NULL)
      {

        //printf("Checkpoint 4 Passed\n");

        while((n = read(mysrv_sock,buffer,sizeof(buffer))) != 0){

          //printf("There were %d bytes read from web socket\n", n);
          while(buffer[d] != '\0'){
            //printf("Value of d is now %d\n",d);
            if(buffer[d] == '\r'){
              if(buffer[d+1] == '\n'){
                if(buffer[d+2] == '\r'){
                  if(buffer[d+3] == '\n'){
                    final_pos = d + 3;
                    break;
                  }
                }
              }
            }
            d++;
          }
          numWrite = fwrite(&buffer[final_pos],1,n,fp);
          //printf("Checkpoint 5 Passed\n");

        }

        printf("Downloaded the page\n");

        //printf("Checkpoint 6 Passed\n");

          //check bytes in buffer for "\r\n\r\n" sequence
      }

      fclose(fp);
      close(mysrv_sock);

      //printf("Checkpoint 7 Passed\n");

      printf("Downloaded number of bytes: %d\n",numWrite);

      //printf("Sent file name to client: %s\n",m_buff);

      // if((p = sprintf(response,"Successful write to file of %d bytes\n",numWrite)) < 0){
      //     printf("The hang up is here\n");
      // }
      n = write(client,response,strlen(response));
      if(n < 0){
        printf("ERROR writing to socket for client\n");
        close(client);
        continue;
      }

      close(client);
    }
      return 0;

}

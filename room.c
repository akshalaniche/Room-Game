#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printHtml(int manna, int gold, char* toFind){
  FILE *html = fopen("room.html", "rt");
  char array[5000];
  fgets(array, 5000, html);
  while( !feof(html) ){
      /* Handle the inventory line here */
      if( strstr(array, "inventory")!=NULL ){
        printf("<input type='hidden' name='inventory' value='%d,%d'>", manna, gold);
	fgets(array, 5000, html);
      }
      if( (strcmp(toFind, "")!=0) && (strstr(array, toFind)!=NULL) ){
        if( strcmp(toFind, "<body>")==0 ){
        /* When the toFind string is "<body>" */
          printf("%s",array);
          printf("<h1>Invalid command! Please try again!</h1>\n");
          fgets(array, 5000, html);
        }
        else if( strcmp(toFind, "Welcome")==0 ){
        /* When the toFind string is "Welcome" */
          printf("%s",array);
          printf("<h2>Wake up! You don't have so much gold! The number of gold you have is: ");
          printf("%d\n </h2>", gold);
          fgets(array, 5000, html);
        }  
      }
      else{
        printf("%s",array);
        fgets(array, 5000, html);
      }
    }
    fclose(html);
}

void updateCSV(int manna, int gold, int room_status){
  FILE* fp = fopen("resources.csv", "w");
  fprintf(fp, "%d,", manna);
  fprintf(fp, "%d,", gold);
  fprintf(fp, "%d", room_status);
  fclose(fp);
}



int main()
{

  char input[250];
  int n = atoi(getenv("CONTENT_LENGTH"));
  fgets(input, n+1, stdin);
  char manna[5];
  char gold[5];
  char command[10];
  
  int i=0;
  for(i=0; (i <= n)&&(input[i]!='='); i++);
  i++;
  int j=0;
  for(; (i <= n)&&(input[i]!='%'); i++,j++){
    manna[j] = input[i];
  }
  manna[j]='\0';
  i+=3;
  int k=0;
  for(; (i <= n)&&(input[i]!='&'); i++,k++){
    gold[k] = input[i];
  }
  gold[k]='\0';
  for(;(i <= n)&&(input[i]!='=');i++);
  i++;
  int l=0;
  for(;(i <= n);i++,l++){
    command[l] = tolower(input[i]);
  }
  command[l]='\0';

  int mannaNumber=atoi(manna);
  int goldNumber=atoi(gold);


/*Read the room's resources*/
  FILE *res= fopen("resources.csv", "r");
  char temp[100];
  fgets(temp, 100, res);
  char *token;
  token=strtok(temp, ",");
  int roomManna=atoi(token);
  token=strtok(NULL, ",");
  int roomGold=atoi(token);
  fclose(res);

/*  int test = 1;
  char drop[5] = "drop";
  int m=0;
  for(; m < 4; m++){
      if (drop[m]!=command[m])
	test=0;
  }*/

  if( strncmp(command, "drop",4) == 0 ){ /* The command is drop n*/
    printf("Content-type: text/html\n\n");
    int droppedGold;
    int params = sscanf(command, "drop+%d", &droppedGold);
    if(params != 1){
      printHtml(mannaNumber, goldNumber, "<body>"); //No number inputted
    }
    else if( droppedGold > goldNumber){
      printHtml(mannaNumber, goldNumber, "Welcome");
    }
    else{
      goldNumber = goldNumber - droppedGold;
      mannaNumber = mannaNumber + droppedGold/2;


      updateCSV(roomManna, roomGold+droppedGold, 1);
      printHtml(mannaNumber, goldNumber, "");
    }
  } else if( strcmp(command, "play")==0 ){ /* The command is play */    
        updateCSV(roomManna, roomGold, 1);    
        
        printf("Content-type: text/html\n\n");
        printf("<html>\n");
        printf("<body>\n");

        /* Styling code block */
        printf("<title>Enchanted forest</title> <style type=\"text/css\">\n.dirbutton{border-radius: 8px; border-width: 2px; border-color: black;");
        printf("padding-top: 5px; padding-bottom: 5px; padding-right: 10px; padding-left: 10px; transition-duration: 0.4s; background-color: white; color:black }");
        printf(".dirbutton:hover{ background-color: #cc9966; color:whitesmoke;}");
        printf(".box{ height: 30px; border-width: 2px; border-color: black; }");
        printf("body{\nbackground-color:gainsboro;\nfont-family:Verdana, Geneva, Tahoma, sans-serif;\n}");
        printf(".center{ text-align: center;} input[type=\"text\"]{font-size:18px;}</style></head>");
   	
        printf("<body class=\"center\">");
        printf("<br/><p class=\"center\"><img src=\"room.jpg\" border= \"2\"/></p>");

        printf("<h1 class=\"center\">You have challenged the master of Nature</h1>");
        printf("<p class=\"center\">The master can manipulate all three of the elements of Fire, Water, and Grass. They might use any one at random against you<br />");
        printf("Your job is to choose one of the elements below and hope that it is effective against whatever the master chose.<br />");
        printf("Fire burns through Grass, Grass absorbs Water, Water washes away Fire.<br/>");
        printf("Choosing the most effective type gives you 3 points, choosing the least effective type gives you only 1 point, and choosing the same type gives you 2 points.<br />");
        printf("To win, you must reach 6 points in three tries. You can always give up by writing QUIT in the box...</p>");
       
        printf("<form class= \"center\" action = \"http://www.cs.mcgill.ca/~aanich/cgi-bin/game.cgi\" method=\"post\">");
        printf("<input type=\"hidden\" name=\"win\" value=\"0\">");
        printf("<input type=\"text\" class=\"box\" style=\"width: 500px; text-align:center;\" name=\"command\">");

        printf("<br><b>Choose:");
        printf("<input type=\"radio\" name=\"choice\" value=\"A\"><img src=\"fire.png\" />Fire &emsp;");
        printf("<input type=\"radio\" name=\"choice\" value=\"B\"><img src=\"water.png\" />Water &emsp;");
        printf("<input type=\"radio\" name=\"choice\" value=\"C\"><img src=\"grass.png\" />Grass &emsp;");

        printf("<input type=\"hidden\" name=\"manna\" value=\"%d\">", mannaNumber);
	      printf("<input type=\"hidden\" name=\"gold\" value=\"%d\"> <br />", goldNumber);
	      printf("<input class=\"dirbutton\" type=\"submit\" value=\"Call upon the forces of Nature\">   <br /> <br /> Score: <br /> ");
        printf("<input type=\"text\" class=\"box\" name=\"score\" value=\"0\" style=\"text-align:center;\" readonly><br />Tries left: <br />");
        printf("<input type=\"text\" class=\"box\" name=\"trials\" value=\"3\" style=\"text-align:center;\" readonly>  <br/></b>");
        printf("</form></body></html>");
    }
      
   else if( strcmp(command,"exit")==0 ){ /* command is EXIT */
      updateCSV(roomManna+mannaNumber, roomGold+goldNumber, 0);

      printf("Content-type: text/html\n\n");
      printf("<html>\n");
      printf("<head>");
      /* Styling code block */
      printf("<title>Enchanted Forest</title> <style type=\"text/css\">\n.dirbutton{border-radius: 8px; border-width: 2px; border-color: black;");
      printf("padding-top: 5px; padding-bottom: 5px; padding-right: 10px; padding-left: 10px; transition-duration: 0.4s; background-color: white; color:black }");
      printf(".dirbutton:hover{ background-color: #cc9966; color:whitesmoke;}");
      printf(".box{ height: 20px; border-width: 2px; border-color: black; }");
      printf("body{text-align:center; background-color:gainsboro;\nfont-family:Verdana, Geneva, Tahoma, sans-serif;\n}");
      printf(".center{ text-align: center;} </style></head>");

      printf("<body>\n");
      printf("<h1>So sad that you're leaving...</h1>\n");
      printf("<img src='exit.jpg' class='center'");
      printf("</br>");
      printf("<h2>Hope to see you again soon!</h2>\n");
      printf("</body>\n");
      printf("</html>\n");
    }

    else if( strcmp(command,"refresh")==0 ){ /* command is REFRESH */
      updateCSV(roomManna, roomGold, 1); //Just in case, to keep the room's status to occupied.

      printf("Content-type: text/html\n\n");
      printHtml(mannaNumber, goldNumber, "");  
   }

else{
      printf("Content-type: text/html\n\n");
      printHtml(mannaNumber, goldNumber, "<body>");      
    }

   return 0;
}



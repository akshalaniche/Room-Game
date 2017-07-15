#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main()
{
    printf("Content-type: text/html\n\n");
    printf("<html>");
	printf("<head>");
    printf("<title>Enchanted Forest</title> <style type=\"text/css\">\n.dirbutton{border-radius: 8px; border-width: 2px; border-color: black;");
    printf("padding-top: 5px; padding-bottom: 5px; padding-right: 10px; padding-left: 10px; transition-duration: 0.4s; background-color: white; color:black }");
    printf(".dirbutton:hover{ background-color: #cc9966; color:whitesmoke;}");
    printf(".box{ height: 30px; border-width: 2px; border-color: black; }");
    printf("body{\nbackground-color:gainsboro; font-family:Verdana, Geneva, Tahoma, sans-serif;}");
    printf(".center{ text-align: center;} input[type=\"text\"]{font-size:18px;} </style></head>");
    
    int n = atoi(getenv("CONTENT_LENGTH"));
    char string[n+1];
    fgets(string,n+1,stdin);
    char win[2];
    char manna[5];
    char gold[5];
    
    int i;
    for(i=0; (i<=n)&&(string[i]!='='); i++);
    i++;
    int f=0;
    //win status
    for(; (i<=n)&&(string[i]!='&'); i++,f++){
        win[f] = string[i];
    }
    win[f]='\0';
    int win_int=atoi(win);

    if (win_int == 1){ //The game is over, the player has chosen their loot
        //Parse the strings 
        char mLoot[5];
        char gLoot[5];
       
        //manna 
        for(; (i<100)&&(string[i]!='='); i++);
        i++;
        int n=0;
        for(; (i<100)&&(string[i]!='&'); i++,n++){
            manna[n] = string[i];
        }
        manna[n]='\0';
        int manna_int=atoi(manna);
        
        //gold
        for(; (i<100)&&(string[i]!='='); i++);
        i++;
        int z=0;
        for(; (i<100)&&(string[i]!='&'); i++,z++){
            gold[z] = string[i];
        }
        gold[z]='\0';
        int gold_int=atoi(gold);
        //mannaLoot
        for(; (i<100)&&(string[i]!='='); i++);
        i++;
        int k=0;
        for(; (i<100)&&(string[i]!='&'); i++,k++){
            mLoot[k] = string[i];
        }
        mLoot[k]='\0';
        int mannaLoot=atoi(mLoot);
        
        //goldLoot
        for(; (i<100)&&(string[i]!='='); i++);
        int y=0;
        i++;
        gLoot[y]=string[i];
        y++;
        gLoot[y]='\0';
        int goldLoot=atoi(gLoot);

        FILE *res= fopen("resources.csv", "r");
        char temp[100];
        fgets(temp, 100, res);
        char *token;
        token=strtok(temp, ",");
        int roomManna=atoi(token);
        token=strtok(NULL, ",");
        int roomGold=atoi(token);
        fclose(res);
        
        if (mannaLoot+goldLoot>5) {
            printf("<body class=\"center\"> <h4>Sadly, you went over your head...<br/> To punish you for your greed, you will not be awarded anything!</h4>");
            printf("<form class=\"center\" action=\"http://www.cs.mcgill.ca/~aanich/cgi-bin/room.cgi\" method=\"post\"> <input type=\"hidden\" name=\"inventory\" value=\"%d,%d\">",manna_int, gold_int );
            printf("<input type=\"hidden\" name=\"command\" value=\"REFRESH\"> <br/>");
            printf("<input type=\"submit\" class=\"dirbutton\" value=\"Go back to the room\"></form></body></html>");
        }
        else{
            //Check if the room has enough resources. If not, give all that we can give, but not more of another resource
            mannaLoot=((roomManna>mannaLoot)?(mannaLoot):(roomManna));
            goldLoot=((roomGold>goldLoot)?(goldLoot):(roomGold));

            roomManna=roomManna-mannaLoot;
            roomGold=roomGold-goldLoot;
            manna_int+=mannaLoot;
            gold_int+=goldLoot;
        
            FILE *fw=fopen("resources.csv","w");
            fprintf(fw, "%d,%d,1", roomManna, roomGold);
            fclose(fw);

            if (gold_int >=100){
                //The player won
		system("python success.py");
                printf("<body class=\"center\"> <h1> CONGRATULATIONS!!! </h1> <h3>You have collected 100 gold pieces </h3>");
                printf("<img src=\"win.jpg\" border=\"2\"/><br /> <p>You found the Golden Path that leaves the magical forest</p>");
                printf("<p>Good luck on the rest of your adventure</p></body></html>");
            } else {
                printf("<body class=\"center\"><img src=\"room.jpg\" border=\"2\"/> <br/> <p> You received %d manna and %d gold. </p>",mannaLoot, goldLoot);
                printf("<form class=\"center\" action=\"http://www.cs.mcgill.ca/~aanich/cgi-bin/room.cgi\" method=\"post\"> <input type=\"hidden\" name=\"inventory\" value=\"%d,%d\">",manna_int, gold_int );
                printf("<input type=\"hidden\" name=\"command\" value=\"REFRESH\"> <br/>");
                printf("<input type=\"submit\" class=\"dirbutton\" value=\"Go back to the room\"></form></body></html>");
            }
        }

    } else { //The game is not over
        char command[10];
        char score[2];
        char trials[2];
        char choice[2];
        //command
        for(; (i<=n)&&(string[i]!='='); i++);
        i++;
        int j=0;
        for(; (i<=n)&&(string[i]!='&'); i++,j++){
            command[j] = tolower(string[i]);
        }
        command[j]='\0';
        //choice
	if (strstr(string, "choice")!= NULL){
        	for(; (i<=n)&&(string[i]!='='); i++);
        	i++;
        	int z=0;
        	for(; (i<=n)&&(string[i]!='&'); i++,z++){
        	   choice[z] = string[i];
        	}
        	choice[z]='\0';
	}
	else{
		choice[0]='0';
		choice[1]='\0';
	}
    
        //manna
        for(; (i<=n)&&(string[i]!='='); i++);
        i++;
        int k=0;
        for(; (i<=n)&&(string[i]!='&'); i++,k++){
            manna[k] = string[i];
        }
        manna[k]='\0';
        int manna_int=atoi(manna);
    
        //gold
        for(; (i<=n)&&(string[i]!='='); i++);
        i++;
        int l=0;
        for(; (i<=n)&&(string[i]!='&'); i++,l++){
            gold[l] = string[i];
        }
        gold[l]='\0';
        int gold_int=atoi(gold);
        
        //score
        for(; (i<=n)&&(string[i]!='='); i++);
        i++;
        int m=0;
        for(; (i<=n)&&(string[i]!='&'); i++,m++){
            score[m] = string[i];
        }
        score[m]='\0';
        int score_int=atoi(score);

        //trials
        for(; (i<=n)&&(string[i]!='='); i++);
        i++;
        int w=0;
        for(; (i<=n)&&(string[i]!=EOF)&&(string[i]!='\0'); i++,w++){
            trials[w] = string[i];
        }
        trials[w]='\0';
        int trials_int=atoi(trials);
        
        if(strcmp(command, "quit")==0) //The player doesn't want to continue, refresh the page
        {
            printf("<body class=\"center\"><br/><br/> <p class='center'><img src=\"room.jpg\" border=\"2\"/> </p> <br/> <h1 class=\"center\"> It's ok if you don't think you can win </h1>");
            printf("<form class=\"center\" action=\"http://www.cs.mcgill.ca/~aanich/cgi-bin/room.cgi\" method=\"post\"> <input type=\"hidden\" name=\"inventory\" value=\"%d,%d\">",manna_int, gold_int );
            printf("<input type=\"hidden\" name=\"command\" value=\"REFRESH\"> <br/>");
            printf("<input class=\"dirbutton\" type=\"submit\" value=\"The master says bye!\"></form></body></html>");
        }
        else{
            
            time_t t;
            srand((unsigned) time(&t));
            char master_choice='A'+rand()%3;
            int state=(choice[0]-master_choice)%3;
            if (choice[0] != '0'){
                if(state==0){
                    score_int+=2;
                } else if (state==1){
                    score_int+=3;
                } else{
                    score_int+=1;
                }
            }
            trials_int--;
            


            if(trials_int<=0){ //After doing the last calculations, there are no more tries left
                if (score_int<6){ //Player lost, print loss screen, add button to refresh 
                    printf("<body class=\"center\"><br/><h1>Boohoo the master has defeated you!</h1> <img src=\"room.jpg\" border=\"2\"");
                    printf("<br/><p>We are really sorry you lost! You only got %d points.  Maybe try again later?<br />", score_int);
                    printf("Anyway, go back to the room by clicking the button below!</p>");
                    printf("<form action=\"http://www.cs.mcgill.ca/~aanich/cgi-bin/room.cgi\" method=\"post\"> <input type=\"hidden\" name=\"inventory\" value=\"%d,%d\"> <input type=\"hidden\" name=\"command\" value=\"REFRESH\">", manna_int, gold_int);
                    printf("<input type=\"submit\" value=\"Go back!\" class=\"dirbutton\"> </form></body></html>");
                } else { //The player won, print the win screen with win=1, let them choose the resources
                    printf("<body class=\"center\"> <h1>WoW You defeated the Master!! You scored %d!</h1>",score_int);
                    FILE *res= fopen("resources.csv", "r");
                    char temp[100];
                    fgets(temp, 100, res);
                    char *token;
                    token=strtok(temp, ",");
                    int roomManna=atoi(token);
                    token=strtok(NULL, ",");
                    int roomGold=atoi(token);
                    fclose(res);
                    printf("<p>You can take upto 5 units of resources from Nature!<br/>");
                    printf("Please don't be greedy: if you try to steal, the master will not let you take anything at all!<br />");
                    printf("You can't obtain something that doesn't exist either: don't try to take more than Nature can provide, you will only get what you can be given... </p>");
                    printf("<p>Please choose from %d manna and %d gold!</p>", roomManna, roomGold);
                    printf("<form action=\"http://www.cs.mcgill.ca/~aanich/cgi-bin/game.cgi\" method=\"post\"><input type=\"hidden\" name=\"win\" value=\"1\"> ");
                    printf("<input type=\"hidden\" name=\"manna\" value=\"%d\" > <input type=\"hidden\" name=\"gold\" value=\"%d\" > <br/>", manna_int, gold_int);
                    printf("Manna: <input type=\"number\" min=\"0\" max=\"5\" class=\"box\" name=\"mannaLoot\"> &emsp;&emsp; Gold: <input type=\"number\" min=\"0\" max=\"5\" class=\"box\" name=\"goldLoot\"><br />");
                    printf("<input type=\"submit\" class=\"dirbutton\" value=\"Ask nicely\"></form></body></html>");
                }
            } else{
                //This should only be executed if there are at least one try left
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
                printf("<input type=\"hidden\" name=\"manna\" value=\"%d\">", manna_int);
                printf("<input type=\"hidden\" name=\"gold\" value=\"%d\"> <br />", gold_int);
                printf("<input class=\"dirbutton\" type=\"submit\" value=\"Call upon the forces of Nature\">   <br /><br />Score:<br />");
                printf("<input type=\"text\" class=\"box\" name=\"score\" value=\"%d\" style=\"text-align:center;\" readonly><br />Tries left:<br />", score_int);
                printf("<input type=\"text\" class=\"box\" name=\"trials\" value=\"%d\" style=\"text-align:center;\" readonly>  <br/>", trials_int);
                printf("</b></form></body></html>");
            }
        }
    }
    return 0;
}

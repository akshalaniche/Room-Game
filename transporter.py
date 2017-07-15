#!/usr/bin/python
import urllib2
from urllib2 import Request
from urllib2 import HTTPError
import cgi
import cgitb
import os
cgitb.enable()


#Collect the data from cgi
form=cgi.FieldStorage()
inventory= form["inventory"].value
URL=form["URL"].value
inv=inventory.replace(","," ").split()
playerManna=int(inv[0])
playerGold=int(inv[1])


#open the resources.csv file
try:
    file = open("resources.csv", "r")
except IOError:
    pass

#Read the only line in the file, giving the resources in the room
#Parses the string into a list, separating at the commas
#The third value gives us the status occupied or not of the room
#It is assumed that there are no white spaces
string=file.readline()
roomManna,roomGold,occupied=string.replace(","," ").split()
file.close()


if ( int(occupied) == 1) :
    #Send back to the room they came from
    print "Content-type: text/html\n\n"
    print """<html>
        <head>
        <title>Enchanted forest</title> <style type="text/css"> 
                .dirbutton{border-radius: 8px;
                    border-width: 2px;
                    border-color: black;
                    padding-top:5px; padding-bottom: 5px;
                    padding-left:10px; padding-right: 10px;
                    transition-duration: 0.4s;
                    background-color: white;
                    color:black;
                }
                .box{ height: 20px; border-width: 2px; border-color: black; }
                .dirbutton:hover{
                    background-color: #cc9966;
                    color:whitesmoke;
                }
                .center{ text-align: center;}  
                body{
                    text-align:center;
                    background-color:gainsboro;
                    font-family:Verdana, Geneva, Tahoma, sans-serif;
                }
                input[type=\"text\"]{font-size:18px;}
                </style></head>"""
    print """<body> <br/><h1>TRANSPORTATION FAILED<br /></h1>
            <img border="2" src="transporter_fail.jpg" /> <br/>
            <h3> The room is occupied! We couldn't possibly let you enter </h3> """
    if (URL == "http://cs.mcgill.ca/~sawadi/cgi-bin/room.cgi"):
        print """<form class="center" action="{}" method="GET">
            <input type="hidden" name="inventory" value="{},{}">
            <input type="hidden" name="command" value="REFRESH">
            <input type="submit" class="dirbutton" value="Maybe you'll be luckier later?">
            </form></body> </html>""".format(URL, playerManna, playerGold)
    else:
        print """<form class="center" action="{}" method="POST">
            <input type="hidden" name="inventory" value="{},{}">
            <input type="hidden" name="command" value="REFRESH">
            <input type="submit" class="dirbutton" value="Maybe you'll be luckier later?">
            </form></body> </html>""".format(URL, playerManna, playerGold)
    
else :
    #The following will only work if the URL is the one linking to room.cgi and if success.py of the other group doesn't take input
    #Let them enter our room and take one manna
    roomManna=int(roomManna) + 1
    URL=str(URL)
    
    
    index=0
    for i in range(20,0,-1):
	    if (URL[i]=='/'):
		    index=i+1
		    break
    
    NewURL=URL[0:index]
    
    #Call success.py from the old room's directory
    success=str(NewURL)+"success.py"
    try:
        req=urllib2.Request(success)
        urllib2.urlopen(req)   
    except HTTPError:
        pass 
    except ValueError:
	    pass
    except urllib2.URLError:
	    pass

    if (playerManna == 1):
        #The player is going to lose. The room takes their gold and adds it to its resources
        roomGold=int(roomGold) + int(playerGold)

        #Update the resources.csv file
        f= open("resources.csv","w")
        f.write("{},{},0".format(roomManna,roomGold))
        f.close()

        print "Content-type: text/html\n\n"
        print """<html>
        <head>
        <title>Enchanted forest</title> <style type="text/css"> 
                .dirbutton{border-radius: 8px;
                    border-width: 2px;
                    border-color: black;
                    padding-top:5px; padding-bottom: 5px;
                    padding-left:10px; padding-right: 10px;
                    transition-duration: 0.4s;
                    background-color: white;
                    color:black;
                }
                .box{ height: 20px; border-width: 2px; border-color: black; }
                .dirbutton:hover{
                    background-color: #cc9966;
                    color:whitesmoke;
                }
                .center{ text-align: center;}  
                body{
                    text-align:center;
                    background-color:gainsboro;
                    font-family:Verdana, Geneva, Tahoma, sans-serif;
                }
                input[type=\"text\"]{font-size:18px;}
                </style></head>"""
        print """<body>
        <br/><h1>GAME OVER</h1> 
        <img src="death.jpg" border="2" style="width:800px">
        <h2> We are sorry, you are out of manna!
        <br />We will keep good care of your gold! </h2>
        </body></html>"""

        os.system("python success.py")


    else:
        #The player loses one manna
        playerManna=playerManna-1

        #Update the resources.csv file
        f= open("resources.csv","w")
        f.write("{},{},1".format(roomManna,roomGold))
        f.close()

        #Uses the command refresh of room.cgi to draw the room with the player's updated inventory 
        print "Content-type: text/html\n\n"
        print """<html>
        <head>
        <title>Enchanted forest</title> <style type="text/css"> 
                .dirbutton{border-radius: 8px;
                    border-width: 2px;
                    border-color: black;
                    padding-top:5px; padding-bottom: 5px;
                    padding-left:10px; padding-right: 10px;
                    transition-duration: 0.4s;
                    background-color: white;
                    color:black;
                }
                .box{ height: 20px; border-width: 2px; border-color: black; }
                .dirbutton:hover{
                    background-color: #cc9966;
                    color:whitesmoke;
                }
                .center{ text-align: center;}  
                body{
                    text-align:center;
                    background-color:gainsboro;
                    font-family:Verdana, Geneva, Tahoma, sans-serif;
                }
                input[type=\"text\"]{font-size:18px;}
                </style></head>"""

        print """<body><br/><h1>You were given the permission to enter this threshold</h1> <br/>
        <img border="2" src="transporter_success.jpg" /><br/>
        <p>Hurry up and click the button below before the room changes its mind!!</p>
        <form action="http://www.cs.mcgill.ca/~aanich/cgi-bin/room.cgi" class="center" method="post"> 
	<input type="hidden" name="inventory" value="{},{}">
        <input type="hidden" name="command" value="REFRESH">
        <input type="submit" class="dirbutton" value="Nature's magic awaits">
        </form></body></html>
        """.format(playerManna, playerGold)     
       
        
    

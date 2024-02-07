dout = open("newdialog.txt",'w+')

with open("dialogs.txt") as dfile:
    line_count = 0
    for line in dfile:
        # Find the start of the string
        indx = line.find('\"')
        if indx is not -1:
            # There should always be another one at the end of the current line
            eindx = line.rfind('\"')
            if eindx == -1:
                print("Error in line!\n--:"+line)
                exit(1)
            # Get the actuall text
            ckstr = line[indx+1 : eindx]
            ckstr = ckstr.replace("\\n",' ')
            print("Got String:"+ckstr)
            # Split the string every 20 characters
            splitstr = [ckstr[i:i+20] for i in range(0, len(ckstr), 20)]
            # Join them all together with a \n
            newckstr = ''.join(splitstr)
            print("Made String:"+str(splitstr))
            
            
            

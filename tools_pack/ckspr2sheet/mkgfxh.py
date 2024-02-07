import re

outfile = open("CK6_GFX.h", "w+")

chunkid = 0
foundstart = False

with open("GFXE_CK6.H") as gfxfile:
    for line in gfxfile:
        clean_line = line[0 : len(line)-1].strip()
        if len(clean_line):
            if(clean_line[0]!='#' and clean_line[0]!='/'):
                linelist = clean_line.replace('(',',').split(',')
                if(len(linelist[0])):
                    if(linelist[0] == "START_LUMP"):
                        foundstart = True
                        chunkid = 1
                    elif (linelist[0] == "END_LUMP"):
                        chunkid = 1
                        foundstart = False
                    else:
                        outfile.write("\t"+linelist[0]+" = "+str(chunkid)+",\n")
                        if(foundstart==True):
                            chunkid = chunkid+1

outfile.close()


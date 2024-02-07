import re

outfile = open("CK4_GFX.h", "w+")

chunkid = 0
foundstart = False
foundspritelist = False

with open("GFXE_CK4.H") as gfxfile:
    for line in gfxfile:
        clean_line = line[0 : len(line)-1].strip()
        if len(clean_line):
            if(clean_line[0]!='#' and clean_line[0]!='/'):
                linelist = clean_line.replace('(',',').split(',')
                if(len(linelist[0])):
                    if(linelist[0] == "START_LUMP"):
                        foundstart = True
                        foundspritelist = True
                        chunkid = 0
                    elif (linelist[0] == "END_LUMP"):
                        foundstart = False
                        chunkid = 0
                    else:
                        if(foundspritelist):
                            if(foundstart==True):
                                outfile.write("\t"+linelist[0]+" = "+str(chunkid+1)+",\n")
                                chunkid = chunkid+1
                            else:
                                outfile.write("\t"+linelist[0]+" = 1,\n")

outfile.close()

